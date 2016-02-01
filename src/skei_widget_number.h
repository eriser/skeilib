#ifndef skei_widget_number_included
#define skei_widget_number_included
//----------------------------------------------------------------------

#include "skei_widget.h"

//----------

class SWidget_Number
: public SWidget {

  protected:
    //float       MValue;
    SColor  MValueColor;
    uint32  MValueAlignment;
    SColor  MBackColor;
    bool    MDrawBackground;
    char    MTextBuf[256];

  public:

    SWidget_Number(SRect ARect, float AValue, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_Nodes";
      MHint = "number";
      //MText           = AText;
      MValue          = AValue;
      MValueColor     = SBlack;
      MValueAlignment = sta_left;
      MBackColor      = SDarkGrey;
      MDrawBackground = false;
    }

    //----------

    virtual ~SWidget_Number() {
    }

  //----------------------------------------

  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      if (MDrawBackground) {
        APainter->setFillColor(MBackColor);
        APainter->fillRect(MRect);
      }
      APainter->setTextColor(MValueColor);
      SFloatToString(MTextBuf,MValue);
      APainter->drawText(MRect,MTextBuf,MValueAlignment);
      SWidget::on_paint(APainter,ARect,AMode);
    }

    //----------

};

//----------------------------------------------------------------------
#endif


