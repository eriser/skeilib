#ifndef template_included
#define template_included
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

#ifdef SKEI_PLUGIN_HAS_EDITOR
  #include "skei_editor.h"
#endif

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
      MInfo.name       = "template";
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
      /*
      switch(AState) {
        case sps_open: break;
        case sps_close: break;
        case sps_sampleRate: break;
        case sps_blockSize: break;
        case sps_suspend: break;
        case sps_resume: break;
        case sps_start: break;
        case sps_stop: break;
        case sps_bypass: break;
        case sps_bypassOff: break;
      }
      */
    }

    //virtual
    void on_transportChange(uint32 ATransport) {
      /*
      if (ATransport & spt_changed) {}  // play, cycle or record state has changed
      if (ATransport & spt_play) {}     // Host sequencer is currently playing
      if (ATransport & spt_cycle) {}    // Host sequencer is in cycle mode
      if (ATransport & spt_record) {}   // Host sequencer is in record mode
      if (ATransport & spt_awrite) {}   // automation write mode active (record parameter changes)
      if (ATransport & spt_aread) {}    // automation read mode active (play parameter changes)
      */
    }

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      /*
      switch(AIndex) {
        case 0: break;
      }
      */
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
      //SMemcpy(AOutputs[0],AInputs[0],ANumSamples*sizeof(SSample));
      //SMemcpy(AOutputs[1],AInputs[1],ANumSamples*sizeof(SSample));
    }

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {
      //float spl0 = *AInputs[0];
      //float spl1 = *AInputs[1];
      //*AOutputs[0] = spl0;
      //*AOutputs[1] = spl1;
    }

    //virtual
    void on_postProcess(void) {
    }

    //------------------------------------------------------------
    // editor
    //------------------------------------------------------------

    #ifdef SKEI_PLUGIN_HAS_EDITOR

    //virtual
    void* on_openEditor(void* AParent) {
      SEditor* editor = new SEditor(this,MEditorRect.w,MEditorRect.h,AParent);
      //editor->fillColor( SLightGrey );
      //editor->fillBackground(true);
      return editor;
    }

    //virtual
    void on_closeEditor(void* AEditor) {
      SEditor* editor = (SEditor*)AEditor;
      delete editor;
    }

    //virtual
    void on_idleEditor(void* AEditor) {
      SEditor* editor = (SEditor*)AEditor;
      if ((editor) && (MEditorIsOpen)) {
        //editor->do_redraw(..);
      }
    }

    #endif // SKEI_PLUGIN_HAS_EDITOR

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif
