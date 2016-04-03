#ifndef test_channel_included
#define test_channel_included
//----------------------------------------------------------------------

// nc -U -l -k /tmp/skei.sock

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//#define SKEI_PLUGIN_HAS_EDITOR
#define SKEI_PLUGIN_IS_SYNTH
#define SKEI_PLUGIN_RECEIVE_MIDI

#ifdef SKEI_PLUGIN_HAS_EDITOR
  #define SKEI_XLIB
  #define SKEI_XRENDER
  #define SKEI_XFT
#endif

#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_CRASHHANDLER

#ifdef SKEI_VST
  //#define SKEI_DEBUG_VST
  #define SKEI_DEBUG_SOCKET
#endif

#define SKEI_NO_MODULE_TICKS

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"

#include "skei_channel.h"
//#include "skei_channel2.h"
#include "skei_interpolate.h"
//#include "skei_modular.h"
#include "skei_module_filter_svf.h"
#include "skei_module_noise.h"
#include "skei_module_param.h"
#include "skei_module_phasor.h"
#include "skei_module_mul.h"
#include "skei_module_voices.h"
#include "skei_parameter.h"
#include "skei_plugin.h"
//#include "skei_voice_manager.h"

#ifdef SKEI_PLUGIN_HAS_EDITOR
#include "skei_editor.h"
#endif

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class myVoice
: public SVoice {

  private:

    float   t,dt,v;
    uint32  MNote;
    float   MBend;

  public:

    myVoice() : SVoice() {
      MNumOutputs = 1;
      MNote = 0;
      MBend = 0;
    }

    //----------

    //virtual
    void on_noteOn(uint32 ANote, float AVelocity) {
      MNote = ANote;
      v = AVelocity;
      t = 0;
      dt = SNoteToHz(ANote+MBend) / MSampleRate;
    }

    //----------

    //virtual
    void on_noteOff(uint32 ANote, float AVelocity) {
      MState = sms_off;
    }

    //----------

    //virtual
    void on_pitchBend(float ABend) {
      MBend = ABend;
      dt = SNoteToHz(MNote+ABend) / MSampleRate;
    }

    //----------

    //virtual
    void on_processTick(uint32 AOffset, uint32 ALength) {
      SAssert( MOutputs[0] != SKEI_NULL );
      SModule::on_processTick(AOffset,ALength);
      float* ptr = MOutputs[0] + AOffset;
      for (uint32 i=0; i<ALength; i++) {
        *ptr++ = (t*2-1)*v;
        t += dt;
        t = SFract(t);
      }
    }

    //----------

    //virtual
    void on_processTickFast(uint32 AOffset) {
      on_processTick(AOffset,SKEI_MODULE_TICKSIZE);
    }

};

//----------

typedef SModule_Voices<myVoice,16> myVoices;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

/*

SModuleInstance* test_create(SModuleInfo* AInfo) {
  SModuleInstance* mi = (SModuleInstance*)SMalloc(sizeof(SModuleInstance));
  return mi;
}

void test_delete(SModuleInstance* AInstance) {
  SFree(AInstance);
}

SModuleInfo testmodule_info = {
  &test_create,   // createInstance;
  &test_delete,   // deleteInstance;
  "testmodule",   // name;
  "skei",         // author;
  0,              // numInputs;
  0,              // numOutputs;
  0,              // numParams;
  SKEI_NULL,      // inputs;
  SKEI_NULL,      // outputs;
  SKEI_NULL       // params;
};

*/

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public SPlugin
{

  private:

    SChannel            MChannel;
    myVoices*           m_voices;
    SModule_Param*      m_param;
    SModule_Phasor*     m_phasor;
    SModule_Mul*        m_mul;
    SModule_Filter_SVF* m_svf;

  public:

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    // resizeBuffer has not been called yet -> buffer ptrs are not set up

    //virtual
    void on_create(void) {
      MInfo.name       = "test_channel";
      MInfo.author     = "skei";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      #ifdef SKEI_PLUGIN_HAS_EDITOR
      MEditorRect = SRect(640,480);
      #endif

      appendParameter( new SParameter("vol",  0.5f ));
      appendParameter( new SParamInt( "mode", 1, 0, 4 ));
      appendParameter( new SParameter("freq", 1.0f ));
      appendParameter( new SParameter("bw",   1.0f ));

      //m_noise = new SModule_Noise();
      m_voices  = new myVoices();
      m_param   = new SModule_Param();
      m_phasor  = new SModule_Phasor();
      m_mul     = new SModule_Mul();
      m_svf     = new SModule_Filter_SVF();

      MChannel.appendModule(m_phasor);
      MChannel.appendModule(m_voices);
      MChannel.appendModule(m_param);
      MChannel.appendModule(m_mul);
      MChannel.appendModule(m_svf);

      MChannel.registerNoteListener(m_voices);
      MChannel.registerNoteListener(m_phasor);

      MChannel.registerBendListener(m_voices);
      MChannel.registerBendListener(m_phasor);

      MChannel.registerParamListener(m_param, 0 );
      MChannel.registerParamListener(m_svf,   1 );

      //SModule* test = new SModule();
      //test->createInstance(&testmodule_info);
      //delete test;

    }

    //----------

    /*
      if we get a blockSize event, and need to resize the internal buffers,
      pointer to inputs/outputs changes..
      so we need to do the connections in resume..

      bitwig crashes if we do this in on_create, probably because this is
      called before we get the blockSize event..
    */

    void connect_modules(void) {
      m_mul->connectInput( 0, m_voices->output(0) );
      m_mul->connectInput( 1, m_param->output(0) );
      m_svf->connectInput( 0, m_mul->output(0) );
      MChannel.setOutput(m_svf,0,1);
    }

    //------------------------------------------------------------
    // events
    //------------------------------------------------------------

    //virtual
    void on_stateChange(uint32 AState) {
      switch(AState) {
        case sps_resume:
          connect_modules();
          break;
        case sps_sampleRate:
          MChannel.on_sampleRate(MState.sampleRate);
          break;
        case sps_blockSize:
          MChannel.on_blockSize(MState.blockSize);
          break;
      }
    }

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      MChannel.on_parameter(AIndex,AValue);
    }

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      MChannel.midiEvent(AOffset,AMsg1,AMsg2,AMsg3);
    };

    //------------------------------------------------------------
    // audio
    //------------------------------------------------------------

    /*
      SKEI_MODULE_TICKSIZE
      1:330, 4:470, 8:530, 16:550, 32:560
      SKEI_NO_MODULE_TICKS:590
    */

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
      MChannel.processBlock(AInputs,AOutputs,ANumSamples);
      SCopyMonoToStereoBuffer(AOutputs, &AOutputs[0], ANumSamples);
      m_voices->postProcessVoices();
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif

