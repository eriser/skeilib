#ifndef skei_widget_menuitem_included
#define skei_widget_menuitem_included
//----------------------------------------------------------------------

#include "skei_widget_text.h"

//----------

class SWidget_MenuItem
: public SWidget_Text {

  private:

    bool  MIsHovering;

  public:

    SWidget_MenuItem(SRect ARect, const char* AText, uint32 AAlignment=swa_none)
    : SWidget_Text(ARect,AText,AAlignment) {
      MName = "SWidget_MenuItem";
      MHint = "menuitem";
      //setFlag(swf_active);
      MDrawBackground = false;//true;
      MIsHovering = false;
      MCursor = smc_finger;
    }

    virtual ~SWidget_MenuItem() {
    }

  public:

    bool hovering(void)         { return MIsHovering; }
    void hovering(bool AHover)  { MIsHovering = AHover; }

  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      //STrace("painting %s\n",MName);
      if (MIsHovering) {
        MTextColor = SLightGrey;
        MBackColor = SDarkGrey;
      }
      else {
        MTextColor = SDarkGrey;
        MBackColor = SLightGrey;
      }
      SWidget_Text::on_paint(APainter,ARect,AMode);
    }

    //virtual
    void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      switch(AButton) {
        case smb_left:
          MValue = 1;
          do_update(this);
          break;
        //smb_right:
        //  FValue := 0;
        //  do_update(Self);
        //  break;
      } // swith
    }

    //virtual
    void on_enter(SWidget* AWidget) { // from
      //DTrace("enter %i (%s')\n",MIndex,MName);
      MIsHovering = true;
      do_redraw(this,MRect);
      SWidget_Text::on_enter(AWidget); // cursor, hint
    }

    //virtual
    void on_leave(SWidget* AWidget) { // to
      //DTrace("leave %i (%s')\n",MIndex,MName);
      MIsHovering = false;
      do_redraw(this,MRect);
      SWidget_Text::on_leave(AWidget);
    }

    //virtual
    void on_reset(int32 AMode=0) {
      MIsHovering = false;
      SWidget_Text::on_reset(AMode); // children
    }

};

//----------------------------------------------------------------------
#endif

