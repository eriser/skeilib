#ifndef skei_voice_manager_included
#define skei_voice_manager_included
//----------------------------------------------------------------------

/*
  polyphonic voice manager
  - max block size: 999999 samples
    a biiig offset that we will hopefully never reach
  - max events per block: #define MAX_EVENTS 1024
      a static buffer for the incoming events, to avoid runtime memory juggling
  - very basic nna/note-stealing (works ok-ish)
  - currently only note on/off
  - converts events in processSample
      could be done in processBlock (does it matter?)
  - not optimized
      but 16 basic saw voices uses only around 0.8% cpu (reaper/wine)
      so i've postponed that until it gets problematic
  - bugs?
*/

//----------

/*

  [assume events in buffer are sorted by their offset in the audiobuffer]

  we have:
  - an array with all available voices (allocated at startup)
  - a list of available voices (initially containing ptr to all available voices)
  - a list of playing voices
  - a list of released (decaying) voices
  note on:
  - grab a voice from the free voices list, and append it to the playing list
  - if no voice available in the free list, take the oldest voice from decaying voices
  note off:
  - move the voice from the playing to the released list
  process:
  - process both playing and released lists,
    call process for each voice, and add their outputs
    voices can set their mState to vs_Off to shut themselves off (see post-process)
  post-process:
  - move all voices with mState == vs_Off from released to free voices list
  - cleanup buffers and counters

  midi:
  we copy all incoming midi events to a buffer.
  during sample processing:
  keep track of current offset (position in buffer), and next event to process
  when offset reaches next event, we fire off note on/ff etc events
  and continue, looking at the next event and its offset


  after the block has finished, we reset the buffers and offset
  and prepare for next block

*/

#include "skei_list.h"
//#include "skei_voice.h"

//#define OVERSAMPLE 4

class SVoiceManager;

//----------------------------------------------------------------------

class SVoice : public SListNode
{
  //friend class SVoiceManager;
  protected:
    SVoiceManager*  MManager;
    uint32          MState;
    float           MSampleRate;
    int32           MMidiNote;
    int32           MMidiVel;
    float           MPitchBend;
  public:
    SVoice(SVoiceManager* AManager) {
      MManager    = AManager;
      MState      = svs_off;
      MSampleRate = 0;
      MMidiNote   = 0;
      MMidiVel    = 0;
      MPitchBend  = 0;
    }
    virtual ~SVoice() {
    }
  public:
    void setState(uint32 AState) {
      MState = AState;
    }
    void setSampleRate(float ARate) {
      MSampleRate = ARate;
    }
    uint32 getState(void) {
      return MState;
    }
  public:
    virtual void  on_noteOn(int ANote, int AVel) {
      MMidiNote = ANote;
      MMidiVel = AVel;
    }
    virtual void  on_noteOff(int ANote, int AVel) {
      MMidiNote = -1;
      MMidiVel = 0;
    }
    virtual void  on_pitchBend(float ABend) {
      MPitchBend = ABend;
    }
    virtual void  on_control(int AIndex, float AVel) {
    }
    virtual void  on_process(float* outs) {
      outs[0] = 0;
      outs[1] = 0;
    }
};

//----------

typedef SArray<SVoice*> SVoiceArray;

//----------------------------------------------------------------------

struct SVoiceEvent {
  int ofs;
  unsigned char msg1,msg2,msg3;
  char padding;
};

#define MAX_EVENTS 1024

//----------

class SVoiceManager {
  private:
    int         MOffset;              // current audio buffer offset
    int         MNextEvent;           // next event offset in audio buffer
    int         MCurrEvent;           // current event (in MEvents buffer)
    int         MNumEvents;           // number of events in buffer
    SVoiceEvent MEvents[MAX_EVENTS];  // events
    int         MNumPlaying;          // number of plauing voices
    float       MSampleRate;
  protected:
    SVoice*     MNoteMap[128];        // map midi-note -> voice (for note-off, etc)
    SVoiceArray MAllVoices;           // all available (pre-allocated) voices
    SList       MFreeVoices;          // list of free (non-playing) voices
    SList       MPlayingVoices;       // list oc active/playing voices
    SList       MReleasedVoices;      // voices in release state/stage
  private:
    float       MPitchBend;
    float       MPitchBendRange;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    SVoiceManager() {
      MOffset     = 0;
      MNextEvent  = 999999;
      MCurrEvent  = 0;
      MNumEvents  = 0;
      SMemset(MEvents,0,sizeof(MEvents));
      MNumPlaying = 0;
      MSampleRate = 0;
      //
      SMemset(MNoteMap,0,sizeof(MNoteMap));
      //
      MPitchBend = 0;
      MPitchBendRange = 2;

    }

