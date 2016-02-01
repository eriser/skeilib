#ifndef _included
#define _included
//----------------------------------------------------------------------

/*

*/

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//----- format -----

//#define SKEI_EXE
//#define SKEI_XLIB
//#define SKEI_XRENDER

//----- debug -----

//#define SKEI_DEBUG
//#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_MEM_PRINT
//#define SKEI_DEBUG_VST

#ifdef SKEI_VST
  #define SKEI_DEBUG_SOCKET
#endif

//----- plugins -----

#define SKEI_PLUGIN_PER_SAMPLE
//#define SKEI_PLUGIN_HAS_EDITOR
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_SEND_MIDI
//#define SKEI_PLUGIN_RECEIVE_MIDI
//#define SKEI_PLUGIN_AUTOSYNC

//----------------------------------------------------------------------

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin
{

  private:

  public:

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    //virtual
    void on_create(void) {
      MInfo.name       = "";
      MInfo.author     = "";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      //
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
    }

    //virtual
    void on_transportChange(uint32 ATransport) {
    }

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
    }

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
    }

    //virtual
    void on_programChange(int32 AIndex) {
    }

    //virtual
    void on_preProgram(int32 AIndex) {
    }

    //virtual
    void on_postProgram(void) {
    }

    //------------------------------------------------------------
    // audio
    //------------------------------------------------------------

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
    }

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {
      float spl0 = *AInputs[0];
      float spl1 = *AInputs[1];
      *AOutputs[0] = spl0;
      *AOutputs[1] = spl1;
    }

    //virtual
    void on_postProcess(void) {
    }

    //------------------------------------------------------------
    // editor
    //------------------------------------------------------------

    //virtual
    void* on_openEditor(void* AParent) {
      return SKEI_NULL;
    }

    //virtual
    void on_closeEditor(void* AEditor) {
    }

    //virtual
    void on_idleEditor(void* AEditor) {
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif



