#ifndef skei_window_xlib_included
#define skei_window_xlib_included
//----------------------------------------------------------------------

/*
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <pthread.h>
//#include <unistd.h> // sleep

#ifdef SKEI_XRENDER
#include <X11/extensions/Xrender.h>
#endif
*/

#include "skei_convert.h"
#include "skei_drawable.h"
#include "skei_painter.h"
#include "skei_rect.h"
#include "skei_surface.h"
#include "skei_widget.h"
#include "skei_window_base.h"

#include "skei_tablet.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class SWindow_Xlib;
typedef SWindow_Xlib SWindow_Implementation;

//----------

struct skei_xlib_motif_hints {
  uint32  flags;
  uint32  functions;
  uint32  decorations;
  int32   inputMode;
  uint32  status;
};

//----------

// http://tronche.com/gui/x/xlib/appendix/b/

static
uint32 skei_xlib_cursors[20] = {
  XC_arrow,             // smc_default
  XC_arrow,             // smc_Arrow
  XC_center_ptr,        // smc_ArrowUp
  XC_sb_down_arrow,     // smc_ArrowDown
  XC_sb_left_arrow,     // smc_ArrowLeft
  XC_sb_right_arrow,    // smc_ArrowRight
  XC_sb_v_double_arrow, // smc_ArrowUpDown
  XC_sb_h_double_arrow, // smc_ArrowLeftRight
  XC_top_left_corner,   // smc_ArrowDiagLeft
  XC_top_right_corner,  // smc_ArrowDiagRight
  XC_fleur,             // smc_Move
  XC_watch,             // smc_Wait
  XC_clock,             // smc_ArrowWait
  XC_hand1,             // smc_Hand
  XC_hand2,             // smc_Finger
  XC_crosshair,         // smc_Cross
  XC_pencil,            // smc_Pencil
  XC_plus,              // smc_Plus
  XC_question_arrow,    // smc_Question
  XC_xterm              // smc_Ibeam
};

//----------

static
char skei_xlib_blank_cursor[] = { 0,0,0,0,0,0,0,0 };

//----------

#ifdef SKEI_LIB
void* skei_xlib_threadproc(void* AData);
#endif
void* skei_xlib_idleproc(void* AData);
void* skei_xlib_timerproc(void* AData);

//----------------------------------------------------------------------
// window
//----------------------------------------------------------------------