    virtual ~SVoiceManager() {
      #ifndef SKEI_NO_AUTODELETE
      deleteVoices();
      #endif
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    float getPitchBend(void) { return MPitchBend; }

  //----------------------------------------
  //
  //----------------------------------------

  private:

    void noteOn(int32 ANote, int32 AVel) {
      SVoice* V = (SVoice*)MFreeVoices.tail();
      if (V) {
        MFreeVoices.removeTail();
      }
      else {
        V = (SVoice*)MReleasedVoices.head();
        if (V) { MReleasedVoices.removeHead(); }
      }
      if (V) {
        MNoteMap[ANote] = V;
        MPlayingVoices.append(V);
        V->setState(svs_playing); //V->MState = svs_playing;
        V->on_noteOn(ANote,AVel);
        MNumPlaying++;
      }
    }

    //----------

    void noteOff(int ANote, int AVel) {
      SVoice* V = MNoteMap[ANote];
      if (V) {
        MNoteMap[ANote] = NULL;
        MPlayingVoices.remove(V);
        MReleasedVoices.append(V);
        V->setState(svs_released); //V->MState = svs_released;
        V->on_noteOff(ANote,AVel);
        MNumPlaying--;
      }
    }

    //----------

    void pitchBend(int32 ABend) {
      MPitchBend = (float)ABend - 8192.0f;
      MPitchBend *= SKEI_INV8192F; //(1/8192);
      MPitchBend *= MPitchBendRange;
      if (MAllVoices.size() > 0) {
        for (int32 i=0; i<MAllVoices.size(); i++) MAllVoices[i]->on_pitchBend(MPitchBend);
      }
    }

  public:

    void control(int AIndex, float AVal) {
      for (int i=0; i<MAllVoices.size(); i++) {
        MAllVoices[i]->on_control(AIndex,AVal);
      }
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    void appendVoice(SVoice* V) {
      MAllVoices.append(V);
      //mFreeVoices.appendNode(V);
      MFreeVoices.insertTail(V);
    }

    //----------

    //virtual
    void deleteVoices(void) {
      for (int i=0; i<MAllVoices.size(); i++) {
        delete MAllVoices[i];
      }
    }

    //----------

    /*
      oversampling:
        ARate * oversample
    */

    //virtual
    void setSampleRate( float ARate ) {

      MSampleRate = ARate;// * OVERSAMPLE;

      for (int i=0; i<MAllVoices.size(); i++) {
        MAllVoices[i]->setSampleRate(MSampleRate);
      }
    }

    void setPitchBendRange(float ARange) {
      MPitchBendRange = ARange;
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void midi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3) {
      //trace(" ofs "<<ofs << " msg1 "<<msg1 << " msg2 "<<msg2 << " msg3 "<<msg3);
      if (MNumEvents<MAX_EVENTS) {
        MEvents[MNumEvents].ofs  = ofs;
        MEvents[MNumEvents].msg1 = msg1;
        MEvents[MNumEvents].msg2 = msg2;
        MEvents[MNumEvents].msg3 = msg3;
        MNumEvents++;
      }
    }

    //----------

    void preProcess(void) {
      MOffset = 0;
      MCurrEvent = 0;
      if (MNumEvents>0) MNextEvent = MEvents[0].ofs;
      else MNextEvent = 999999;
    }

    //----------

    void processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
    }

    void processSample(SSample** AInputs, SSample** AOutputs) {
    }

    // stereo
    void process(float* outs) {
      // events
      while (MOffset==MNextEvent) {
        //int chn  = mEvents[mCurrEvent].msg1 & 0x0f;
        int msg  = MEvents[MCurrEvent].msg1 & 0xf0;
        int note = MEvents[MCurrEvent].msg2;
        int vel  = MEvents[MCurrEvent].msg3;
        switch(msg) {
          case 0x90:
            if (vel==0) noteOff(note,vel);
            else noteOn(note,vel);
            break;
          case 0x80:
            note = MEvents[MCurrEvent].msg2;
            vel  = MEvents[MCurrEvent].msg3;
            noteOff(note,vel);
            break;
          case 0xE0:
            int32 bend = MEvents[MCurrEvent].msg2 + (MEvents[MCurrEvent].msg3 * 128);
            pitchBend( bend );
            break;
        }
        MCurrEvent++;
        if (MCurrEvent<MNumEvents) MNextEvent = MEvents[MCurrEvent].ofs;
        else MNextEvent = 999999;
      }
      MOffset++;
      //float out = 0;
      float left = 0;
      float right = 0;
      float _outs[2];
      // playing voices
      SVoice* V = (SVoice*)MPlayingVoices.head();
      while (V) {
        //out += V->process();
        V->on_process(&_outs[0]);
        left += _outs[0];
        right += _outs[1];
        V = (SVoice*)V->next();
      }
      // released voices
      V = (SVoice*)MReleasedVoices.head();
      while (V) {
        //if (V->MState!=svs_off) {
        if (V->getState()!=svs_off) {
          //out += V->process();
          V->on_process(&_outs[0]);
          left += _outs[0];
          right += _outs[1];
        }
        V = (SVoice*)V->next();
      }
      //return out;
      outs[0] = left;
      outs[1] = right;
    }

    //----------

    void postProcess(void) {
      SVoice* V = (SVoice*)MReleasedVoices.head();
      while (V) {
        //if (V->MState==svs_off) {
        if (V->getState()==svs_off) {
          SVoice* next = (SVoice*)V->next();
          MReleasedVoices.remove(V);
          MFreeVoices.append(V);
          V = next;
        }
        else V = (SVoice*)V->next();
      }
      MNumEvents = 0;
    }

};

//----------

#undef MAX_EVENTS
//#undef OVERSAMPLE

//----------------------------------------------------------------------
#endif
