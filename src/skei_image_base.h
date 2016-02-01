#ifndef skei_image_base_included
#define skei_image_base_included
//----------------------------------------------------------------------

#include "skei_bitmap.h"

class SImage_Base {
  public:
    SImage_Base(SDrawable* ADrawable, SBitmap* ABitmap) {}
    virtual ~SImage_Base() {}
};

//----------------------------------------------------------------------
#endif


