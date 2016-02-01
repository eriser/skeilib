#ifndef skei_xlib_included
#define skei_xlib_included
//----------------------------------------------------------------------

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

/*
#include <pthread.h>
//#include <unistd.h> // sleep
#ifdef SKEI_XRENDER
#include <X11/extensions/Xrender.h>
#endif
*/

//----------------------------------------------------------------------

/*
  * this is called both when enumerating, and when initializing the plugin

*/

//----------------------------------------------------------------------

#ifndef SKEI_LIB

bool need_xinitthreads_call = true;

/*
  this is a bit hack-ish, isn't it?
  maybe we should use a SGlobal?
*/

bool skei_init_xlib() {
  if (need_xinitthreads_call) {
    STrace("calling XInitThreads\n");
    XInitThreads();
  }
  return true;
}

//----------

static bool skei_xlib_initialized = skei_init_xlib();

#endif

//----------------------------------------------------------------------




/*
  The server uses the specified drawable to determine on which screen
  to create the pixmap. The pixmap can be used only on this screen and
  only with other drawables of the same depth (see XCopyPlane for an
  exception to this rule).
*/

Pixmap xlib_create_pixmap(Display* ADisplay, Drawable ADrawable, int32 AWidth, int32 AHeight, int32 ADepth) {
  return XCreatePixmap(ADisplay,ADrawable,AWidth,AHeight,ADepth);
}

//----------

/*
  The XFreePixmap() function first deletes the association between the pixmap ID and the pixmap.
  Then, the X server frees the pixmap storage when there are no references to it.
*/

void xlib_delete_pixmap(Display* ADisplay, Pixmap APixmap) {
  XFreePixmap(ADisplay,APixmap);
}


//----------------------------------------------------------------------
#endif


