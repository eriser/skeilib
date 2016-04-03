#ifndef skei_widget_popup_included
#define skei_widget_popup_included
//----------------------------------------------------------------------

#include "skei_widget_color.h"

//----------

class SWidget_Popup
: public SWidget_Color {

  private:

    bool      MIsOpen;
    SColor    MBorderColor;
    bool      MDrawBorder;

  public:

    SWidget_Popup(SRect ARect, uint32 AAlignment=swa_none)
    : SWidget_Color(ARect,swa_none) {
      MName = "SWidget_Popup";
      MHint = "popup";
      clearFlag(swf_active);
      clearFlag(swf_visible);
      MIsOpen = false;
      MColor = SLightGrey;
      MBorderColor = SDarkGrey;
      MDrawBorder = true;
    }

    virtual ~SWidget_Popup() {
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void      borderColor(SColor AColor) { MBorderColor = AColor; }
    void      drawColor(bool ADraw) { MDrawBorder = ADraw; }
    bool      isOpen(void)          { return MIsOpen; }

  public:

    void open(int32 AXpos, int32 AYpos) {
      MIsOpen = true;
      on_setPos(AXpos,AYpos);
      do_cursor(this,smc_default);
      setFlag(swf_active);
      setFlag(swf_visible);
      //on_setPos(AXpos,AYpos);
      //on_align;
      do_redraw(this,MRect);
      do_modal(this,0);
    }

    //----------

    void close(void) {
      MIsOpen = false;
      do_modal(SKEI_NULL,0);
      clearFlag(swf_active);
      clearFlag(swf_visible);
      do_redraw(this,MRect);
      // which of these three are needed.. todo: investigate
      MHoverWidget = SKEI_NULL;
      MCapturedWidget = SKEI_NULL;
      MModalWidget = SKEI_NULL;
//      on_reset(); // FIsHovering = false, for all menuitems...
    }

  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      //STrace("painting %s\n",MName);
      SWidget_Color::on_paint(APainter,ARect,AMode);
      if (MDrawBorder) {
        APainter->setDrawColor(MBorderColor);
        APainter->drawRect(MRect);
      }
    }

    //virtual
    //void do_update(SWidget* AWidget) {
    //  MSelected = AWidget->index();
    //  MSelectedWidget = AWidget;
    //  //KTrace(['KWidget_Menu.do_update: ',FSelected{Index},KODE_CR]);
    //  //close;
    //  SWidget_Color::do_update(this);
    //}

};

//----------------------------------------------------------------------
#endif


