#ifndef skei_plugin_included
#define skei_plugin_included
//----------------------------------------------------------------------

/*
  SPlugin_Base
  SPlugin_Parameters
  SPlugin_Programs
  SPlugin_Editor
  SPlugin_Implementation
> SPlugin
*/

#ifdef SKEI_EXE
  #include "skei_plugin_exe.h"
#endif

#ifdef SKEI_VST
  #include "skei_plugin_vst.h"
#endif

//----------

class SPlugin
: public SPlugin_Implementation {

  public:

    SPlugin()
    : SPlugin_Implementation() {
    }

    virtual
    ~SPlugin() {
    }

  //----------------------------------------
  //
  //----------------------------------------

};

//----------------------------------------------------------------------
#endif
