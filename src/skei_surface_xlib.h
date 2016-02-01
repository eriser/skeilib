#ifndef skei_surface_xlib_included
#define skei_surface_xlib_included
//----------------------------------------------------------------------

/*
  pixmap

  In contrast to windows, where drawing has no effect if the window is not
  visible, a pixmap can be drawn to at any time because it resides in memory.
*/


/*
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include "skei_xlib.h"

#ifdef SKEI_XRENDER
#include <X11/extensions/Xrender.h>
#include "skei_xrender.h"
#endif
*/

#include "skei_drawable.h"
#include "skei_surface_base.h"

//----------

/*
#ifdef SKEI_XRENDER

static XRenderPictFormat argb_templ = {
  0,              // id
  PictTypeDirect, // type
  32,             // depth
  {               // direct
    16,           // direct.red
    0xff,         // direct.redMask
    8,            // direct.green
    0xff,         // direct.greenMask
    0,            // direct.blue
    0xff,         // direct.blueMask
    24,           // direct.alpha
    0xff,         // direct.alphaMask
  },
  0,              // colormap
};

static uint32 argb_templ_mask =
  PictFormatType      |
  PictFormatDepth     |
  PictFormatRed       |
  PictFormatRedMask   |
  PictFormatGreen     |
  PictFormatGreenMask |
  PictFormatBlue      |
  PictFormatBlueMask  |
  PictFormatAlpha     |
  PictFormatAlphaMask;

#endif
*/

//----------------------------------------------------------------------

class SSurface_Xlib;
typedef SSurface_Xlib SSurface_Implementation;

//----------

class SSurface_Xlib
: public SSurface_Base
, public SDrawable  {

  private:

    Display*  MDisplay;
    Pixmap    MPixmap;
    int32     MWidth;
    int32     MHeight;
    int32     MDepth;
    #ifdef SKEI_XRENDER
    Picture   MPicture;
    #endif

  //----------------------------------------
  //
  //----------------------------------------

  public:

    /*
      we can create the pixmap from the rootwindow drawable..
      DefaultRootWindow(FDisplay);
      DefaultDepthOfScreen
      (but we need the display pointer later)
    */

    SSurface_Xlib(SDrawable* ADrawable, int32 AWidth, int32 AHeight)
    : SSurface_Base(ADrawable,AWidth,AHeight) {

      MDisplay  = ADrawable->display();
      MWidth    = AWidth;
      MHeight   = AHeight;
      MDepth    = ADrawable->depth(); //ADepth;

      //STrace("surface depth %i\n",MDepth); // prints 24

      MPixmap = xlib_create_pixmap(MDisplay,ADrawable->drawable(),MWidth,MHeight,MDepth);
      #ifdef SKEI_XRENDER
      MPicture = xrender_create_picture(MDisplay,MPixmap,MDepth);
      #endif
    }

    //----------

    SSurface_Xlib(SDrawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth)
    : SSurface_Base(ADrawable,AWidth,AHeight,ADepth) {
      MDisplay  = ADrawable->display();
      MWidth    = AWidth;
      MHeight   = AHeight;
      MDepth    = ADepth;
      MPixmap = xlib_create_pixmap(MDisplay,ADrawable->drawable(),MWidth,MHeight,MDepth);
      #ifdef SKEI_XRENDER
      MPicture = xrender_create_picture(MDisplay,MPixmap,MDepth);
      #endif
    }

    //----------

    virtual ~SSurface_Xlib() {
      xlib_delete_pixmap(MDisplay,MPixmap);
      #ifdef SKEI_XRENDER
      xrender_delete_picture(MDisplay,MPicture);
      #endif
    }

  //----------------------------------------
  // drawable
  //----------------------------------------

  public:
    Display*  display(void)   { return MDisplay; }
    Drawable  drawable(void)  { return MPixmap; }
    int32     width(void)     { return MWidth; }
    int32     height(void)    { return MHeight; }
    int32     depth(void)     { return MDepth; }
    #ifdef SKEI_XRENDER
    Picture   picture(void)   { return MPicture; }
    #endif

  //----------------------------------------
  // internal
  //----------------------------------------

  private:

    #if 0

    /*
      The server uses the specified drawable to determine on which screen
      to create the pixmap. The pixmap can be used only on this screen and
      only with other drawables of the same depth (see XCopyPlane for an
      exception to this rule).
    */

    Pixmap create_pixmap(Drawable ADrawable) {
      return XCreatePixmap(MDisplay,ADrawable,MWidth,MHeight,MDepth);
    }

    //----------

    /*
      The XFreePixmap() function first deletes the association between the pixmap ID and the pixmap.
      Then, the X server frees the pixmap storage when there are no references to it.
    */

    //void delete_pixmap(void) {
    //  XFreePixmap(MDisplay,MPixmap);
    //}

    void delete_pixmap(Pixmap APixmap) {
      XFreePixmap(MDisplay,APixmap);
    }


    //----------

    #ifdef SKEI_XRENDER

    /*
      http://www.winehq.org/pipermail/wine-patches/2005-August/020119.html
      Avoid using XRenderFindStandardFormat as older libraries don't have it
    */

    XRenderPictFormat* find_format(Display* ADisplay, int32 ADepth) {
      //case ADepth of
      //  24: result := XRenderFindStandardFormat(FDisplay,PictStandardRGB24);
      //  32: result := XRenderFindStandardFormat(FDisplay,PictStandardRGB32);
      //end;
      switch(ADepth) {
        case 24: argb_templ.depth = 24; break;
        case 32: argb_templ.depth = 32; break;
      }
      return XRenderFindFormat(MDisplay, argb_templ_mask, &argb_templ, 0);
    }

    //----------

    //void create_picture(Drawable ADrawable) {
    Picture create_picture(Drawable ADrawable) {
      XRenderPictFormat*        fmt;
      XRenderPictureAttributes  pict_attr;
      int32                     pict_bits;
      /*
      if FDepth = 24 then  fmt := find_format(FDisplay,24{PictStandardRGB24})
      else fmt := find_format(FDisplay,32{PictStandardARGB32});
      */
      if (MDepth == 24) fmt = XRenderFindStandardFormat(MDisplay,PictStandardRGB24);
      else fmt = XRenderFindStandardFormat(MDisplay,PictStandardARGB32);
      pict_attr.poly_edge = PolyEdgeSmooth;
      pict_attr.poly_mode = PolyModeImprecise;
      /*
      pict_attr.component_alpha = true;
      */
      pict_bits = /*CPComponentAlpha |*/ CPPolyEdge | CPPolyMode;
      return  XRenderCreatePicture(
        MDisplay,
        ADrawable,//MPixmap,
        fmt,
        pict_bits,
        &pict_attr
      );
    }

    //----------

    void delete_picture(Picture APicture) {
      XRenderFreePicture(MDisplay,APicture);
    }

    #endif // SKEI_XRENDER

    #endif // 0

};

//----------------------------------------------------------------------
#endif
