#ifndef skei_widget_switch_included
#define skei_widget_switch_included
//----------------------------------------------------------------------

#include "skei_widget.h"

//----------

class SWidget_Switch
: public SWidget {

  protected:
    uint32      MTextAlign;
    SColor      MBackOnColor;
    SColor      MBackOffColor;
    SColor      MTextOnColor;
    SColor      MTextOffColor;
    const char* MText;
    const char* MTextOff;

  public:

    void backOnColor(SColor AColor)   { MBackOnColor = AColor; }
    void backOffColor(SColor AColor)  { MBackOffColor = AColor; }
    void textOnColor(SColor AColor)  { MTextOnColor = AColor; }
    void textOffColor(SColor AColor)  { MTextOffColor = AColor; }
    void textAlign(uint32 AAlign)     { MTextAlign = AAlign; }

  public:

    SWidget_Switch(SRect ARect, const char* ATextOn, const char* ATextOff, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName         = "SWidget_Switch";
      MHint         = "switch";
      MText         = ATextOn;
      MTextOff      = ATextOff;
      MValue        = 0;
      MTextAlign    = sta_center;
      MCursor       = smc_finger;
      MBackOnColor  = SLightGrey;
      MBackOffColor = SDarkGrey;
      MTextOnColor  = SDarkGrey;
      MTextOffColor = SLightGrey;
    }

    //----------

    virtual ~SWidget_Switch() {
    }

  //----------------------------------------

  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      if (MValue > 0.5) {
        APainter->setFillColor(MBackOnColor);
        APainter->fillRect(MRect);
        APainter->setTextColor(MTextOnColor);
        APainter->drawText(MRect,MText,MTextAlign);
      } else {
        APainter->setFillColor(MBackOffColor);
        APainter->fillRect(MRect);
        APainter->setTextColor(MTextOffColor);
        APainter->drawText(MRect,MTextOff,MTextAlign);
      }
      SWidget::on_paint(APainter,ARect,AMode);
    }

    //----------

    //virtual
    void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      SWidget::on_mouseDown(AXpos,AYpos,AButton,AState);
      if (MValue < 0.5) MValue = 1; else MValue = 0;
      do_update(this);
      do_redraw(this,MRect);

    }

};

//----------------------------------------------------------------------
#endif
