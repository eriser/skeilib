#ifndef skei_widget_scrollbar_included
#define skei_widget_scrollbar_included
//----------------------------------------------------------------------

#include "skei_widget.h"

//----------

class SWidget_ScrollBar
: public SWidget {

  private:
    bool    MIsDragging;
    float   MPixelSize;
    int32   MClickX;
    int32   MClickY;

  protected:

    SColor  MBackColor;
    SColor  MThumbColor;
    float   MThumbSize;   // 0..1

  public:

    void thumbsize(float ASize) { MThumbSize = ASize; };

  public:

    SWidget_ScrollBar(SRect ARect, float AValue, bool AVertical=false, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_ScrollBar";
      MHint = "scrollbar";
      MIsDragging = false;
      MThumbSize = 0.4;
      MValue = AValue;
      if (AVertical) {
        setFlag(swf_vertical); // setFlag(MFlags,swf_vertical);
        MCursor = smc_arrowUpDown;
      }
      else {
        clearFlag(swf_vertical); // clearFlag(MFlags, swf_vertical);
        MCursor = smc_arrowLeftRight;
      }
      MBackColor = SDarkGrey;
      MThumbColor = SLightGrey;
    }

    virtual ~SWidget_ScrollBar() {
    }

  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {

      APainter->setFillColor(MBackColor);
      APainter->fillRect(MRect);

      //APainter->setDrawColor(MBorderColor);
      //APainter->drawRect(MRect.x,MRect.y,MRect.x2,MRect.y2);
      if (hasFlag(swf_vertical)) {
        float r = (float)MRect.h;
        //KTrace("draw vertical");
        int32 thumb = floorf( r * MThumbSize );
        int32 ipos  = floorf( (r-thumb) * MValue );
        int32 x = MRect.x;
        int32 y = MRect.y + ipos;
        int32 x2 = MRect.x2();
        int32 y2 = y + thumb;
        int32 w = (x2-x-0);
        int32 h = (y2-y-1);

        APainter->setFillColor(MThumbColor);
        APainter->fillRect(x,y,x+w,y+h);
        //APainter->setDrawColor(MBorderColor);
        //APainter->drawRect(x,y,x+w,y+h);
      }
      else {
        // horizontal
        float r = (float)MRect.w;
        int32 thumb = floorf( r * MThumbSize );
        int32 ipos  = floorf( (r-thumb) * MValue );
        int32 x = MRect.x + ipos;
        int32 y = MRect.y;
        int32 x2 = x + thumb;
        int32 y2 = MRect.y2();
        int32 w = (x2-x-1);
        int32 h = (y2-y+0);
        APainter->setFillColor(MThumbColor);
        APainter->fillRect(x,y,x+w,y+h);
        //APainter->setDrawColor(MBorderColor);
        //APainter->drawRect(x,y,x+w,y+h);
      }

    }

    //virtual
    void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (AButton == smb_left) {
        MIsDragging = true;
        int32 thumbsize, numpixels;
        if (hasFlag(swf_vertical)) {
          //STrace("click vertical\n");
          thumbsize = floorf( (float)MRect.h * MThumbSize );
          numpixels = MRect.h - thumbsize;
        }
        else {
          thumbsize = floorf( (float)MRect.w * MThumbSize );
          numpixels = MRect.w - thumbsize;
        }
        if (numpixels <= 0) numpixels = 1;  // HACK
        MPixelSize = 1.0 / (float)numpixels;
        MClickX = AXpos;
        MClickY = AYpos;
      }
    }

    //virtual
    void on_mouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (AButton == smb_left) MIsDragging = false;
    }

    //virtual
    void on_mouseMove(int32 AXpos, int32 AYpos, int32 AState) {
      if (MIsDragging) {
        int32 deltax = AXpos - MClickX;
        int32 deltay = AYpos - MClickY;
        if (hasFlag(swf_vertical)) {
          //KTrace("move vertical");
          MValue += ( (float)deltay * MPixelSize );
        }
        else {
          MValue += ( (float)deltax * MPixelSize );
        }
        MValue = SClamp(MValue,0,1);
        do_update(this);
        do_redraw(this,MRect);
        MClickX = AXpos;
        MClickY = AYpos;
      }
    }


};

//----------------------------------------------------------------------
#endif

