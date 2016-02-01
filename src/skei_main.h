#ifndef skei_main_included
#define skei_main_included
//----------------------------------------------------------------------

//#ifndef SKEI_MAIN

//----------------------------------------------------------------------
// exe
//----------------------------------------------------------------------

#ifdef SKEI_EXE

  #define SKEI_MAIN(SKEI_PLUGIN)            \
    int main(void) {                        \
      SPlugin* plugin = new SKEI_PLUGIN();  \
      plugin->on_create();                  \
      plugin->main(SKEI_NULL);              \
      plugin->on_destroy();                 \
      delete plugin;                        \
      return 0;                             \
    }

#endif // SKEI_EXE

//----------------------------------------------------------------------
// vst
//----------------------------------------------------------------------

#ifdef SKEI_VST

  // where is dllexport?
  // move these to skei_plugin_vst.h ??
  #include "extern/aeffect.h"
  #include "extern/aeffectx.h"

  //----------

  // VSTPluginMain - not recognized by bitwig studio (yet)
  // will be fixed in next version..

  //#define SKEI_ASM_MAIN_SYMBOL asm ("VSTPluginMain");

  #ifdef SKEI_LINUX
    //#define SKEI_ASM_MAIN_SYMBOL asm ("main");
    #define SKEI_ASM_MAIN_SYMBOL asm ("VSTPluginMain");
  #endif // SKEI_LINUX

  #ifdef SKEI_WIN32
    //#define SKEI_ASM_MAIN_SYMBOL asm ("_main");
    #define SKEI_ASM_MAIN_SYMBOL asm ("_VSTPluginMain");
  #endif // SKEI_WIN32

  //----------

  /*
    TODO:
    * be sure we handle multiple instances properly..
      main() is called for each instance
      (and for the initial enumeration)
  */

  //----------

  //AEffect*  main(audioMasterCallback audioMaster) SKEI_ASM_MAIN_SYMBOL

  AEffect*  main_plugin(audioMasterCallback audioMaster) SKEI_ASM_MAIN_SYMBOL
  #define   main main_plugin


  /*
    the 'plugin' object is deleted in:
    skei_plugin_vst.h
      vst_dispatcher_callback()
      if (opcode==effClose)
  */

  #define SKEI_MAIN(SKEI_PLUGIN)                                \
    AEffect* main(audioMasterCallback audioMaster) {            \
      if (!audioMaster(0,audioMasterVersion,0,0,0,0)) return 0; \
      SPlugin* plugin = new SKEI_PLUGIN();                      \
      plugin->on_create();                                      \
      return (AEffect*)plugin->main((void*)audioMaster);        \
    }

#endif // SKEI_VST

//----------------------------------------------------------------------

//#endif // SKEI_MAIN

//----------------------------------------------------------------------
#endif
