#ifndef test_channel_included
#define test_channel_included
//----------------------------------------------------------------------

// nc -U -l -k /tmp/skei.sock

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//----- plugins -----

//#define SKEI_PLUGIN_PER_SAMPLE
//#define SKEI_PLUGIN_HAS_EDITOR
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_SEND_MIDI
//#define SKEI_PLUGIN_RECEIVE_MIDI
//#define SKEI_PLUGIN_AUTOSYNC

//----- format -----

#ifdef SKEI_PLUGIN_HAS_EDITOR
  #define SKEI_XLIB
  #define SKEI_XRENDER
  #define SKEI_XFT
#endif

//----- debug -----

#define SKEI_DEBUG_MEM

#ifdef SKEI_VST
  //#define SKEI_DEBUG_VST
  #define SKEI_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"
#include "skei_audio_effect.h"
#include "skei_parameter.h"

#ifdef SKEI_PLUGIN_HAS_EDITOR
#include "skei_editor.h"
#endif

//----------------------------------------------------------------------
// channel
//----------------------------------------------------------------------

class myEffect : public SAudio_Effect {

  private:

    float MGain;

  public:

    myEffect() : SAudio_Effect() {
      MGain = 1.0f;
    }

    //virtual
    ~myEffect() {
    }

  public:

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, uint32 ALength) {
      copyStereoBuffer(MOutputs,AInputs,ALength);
      scaleStereoBuffer(MOutputs,MGain,ALength);
      copyStereoBuffer(AOutputs,MOutputs,ALength);
    }

    //virtual
    void on_control(uint32 AIndex, float AValue, int32 AChannel=-1) {
      switch(AIndex) {
        case 0:
          MGain = AValue;
          break;
      }
    }



};

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin
{

  private:

    myEffect MEffect;

  public:

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    //virtual
    void on_create(void) {
      MInfo.name       = "test_channel";
      MInfo.author     = "skei";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      //
      #ifdef SKEI_PLUGIN_HAS_EDITOR
      MEditorRect = SRect(640,480);
      #endif
      //
      appendParameter( new SParameter("Gain",1.0f) );

    }

    //------------------------------------------------------------
    // events
    //------------------------------------------------------------

    //virtual
    void on_stateChange(uint32 AState) {
      switch(AState) {
        case sps_sampleRate:
          MEffect.on_setSampleRate(MState.sampleRate);
          break;
        case sps_blockSize:
          MEffect.on_setBlockSize(MState.blockSize);
          break;
      }
    }

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      MEffect.on_control(AIndex,AValue);
    }

    //------------------------------------------------------------
    // audio
    //------------------------------------------------------------

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
      MEffect.on_processBlock(AInputs,AOutputs,ANumSamples);
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif

