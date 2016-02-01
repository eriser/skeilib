#ifndef skei_plugin_listener_included
#define skei_plugin_listener_included
//----------------------------------------------------------------------


/*
// plugin -> editor communication
  the part of an editor that the plugin knows about
  what the plugin can ask from the editor
*/

#include "skei_parameter.h"


class SPlugin_Listener {
public:
  //virtual void updateParameterFromPlugin(int32 AIndex, float AValue) {}
  virtual void updateParameterFromPlugin(SParameter* AParameter, float AValue) {}
};

//----------------------------------------------------------------------
#endif
