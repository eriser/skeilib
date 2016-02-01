#ifndef test_module_included
#define test_module_included
//----------------------------------------------------------------------

#define SKEI_DEBUG_MEM
#ifdef SKEI_VST
  #define SKEI_DEBUG_SOCKET
#endif
//#define SKEI_PLUGIN_PER_SAMPLE
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_RECEIVE_MIDI

//----------

#include "skei.h"
#include "skei_plugin.h"
#include "skei_module.h"
#include "skei_module_noise.h"

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin
{

  private:
    SModule MAudioModule;
    float   MAudioBuffer[65536];

  public:

    //virtual
    void on_create(void) {
      MInfo.name       = "test_module";
      MInfo.author     = "skei";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;

      SModule* m;
      // noise
      m = new SModule_Noise();
      m->connectOutput( 0, MAudioBuffer );
      MAudioModule.appendModule(m);
      // multiply
      m = new SModule_Mul();
      m->connectInput( 0, MAudioBuffer );
      m->connectOutput( 0, MAudioBuffer );
      m->handleControl( 0, 0.1f );
      MAudioModule.appendModule(m);

      MAudioModule.setOverSample( 1 );
      SMemset(MAudioBuffer,0,sizeof(MAudioBuffer));
    }

    //----------

    //virtual
    void on_stateChange(uint32 AState) {
      switch(AState) {
        case sps_sampleRate:
          MAudioModule.setSampleRate(MState.sampleRate);
          break;
        case sps_blockSize:
          MAudioModule.setBlockSize(MState.blockSize);
          break;
        case sps_resume:
          MAudioModule.setSampleRate(MState.sampleRate);
          MAudioModule.setBlockSize(MState.blockSize);
          break;
      }
    }

    //----------

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
      MAudioModule.setBlockSize(ANumSamples);
      MAudioModule.processBlock();
      // copy result
      int32 num = ANumSamples * sizeof(SSample);
      SMemcpy( AOutputs[0], MAudioBuffer, num );
      SMemcpy( AOutputs[1], MAudioBuffer, num );
    }

};

//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif
