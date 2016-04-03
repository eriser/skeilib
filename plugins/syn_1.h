#ifndef syn_1_included
#define syn_1_included
//----------------------------------------------------------------------

// nc -U -l -k /tmp/skei.sock

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//----- plugins -----

#define SKEI_PLUGIN_HAS_EDITOR
#define SKEI_PLUGIN_IS_SYNTH
#define SKEI_PLUGIN_RECEIVE_MIDI
//#define SKEI_PLUGIN_SEND_MIDI
//#define SKEI_PLUGIN_PER_SAMPLE
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
#include "syn_1/syn_1_voice.h"
#include "syn_1/skei_voice_manager2.h"


#ifdef SKEI_PLUGIN_HAS_EDITOR
  #include "skei_editor.h"
  #include "syn_1/syn_1_editor.h"
#endif

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

typedef SVoiceManager2<syn_1_voice,16> syn_1_voice_manager;

//----------

class syn_1
: public SPlugin
{

  private:

    syn_1_voice_manager MVoiceManager;

  public:

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    //virtual
    void on_create(void) {
      MInfo.name       = "syn_1";
      MInfo.author     = "skei";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      //
      MVoiceManager.on_create();
      //
      #ifdef SKEI_PLUGIN_HAS_EDITOR
      MEditorRect = SRect(640,480);
      #endif
      //
    }

    //----------

    //virtual
    void on_destroy(void) {
      MVoiceManager.on_destroy();
    }

    //------------------------------------------------------------
    // events
    //------------------------------------------------------------

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      MVoiceManager.on_parameterChange(AIndex,AValue);
    }

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      MVoiceManager.on_midiEvent(AOffset,AMsg1,AMsg2,AMsg3);
    }

    //------------------------------------------------------------
    // audio
    //------------------------------------------------------------

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
      MVoiceManager.on_processBlock(AInputs,AOutputs,ANumSamples);
    }

    //virtual
    void on_postProcess(void) {
      MVoiceManager.on_postProcess();
    }

    //------------------------------------------------------------
    // editor
    //------------------------------------------------------------

    #ifdef SKEI_PLUGIN_HAS_EDITOR

    //virtual
    void* on_openEditor(void* AParent) {
      syn_1_editor* editor = new syn_1_editor(this,MEditorRect.w,MEditorRect.h,AParent);
      return editor;
    }

    //virtual
    void on_closeEditor(void* AEditor) {
      syn_1_editor* editor = (syn_1_editor*)AEditor;
      delete editor;
    }

    //virtual
    void on_idleEditor(void* AEditor) {
      syn_1_editor* editor = (syn_1_editor*)AEditor;
      if ((editor) && (MEditorIsOpen)) {
        //editor->do_redraw(..);
      }
    }

    #endif // SKEI_PLUGIN_HAS_EDITOR

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(syn_1);

//----------------------------------------------------------------------
#endif

