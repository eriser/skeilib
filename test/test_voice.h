#ifndef test_voice_included
#define test_voice_included
//----------------------------------------------------------------------

// nc -U -l -k /tmp/skei.sock

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//----- plugins -----

#define SKEI_PLUGIN_IS_SYNTH
#define SKEI_PLUGIN_RECEIVE_MIDI
//#define SKEI_DEBUG_MEM
#ifdef SKEI_VST
  //#define SKEI_DEBUG_VST
  #define SKEI_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"
#include "skei_random.h"
//#include "skei_voice_manager.h"

//----------------------------------------------------------------------
// voice
//----------------------------------------------------------------------

/*

class myVoice : public SVoice {

  private:

    float t,dt;

  public:

    myVoice() : SVoice() {
      t   = 0.0f;
      dt  = 1.0f/100.0f; // 100 samples = 441 hz
    }

    void on_noteOn(uint32 ANote, float AVelocity) {
      dt = SNoteToHz(ANote) / MSampleRate;
    }

    //void on_noteOff(uint32 ANote, float AVelocity) { MState = svs_off; }
    //void on_control(uint32 AIndex, float AValue) {}
    //void on_pitchBend(float ABend) {}

    void on_process(SSample** AInputs, SSample** AOutputs, uint32 ALength) {
      //float* in0  = AInputs[0];
      //float* in1  = AInputs[1];
      float* out0 = AOutputs[0];
      float* out1 = AOutputs[1];
      for (uint32 i=0; i<ALength; i++) {
        float n = t*2 - 1;
        *out0++ = n * 0.1f;
        *out1++ = n * 0.1f;
        t += dt;
        t = SFract(t);
      }
    }

};

typedef SVoiceManager<myVoice,16> myVoiceManager;

*/

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin
{

  private:

//    myVoiceManager  vm;

  public:

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    //virtual
    void on_create(void) {
      MInfo.name       = "test_voice";
      MInfo.author     = "skei";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
    }

    //----------

    //virtual
    void on_destroy(void) {
    }

    //------------------------------------------------------------
    // events
    //------------------------------------------------------------

    //virtual
    void on_stateChange(uint32 AState) {
      switch(AState) {
        //case sps_resume:
        //  vm.reset();
        //  break;
        case sps_sampleRate:
//          vm.setSampleRate(MState.sampleRate);
          break;
        case sps_blockSize:
//          vm.setBlockSize(MState.blockSize);
          break;
      }
    }

    //----------

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
//      vm.parameterChange(AIndex,AValue);
    }

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
//      vm.midiEvent(AOffset,AMsg1,AMsg2,AMsg3);
    }

    //------------------------------------------------------------
    // audio
    //------------------------------------------------------------

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
//      vm.processBlock(AInputs,AOutputs,ANumSamples);
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif

