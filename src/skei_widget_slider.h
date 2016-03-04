#ifndef skei_widget_slider_included
#define skei_widget_slider_included
//----------------------------------------------------------------------

#include "skei_widget_value.h"

//----------

class SWidget_Slider
: public SWidget_Value {

  protected:
    SColor  MBarColor;
    bool    MDrawName;

  public:

    SWidget_Slider(SRect ARect, float AValue, uint32 AAlignment=swa_none)
    : SWidget_Value(ARect,AValue,AAlignment) {
      MName  = "SWidget_Slider";
      MHint = "slider";
      MBackColor = SGrey;
      MBarColor = SDarkGrey;
      MTextColor = SLightYellow;
      MValueColor = SLightGrey;
      MDrawName = true;
      //MCursor = smc_arrowUpDown;
    }

    virtual ~SWidget_Slider() {
    }

    //----------

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      float fw = (float)MRect.w * MValue;
      int32 w = (int)fw;

      const char* nt;
      const char* vt;
      if (MParameter) {
        SParameter* param = (SParameter*)MParameter;
        nt = param->name();
        vt = param->displayText(MValue);
      }
      else {
        nt = MName;
        SFloatToString(MTextBuf,MValue);
        //KFloatToString(txt,FValue);
        vt = MTextBuf;//txt;
      }

      // bar

      if (w > 0) {
        APainter->setFillColor(MBarColor);
        APainter->fillRect(MRect.x,MRect.y,MRect.x+w-1,MRect.y2());

        // border
        //ACanvas.setDrawColor(FBorderColor);
        //ACanvas.drawRect(FRect.x,FRect.y,FRect.x+w-1,FRect.y2);
      }

      // background

      if (w < MRect.w) {
        APainter->setFillColor(MBackColor);
        APainter->fillRect(MRect.x+w,MRect.y,MRect.x2(),MRect.y2());

      }

      // border
      //ACanvas.setDrawColor(FBorderColor);
      //ACanvas.drawRect(FRect.x,FRect.y,FRect.x2,FRect.y2);

      // name
      if (MDrawName) {
        //nt = MName;
        APainter->setTextColor(MTextColor);
        APainter->drawText(MRect.x+4,MRect.y,MRect.x2(),MRect.y2(),nt/*FName*/, sta_left);
      }

      // value
      APainter->setTextColor(MValueColor);
      APainter->drawText(MRect.x,MRect.y,MRect.x2()-4,MRect.y2(),vt/*FTextBuf*/, sta_right/*center*/);
    }

};

//----------------------------------------------------------------------
#endif
