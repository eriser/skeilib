
/*
  TODO:
  - restart buffer when starting/stopping playback..
    or synchronize with host..
  - playback speed, slice index
*/

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//#define SKEI_EXE
//#define SKEI_XLIB

//#define SKEI_DEBUG
//#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_MEM_PRINT
#define SKEI_DEBUG_SOCKET
//#define SKEI_DEBUG_VST

#define SKEI_PLUGIN_PER_SAMPLE
//#define SKEI_PLUGIN_HAS_EDITOR
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_SEND_MIDI
#define SKEI_PLUGIN_RECEIVE_MIDI
#define SKEI_PLUGIN_AUTOSYNC

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"

#include "skei_host_sync.h"
#include "skei_midi_timing.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

const char* txt_past_future[2] = {
  "past",
  "future"
};

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin
, public SHostSync_Listener
, public SMidiTiming_Listener
{

  private:
    float         par_bufsize;
    float         par_size;
    float         par_offset;
    float         par_pitch;
    bool          par_past;

  private:
    float         MBeatSize;
    float         MBuffer[ (1024*1024) * 2 ]; // 2mb
    float         MBufferSize;
    float         MReadPos;
    float         MWritePos;
    bool          MHostLoop;
    SHostSync*    MHostSync;
    SMidiTiming*  MMidiTiming;
    uint32        MKeyDown;
    uint32        MKeyPrev;
    //float         MNumSlices;
    bool          MLoopActive;
    float         MLoopStart;
    float         MLoopSize;
    float         MLoopEnd;
    uint32        MLoopPrev;

    float         MPitchBend;

  public:

    //virtual
    void on_create(void) {

      SMemset(MBuffer,0,sizeof(MBuffer));
      MHostSync     = new SHostSync(this);
      MMidiTiming   = new SMidiTiming(this);
      par_bufsize   = 0.0f;
      par_size      = 0.0f;
      par_offset    = 0.0f;
      par_pitch     = 1.0f;
      par_past      = true;
      //MNumSlices  = 0;
      MBufferSize   = 0.0f;
      MReadPos      = 0.0f;
      MWritePos     = 0.0f;
      MBeatSize     = 0.0f;
      MKeyDown      = 0;
      MKeyPrev      = 0;
      MLoopActive   = false;
      MLoopStart    = 0.0f;
      MLoopSize     = 0.0f;
      MLoopEnd      = 0.0f;
      MLoopPrev     = 0.0f;
      MPitchBend    = 0.0f;
      MHostLoop     = false;

      MInfo.name          = "fx_repeater";
      MInfo.author        = "skei";
      MInfo.product       = MInfo.name;
      MInfo.uniqueId      = SKEI_MAGIC + 0x0000;
      MInfo.numInputs     = 2;
      MInfo.numOutputs    = 2;

      appendParameter( new SParamInt(   "buffer", 4, 1, 16 ));
      appendParameter( new SParamFloat( "size1",  SKEI_INV32F*16.0f, SKEI_INV32F, 1.0f, SKEI_INV32F ));
      appendParameter( new SParamFloat( "size2",  SKEI_INV32F*8.0f,  SKEI_INV32F, 1.0f, SKEI_INV32F ));
      appendParameter( new SParamFloat( "size3",  SKEI_INV32F*4.0f,  SKEI_INV32F, 1.0f, SKEI_INV32F ));
      appendParameter( new SParamFloat( "size4",  SKEI_INV32F*2.0f,  SKEI_INV32F, 1.0f, SKEI_INV32F ));
      appendParameter( new SParameter(  "offset", 0 ));
      appendParameter( new SParamFloat( "pitch",  0, -1, 1 ));
      appendParameter( new SParamText(  "start",  0, 2, txt_past_future ));

      MHostSync->appendInterval( SKEI_INV32F*16.0f );
      MHostSync->appendInterval( SKEI_INV32F*8.0f );
      MHostSync->appendInterval( SKEI_INV32F*4.0f );
      MHostSync->appendInterval( SKEI_INV32F*2.0f );


    }

    //----------

    //virtual
    void on_destroy(void) {
      delete MHostSync;
      delete MMidiTiming;
    }

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    virtual void on_miditiming_event(uint32 msg, uint32 chn, uint32 idx, uint32 val) {
      switch(msg) {
        case 0x90: // note on
          // if val==0 -> noteoff
          if (idx==36) { MKeyDown = SBitSet(MKeyDown,0); }
          if (idx==37) { MKeyDown = SBitSet(MKeyDown,1); }
          if (idx==38) { MKeyDown = SBitSet(MKeyDown,2); }
          if (idx==39) { MKeyDown = SBitSet(MKeyDown,3); }
          break;
        case 0x80: // note off
          if (idx==36) { MKeyDown = SBitClear(MKeyDown,0); }
          if (idx==37) { MKeyDown = SBitClear(MKeyDown,1); }
          if (idx==38) { MKeyDown = SBitClear(MKeyDown,2); }
          if (idx==39) { MKeyDown = SBitClear(MKeyDown,3); }
          break;
        case 0xe0: // pitch bend
          MPitchBend = idx + (val<<7);          // 0..16383
          MPitchBend /= 16383.0f;               // 0..1
          MPitchBend = (MPitchBend*2) - 1;      // -1..1
          //DTrace("pitchbend: %f\n",MPitchBend);
          break;
      }
    }

    //----------

    virtual void on_hostsync_tick(uint32 AIndex) {
      bool keys_changed = false;
      if (  SBitGet(MKeyDown,AIndex) && !SBitGet(MKeyPrev,AIndex)) { keys_changed = true; }
      if ( !SBitGet(MKeyDown,AIndex) &&  SBitGet(MKeyPrev,AIndex)) { keys_changed = true; }
      if (keys_changed) {
        MKeyPrev = MKeyDown;
        uint32 loop = 0;
        if (MKeyDown & 1) loop = 1;
        if (MKeyDown & 2) loop = 2;
        if (MKeyDown & 4) loop = 3;
        if (MKeyDown & 8) loop = 4;
        //DTrace("loop:%i\n",loop);
        if (loop>0) {
          MLoopSize = MBeatSize * MHostSync->getSliceSize(loop-1);
          if (!MLoopActive) {
            if (par_past) {
              // if MWritePos = 0, MLoopSize will be negative!
              MLoopStart = MWritePos - MLoopSize;
              if (MLoopStart<0) MLoopStart += MBufferSize;
            }
            else {
              MLoopStart = MWritePos;
            }
          } // ! loop_active
          MLoopEnd = MLoopStart + MLoopSize;
          SAssert(MLoopStart >= 0);
          SAssert(MLoopStart <= (MBufferSize-MLoopSize)); //
          SAssert(MLoopStart <  MLoopEnd);
          MLoopActive = true;
          MReadPos = MLoopStart;
          //MReadPos = MLoopStart + SModulo((MReadPos-MLoopStart),MLoopSize);
          //MNumSlices = MBufferSize / MLoopSize;
          //DTrace("num slices: %f\n",MNumSlices);
        } // loop>0
        else {
          MLoopActive = false;
          MReadPos = MWritePos;
        } // ! loop>0
      } // keys_changed
    }

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    //virtual
    void on_transportChange(uint32 ATransport) {
      if (ATransport & (spt_changed|spt_play)) {
        MHostSync->restart();
        MReadPos = 0;
        MWritePos = 0;
      }
      MHostLoop = (ATransport&spt_cycle);
    }

    //----------

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      MMidiTiming->event(AOffset,AMsg1,AMsg2,AMsg3);
    }

    //----------------------------------------
    // parameters/programs
    //----------------------------------------

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      switch(AIndex) {
        case 0: par_bufsize = AValue; break;
        case 1: par_size    = AValue; MHostSync->setSliceSize(0,par_size);  break;
        case 2: par_size    = AValue; MHostSync->setSliceSize(1,par_size);  break;
        case 3: par_size    = AValue; MHostSync->setSliceSize(2,par_size);  break;
        case 4: par_size    = AValue; MHostSync->setSliceSize(3,par_size);  break;
        case 5: par_offset  = AValue; break;
        case 6: par_pitch   = AValue >= 0 ? (AValue+1) : (1/(1-AValue)); break;
        case 7: par_past    = AValue < 0.5 ? true : false; break;
      }
    }

    /*

    +1      2
    +0-5    1.5
    0       1

    0       1
    -0.5    1/(1--0.5) = 1/1.5 = 0.66
    -1      1/(1--1)   = 1/2   = 0.5

    */


    //----------------------------------------
    // process
    //----------------------------------------

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {

      MBeatSize = ( MState.sampleRate * 60.0f / MState.tempo );
      MBufferSize = par_bufsize * MBeatSize;

      MHostSync->setTempo(MState.tempo);
      MHostSync->setSampleRate(MState.sampleRate);
      MHostSync->setBeatPos(MState.beatPos);
      MHostSync->setLoop( MHostLoop, MState.loopStart, MState.loopEnd );

      MMidiTiming->preProcess();
    }

    //----------

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {

      float in0 = *AInputs[0];
      float in1 = *AInputs[1];
      float out0 = 0.0f;
      float out1 = 0.0f;

      if (MState.playState&spt_play) {

        //----------

        MMidiTiming->process();
        MHostSync->process();

        //----------

        if (MBufferSize>0) {
          if (MLoopActive) {
            MReadPos = MLoopStart + SModulo((MReadPos-MLoopStart),MLoopSize);
            SAssert( MReadPos >= MLoopStart );
            SAssert( MReadPos <  MLoopEnd ); // !!! crashes
          }
          else {
            MReadPos  = SModulo(MReadPos,MBufferSize);
          }
          MWritePos = SModulo(MWritePos,MBufferSize);
        }
        else {
          MReadPos  = 0;
          MWritePos = 0;
        }

        //----------

        uint32 w2 = STrunc( MWritePos * 2.0f );
        MBuffer[w2] = in0;
        MBuffer[w2+1] = in1;
        MWritePos += 1.0f;

        //----------

        uint32 r2 = STrunc( MReadPos * 2.0f );
        float buf0 = MBuffer[r2];
        float buf1 = MBuffer[r2+1];

        // pitch bend

        float readpos_add = 1.0f;
        if (MPitchBend>0.0f) readpos_add += MPitchBend;
        if (MPitchBend<0.0f) readpos_add += (MPitchBend * 0.5f);

        if (MLoopActive) MReadPos += readpos_add;
        else MReadPos += 1.0f;

        //----------

        out0 = buf0;
        out1 = buf1;

      }

      *AOutputs[0] = out0;
      *AOutputs[1] = out1;
    }

    //----------

    //virtual
    void on_postProcess(void) {
      MMidiTiming->postProcess();
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);
