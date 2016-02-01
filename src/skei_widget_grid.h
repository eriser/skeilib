#ifndef skei_widget_grid_included
#define skei_widget_grid_included
//----------------------------------------------------------------------

class SWidget_Grid : public SWidget {

  protected:
    int32   MWidth;       // columns
    int32   MHeight;      // rows
    SColor  MBackColor;
    SColor  MGridColor;

  public:

    int32 width(void) { return MWidth; }
    int32 height(void) { return MHeight; }
    void  width(int32 AWidth) { MWidth = AWidth; }
    void  height(int32 AHeight) { MHeight = AHeight; }

  public:

    SWidget_Grid(SRect ARect, int32 AWidth, int32 AHeight, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_Grid";
      MHint = "grid";
      MWidth = AWidth;//3;
      MHeight = AHeight;//3;
      //MCursor = kmc_Finger;
      MBackColor = SLightGrey;
      MGridColor = SDarkGrey;
    }

    /*
      TODO: only draw visible cells..
    */

    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      if ((MWidth > 0) && (MHeight > 0)) {
        float xcell = ( (float)MRect.w / (float)MWidth );
        float ycell = ( (float)MRect.h / (float)MHeight );
        // background
        //APainter->setFillColor(MBackColor);
        //APainter->fillRect(MRect.x,MRect.y,MRect.x2-1,MRect.y2-1);
        // cells
        for (int32 xx=0; xx<MWidth; xx++) {
          for (int32 yy=0; yy<MHeight; yy++) {
            SRect R = SRect( MRect.x+floorf(xx*xcell), MRect.y+floorf(yy*ycell), floorf(xcell), floorf(ycell) );
            on_paintCell(APainter,R,xx,yy);
          }
        }
        // grid
        float x = (float)MRect.x + xcell - 1;
        float y = (float)MRect.y + ycell - 1;
        //aCanvas->selectPen(mGridPen);
        APainter->setDrawColor( MGridColor );
        if (MWidth > 1) {
          for (int32 i=0; i<MWidth-1; i++) {
            APainter->drawLine( floorf(x), MRect.y, floorf(x), MRect.y2() );
            x += xcell;
          }
        }
        if (MHeight > 1) {
          for (int32 i=0; i<MHeight-1; i++) {
            APainter->drawLine( MRect.x, floorf(y), MRect.x2(), floorf(y) );
            y += ycell;
          }
        }
      }
      //APainter->setDrawColor(MBorderColor);
      //APainter->drawRect(MRect.x,MRect.y,MRect.x2{-1},MRect.y2{-1});
    }

    void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      float xcell = ( (float)MRect.w / (float)MWidth );
      float ycell = ( (float)MRect.h / (float)MHeight );
      int32 x = floorf( (float)(AXpos-MRect.x) / xcell );
      int32 y = floorf( (float)(AYpos-MRect.y) / ycell );
      on_clickCell(x,y,AButton);
      do_update(this);
    }

  public:

    virtual void on_clickCell(int32 AX, int32 AY, int32 AB) {}
    virtual void on_paintCell(SPainter* APainter, SRect ARect, int32 AX, int32 AY) {}

};

//----------------------------------------------------------------------
#endif

