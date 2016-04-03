#ifndef skei_widget_value_included
#define skei_widget_value_included
//----------------------------------------------------------------------

#include "skei_widget.h"

//----------

class SWidget_Value
: public SWidget {

  private:
    float   MSens;
  protected:
    SColor  MBackColor;
    SColor  MTextColor;
    SColor  MValueColor;
    int32   MClickX;
    int32   MClickY;
    float   MClickVal;
    bool    MDrag;
    char    MTextBuf[256];
    bool    MHorizontal;
    float   MSensitivity;
    float   MSensitivity2;


  public:

    SWidget_Value(SRect ARect, float AValue, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_Value";
      MHint = "value";
      MValue        = AValue;
      MBackColor    = SDarkGrey;
      MValueColor   = SLightGrey;
      MTextColor    = SLightYellow;//Grey;
      MSens         = 0.0f;
      MName         = "SWidget_Value";
      MDrag         = false;
      MHorizontal   = false;
      MSensitivity  = 1.0f / 200.0f;
      MSensitivity2 = 1.0f / 2000.0f;
      MCursor       = smc_arrowUpDown;
      MTextBuf[0]   = 0;

      MClickVal = 0;

    }

    //----------

    virtual ~SWidget_Value() {
    }

  //----------------------------------------

  public:

    void backColor(SColor AColor) { MBackColor = AColor; }
    void valueColor(SColor AColor) { MValueColor = AColor; }
    void textColor(SColor AColor) { MTextColor = AColor; }
    void horizontal(bool AHoriz) { MHorizontal = AHoriz; }


  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      APainter->setFillColor(MBackColor);
      APainter->fillRect(MRect);
      const char* vv;
      const char* vt;
      if (MParameter) {
        SParameter* param = (SParameter*)MParameter;
        vv = param->displayText( MValue );
        vt = param->name();
      }
      else {
        SFloatToString(MTextBuf,MValue);
        vv = MTextBuf;
        vt = MName;
      }

      APainter->setTextColor(MTextColor);
      APainter->drawText(MRect.x+2,MRect.y,MRect.x2(),MRect.y2(),vt, sta_left);

      APainter->setTextColor(MValueColor);
      APainter->drawText(MRect.x,MRect.y,MRect.x2()-2,MRect.y2(),vv, sta_right);

      SWidget::on_paint(APainter,ARect,AMode);
    }

    //----------

    //virtual
    void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (AButton==smb_left) {
        MDrag     = true;
        MClickX   = AXpos;
        MClickY   = AYpos;
        MClickVal = MValue;
        if (AState&smk_ctrl) MSens = MSensitivity2; else MSens = MSensitivity;
      }
      if ((AButton==smb_right) && (AState&smk_ctrl)) {
        if (MParameter) {
          SParameter* param = (SParameter*)MParameter;
          MValue = param->defaultValue();
        }
        do_update(this);
        do_redraw(this,MRect);
      }
    }

    //----------

    //virtual
    void on_mouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      MDrag = false;
    }

    //----------

    //virtual
    void on_mouseMove(int32 AXpos, int32 AYpos, int32 AState) {
      //if KHasFlag(AState,kmb_ctrl) then s := MSensitivity2 else s := MSensitivity;
      if (MDrag) {
        float m;
        if (MHorizontal) m = (float)(AXpos-MClickX);
        else m = (float)(MClickY-AYpos);
        float v = MClickVal + (m*MSens);
        //MValue := KClamp(v,0,1);
        v = SClamp(v,0.0f,1.0f);
        if (v != MValue) {
          MValue = v;
          do_update(this);
          do_redraw(this,MRect);
        }
      }
    }

    //----------

};

//----------------------------------------------------------------------
#endif
