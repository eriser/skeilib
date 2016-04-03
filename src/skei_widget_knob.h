#ifndef skei_widget_knob_included
#define skei_widget_knob_included
//----------------------------------------------------------------------

#include "skei_widget_value.h"

//----------

class SWidget_Knob
: public SWidget_Value {

  protected:
    //SColor MBarColor;
    SColor MCenterColor;

  public:

    SWidget_Knob(SRect ARect, float AValue, uint32 AAlignment=swa_none)
    : SWidget_Value(ARect,AValue,AAlignment) {
      MName = "SWidget_Knob";
      MHint = "knob";
      MBackColor = SGrey;
      //MBarColor = SWhite;
      MValueColor = SWhite;
      MCenterColor = SLightGrey;
      MCursor = smc_arrowUpDown;
    }

    virtual ~SWidget_Knob() {
    }

  public:

    //void barColor(SColor AColor) { MBarColor = AColor; }
    void centerColor(SColor AColor) { MCenterColor = AColor; }



  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      int32 s = MRect.h / 5; //(3*2);
      // background
      //APainter->setFillColor(MBackColor);
      //APainter->fillRect(MRect);
      //APainter->setDrawColor(FBorderColor);
      //APainter->drawRect(FRect.x,FRect.y,FRect.x2,FRect.y2);

      // background arc

      APainter->setFillColor(MBackColor);
      APainter->fillArc(MRect.x+3,MRect.y+3,MRect.x2()-3,MRect.y2()-3,-0.4,0.8);

      // value arc

      APainter->setFillColor(MValueColor);
      APainter->fillArc(MRect.x+3,MRect.y+3,MRect.x2()-3,MRect.y2()-3,-0.4,MValue*0.8);

      // clear center

      APainter->setFillColor(MCenterColor);
      APainter->fillArc(MRect.x+3+s,MRect.y+3+s,MRect.x2()-3-s,MRect.y2()-3-s,0,1);

    }

};

//----------------------------------------------------------------------
#endif

