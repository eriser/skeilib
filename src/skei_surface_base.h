#ifndef skei_surface_base_included
#define skei_surface_base_included
//----------------------------------------------------------------------

#include "skei_drawable.h"

class SSurface_Base {
  public:
    SSurface_Base(SDrawable* ADrawable, int32 AWidth, int32 AHeight) {}
    SSurface_Base(SDrawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth) {}
    virtual ~SSurface_Base() {}
};

//----------------------------------------------------------------------
#endif

