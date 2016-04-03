#ifndef skei_voice_manager_included
#define skei_voice_manager_included
//----------------------------------------------------------------------

#if 0

/*
  * wouldn't it be better if MAllVoices was set up from sub-modules?
  * MPE - 16 channels
  * and, should SModule have noteOn, noteOff, pitchBend ?

  * midi is not sample accurate..
    currently sent directly to the modules when we receive them in the plugin.
    a) sample accurate (events and counting samples)
    b) at the start of each tick..

*/

//----------------------------------------------------------------------

#include "skei_module.h"
#include "skei_audio_utils.h"
#include "skei_list.h"
#include "skei_voice.h"

//----------------------------------------------------------------------

template <typename _V, int _N>
class SVoiceManager
: public SModule
{

  private:
    float         MPitchBend;
    float         MPitchBendRange;
  private:
    _V            MAllVoices[_N];
    SList         MFreeVoices;
    SList         MPlayingVoices;
    SList         MReleasedVoices;
    _V*           MNoteMap[128*16];
    uint32        MNumPlaying;

  public:

    SVoiceManager(void)
    : SModule() {
      MPitchBend      = 0;
      MPitchBendRange = 2;
      for (uint32 i=0; i<_N; i++) MFreeVoices.append( &MAllVoices[i] );
      SMemset(MNoteMap,0,sizeof(MNoteMap));
      MNumPlaying     = 0;
      MNumOutputs     = 2;
    }

    virtual ~SVoiceManager() {
    }

  //----------------------------------------
  // accessors
  //----------------------------------------

  public:

    uint32  numPlaying(void)              { return MNumPlaying; }
    void    pitchBendRange(float ARange)  { MPitchBendRange = ARange; }

  //----------------------------------------
  // voices
  //----------------------------------------

  private:

    _V* findFreeVoice(void) {
      // try free voices first
      _V* voice = (_V*)MFreeVoices.tail();
      if (voice) {
        MFreeVoices.removeTail();
        return voice;
      }
      // if no free voices, grab (oldest) released voices
      else {
        // voices are appended to the end (tail), so head is the oldest one
        voice = (_V*)MReleasedVoices.head();
        if (voice) {
          MReleasedVoices.removeHead();
          return voice;
        }
      }
      return SKEI_NULL;
    }

    //----------

    /*
      process voice (to internal buffer), and add result to ABuffer
    */

    void processTickVoice(SVoice* AVoice, uint32 ALength, SSample** ABuffer) {
      //AVoice->process(ALength);
      AVoice->on_processTick(0,ALength);
      SSample* vptr = AVoice->output(0);
      SAddMonoBuffer(ABuffer,&vptr,ALength);
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //void reset(void) {
    //  for (uint32 i=0; i<NUM; i++) MAllVoices[i].reset();
    //}

    //----------

    //virtual
    void on_sampleRate(float ARate) {
      SModule::on_sampleRate(ARate);
      for (uint i=0; i<_N; i++) MAllVoices[i].on_sampleRate(MSampleRate);
    }

    //----------

    //virtual
    void on_blockSize(uint32 ASize) {
      SModule::on_blockSize(ASize);
      for (uint i=0; i<_N; i++) MAllVoices[i].on_blockSize(MBlockSize);
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    void on_parameter(uint32 AIndex, float AValue) {
      //SModule::control(AIndex,AValue);
      for (uint32 i=0; i<_N; i++) MAllVoices[i].on_parameter(AIndex,AValue);
    }

    //----------

    //virtual
    void on_control(uint32 AIndex, float AValue, uint32 AChannel) {
      //SModule::control(AIndex,AValue);
      for (uint32 i=0; i<_N; i++) MAllVoices[i].on_control(AIndex,AValue);
    }

    //----------

    //virtual
    void on_noteOn(uint32 ANote, float AVelocity, uint32 AChannel) {
      //SModule::noteOn(ANote,AVelocity,AChannel);
      _V* voice = findFreeVoice();
      if (voice) {
        MNoteMap[AChannel*16 + ANote] = voice;
        MPlayingVoices.append(voice);
        voice->state(sms_active);
        voice->on_noteOn(ANote,AVelocity);
        MNumPlaying++;
      }
    }

    //----------

    //virtual
    void on_noteOff(uint32 ANote, float AVelocity, uint32 AChannel) {
      //SModule::noteOff(ANote,AVelocity,AChannel);
      _V* voice = MNoteMap[AChannel*16 + ANote];
      if (voice) {
        MNoteMap[AChannel*16 + ANote] = SKEI_NULL;
        MPlayingVoices.remove(voice);
        MReleasedVoices.append(voice);
        voice->state(sms_finished);
        voice->on_noteOff(ANote,AVelocity);
        MNumPlaying--;
      }
    }

    //----------

    /*
      pitchbend is converted to -1..1, then scaled by pitchBend range
      and sent to all voices
    */

    //virtual
    void on_pitchBend(float ABend, uint32 AChannel) {
      //SModule::pitchBend(ABend,AChannel);
      MPitchBend = (float)ABend - 8192.0f;
      MPitchBend *= SKEI_INV8192F;
      MPitchBend *= MPitchBendRange;
      for (uint32 i=0; i<_N; i++) MAllVoices[i].on_pitchBend(MPitchBend);
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    void on_processTick(uint32 AOffset, uint32 ALength) {
      SModule::on_processTick(AOffset,ALength);
      SSample* buffer = MOutputs[0] + AOffset;
      SClearMonoBuffer(&buffer,ALength);
      _V* voice;
      // process playing voices
      voice = (_V*)MPlayingVoices.head();
      while (voice) {
        processTickVoice(voice,ALength,&buffer);
        voice = (_V*)voice->next();
      }
      // released voices
      voice = (_V*)MReleasedVoices.head();
      while (voice) {
        processTickVoice(voice,ALength,&buffer);
        voice = (_V*)voice->next();
      }
    }

    //----------

    // todo

    //virtual
    void on_processTickFast(uint32 AOffset) {
      //SModule::processTickFast(AOffset);
      on_processTick(AOffset, SKEI_MODULE_TICKSIZE);
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    // if any of the released voices has changed state to svs_off,
    // move the voice from released voices list to free voices list

    void postProcessVoices(void) {
      _V* voice = (_V*)MReleasedVoices.head();
      while (voice) {
        if (voice->state()==sms_off) {
          _V* next = (_V*)voice->next();
          MReleasedVoices.remove(voice);
          MFreeVoices.append(voice);
          voice = next;
        }
        else {
          voice = (_V*)voice->next();
        }
      }
    }

};

#endif // 0

//----------------------------------------------------------------------
#endif
