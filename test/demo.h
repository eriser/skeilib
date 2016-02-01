
//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//----- config -----

//#define SKEI_EXE
//#define SKEI_VST

#define SKEI_XLIB
#define SKEI_XRENDER
#define SKEI_XFT
//#define SKEI_CAIRO

//----- debug -----

//#define SKEI_DEBUG
#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_MEM_PRINT
//#define SKEI_DEBUG_VST

#ifdef SKEI_VST
  #define SKEI_DEBUG_SOCKET
#endif

//----- plugin -----

//#define SKEI_PLUGIN_PER_SAMPLE
#define SKEI_PLUGIN_HAS_EDITOR
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_SEND_MIDI
//#define SKEI_PLUGIN_RECEIVE_MIDI
//#define SKEI_PLUGIN_AUTOSYNC

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"
#include "demo/demo_editor.h"

#include "skei_widget_timeline.h"
#include "skei_widget_graph.h"

//----------------------------------------------------------------------
// (global) data
//----------------------------------------------------------------------

const char* param_txt[] = {"one", "two", "three" };

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin {

  public:

    //virtual
    void on_create(void) {
      MInfo.name          = "skeilib demo";
      MInfo.author        = "skei";
      MInfo.product       = MInfo.name;
      MInfo.uniqueId      = SKEI_MAGIC + 0x0000;
      MInfo.numInputs     = 2;
      MInfo.numOutputs    = 2;
      MEditorRect         = SRect(640,480);
      appendParameter( new SParameter(  "param",  0 ));
      appendParameter( new SParamFloat( "float",  0, -5, 5, 0.25 ));
      appendParameter( new SParamInt(   "int",    0,  0, 100 ));
      appendParameter( new SParamText(  "txt",    0,  3, param_txt ));
    }

    //----------

    //virtual
    void on_destroy(void) {
    }

    //----------------------------------------
    // editor
    //----------------------------------------

    //virtual
    void* on_openEditor(void* AParent) {
      myEditor* editor = new myEditor(this,MEditorRect.w,MEditorRect.h,AParent);

      STrace("myEditor->parent() = %x\n",editor->parent());

      // connect parameters
      //for (int i=0; i<9; i++) connect( (SWidget*)subWidget(i), parameter(i) );
      //connect( (SWidget*)panel1->subWidget(6), parameter(1) );
      return editor;
    }

    //----------

    //virtual
    void on_closeEditor(void* AEditor) {
      myEditor* editor = (myEditor*)AEditor;
      delete editor;
    }

    //----------

    //virtual
    void on_idleEditor(void* AEditor) {
      //myEditor* editor = (myEditor*)AEditor;
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);
