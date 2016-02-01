#ifndef skei_image_included
#define skei_image_included
//----------------------------------------------------------------------

#include "skei_bitmap.h"



// 0xaarrggbb

#ifdef SKEI_LINUX
#include "skei_image_xlib.h"
#endif // SKEI_LINUX

#ifdef SKEI_WIN32
#include "skei_image_win32.h"
#endif // SKEI_LINUX

//----------

//typedef SImage_Implementation SImage;


class SImage
: public SImage_Implementation {

  public:

    SImage(SDrawable* ADrawable, SBitmap* ABitmap, int32 ADepth)
    : SImage_Implementation(ADrawable,ABitmap,ADepth) {
    }


};

//----------------------------------------------------------------------
#endif

