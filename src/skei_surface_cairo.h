#ifndef skei_surface_cairo_included
#define skei_surface_cairo_included
//----------------------------------------------------------------------

/*
#ifdef SKEI_LINUX
  #include <X11/X.h>
  #include <X11/Xlib.h>
  #include <cairo/cairo-xlib.h>
  //#include <cairo/cairo-xrender.h>
#endif
*/

/*
#ifdef SKEI_WIN32
  Windows
  CairoWin32
#endif
*/

#include <cairo/cairo.h>

//#include <X11/Xutil.h>
//#include "skei_xlib.h"
//#ifdef SKEI_XRENDER
  //#include <X11/extensions/Xrender.h>
  //#include "skei_xrender.h"
//#endif



#include "skei_drawable.h"
#include "skei_surface_base.h"

//----------------------------------------------------------------------

class SSurface_Cairo;
typedef SSurface_Cairo SSurface_Implementation;

//----------

class SSurface_Cairo
: public SSurface_Base
, public SDrawable {

  private:

    Display*  MDisplay;
    //Pixmap    MPixmap;
    //#ifdef SKEI_XRENDER
    //Picture   MPicture;
    //#endif
    int32     MWidth;
    int32     MHeight;
    int32     MDepth;

    cairo_surface_t*  MCairoSurface;


  //----------------------------------------
  //
  //----------------------------------------

  public:

    SSurface_Cairo(SDrawable* ADrawable, int32 AWidth, int32 AHeight)
    : SSurface_Base(ADrawable,AWidth,AHeight) {
      MDisplay  = ADrawable->display();
      MWidth    = AWidth;
      MHeight   = AHeight;
      MDepth    = ADrawable->depth(); //ADepth;
      //MCairoSurface = cairo_surface_create_similar(sc._surface,CAIRO_CONTENT_COLOR_ALPHA,AWidth,AHeight);
      //cairo_xlib_surface_set_size(sfc, x, y)
      int32   screen = XDefaultScreen(MDisplay);
      Visual* visual = XDefaultVisual(MDisplay,screen);
      //MCairoSurface = cairo_xlib_surface_create(MDisplay,ADrawable->drawable(),DefaultVisual(MDisplay,MScreen),AWidth,AHeight);
      MCairoSurface = cairo_xlib_surface_create(MDisplay,ADrawable->drawable(),visual,AWidth,AHeight);
    }

    //----------

    SSurface_Cairo(SDrawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth)
    : SSurface_Base(ADrawable,AWidth,AHeight,ADepth) {
      MDisplay  = ADrawable->display();
      MWidth    = AWidth;
      MHeight   = AHeight;
      MDepth    = ADepth;
      cairo_format_t fmt;
      switch(ADepth) {
        case 1:   fmt = CAIRO_FORMAT_A1; break;
        case 8:   fmt = CAIRO_FORMAT_A8; break;
        case 24:  fmt = CAIRO_FORMAT_RGB24; break;
        case 32:  fmt = CAIRO_FORMAT_ARGB32; break;
      }
      MCairoSurface = cairo_image_surface_create(fmt,AWidth,AHeight);
    }

    //----------

    virtual ~SSurface_Cairo() {
      cairo_surface_destroy(MCairoSurface);
    }

  //----------------------------------------
  // drawable
  //----------------------------------------

  public:
    Display*  display(void)   { return MDisplay; }
    //Drawable  drawable(void)  { return MPixmap; }
    //#ifdef SKEI_XRENDER
    //Picture   picture(void)   { return MPicture; }
    //#endif
    int32     width(void)     { return MWidth; }
    int32     height(void)    { return MHeight; }
    int32     depth(void)     { return MDepth; }

  //----------------------------------------
  //
  //----------------------------------------

//----------------------------------------------------------------------
// linux
//----------------------------------------------------------------------

/*

{$ifdef KODE_LINUX}

{
  Creates an Xlib surface that draws to the given drawable. The way that colors
  are represented in the drawable is specified by the provided visual.

  Note: If drawable is a Window, then the function
  cairo_xlib_surface_set_size() must be called whenever the size of the
  window changes.

  drawable: an X Drawable, (a Pixmap or a Window)
  visual: the visual to use for drawing to drawable. The depth of the visual
  must match the depth of the drawable. Currently, only TrueColor visuals are
  fully supported.
}

constructor KSurface_Cairo.create(ADisplay:PDisplay; ADrawable:TDrawable; AVisual:PVisual; AWidth,AHeight:longint);
begin
  inherited create;
  FSurface := cairo_xlib_surface_create(ADisplay,ADrawable,AVisual,AWidth,AHeight);
  FWidth := AWidth;
  FHeight := AHeight;
  //FDepth := 0;
end;

//----------

{
  Creates an Xlib surface that draws to the given bitmap. This will be drawn to
  as a CAIRO_FORMAT_A1 object.
  pixmap : an X Drawable, (a depth-1 Pixmap)
}

//constructor KSurface_Cairo.create(ADisplay:PDisplay; APixmap:TPixmap; AScreen:PScreen; AWidth,AHeight:longint);
//begin
//  inherited create;
//  cairo_xlib_surface_create_for_bitmap(ADisplay,APixmap,AScreen,AWidth,AHeight);
//end;

//----------

{
  Informs cairo of the new size of the X Drawable underlying the surface. For
  a surface created for a Window (rather than a Pixmap), this function must be
  called each time the size of the window changes. (For a subwindow, you are
  normally resizing the window yourself, but for a toplevel window, it is
  necessary to listen for ConfigureNotify events.)
  A Pixmap can never change size, so it is never necessary to call this
  function on a surface created for a Pixmap.
}

procedure KSurface_Cairo.setSize(AWidth,AHeight:longint);
begin
  cairo_xlib_surface_set_size(FSurface,AWidth,AHeight);
  FWidth := AWidth;
  FHeight := AHeight;
end;

//----------

{
  Informs cairo of a new X Drawable underlying the surface. The drawable must
  match the display, screen and format of the existing drawable or the
  application will get X protocol errors and will probably terminate. No
  checks are done by this function to ensure this compatibility.
}

// void cairo_xlib_surface_set_drawable(cairo_surface_t *surface, Drawable drawable, int width, int height);

//----------

{
  Get the underlying X Drawable used for the surface.
}

// Drawable cairo_xlib_surface_get_drawable(cairo_surface_t *surface);

//----------

{
  Get the X Display for the underlying X Drawable.
}

// Display* cairo_xlib_surface_get_display(cairo_surface_t *surface);

//----------

{
  Get the X Screen for the underlying X Drawable.
}

// Screen* cairo_xlib_surface_get_screen(cairo_surface_t *surface);

//----------

{
  Gets the X Visual associated with surface, suitable for use with the
  underlying X Drawable. If surface was created by cairo_xlib_surface_create(),
  the return value is the Visual passed to that constructor.
  Returns: the Visual or NULL if there is no appropriate Visual for surface.
}

// Visual* cairo_xlib_surface_get_visual(cairo_surface_t *surface);

//----------

{
  Get the width of the X Drawable underlying the surface in pixels.
}

// int cairo_xlib_surface_get_width(cairo_surface_t *surface);

//----------

{
  Get the height of the X Drawable underlying the surface in pixels.
}

// int cairo_xlib_surface_get_height(cairo_surface_t *surface);

//----------

{
  Get the number of bits used to represent each pixel value.
}

// int cairo_xlib_surface_get_depth(cairo_surface_t *surface);

//----------

{$endif} // KODE_LINUX

//----------------------------------------------------------------------
// win32
//----------------------------------------------------------------------

{$ifdef KODE_WIN32}

{
  Creates a cairo surface that targets the given DC. The DC will be queried for
  its initial clip extents, and this will be used as the size of the cairo
  surface. The resulting surface will always be of format CAIRO_FORMAT_RGB24
}

constructor KSurface_Cairo.create(hdc:HDC);
begin
  inherited create;
  FSurface := cairo_win32_surface_create(hdc);
  //FWidth := 0;
  //FHeight := 0;
  //FDepth := 0; // 24
end;

//----------

{
  Creates a device-independent-bitmap surface not associated with any particular
  existing surface or device context. The created bitmap will be uninitialized.
}

constructor KSurface_Cairo.create(hdc:HDC; AWidth,AHeight,ADepth:LongInt);
var
  fmt : cairo_format_t;
begin
  inherited create;
  case ADepth of
     1: fmt := CAIRO_FORMAT_A1;
     8: fmt := CAIRO_FORMAT_A8;
    24: fmt := CAIRO_FORMAT_RGB24;
    32: fmt := CAIRO_FORMAT_ARGB32;
  end;
  FSurface := cairo_win32_surface_create_with_ddb(hdc,fmt,AWidth,AHeight);
  FWidth := AWidth;
  FHeight := AHeight;
  FDepth := ADepth;
end;

//----------

{
  Creates a device-independent-bitmap surface not associated with any particular
  existing surface or device context. The created bitmap will be uninitialized.
}

constructor KSurface_Cairo.create(AWidth,AHeight,ADepth:LongInt);
var
  fmt : cairo_format_t;
begin
  inherited create;
  case ADepth of
     1: fmt := CAIRO_FORMAT_A1;
     8: fmt := CAIRO_FORMAT_A8;
    24: fmt := CAIRO_FORMAT_RGB24;
    32: fmt := CAIRO_FORMAT_ARGB32;
  end;
  FSurface := cairo_win32_surface_create_with_dib(fmt,AWidth,AHeight);
  FWidth := AWidth;
  FHeight := AHeight;
  FDepth := ADepth;
end;

//------------------------------

destructor KSurface_Cairo.destroy;
begin
  cairo_surface_destroy(FSurface);
  inherited;
end;

//------------------------------

procedure KSurface_Cairo.setSize(AWidth,AHeight:longint);
begin
  FWidth := AWidth;
  FHeight := AHeight;
end;

{$endif} // KODE_WIN32

*/

};

//----------------------------------------------------------------------
#endif

