#ifndef skei_widget_sizer_included
#define skei_widget_sizer_included
//----------------------------------------------------------------------

#include "skei_widget.h"

//----------

class SWidget_Sizer
: public SWidget {


  protected:
    int32     prevx,prevy;
    bool      MIsDragging;
    SWidget*  MTarget;
    //int32     MSizeCursor;
    int32     MMode;
    SColor    MFillColor;

  public:
    //property target : KWidget read FTarget write FTarget;
    //property cursor : longint read FSizeCursor write FSizeCursor;
    //property fillcolor : KColor read FFillColor write FFillColor;

  public:

    SWidget_Sizer(SRect ARect, uint32 AMode=ssm_none, SWidget* ATarget=SKEI_NULL, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName       = "SWidget_Sizer";
      MHint       = "sizer";
      MTarget     = ATarget;
      MIsDragging = false;
      MMode       = AMode;
      switch(MMode) {
        case ssm_none:     MCursor = smc_default; break;
        case ssm_left:     MCursor = smc_arrowLeftRight; break;
        case ssm_right:    MCursor = smc_arrowLeftRight; break;
        case ssm_top:      MCursor = smc_arrowUpDown; break;
        case ssm_bottom:   MCursor = smc_arrowUpDown; break;
        case ssm_window:   MCursor = smc_arrowDiagLeft; break;
      }
      MFillColor = SDarkRed;
    }

    virtual ~SWidget_Sizer() {
    }

  public:

    virtual void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      APainter->setFillColor(MFillColor);
      if (MMode == ssm_window) APainter->fillTri( MRect.x2()-1, MRect.y, MRect.x2()-1, MRect.y2()-1, MRect.x, MRect.y2()-1 );
      else APainter->fillRect(MRect);
    }

    virtual void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (AButton == smb_left) {
        MIsDragging = true;
        prevx = AXpos;
        prevy = AYpos;
      }
    }

    virtual void on_mouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (AButton == smb_left) MIsDragging = false;
    }

    virtual void on_mouseMove(int32 AXpos, int32 AYpos, int32 AState) {
      if (MIsDragging) {
        int32 deltax = AXpos - prevx;
        int32 deltay = AYpos - prevy;
        switch(MMode) {
          case ssm_left:
            deltay = 0;
            break;
          case ssm_right:
            deltay = 0;
            deltax = -deltax;
            break;
          case ssm_top:
            deltax = 0;
            break;
          case ssm_bottom:
            deltax = 0;
            deltay = -deltay;
            break;
          //ksm_Window:
          //  begin
          //  end;
          //  break;

        } // switch mode
        //if FMode = ksm_Horizontal then deltax := 0;
        //if FMode = ksm_Vertical then deltay := 0;
        if (MMode == ssm_window) {
          if (MParent) MParent->do_sizer(this,deltax,deltay,MMode);
        }
        else {
          //STrace("calling target\n");
          if (MTarget) MTarget->do_sizer(/*this*/MTarget,deltax,deltay,MMode);
          //else self.do_sizer(self,deltax,deltay,FMode);
        }
        prevx = AXpos;
        prevy = AYpos;
      } // dragging
      //axWidget::doMouseMove(aXpos,aYpos,aButton);
    }

};

//----------------------------------------------------------------------
#endif

