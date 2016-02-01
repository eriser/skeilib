#ifndef skei_surface_included
#define skei_surface_included
//----------------------------------------------------------------------

#ifdef SKEI_LINUX
  //#ifdef SKEI_CAIRO
  //  #include "skei_surface_cairo.h"
  //#else
  //#ifdef SKEI_XLIB
    #include "skei_surface_xlib.h"
  //#endif
#endif // SKEI_LINUX

#ifdef SKEI_WIN32
  #include "skei_surface_win32.h"
#endif // SKEI_LINUX

#include "skei_bitmap.h"
#include "skei_image.h"

//----------

//typedef SSurface_Implementation SSurface;


class SSurface
: public SSurface_Implementation {

  public:

    SSurface(SDrawable* ADrawable, int32 AWidth, int32 AHeight)
    : SSurface_Implementation(ADrawable,AWidth,AHeight) {
    }

    SSurface(SDrawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth)
    : SSurface_Implementation(ADrawable,AWidth,AHeight,ADepth) {
    }

    virtual ~SSurface() {
    }

    //----------

  public:

};

//----------------------------------------------------------------------
#endif
