#ifndef skei_window_base_included
#define skei_window_base_included
//----------------------------------------------------------------------

#include "skei_painter.h"
#include "skei_widget.h"

//----------

class SWindow_Base
: public SWidget {

  public:

    SWindow_Base(int32 AWidth, int32 AHeight, void* AParent=SKEI_NULL)
    : SWidget(SRect(AWidth,AHeight),swa_fillClient) {
      MName = "SWindow_Base";
    }

    //----------

    virtual ~SWindow_Base() {
    }

  //----------------------------------------
  // on_
  //----------------------------------------

  public:

    virtual void    on_timer(void) { /*STrace("on_timer\n");*/ }
    virtual void    on_idle(void) {}

  //----------------------------------------
  // access
  //----------------------------------------

  public:

  //----------------------------------------
  // buffer
  //----------------------------------------

  protected:

    // implemented in skei_window.h

    virtual void createBuffer(uint32 AWidth, uint32 AHeight) {}
    virtual void destroyBuffer(void) {}
    virtual void resizeBuffer(uint32 AWidth, uint32 AHeight) {}
    virtual void paintBuffer(SRect ARect) {}
    virtual void paintBuffer(void) {}
    virtual void paintBuffer(SWidget* AWidget) {}
    virtual void blitBuffer(SRect ARect) {}

  //----------------------------------------
  // implementation
  //----------------------------------------

  public:

    // implemented in skei_window-*.h (platform specific)

    virtual void setPos(int32 AXpos, int32 AYpos) {}
    virtual void setSize(int32 AWidth, int32 AHeight) {}
    virtual void setTitle(const char* ATitle) {}
    virtual void open(void) {}
    virtual void close(void) {}
    virtual void flush(void) {}
    virtual void sync(void) {}
    virtual void lock(void) {}
    virtual void unlock(void) {}
    virtual void beginPaint(void) {}
    virtual void endPaint(void) {}
    virtual void invalidate(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) {}
    virtual void reparent(void* AParent) {}
    virtual void sendEvent(uint32 AValue) {}
    virtual void eventLoop(void) {}
    virtual void renderLoop(void) {}
    virtual void setCursor(int32 ACursor) {}
    virtual void setCursorPos(int32 AXpos, int32 AYpos) {}
    virtual void showCursor(void) {}
    virtual void hideCursor(void) {}
    virtual void grabCursor(void) {}
    virtual void releaseCursor(void) {}
    virtual void resetCursor(void) {}
    virtual void grabKeyboard(void) {}
    virtual void releaseKeyboard(void) {}
    virtual void startTimer(int32 ms) {}
    virtual void stopTimer(void) {}

    virtual void* loadFont(const char* AName, int32 ASize) { return SKEI_NULL; }
    virtual void  unloadFont(void* AFont) {}

    // helpers

    virtual void invalidate(SRect ARect) {
      invalidate(ARect.x,ARect.y,ARect.w,ARect.h);
    }

};

//----------------------------------------------------------------------
#endif
