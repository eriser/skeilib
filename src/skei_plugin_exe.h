#ifndef skei_plugin_exe_included
#define skei_plugin_exe_included
//----------------------------------------------------------------------

/*
  SPlugin_Base
  SPlugin_Parameters
  SPlugin_Programs
  SPlugin_Editor
> SPlugin_Implementation
  SPlugin

  todo: idle thread -> timer..

*/

//#include "skei_plugin_editor.h"
#include "skei_plugin_base.h"

#ifdef SKEI_PLUGIN_HAS_EDITOR
#include "skei_editor.h"
#endif


//----------

class SPlugin_Exe;
typedef SPlugin_Exe SPlugin_Implementation;

//----------

class SPlugin_Exe
: public SPlugin_Base
//: public SPlugin_Editor
{

  public:

    SPlugin_Exe()
    //: SPlugin_Editor() {
    : SPlugin_Base() {
    }

    virtual ~SPlugin_Exe() {
    }

  //----------------------------------------
  // SPlugin_Base
  //----------------------------------------

  public:

    //virtual
    void* main(void* APointer=SKEI_NULL) {
      #ifdef SKEI_PLUGIN_HAS_EDITOR

      SEditor* editor = (SEditor*)on_openEditor(SKEI_NULL);
      if (editor) {
        MEditor = editor;
        setDefaultParameterValues();

        //updateEditorInIdle();
        #ifdef SKEI_LINUX_IDLE_THREAD
        //idleFromEditor(); // hack? initial parameter setup..
        on_idleEditor(MEditor);
        updateEditorInIdle();
        #endif

        editor->on_align();
        editor->paintBuffer();
        editor->open();
        editor->eventLoop();
        editor->close();
        on_closeEditor(editor);
      } // editor
      #endif
      return SKEI_NULL;
    }

};

//----------------------------------------------------------------------
#endif

