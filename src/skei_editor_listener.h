#ifndef skei_editor_listenerincluded
#define skei_editor_listenerincluded
//----------------------------------------------------------------------


/*
  editor-> plugin communication
  the part of a plugin that the editor knows about
  what the editor can ask from the plugin
*/

//----------

class SEditor_Listener {
public:

  #ifdef SKEI_PLUGIN_HAS_EDITOR

  virtual void updateParameterFromEditor(int32 AIndex, float AValue) {}
  virtual void resizeWindowFromEditor(int32 AWidth, int32 AHeight) {}

  //#ifdef SKEI_LINUX_IDLE_THREAD
  // also used in standalone exe (eventLoop)
  virtual void idleFromEditor(void) {}
  //#endif // SKEI_LINUX_IDLE_THREAD

  // hack:
  // window idle timer callback -> plugin..
  //virtual void on_idleEditor(void) {}
  //virtual void updateEditorInIdle(void) {}

  #endif // SKEI_PLUGIN_HAS_EDITOR

};


//----------------------------------------------------------------------
#endif
