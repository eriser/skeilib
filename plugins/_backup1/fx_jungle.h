
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
//#define SKEI_PLUGIN_RECEIVE_MIDI
#define SKEI_PLUGIN_AUTOSYNC

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"

#define BUFFER_SIZE       (1024*1024)

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin
//, public SHostSync_Listener
//, public SMidiTiming_Listener
{

  private:
    float   MBuffer[BUFFER_SIZE*2];
    uint32  MRecPos;
    uint32  MPlayPos;
    uint32  MBeatSize;
    uint32  MLoopStart;
    uint32  MLoopSize;
    bool    MLooping;

    bool    MButton,MButtonPrev;
    bool    MButton1,MButton2,MButton3,MButton4;
  //bool    MButton1Prev,MButton2Prev,MButton3Prev,MButton4Prev;

    float MSpeed;

  public:

    //virtual
    void on_create(void) {

      MInfo.name          = "fx_stutter";
      MInfo.author        = "skei";
      MInfo.product       = MInfo.name;
      MInfo.uniqueId      = SKEI_MAGIC + 0x0000;
      MInfo.numInputs     = 2;
      MInfo.numOutputs    = 2;

      appendParameter( new SParamInt(  "1/2",     0,0,1 ));
      appendParameter( new SParamInt(  "1/4",     0,0,1 ));
      appendParameter( new SParamInt(  "1/8",     0,0,1 ));
      appendParameter( new SParamInt(  "1/16",    0,0,1 ));

      MRecPos     = 0;
      MPlayPos    = 0;
      MBeatSize   = 0;
      MLooping    = false;
      MLoopStart  = 0;
      MLoopSize   = 0;

      MButton     = false;
      MButtonPrev = false;
      MButton1    = false;
      MButton2    = false;
      MButton3    = false;
      MButton4    = false;

      MSpeed      = 1;

    }

    //----------

    //virtual
    void on_destroy(void) {
    }

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    /*
    //virtual
    void on_transportChange(uint32 ATransport) {
      if (ATransport & (spt_changed|spt_play)) {
        MHostSync->restart();
        MReadPos = 0;
        MWritePos = 0;
      }
      MHostLoop = (ATransport&spt_cycle);
    }
    */

    //----------

    /*
    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      MMidiTiming->event(AOffset,AMsg1,AMsg2,AMsg3);
    }
    */

    //----------------------------------------
    // parameters/programs
    //----------------------------------------

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      switch(AIndex) {
        case 0: MButton1 = (AValue>0.5f); break;
        case 1: MButton2 = (AValue>0.5f); break;
        case 2: MButton3 = (AValue>0.5f); break;
        case 3: MButton4 = (AValue>0.5f); break;
      }
      MButton = false;
      if ((MButton1) || (MButton2) || (MButton3) || (MButton4)) MButton = true;
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
      MBeatSize = (MState.sampleRate * 60) / MState.tempo;
      if (MButton) {
        if (MButton1) MLoopSize = MBeatSize/2;
        if (MButton2) MLoopSize = MBeatSize/4;
        if (MButton3) MLoopSize = MBeatSize/8;
        if (MButton4) MLoopSize = MBeatSize/16;
        if (!MButtonPrev) {
          MLooping = true;
          MLoopStart = MRecPos;// - MLoopSize;
          MPlayPos = 0;
          MButtonPrev = true;
        }
      } else {
        MLooping = false;
        MButtonPrev = false;
      }
    }

    //----------

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {
      float in0 = *AInputs[0];
      float in1 = *AInputs[1];
      float out0 = in0;
      float out1 = in1;

      uint32 r = MRecPos * 2;
      MBuffer[r] = in0;
      MBuffer[r+1] = in1;
      MRecPos += 1;
      MRecPos %= BUFFER_SIZE;

      if (MLooping) {
        uint32 p = MLoopStart + MPlayPos;
        p %= BUFFER_SIZE;
        p *= 2;
        out0 = MBuffer[p];
        out1 = MBuffer[p+1];
        MPlayPos += 1;
        MPlayPos %= MLoopSize;
      }

      *AOutputs[0] = out0;
      *AOutputs[1] = out1;
    }

    //----------

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);