class SWindow_Xlib
: public SWindow_Base
, public SDrawable {

  friend class SWindow;
  friend void* skei_xlib_threadproc(void* AData);
  friend void* skei_xlib_threadproc_sleep(void* AData);
  friend void* skei_xlib_idleproc(void* AData);
  friend void* skei_xlib_timerproc(void* AData);

  private:

    //SLock MLock;

  private:

    Display*            MDisplay;
    Window              MWindow;
    Window              MParentWindow;
    Window              MRootWindow;
    int32               MScreenNum;
    int32               MScreenDepth;
    Visual*             MVisual;
    Colormap            MColormap;
    Pixmap              MEmptyPixmap;
    XColor              MBlack;
    Atom                MDeleteAtom;
    Atom                MCustomAtom;
    Atom                MIdleAtom;
    XExposeEvent        MExposeEvent;   // invalidate
    XClientMessageEvent MSendEvent;     // sendEvent
    XClientMessageEvent MIdleEvent;     // idleEvent
    int32               MWindowCursor;
    pthread_t           MTimerThread;
    bool                MTimerThreadActive;
    int32               MTimerSleep;
    #ifdef SKEI_LIB
    pthread_t           MEventThread;
    bool                MEventThreadActive;
    #endif
    pthread_t           MIdleThread;
    bool                MIdleThreadActive;
    bool                MWindowExposed;
    bool                MWindowMapped;

    #ifdef SKEI_XFT
    //XftFont*      MXftFont;
    //XftDraw*      MXftDraw;
    XftColor      MXftColor;
    bool          font_color_allocated;
    #endif

    #ifdef SKEI_TABLET
    STablet       MTablet;
    bool          tablet_press;
    bool          tablet_press_prev;
    int32         tablet_x_prev;
    int32         tablet_y_prev;
    #endif

  //----------------------------------------
  //
  //----------------------------------------

  public:

    /*
      By convention, the right and bottom edges of the rectangle are
      normally considered exclusive. In other words, the pixel whose
      coordinates are ( right, bottom ) lies immediately outside of the
      rectangle. For example, when RECT is passed to the FillRect function,
      the rectangle is filled up to, but not including, the right column and
      bottom row of pixels. This structure is identical to the RECTL
      structure.

        const char *display = ":0"; // Use NULL for default (environment variable)
        if (!(d_dpy = XOpenDisplay(display))) {
          fprintf(stderr, "Couldn't open X11 display\n");
          return 100;
        }

      ---

      we call XOpenDisplay with a ptr to swamask..
      does xlib copy this, or should we need to save it as a class member?

    */

    SWindow_Xlib(int32 AWidth, int32 AHeight, void* AParent=SKEI_NULL)
    : SWindow_Base(AWidth,AHeight,AParent) {

      MName = "SWindow_XLib";

      MWindowCursor       = -1;
      MTimerThread        = 0;
      MTimerThreadActive  = false;
      MTimerSleep         = 30;
      #ifdef SKEI_LIB
      MEventThread        = 0;
      MEventThreadActive  = false;
      MIdleThreadActive   = false;
      #endif

      //MWindowCreated      = false;
      MWindowExposed      = false;
      MWindowMapped       = false;
      //MWidget         = SKEI_NULL;

      MDisplay    = XOpenDisplay(SKEI_NULL);
      MRootWindow = DefaultRootWindow(MDisplay);

      //if (AParent) MParentWindow = (Window)AParent;
      //else MParentWindow = MRootWindow;

      #ifdef SKEI_LIB
      MParentWindow = (Window)AParent;
      #else
      MParentWindow = MRootWindow;
      #endif

      //STrace("MParentWindow = %i\n",MParentWindow);
      MScreenNum        = XDefaultScreen(MDisplay);
      MVisual           = XDefaultVisual(MDisplay, MScreenNum);
      MColormap         = XCreateColormap(MDisplay, MParentWindow, MVisual, AllocNone); // -> XFreeColors
      MScreenDepth      = XDefaultDepth(MDisplay, MScreenNum);
      uint32 eventmask  =
        ExposureMask        |
        StructureNotifyMask |
        PropertyChangeMask  |
        ButtonPressMask     |
        ButtonReleaseMask   |
        PointerMotionMask   |
        KeyPressMask        |
        KeyReleaseMask      |
        ClientMessage;
      XSetWindowAttributes swa;
      swa.event_mask        = eventmask;  // http://tronche.com/gui/x/xlib/window/attributes/event-and-do-not-propagate.html
      swa.background_pixmap = None;       // http://tronche.com/gui/x/xlib/window/attributes/background.html
      swa.border_pixel      = 0;
      swa.colormap          = MColormap;  // http://tronche.com/gui/x/xlib/window/attributes/colormap.html
      uint32 swamask        = CWEventMask
                            | CWBackPixmap
                            | CWBorderPixel
                            | CWColormap;
      MWindow = XCreateWindow(
        MDisplay,
        MParentWindow,
        MRect.x,  // x
        MRect.y,  // y
        MRect.w,  //MWidth,
        MRect.h,  //MHeight,
        0,
        MScreenDepth,
        InputOutput,
        MVisual,
        swamask,
        &swa
      );
      //SAssert(MWindow != None);
      //STrace("MWindow = %i\n",MWindow);
      #ifdef SKEI_LIB
      removeDecoration();
      #else
      wantQuitEvent();
      #endif
      MEmptyPixmap = XCreateBitmapFromData(MDisplay, MWindow, skei_xlib_blank_cursor, 8, 8);
      MBlack.red = 0;
      MBlack.green = 0;
      MBlack.blue = 0;
      MBlack.flags = (DoRed | DoGreen | DoBlue);
      XAllocColor(MDisplay,XDefaultColormap(MDisplay,0),&MBlack);
      MWindowCursor = -1;

      #ifdef SKEI_XFT
      //initFonts();
      //MXftDraw = XftDrawCreate(
      //  MDisplay,
      //  MWindow,//MDrawable,
      //  DefaultVisual(MDisplay,DefaultScreen(MDisplay)),
      //  DefaultColormap(MDisplay,DefaultScreen(MDisplay))
      //);
      font_color_allocated = false;
      #endif

      #ifdef SKEI_TABLET
      bool tabl = MTablet.load(MDisplay,MWindow);
      STrace("tablet.load %i\n",tabl);
      tablet_press = false;
      tablet_press_prev = false;
      tablet_x_prev = 0;
      tablet_y_prev = 0;
      #endif

    }

    //----------

    /*

      The pthread_join() function suspends execution of the calling thread
      until the target thread terminates, unless the target thread has
      already terminated.

      ---

      The XDestroyWindow() function destroys the specified window as well
      as all of its subwindows.If the window specified by the w argument is
      mapped, it is unmapped automatically.

      ---

      The XCloseDisplay() function closes the connection to the X server for the
      display specified in the Display structure and destroys all windows,
      resource IDs (Window, Font, Pixmap, Colormap, Cursor, and GContext), or
      other resources that the client has created on this display, unless the
      close-down mode of the resource has been changed (see XSetCloseDownMode()).
      Therefore, these windows, resource IDs, and other resources should never be
      referenced again or an error will be generated. Before exiting, you should
      call XCloseDisplay() explicitly so that any pending errors are reported as
      XCloseDisplay() performs a final XSync() operation.
    */

    virtual ~SWindow_Xlib() {

      #ifdef SKEI_TABLET
      MTablet.unload();
      #endif

      #ifdef SKEI_XFT
      if (font_color_allocated)
        XftColorFree(
          MDisplay,
          DefaultVisual(MDisplay,DefaultScreen(MDisplay)),
          DefaultColormap(MDisplay,DefaultScreen(MDisplay)),
          &MXftColor
        );
      //XftDrawDestroy(MXftDraw);
      #endif

      if (MTimerThreadActive) stopTimer();

      //#ifdef SKEI_LIB
      //if (MEventThreadActive || MIdleThreadActive) stopThreads();
      //#else
      //if (MIdleThreadActive) stopThreads();
      //#endif

      stopThreads();



      XFreePixmap(MDisplay,MEmptyPixmap);
      if (MWindowCursor >= 0) XFreeCursor(MDisplay,MWindowCursor);
      XDestroyWindow(MDisplay,MWindow);
      //XFlush(MDisplay);
      XCloseDisplay(MDisplay);


    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    //void create(SWidget_Listener* AListener, uint32 AWidth, uint32 AHeight, Window AParent=0) {
    //}

    //----------

    //virtual
    //void destroy(void) {
    //}

  //--------------------------------------------------
  // SDrawable
  //--------------------------------------------------

  public:

    Display*  display(void)   { return MDisplay; }
    Drawable  drawable(void)  { return MWindow; }
    int32     width(void)     { return MRect.w; }
    int32     height(void)    { return MRect.h; }
    int32     depth(void)     { return MScreenDepth; }

  //----------------------------------------
  // internal
  //----------------------------------------

  private:

    #ifdef SKEI_TABLET
    void tabletHandler(void) {
      int32   x = MTablet.xpos();
      int32   y = MTablet.ypos();
      float   z = MTablet.pressure();
      on_tabletEvent(x,y,z);
    }
    #endif

    //----------

    void eventHandler(XEvent* AEvent) {

      //MLock.lock();

      //int32 x, y, b, s, k; // w, h
      //int32 w,h;
      //SRect rec;
      switch(AEvent->type) {

        case ButtonPress: {
          int32 x = AEvent->xbutton.x;
          int32 y = AEvent->xbutton.y;
          int32 b = remapButton(AEvent->xbutton.button);
          int32 s = remapKey(AEvent->xbutton.state);
          /*if (MListener) MListener->*/on_mouseDown(x, y, b, s);
        }
        break;

        case ButtonRelease: {
          int32 x = AEvent->xbutton.x;
          int32 y = AEvent->xbutton.y;
          int32 b = remapButton(AEvent->xbutton.button);
          int32 s = remapKey(AEvent->xbutton.state);
          /*if (MListener) MListener->*/on_mouseUp(x, y, b, s);
        }
        break;

        case ClientMessage: {
          int32 i = AEvent->xclient.data.l[0];
          switch(i) {
            case sts_timer:
              /*if (MWindowMapped && MWindowExposed)*/
              on_timer();
              break;
            case sts_idle:
              //STrace("sts_idle\n");
              if (MWindowMapped && MWindowExposed) on_idle();  // on_idle = in widget
              break;
            //case sts_kill:
          }
        }
        break;

        // we don't get an ConfigureNotify event in a vst (ext2)

        case ConfigureNotify: {
          //STrace("--- ConfigureNotify\n");
          //int32 x = AEvent->xconfigure.x;
          //int32 y = AEvent->xconfigure.y;
          int32 w = AEvent->xconfigure.width;
          int32 h = AEvent->xconfigure.height;

          // see if there are more, and use last w/h
          while (XCheckTypedWindowEvent(MDisplay, AEvent->xconfigure.window, ConfigureNotify, AEvent)) {
            w = AEvent->xconfigure.width;
            h = AEvent->xconfigure.height;
          }

          if ((w != MRect.w) || (h != MRect.h)) {
            //FWinSurface.setSize(w,h); // TODO - cairo?
            resizeBuffer(w,h);
            //MWidth = w;
            //MHeight = h;
            /*if (MListener) MListener->*/on_setSize(w,h);
            if (hasFlag(swf_autoalign)) on_align();
            //paintBuffer();//redraw;
            paintBuffer(this);//redraw;
          }
        }
        break;

        // no drawing is allowed until the first Expose event arrives.

        case Expose: {
          //STrace("--- Expose\n");
          MWindowExposed = true;
          SRect rec;
          rec.x = AEvent->xexpose.x;
          rec.y = AEvent->xexpose.y;
          rec.w = AEvent->xexpose.width;
          rec.h = AEvent->xexpose.height;
          // combine all exposes, instead of doing them one by on..
          // flicker?
          while (XCheckTypedWindowEvent(MDisplay,AEvent->xexpose.window,Expose,AEvent)) {
            rec.combine(AEvent->xexpose.x,AEvent->xexpose.y,AEvent->xexpose.width,AEvent->xexpose.height);
          }
          beginPaint();
          //paintBuffer(self,rec);
          blitBuffer(rec);
          endPaint();
        }
        break;

        case KeyPress: {
          //int32 k = AEvent->xkey.keycode;
          int32 s = AEvent->xkey.state;
          char c[2];
          //int l;
          KeySym keysym; // = XID = unsigned long
          /*l =*/ XLookupString(&AEvent->xkey, c, 1, &keysym, SKEI_NULL);
          //if (l==0) c[0] = '\0';
          uint32 ks = SRemapKeyboard(keysym);
          //STrace("key %i\n",ks);
          //DTrace("keycode:%i, state:%i, l:0x%02X = '%s', ks:0x%04X = %c\n",k,s,c[0],c,ks,ks);
          /*if (MListener) MListener->*/on_keyDown(/*k*/ks,s);
        }
        break;

        case KeyRelease: {
          //int32 k = AEvent->xkey.keycode;
          int32 s = AEvent->xkey.state;
          char c[2];
          //int l;
          KeySym keysym; // = XID = unsigned long
          /*l =*/ XLookupString(&AEvent->xkey, c, 1, &keysym, SKEI_NULL);
          //if (l==0) c[0] = '\0';
          uint32 ks = SRemapKeyboard(keysym);
          //STrace("key %i\n",ks);
          //DTrace("keycode:%i, state:%i, l:0x%02X = '%s', ks:0x%04X = %c\n",k,s,c[0],c,ks,ks);
          /*if (MListener) MListener->*/on_keyUp(/*k*/ks, s);
        }
        break;

        case MapNotify: {
          MWindowMapped = true;
          //if (draw->expect_map_change) {
          //  draw->expect_map_change = false;
          //} else {
          //  draw->iconify_notify(true);
          //  draw->event_handlers->Iconify_Notify(draw, true);
          //}
        }
        break;

        case MotionNotify: {
          int32 x = AEvent->xbutton.x;
          int32 y = AEvent->xbutton.y;
          int32 s = AEvent->xbutton.state;
          /*if (MListener) MListener->*/on_mouseMove(x, y, s);
        }
        break;

        //case PropertyNotify: {
        //  atomname := XGetAtomName(MDisplay,AEvent->xproperty.atom);
        //}

        case UnmapNotify: {
          MWindowMapped = false;
        }

        //else KTrace('unhandled event: ',[AEvent->_type]);

      } // case

      //MLock.unlock();

    }


    //----------

    #define MWM_HINTS_DECORATIONS (1 << 1)
    #define PROP_MOTIF_WM_HINTS_ELEMENTS 5

    void removeDecoration(void) {
      skei_xlib_motif_hints hints;
      //SMemset(&hints,0,sizeof(hints));   // shouldn't be necessary???
      hints.flags = MWM_HINTS_DECORATIONS;
      hints.decorations = 0;
      Atom prop = XInternAtom(MDisplay, "_MOTIF_WM_HINTS", true);
      if (prop != None) {
        XChangeProperty(MDisplay, MWindow, prop, prop, 32, PropModeReplace, (unsigned char*)&hints, PROP_MOTIF_WM_HINTS_ELEMENTS);
      }
    }

    #undef MWM_HINTS_DECORATIONS
    #undef PROP_MOTIF_WM_HINTS_ELEMENTS

    //----------

    // want WM_DELETE_WINDOW ClientMessage

    void wantQuitEvent(void) {
      MDeleteAtom = XInternAtom(MDisplay, "WM_DELETE_WINDOW", true);
      if (MDeleteAtom != None) {
        XSetWMProtocols(MDisplay, MWindow, &MDeleteAtom, 1);
      }
    }

    //----------

    int32 remapButton(int32 AButton) {
      //STrace("button: %i\n",AButton);
      int32 result = smb_none;
      switch(AButton) {
        case 1: result = smb_left;      break;
        case 2: result = smb_middle;    break;
        case 3: result = smb_right;     break;
        case 4: result = smb_wheelUp;   break;
        case 5: result = smb_wheelDown; break;
        case 8: result = smb_backward;  break;
        case 9: result = smb_forward;   break;
      }
      return result;
    }

    //----------

    int32 remapKey(int32 AState) {
      int32 result = smb_none;
      if ((AState & 1  ) != 0) result |= smk_shift;
      if ((AState & 2  ) != 0) result |= smk_caps;  // todo
      if ((AState & 4  ) != 0) result |= smk_ctrl;
      if ((AState & 8  ) != 0) result |= smk_alt;
      if ((AState & 128) != 0) result |= smk_altgr; // todo
      return result;
    }

    //----------

    /*
      X is based upon a client-server architecture
      The map request is asynchronous, meaning that the time this instruction is
      executed doesn't tell us when the window is actually mapped.
      To be sure, we've to wait for the server to send us a MapNotify event

      When we have exited the loop, we have good confidence that the window appears
      on the screen. Actually, this may not be the case since, for example, the
      user may have iconified it using the window manager, but for now, we assume
      the window actually appears.

      ----------

      Another type of event you may wish to check for is the MapNotify event.
      This is raised when the window is mapped to the screen.
      So far we have assumed that the window was mapped as soon as we called
      XMapWindow or XMapRaised. In practise the window manager may have intervened
      and delayed the process. To verify that the window has been mapped (before we
      start drawing in it) you can ask for a MapNotify event via the
      StructureNotifyMask. An alternative is to just wait for the first Expose
      event before starting to draw on the window.

      ----------

      Note that you may not get events about your own windows getting mapped.
      This is because the WM is likely to reparent top-level windows to be children
      not of the root, but of intermediate decoration windows.
      There are two ways to cope with the situation:
      * Check if your window parent, the parent of the parent, ... etc
        is the mapped window of the event.
      * Add XSelectInput (d, yourwindow, StructureNotifyMask); to the mix.
      Note the first select has SubstructureNotifyMask and the second one
      StructureNotifyMask, a different mask.

      ----------


      1)

      for(;;) {
         XEvent e;
         XNextEvent(dpy, &e);
         if (e.type == MapNotify) break;
      }

      2)

      Waiting for MapNotify to inform that the window is mapped onto the screen
      for (XEvent e; e.type != MapNotify; XNextEvent(d_dpy, &e));

      #ifdef H_WAITFORNOTIFY
      static Bool h_WaitForNotify(Display* d, XEvent* e, char* arg)
      {
        return (e->type==MapNotify) & (e->xmap.window==(Window)arg);
      }
      #endif
      XEvent event;
      XIfEvent(m_Display,&event,h_WaitForNotify,(XPointer)m_Window);
    */

    void waitForMapNotify(void) {
      XFlush(MDisplay);
      while (1) {
        XEvent e;
        XNextEvent(MDisplay,&e);
        if (e.type == MapNotify) break;
      }
    }

    //----------


    void startThreads(void) {
      #ifdef SKEI_LIB
      MEventThreadActive = true;
      pthread_create(&MEventThread,SKEI_NULL,skei_xlib_threadproc,this);
      #endif
      MIdleThreadActive = true;
      pthread_create(&MIdleThread,SKEI_NULL,skei_xlib_idleproc,this);
    }

    //----------

      //if (MTimerThreadActive) stopTimer();
      //#ifdef SKEI_LIB
      //if (MEventThreadActive || MIdleEventThreadActive) stopThreads();
      //#else
      //if (MIdleThreadActive) stopThreads();
      //#endif



    void stopThreads(void) {
      void* ret = SKEI_NULL;
      #ifdef SKEI_LIB
      if (MEventThreadActive) {
        MEventThreadActive = false;
        sendEvent(sts_kill);
        pthread_join(MEventThread,&ret);
      }
      #endif
      if (MIdleThreadActive) {
        MIdleThreadActive = false;
        pthread_cancel(MIdleThread);
        pthread_join(MIdleThread,&ret);
      }
    }


  //--------------------------------------------------
  // implementation
  //--------------------------------------------------

  public:

    //------------------------------
    // set
    //------------------------------

    //virtual
    void setPos(int32 AXpos, int32 AYpos) {
      XWindowChanges attr;
      attr.x = AXpos;
      attr.y = AYpos;
      XConfigureWindow(MDisplay,MWindow,CWX | CWY,&attr);
      XFlush(MDisplay);
    }

    //----------

    //virtual
    void setSize(int32 AWidth, int32 AHeight) {
      //MWidth = AWidth;
      //MHeight = AHeight;
      MRect.w = AWidth;
      MRect.h = AHeight;
      XResizeWindow(MDisplay,MWindow,MRect.w,MRect.h);
      XFlush(MDisplay);
    }

    //----------

    /*
      The third and fourth items indicate the name which appears at the top of
      the window and the name of the minimized window respectively.
    */


    //virtual
    void setTitle(const char* ATitle) {
      XSetStandardProperties(MDisplay,MWindow,ATitle,ATitle,None,SKEI_NULL,0,SKEI_NULL);
    }

    //------------------------------
    // basic
    //------------------------------

    /*
      http://tronche.com/gui/x/xlib-tutorial/
      - "Did you wait for a MapNotify before drawing your line ?"
        (more wizard doubletalk)
      - "No, why ?
      - X has a stateless drawing model, the content of the window may be lost when
        the window isn't on the screen." (overflow, why can't these wizard guys
        speak just like you and me ?) "You've to wait for a MapNotify before
        drawing."

        for(;;) {
          XEvent e;
          XNextEvent(dpy, &e);
          if (e.type == MapNotify) break;
        }

      ---

      * as a vst (in jost), i don't get MapNotify events for my own window (only parent?
      * freezes x in certain situations (in a dual monitor setup)

      ---

      http://tronche.com/gui/x/xlib/window/XMapWindow.html
      If the window is an InputOutput window, XMapWindow() generates Expose events
      on each InputOutput window that it causes to be displayed. If the client maps
      and paints the window and if the client {s processing events, the window
      is painted twice. To avoid this, first ask for Expose events and then map the
      window, so the client processes input events as usual. The event list will
      include Expose for each window that has appeared on the screen. The client's
      normal response to an Expose event should be to repaint the window. This
      method usually leads to simpler programs and to proper interaction with
      window managers.

      ---

      https://forum.winehq.org/viewtopic.php?f=2&t=23924
      // FIXME without this ugly delay, the VST window sometimes stays black.
      usleep(100000);
      XMapWindow(display, child);
      XSync(display, false);
      XCloseDisplay(display);

      ---

      // holos:
      virtual void show(void) {
        XMapWindow(m_Display,m_Window);
        XEvent event;
        XIfEvent(m_Display,&event,h_WaitForNotify,(char*)m_Window);
      }
    }

    //function _WaitForNotify(d:PDisplay; e:PXEvent; arg:TXPointer) : LongInt;
    //{
    //  if (e->_type = MapNotify) and (e->xmap.window = TWindow(arg) ) then result := 1
    //  else result := 0;
    //end;

    ---

      DPF: uses XMapRaised
    */

    //virtual
    void open() {
      XMapRaised(MDisplay,MWindow);
      //setOwner(this);
      #ifdef SKEI_LINUX_WAIT_FOR_MAPNOTIFY
      waitforMapNotify;
      #endif
      //#ifdef SKEI_LIB
      startThreads();
      //#endif
    }

    //----------

    //virtual
    void close(void) {
      //#ifdef SKEI_LIB
      stopThreads();
      //#endif
      XUnmapWindow(MDisplay,MWindow);
      XFlush(MDisplay);
      //MWindowExposed = false;
    }

    //----------

    //virtual
    void flush(void) {
      XFlush(MDisplay);
    }

    //----------

    /*
      http://forums.libsdl.org/viewtopic.php?t=5720&sid=16cd6404ab2715a87d39bd48b3fc8fc0

      Calling Xsync causes a round trip to the server. The calling thread is
      forced to wait while the round trip takes place. While that thread is
      waiting no other thread can use Xlib. That means that Xsync is the
      opposite of what you want to use in a multithreaded program. Using
      your own mutexes is a bad idea because you can't be sure that other
      parts of the code, parts of libraries you have linked in, are not
      making their own multi-threaded calls to Xlib. To make sure that Xlib
      is actually being accessed properly you must use SinitThreads() and
      Xlibs own lock and unlock operations.
    */

    //virtual
    void sync(void) {
      XSync(MDisplay,false);
    }

    //----------

    //virtual
    void lock(void) {
      XLockDisplay(MDisplay);
    }

    //----------

    //virtual
    void unlock(void) {
      XUnlockDisplay(MDisplay);
    }

    //------------------------------
    // painting
    //------------------------------

    //virtual
    void beginPaint(void) {
    }

    //----------

    //virtual
    void endPaint(void) {
    }

    //----------

    /*
      should this be static? what if we're sending off an event, and
      returning from this method.. the ev variable goes out of scope,
      and the event points to 'dangling pointer'???
    */

    //virtual
    void invalidate(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) {
      MExposeEvent.type     = Expose;
      MExposeEvent.display  = MDisplay;
      MExposeEvent.window   = MWindow; // mParent;
      MExposeEvent.x        = AXpos;
      MExposeEvent.y        = AYpos;
      MExposeEvent.width    = AWidth;
      MExposeEvent.height   = AHeight;
      MExposeEvent.count    = 0;
      XSendEvent(MDisplay,MWindow,False,ExposureMask,(XEvent*)&MExposeEvent);
      XFlush(MDisplay);
    }

    //------------------------------
    // hierarchy, events
    //------------------------------

    /*
      http://tronche.com/gui/x/xlib/window-and-session-manager/XReparentWindow.html
      A BadMatch error results if: The new parent window is not on the same
      screen as the old parent window.
    */

    //virtual
    void reparent(void* AParent) {
      MParentWindow= (Window)AParent;
      XReparentWindow(MDisplay,MWindow,MParentWindow,0,0);
      XFlush(MDisplay);
    }

    //----------

    //virtual
    void sendEvent(uint32 AValue) {
      //if (AValue==sts_kill) {
      //  STrace("sending event: sts_kill\n");
      //  STrace("  MDisplay %x\n",MDisplay);
      //  STrace("  MWindow  %i\n",MWindow);
      //}
      MSendEvent.type         = ClientMessage;
      MSendEvent.message_type = MCustomAtom;
      MSendEvent.display      = MDisplay;
      MSendEvent.window       = MWindow;
      MSendEvent.format       = 32;
      MSendEvent.data.l[0]    = AValue;
      XSendEvent(MDisplay,MWindow,False,NoEventMask,(XEvent*)&MSendEvent);
      XFlush(MDisplay);
    }

    //----------

    /*
      TODO: idle?
    */

    //virtual
    void eventLoop(void) {
      startThreads();
      while (1) {
        XEvent event;
        XNextEvent(MDisplay, &event);
        #ifdef SKEI_TABLET
        if (MTablet.handleEvent(&event) == TABLET_OK) {
          //STrace("tablet x=%i, y=%i, z=%i\n",MTablet.Info->PosX, EasyTab->PosY, EasyTab->Pressure );
          tabletHandler();
          continue;
        }
        #endif
        uint32 data = event.xclient.data.l[0];
        if ((event.type==ClientMessage) && (data==MDeleteAtom)) break;
        else eventHandler(&event);
      } // while
      stopThreads();
    }

    //----------

    //virtual
    void renderLoop(void) {
      //TODO
    }

    //------------------------------
    // mouse
    //------------------------------

    //virtual
    void  setCursor(int32 ACursor) {
      //if (ACursor != MPrevCursor) MPrevCursor = ACursor;
      if (ACursor < 0) ACursor = 0;
      if (MWindowCursor >= 0) resetCursor();
      MWindowCursor = XCreateFontCursor(MDisplay, skei_xlib_cursors[ACursor]);
      XDefineCursor(MDisplay,MWindow,MWindowCursor);
    }

    //----------

    // http://stackoverflow.com/questions/2433447/how-to-set-mouse-cursor-position-in-c-on-linux

    //virtual
    void setCursorPos(int32 AXpos, int32 AYpos) {
      XWarpPointer(MDisplay,None,MWindow,0,0,0,0,AXpos,AYpos);
      //Flush the output buffer, therefore updates the cursor's position.
      //XFlush(MDisplay);
    }

    void getCursorPos(int32* AXpos, int32* AYpos) {
      Window root,child;
      int root_xpos,root_ypos;
      int child_xpos,child_ypos;
      unsigned int mask;
      XQueryPointer(
        MDisplay,
        MWindow,
        &root,
        &child,
        &root_xpos,
        &root_ypos,
        &child_xpos,
        &child_ypos,
        &mask
      );
      *AXpos = child_xpos;
      *AYpos = child_ypos;
    }


    //----------

    //virtual
    void showCursor(void) {
      setCursor(smc_default);
    }

    //----------

    //virtual
    void hideCursor(void) {
      if (MWindowCursor >= 0) resetCursor();
      MWindowCursor = XCreatePixmapCursor(MDisplay, MEmptyPixmap, MEmptyPixmap, &MBlack, &MBlack, 0, 0);
      XDefineCursor(MDisplay, MWindow, MWindowCursor);
    }

    //----------

    //virtual
    void grabCursor(void) {
      int32 which = ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
      XGrabPointer(MDisplay, MWindow, False, which, GrabModeSync, GrabModeAsync, MWindow,None, CurrentTime);
    }

    //----------

    //virtual
    void releaseCursor(void) {
      XUngrabPointer(MDisplay,CurrentTime);
    }

    //----------

    //virtual
    void resetCursor(void) {
      XUndefineCursor(MDisplay,MWindow);
      XFreeCursor(MDisplay,MWindowCursor);
      MWindowCursor = -1;
    }

    //------------------------------
    // keyboard
    //------------------------------

    //virtual
    void grabKeyboard(void) {
    }

    //----------

    //virtual
    void releaseKeyboard(void) {
    }

    //------------------------------
    // timer
    //------------------------------

    //virtual
    void startTimer(int32 ms) {
      MTimerSleep = ms;
      MTimerThreadActive = true;
      pthread_create(&MTimerThread,SKEI_NULL,skei_xlib_timerproc,this);
    }

    //----------

    //virtual
    void stopTimer(void) {
      void* ret;
      MTimerThreadActive = false;
      pthread_join(MTimerThread,&ret);
      //STrace("timer stopped..\n");
    }

    //------------------------------
    // fonts
    //------------------------------

    /* "arial-32" */

    //virtual
    void* loadFont(const char* AName, int32 ASize) {
      #ifdef SKEI_XFT
        char buffer[256] = "";
        char temp[16];
        SStrcat(buffer,AName);
        SStrcat(buffer,"-");
        SStrcat(buffer,SIntToString(temp,ASize,15));
        return XftFontOpenName(MDisplay,MScreenNum,buffer);
      #else
        return SKEI_NULL;
      #endif
    }

    //----------

    //virtual
    void unloadFont(void* AFont) {
      #ifdef SKEI_XFT
        XftFontClose(MDisplay,(XftFont*)AFont);
      #endif
    }

    //----------

    void setIdleAtom(Atom atm) {
      MIdleAtom = atm;
    }

};

//----------------------------------------------------------------------
// thread proc
//----------------------------------------------------------------------

/*
  notes to self:
  - serialized (?).. we receive all events through this thread function,
    and we call the window's event handler for each of them, and process
    it there before we return here, and check for new events..
  - we're polling for events, not being called (possibly reentrant) like a
    callback, so while we're processing one event, we will not get another
    one interrupting us..
  - so, if ALL drawing originates from the window's event handler, it should
    be safe?
  - and we won't get interrupted by a Expose event when we're for example
    still processing a ConfigureNotify event?

  - how does effEditIdle fit into all of this?
    > we don't use effEditIdle in linux! the event thread calls on_idle
      at regular intervals..

  ---

  at the moment, this is used only in gui plugins
  window.open/show

  ---

  The pthread_exit() function terminates the calling thread and makes the value
  ptr available to any successful join with the terminating thread.

*/

//----------

/*
  todo:
  some kind of timerpool system, where widgets (or whatever) - STimerListener
  can register itself.. we set the SKEI_LINUX_THREADPROC_IDLESLEEP to something
  small, like 1 ms, and each timer can have a ticks variable.. each 1ms we check
  this list, and update counters, and possibly call on_timer() for the
  listeners
*/

//----------

/*
  so.. Display* can not be used across threads..
  we create the window, which sets up the Display*..
  if window->open, we create this thread (which uses the Display*, hmm)

  and also, in a vst plugin, is effEditOpen and effEditIdle called from the
  same thread?
*/

//static SLock MEventThreadLock;

    // test
    //MEventThreadLock.lock();

    //Display* dsp = XOpenDisplay(SKEI_NULL);
    //uint32 eventmask  =
    //  ExposureMask        |
    //  StructureNotifyMask |
    //  PropertyChangeMask  |
    //  ButtonPressMask     |
    //  ButtonReleaseMask   |
    //  PointerMotionMask   |
    //  KeyPressMask        |
    //  KeyReleaseMask      |
    //  ClientMessage;
    //XSelectInput(dsp,win->MWindow,eventmask);

#ifdef SKEI_LIB

void* skei_xlib_threadproc(void* AData) {
  SWindow_Xlib* win = (SWindow_Xlib*)AData;
  if (win) {
    while (win->MEventThreadActive) {
      XEvent ev;
      XNextEvent(win->MDisplay, &ev);
      #ifdef SKEI_TABLET
      if (MTablet.handleEvent(&event) == TABLET_OK) {
        //STrace("tablet x=%i, y=%i, z=%i\n",MTablet.Info->PosX, EasyTab->PosY, EasyTab->Pressure );
        tabletHandler();
        continue;
      }
      #endif
      //if (ev.xany.display != win->MDisplay || ev.xany.window != win->MWindow) continue;
      if (ev.type == ClientMessage) {
        XClientMessageEvent* cev = (XClientMessageEvent*)&ev;
        uint32 dta = ev.xclient.data.l[0];
        if (cev->message_type == win->MCustomAtom) {
          if (dta == sts_kill) pthread_exit(SKEI_NULL);
          else win->eventHandler(&ev);
        } // custom atom
      } //ClientMessage
      else win->eventHandler(&ev);
    } // while ..
  } // win
  return SKEI_NULL;
}

#endif // SKEI_LIB

//----------------------------------------------------------------------
// sleep proc
//----------------------------------------------------------------------

#ifdef SKEI_LIB
#ifdef SKEI_LINUX_SLEEP_THREAD

void* skei_xlib_threadproc_sleep(void* AData) {
  SWindow_Xlib* win = (SWindow_Xlib*)AData;
  if (win) {
    while (win->MEventThreadActive) {
      while (XPending(win->MDisplay) > 0) {
        XEvent ev;
        XNextEvent(win->MDisplay, &ev);
        #ifdef SKEI_TABLET
        if (MTablet.handleEvent(&event) == TABLET_OK) {
          //STrace("tablet x=%i, y=%i, z=%i\n",MTablet.Info->PosX, EasyTab->PosY, EasyTab->Pressure );
          tabletHandler();
          continue;
        }
        #endif
        if (ev.type == ClientMessage) {
          XClientMessageEvent* cev = (XClientMessageEvent*)&ev;
          uint32 dta = ev.xclient.data.l[0];
          if (cev->message_type == win->MCustomAtom) {
            if (dta == sts_kill) pthread_exit(SKEI_NULL);
            else win->eventHandler(&ev);
          } // custom atom
        } //ClientMessage
        else
          win->eventHandler(&ev);
      } // pending
      //win->sendEvent(sts_idle);
      if (win->MWindowMapped && win->MWindowExposed) win->on_idle();
      SSleep(SKEI_LINUX_SLEEP_MS);
    } // while ..
  }
  //STrace("sleep thread returning\n");
  return SKEI_NULL;
}


#endif // SKEI_LINUX_SLEEP_THREAD
#endif // SKEI_LIB

//----------------------------------------------------------------------
// idle proc
//----------------------------------------------------------------------

/*
  sends client message sts_idle to window at regular intervals
  end thread by setting MIdleThreadActive to false
*/

//#ifdef SKEI_LIB

//static XClientMessageEvent event;

void* skei_xlib_idleproc(void* AData) {
  int prev_cancel_type;
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,&prev_cancel_type);
  SWindow_Xlib* win = (SWindow_Xlib*)AData;
  if (win) {
    //Atom atom = XInternAtom(dsp,"idle",true);
    //Display* dsp = win->MDisplay;
    Display* dsp = XOpenDisplay(SKEI_NULL);
    while (win->MIdleThreadActive) {
      if (win->MWindowMapped && win->MWindowExposed) {
        XClientMessageEvent* event = &win->MIdleEvent;
        //win->on_idle();
        SMemset(event,0,sizeof(XClientMessageEvent));
        event->type           = ClientMessage;
        event->message_type  = 0;//MIdleAtom;
        event->display       = dsp;//MDisplay;
        event->window        = win->MWindow;
        event->format        = 32;
        event->data.l[0]     = sts_idle;
        XSendEvent(dsp,win->MWindow,False,NoEventMask,(XEvent*)event);
        XFlush(dsp);
      } // mapped & active
      SSleep(SKEI_LINUX_IDLE_MS);
    } // while active
    //XSync(dsp,false);
    XCloseDisplay(dsp);
  } // win
  return SKEI_NULL;
}

//#endif //  SKEI_LIB

//----------------------------------------------------------------------
// timer proc
//----------------------------------------------------------------------

/*
  NOTE: could be called concurrent with threadproc!
  post message to xlib queue, to be sure the events are processed
  'in order' and in the right thread..
    win->sendEvent(sts_timer);
*/

void* skei_xlib_timerproc(void* AData) {
  SWindow_Xlib* win = (SWindow_Xlib*)AData;
  if (win) {
    while (win->MTimerThreadActive) {
      //if (MWindowMapped && MWindowExposed)
      win->on_timer();
      //win->sendEvent(sts_timer);
      SSleep(win->MTimerSleep); // hmm..
    }
  }
  return SKEI_NULL;
}

//----------------------------------------------------------------------
#endif
