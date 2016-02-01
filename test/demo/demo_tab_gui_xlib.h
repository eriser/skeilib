#ifndef demo_tab_gui_xlib_included
#define demo_tab_gui_xlib_included
//----------------------------------------------------------------------

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include "skei_widget.h"
#include "skei_widget_string.h"
#include "skei_widget_text.h"

//#include "skei_basepath.h"

//----------

class demo_tab_gui_xlib
: public SWidget {

  private:

    char    temp[256];

    SString MScreenInfo;
    SString MVisualInfo;

  public:

    demo_tab_gui_xlib()
    : SWidget( SRect(0), swa_fillClient) {

      /*
        the widget is still being contructed, and has not been appended to
        its parent, so we can't find the owner (yet)..
      */

      SWidget* owner = getOwner();
      STrace("MOwner: %x\n",owner);

      //SWindow_Xlib* window = (SWindow_Xlib*)owner;
      //STrace("  name: %s\n",owner->name());
      //Display* display = window->display();

      Display* display = XOpenDisplay(SKEI_NULL);
        int32 screen = XDefaultScreen(display);
        int32 width = XDisplayWidth(display,screen);
        int32 height = XDisplayHeight(display,screen);
        int32 depth = XDefaultDepth(display,screen);
        Visual* visual = XDefaultVisual(display,screen);
      XCloseDisplay(display);

      MScreenInfo = "screen ";
      MScreenInfo += SIntToString(temp,screen);
      MScreenInfo += " : width ";
      MScreenInfo += SIntToString(temp,width);
      MScreenInfo += " height ";
      MScreenInfo += SIntToString(temp,height);
      MScreenInfo += " depth ";
      MScreenInfo += SIntToString(temp,depth);
      appendWidget( new SWidget_String( SRect(16), &MScreenInfo, swa_fillTop) );

      MVisualInfo = "visual : ";
      switch(visual->c_class) {
        case StaticGray:  MVisualInfo += "StaticGray";  break;
        case StaticColor: MVisualInfo += "StaticColor"; break;
        case TrueColor:   MVisualInfo += "TrueColor";   break;
        case GrayScale:   MVisualInfo += "GrayScale";   break;
        case PseudoColor: MVisualInfo += "PseudoColor"; break;
        case DirectColor: MVisualInfo += "DirectColor"; break;
      }
      MVisualInfo += " mask 0x";
      MVisualInfo += SIntToString(temp,visual->red_mask,10,16);
      MVisualInfo += ",0x";
      MVisualInfo += SIntToString(temp,visual->green_mask,10,16);
      MVisualInfo += ",0x";
      MVisualInfo += SIntToString(temp,visual->blue_mask,10,16);
      MVisualInfo += ", bits ";
      MVisualInfo += SIntToString(temp,visual->bits_per_rgb);
      MVisualInfo += ", map ";
      MVisualInfo += SIntToString(temp,visual->map_entries);
      appendWidget( new SWidget_String( SRect(16), &MVisualInfo, swa_fillTop) );

    }

    //----------

    virtual ~demo_tab_gui_xlib() {
    }

    //----------

};

//----------------------------------------------------------------------
#endif

