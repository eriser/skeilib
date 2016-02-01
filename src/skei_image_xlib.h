#ifndef skei_image_xlib_included
#define skei_image_xlib_included
//----------------------------------------------------------------------

#include "skei_image_base.h"
#include "skei_bitmap.h"

class SImage_Xlib;
typedef SImage_Xlib SImage_Implementation;

//----------

class SImage_Xlib
: public SImage_Base {

  private:
    Display*  MDisplay;
    XImage*   MImage;
    int32    MDepth;
    SBitmap*  MBitmap;

  public:

    int32  width(void)   { return MBitmap->width(); }
    int32  height(void)  { return MBitmap->height(); }
    //uint32  depth(void)   { return MBitmap->depth(); }
    uint32* buffer(void)  { return MBitmap->buffer(); }

  public:

    /*
      //http://tronche.com/gui/x/xlib/utilities/XCreateImage.html
      does not allocate space for the image itself
      initializes the structure byte-order, bit-order, and bitmap-unit values from the display

      //http://www.sbin.org/doc/Xlib/chapt_06.html
      XCreateImage()
      Allocates memory for an XImage structure and sets various members.
      Note that it uses the server's data format, which is often not appropriate.
      The byte- and bit-order fields should usually be changed directly to the client-native format.
      However, then the call _XInitImageFuncPtrs(image) should be issued to reset the mapping
      to the appropriate versions of the functions for manipulating the image.
      This call is supposed to be private to Xlib and, therefore, should be watched for changes
      in later releases, but this is currently the accepted method.
    */

    //SImage_Xlib(SDrawable* ADrawable, SBitmap* ABitmap)
    SImage_Xlib(SDrawable* ADrawable, SBitmap* ABitmap, int32 ADepth)
    : SImage_Base(ADrawable,ABitmap) {
      MDisplay = ADrawable->display();
      MBitmap = ABitmap;
      //MDepth  = ADrawable->depth(); // 32; // DefaultDepth(MDisplay,DefaultScreen(MDisplay));
      MDepth = ADepth;
      MImage = XCreateImage(
        MDisplay,
        DefaultVisual(MDisplay,DefaultScreen(MDisplay)),  //CopyFromParent // visual
        MDepth,                   // depth
        ZPixmap,                  // format
        0,                        // offset
        (char*)MBitmap->buffer(), // data
        MBitmap->width(),
        MBitmap->height(),        // size
        32,                       // pad
        0                         // bytes per line
      );
    }

    //----------

    /*
      //http://www.sbin.org/doc/Xlib/chapt_06.html
      XDestroyImage()
      Frees the data field in an image structure if the image structure was allocated in the application.
      If the image was created using XCreateImage(), XGetImage(), or XGetSubImage(),
      XDestroyImage() frees both the data and the image structure. Note that if the image data is stored
      in static memory in the application, it cannot be freed
      --to free an image created with XCreateImage() that has statically allocated data, you must set NULL
      nto the data field before calling XDestroyImage().
    */

    virtual ~SImage_Xlib() {
      MImage->data = SKEI_NULL;   // we want to delete it ourselves...
      XDestroyImage(MImage);      // frees data too
    }

    //----------

    XImage* image(void) { return MImage; }

};

//----------------------------------------------------------------------
#endif
