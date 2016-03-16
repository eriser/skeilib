#ifndef skei_widget_menu_included
#define skei_widget_menu_included
//----------------------------------------------------------------------

#include "skei_widget_menuitem.h"
//#include "skei_widget_panel.h"
#include "skei_widget_color.h"

//----------

class SWidget_Menu
: public SWidget_Color {

  private:

    int32     MSelected;
    SWidget*  MSelectedWidget;
    bool      MIsOpen;
    //SColor    MBorderColor;

  public:

    SWidget_Menu(SRect ARect, uint32 AAlignment=swa_none)
    : SWidget_Color(ARect,swa_none) {
      MName = "SWidget_Menu";
      MHint = "menu";
      clearFlag(swf_active);
      clearFlag(swf_visible);
      MSelected = -1;
      MIsOpen = false;
      MColor = SLightGrey;
      //MBorderColor = SDarkGrey;

    }

    virtual ~SWidget_Menu() {
    }

  public:

    int32     selected(void)        { return MSelected; }
    SWidget*  selectedWidget(void)  { return MSelectedWidget; }
    bool      isOpen(void)          { return MIsOpen; }

  public:

    void deselect(void) {
      if ((MSelected >= 0) && (MSelected < (int32)MSubWidgets.size())) {
        //SWidget_MenuItem* menuitem = (SWidget_MenuItem*)MSubWidgets[MSelected];
        SWidget_MenuItem* menuitem = (SWidget_MenuItem*)MSelectedWidget;
        menuitem->hovering(false);
      }
    }

    /*
      todo:
      - center around mouse cursor, so that we initially point at
        item 1
      - make sure menu fits on screen/widget
    */

    void open(int32 AXpos, int32 AYpos) {
      MIsOpen = true;
      MSelected = -1;
      MSelectedWidget = SKEI_NULL;
      on_setPos(AXpos,AYpos);
      do_cursor(this,smc_default);
      setFlag(swf_active);
      setFlag(swf_visible);
      //on_setPos(AXpos,AYpos);
      //on_align;
      do_redraw(this,MRect);
      do_modal(this,0);
    }

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
      on_reset(); // FIsHovering = false, for all menuitems...
    }

  public:

    //virtual
//    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
//      //STrace("painting %s\n",MName);
//      SWidget_Panel::on_paint(APainter,ARect,AMode);
//      //APainter->setDrawColor(MBorderColor);
//      //APainter->drawRect(MRect);
//    }

    //virtual
    void do_update(SWidget* AWidget) {
      MSelected = AWidget->index();
      MSelectedWidget = AWidget;
      //KTrace(['KWidget_Menu.do_update: ',FSelected{Index},KODE_CR]);
      //close;
      SWidget_Color::do_update(this);
    }

};

//----------------------------------------------------------------------
#endif

