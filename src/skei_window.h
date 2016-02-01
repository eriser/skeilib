#ifndef skei_window_included
#define skei_window_included
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_painter.h"
#include "skei_rect.h"
#include "skei_surface.h"
#include "skei_widget.h"

//----------

#ifdef SKEI_LINUX
#include "skei_window_xlib.h"
#endif // SKEI_LINUX

#ifdef SKEI_WIN32
#include "skei_window_win32.h"
#endif // SKEI_LINUX

//----------------------------------------------------------------------


class SWindow
: public SWindow_Implementation {

  private:
    SPainter*   MWindowPainter;
    uint32      MBufferWidth;
    uint32      MBufferHeight;
    SPainter*   MBufferPainter;
    SSurface*   MBufferSurface;

  public:
    SPainter*   windowPainter(void) { return MWindowPainter; }
    uint32      bufferWidth(void)   { return MBufferWidth; }
    uint32      bufferHeight(void)  { return MBufferHeight; }
    SPainter*   bufferPainter(void) { return MBufferPainter; }
    SSurface*   bufferSurface(void) { return MBufferSurface; }

  public:

    SWindow(int32 AWidth, int32 AHeight, void* AParent=SKEI_NULL)
    : SWindow_Implementation(AWidth,AHeight,AParent) {
      MName = "SWindow";

      //STrace("SWindow()\n");
      //STrace("  AWidth %i\n",AWidth);
      //STrace("  AHeight %i\n",AHeight);
      //STrace("  MRect.w %i\n",MRect.w);
      //STrace("  MRect.h %i\n",MRect.h);
      MWindowPainter = new SPainter(this);
      createBuffer(AWidth,AHeight);
    }

    //----------

    virtual ~SWindow() {
      destroyBuffer();
      delete MWindowPainter;
    }

  //----------------------------------------
  // buffer
  //----------------------------------------

  public:

    //virtual
    void createBuffer(uint32 AWidth, uint32 AHeight) {
      MBufferWidth    = AWidth;
      MBufferHeight   = AHeight;
      MBufferSurface  = new SSurface(this,AWidth,AHeight);
      MBufferPainter  = new SPainter(MBufferSurface);
    }

    //----------

    //virtual
    void destroyBuffer(void) {
      if (MBufferPainter) delete MBufferPainter;
      if (MBufferSurface) delete MBufferSurface;
      MBufferPainter  = SKEI_NULL;
      MBufferSurface  = SKEI_NULL;
      MBufferWidth    = 0;
      MBufferHeight   = 0;
    }

    //----------

    void resizeBuffer(uint32 AWidth, uint32 AHeight) {
      uint32 w = MBufferWidth;
      uint32 h = MBufferHeight;
      if ((w==0) || (h==0)) {
        w = AWidth;
        h = AHeight;
      }
      while (AWidth  > w) w *= 2;
      while (AHeight > h) h *= 2;
      #ifndef SKEI_NO_SHRINK_BACKBUFFER
      while (AWidth  < (w >> 1)) w >>= 1;
      while (AHeight < (h >> 1)) h >>= 1;
      #endif
      if ((w != MBufferWidth) || (h != MBufferHeight)) {
        destroyBuffer();
        createBuffer(w,h);
        //redraw;
      }
    }

    //----------

    void paintBuffer(SRect ARect) {
      if (MBufferPainter) {
        MBufferPainter->pushClip(ARect);

        if (MModalWidget) { MModalWidget->on_paint(MBufferPainter,ARect); }
        else

        on_paint(MBufferPainter,ARect);

        MBufferPainter->popClip();
        if (MWindowExposed && MWindowMapped) invalidate( ARect.x, ARect.y, ARect.w, ARect.h ) ;

      }
    }

    //----------

    void paintBuffer(void) {
      paintBuffer( MRect );
    }

    //----------

    void paintBuffer(SWidget* AWidget) {
      paintBuffer( AWidget->rect() );
      //SRect rect = AWidget->rect();
      //if (MBufferPainter) {
      //  MBufferPainter->pushClip(rect);
      //  AWidget->on_paint(MBufferPainter,rect);
      //  MBufferPainter->popClip();
      //  if (MWindowExposed && MWindowMapped) invalidate( rect.x, rect.y, rect.w, rect.h ) ;
      //}
    }

    //----------

    //virtual
    void blitBuffer(SRect ARect) {
      if (MWindowExposed && MWindowMapped) {
        if ((MBufferSurface) && (MBufferPainter)) {
          MWindowPainter->drawSurface( ARect.x,ARect.y,MBufferSurface,ARect.x, ARect.y, ARect.w, ARect.h );
        }
      }
    }

  //----------------------------------------
  // do_ : widget listener
  //----------------------------------------

  /*
    events from sub-widgets
  */

  public:

    //virtual
    void do_move(SWidget* AWidget, int32 AXpos, int32 AYpos) {
    }

    //----------

    //virtual
    void do_sizer(SWidget* AWidget, int32 ADeltaX, int32 ADeltaY, int32 AMode) {
      // we don't get resize messages from the host, so we 'simulate' it ???
      #ifdef SKEI_LIB
        // hmm.. do we receive size messages when/if the host resizes the editor?
        // if not, who resizes the backbuffer?
        on_align();
        paintBuffer();
      #else
      //#ifndef SKEI_LIB
        if (AMode==ssm_window) {
          int32 w = MRect.w + ADeltaX;
          int32 h = MRect.h + ADeltaY;
          if (w <= 32) w = 32; // resize (?) crashes with 0 or negative sizes..
          if (h <= 32) h = 32;
          setSize(w,h);
          /*if ( (w!=MBufferWidth) || (h!=MBufferHeight) )*/ resizeBuffer(w,h);
        }
        if (hasFlag(swf_autoalign)) on_align();
        paintBuffer();
      #endif
    }

    //----------

    //virtual
    void do_resize(SWidget* AWidget, int32 AWidth, int32 AHeight) {
      if (hasFlag(swf_autoalign)) on_align();
      paintBuffer(); // redraw entire window.. in case alignment has changed..
    }

    //----------

    //virtual
    void do_redraw(SWidget* AWidget, SRect ARect) {
      //STrace("SWindow.do_redraw %s\n",AWidget->name());
      paintBuffer(AWidget);
    }

    //----------

    //virtual
    void do_update(SWidget* AWidget) {
      //STrace("SWindow.do_update\n");
    }

    //----------

    //virtual
    void do_cursor(SWidget* AWidget, int32 ACursor) {
      setCursor(ACursor);
    }

    //----------

    //virtual
    void do_hint(SWidget* AWidget, const char* AHint) {
    }

    //----------

    //virtual
    void do_modal(SWidget* AWidget, int32 AMode=0) {
      //if AMode = 0 then FModalWidget := nil
      /*else*/ MModalWidget = AWidget;
    }

    //----------

    //virtual
    void do_wantkeys(SWidget* AWidget) {
      if (AWidget) {
        grabKeyboard();
        MWantKeysWidget = AWidget;
      }
      else {
        releaseKeyboard();
        MWantKeysWidget = SKEI_NULL;
      }
    }

};

//----------------------------------------------------------------------
#endif
