#ifndef skei_widget__included
#define skei_widget__included
//----------------------------------------------------------------------

#include "skei_widget.h"

//----------

class SWidget_
: public SWidget {

  public:

    SWidget_(SRect ARect, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_";
      //MHint = "";
      //MCursor = smc_default;
    }

    virtual ~SWidget_() {
    }

  /*

  public:

    virtual void on_reset(int32 AMode=0) {}
    virtual void on_setPos(int32 AXpos, int32 AYpos) {}
    virtual void on_setSize(int32 AWidth, int32 AHeight) {}
    virtual void on_scroll(int32 ADeltaX, int32 ADeltaY) {}
    //virtual void on_move(int32 AXdelta, int32 AYdelta) {}
    //virtual void on_resize(int32 AWidth, int32 AHeight) {}
    virtual void on_align(void) {}
    virtual void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {}
    virtual void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_mouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_mouseMove(int32 AXpos, int32 AYpos, int32 AState) {}
    virtual void on_mouseDoubleClick(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_keyDown(int32 AKey, int32 AState) {}
    virtual void on_keyUp(int32 AKey, int32 AState) {}
    virtual void on_enter(SWidget* AWidget) {}
    virtual void on_leave(SWidget* AWidget) {}
    //virtual void on_timer(void) {}
    //virtual void on_idle(void) {}

  public:

    virtual void do_move(SWidget* AWidget, int32 AXpos, int32 AYpos) {}
    virtual void do_sizer(SWidget* AWidget, int32 ADeltaX, int32 ADeltaY, int32 AMode) {}
    virtual void do_resize(SWidget* AWidget, int32 AWidth, int32 AHeight) {}
    virtual void do_redraw(SWidget* AWidget, SRect ARect) {}
    virtual void do_update(SWidget* AWidget) {}
    virtual void do_cursor(SWidget* AWidget, int32 ACursor) {}
    virtual void do_hint(SWidget* AWidget, const char* AHint) {}
    virtual void do_modal(SWidget* AWidget, int32 AMode=0) {}
    virtual void do_wantkeys(SWidget* AWidget) {}

  */

};

//----------------------------------------------------------------------
#endif
