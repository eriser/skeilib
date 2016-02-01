#ifndef skei_widget_button_included
#define skei_widget_button_included
//----------------------------------------------------------------------

#include "skei_widget.h"

//----------

class SWidget_Button
: public SWidget {

  protected:
    uint32      MTextAlign;
    SColor      MBackOnColor;
    SColor      MBackOffColor;
    SColor      MTextOnColor;
    SColor      MTextOffColor;
    const char* MText;
    const char* MTextOff;
    //SColor      MBackOnColorInactive;
    //SColor      MBackOffColorInactive;
    //SColor      MTextOnColorInactive;
    //SColor      MTextOffColorInactive;

  public:

    SWidget_Button(SRect ARect, const char* ATextOn, const char* ATextOff, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName         = "SWidget_Button";
      MHint         = "button";
      MText         = ATextOn;
      MTextOff      = ATextOff;
      MValue        = 0;
      MTextAlign    = sta_center;
      MCursor       = smc_finger;
      MBackOnColor  = SLightGrey;
      MBackOffColor = SDarkGrey;
      MTextOnColor  = SDarkGrey;
      MTextOffColor = SLightGrey;
      //clearFlag(swf_active);
      //MBackOnColorInactive  = MBackOnColor;
      //MBackOffColorInactive = MBackOffColor;
      //MTextOnColorInactive  = MTextOnColor;
      //MTextOffColorInactive = MTextOffColor;
      //MBackOnColorInactive.blend(0.5,SGrey);
      //MBackOffColorInactive.blend(0.5,SGrey);
      //MTextOnColorInactive.blend(0.5,SGrey);
      //MTextOffColorInactive.blend(0.5,SGrey);
    }

    //----------

    virtual ~SWidget_Button() {
    }

  //----------------------------------------

  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {

      //if (MFlags&swf_active) {
        if (MValue > 0.5) {
          APainter->setFillColor( MBackOnColor );
          APainter->fillRect(MRect);
          APainter->setTextColor(MTextOnColor);
          APainter->drawText(MRect,MText,MTextAlign);
        } else {
          APainter->setFillColor(MBackOffColor);
          APainter->fillRect(MRect);
          APainter->setTextColor(MTextOffColor);
          APainter->drawText(MRect,MTextOff,MTextAlign);
        }
      //}
      //else {
      //  if (MValue > 0.5) {
      //    APainter->setFillColor(MBackOnColorInactive);
      //    APainter->fillRect(MRect);
      //    APainter->setTextColor(MTextOnColorInactive);
      //    APainter->drawText(MRect,MText,MTextAlign);
      //  } else {
      //    APainter->setFillColor(MBackOffColorInactive);
      //    APainter->fillRect(MRect);
      //    APainter->setTextColor(MTextOffColorInactive);
      //    APainter->drawText(MRect,MTextOff,MTextAlign);
      //  }
      //}

      SWidget::on_paint(APainter,ARect,AMode);
    }

    //----------

    //virtual
    void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      SWidget::on_mouseDown(AXpos,AYpos,AButton,AState);
      //if (MValue < 0.5) MValue = 1; else MValue = 0;
      if (AButton==smb_left) {
        MValue = 1;
        do_update(this);
        do_redraw(this,MRect);
      }
    }

    //----------

    //virtual
    void on_mouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      SWidget::on_mouseUp(AXpos,AYpos,AButton,AState);
      //if (MValue < 0.5) MValue = 1; else MValue = 0;
      if (AButton==smb_left) {
        MValue = 0;
        do_update(this);
        do_redraw(this,MRect);
      }
    }


};

//----------------------------------------------------------------------
#endif
