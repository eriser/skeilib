#ifndef skei_painter_included
#define skei_painter_included
//----------------------------------------------------------------------

#include "skei_integer.h"

#ifdef SKEI_LINUX
#include "skei_painter_xlib.h"
#endif // SKEI_LINUX

#ifdef SKEI_WIN32
#include "skei_painter_win32.h"
#endif // SKEI_LINUX

//----------

//typedef SPainter_Implementation SPainter;

class SPainter
: public SPainter_Implementation {

  public:

    SPainter(SDrawable* ADrawable)
    : SPainter_Implementation(ADrawable) {
    }

    virtual ~SPainter() {
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    virtual
    void tileSurface(int32 ADstX, int32 ADstY, int32 ADstW, int32 ADstH, SSurface* ASurface, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
    //var
    //  x,y,w,h : longint;
    //  ww,hh : longint;
      int32 y = ADstY;
      int32 h = ADstH;
      while (h > 0) {
        int32 hh = SMinI(h,ASrcH);
        int32 x = ADstX;
        int32 w = ADstW;
        while (w > 0) {
          int32 ww = SMinI(w,ASrcW);
          drawSurface(x,y,ASurface,ASrcX,ASrcY,ww,hh);
          x += ASrcW;
          w -= ASrcW;
        }
        y += ASrcH;
        h -= ASrcH;
      }
    }

    //----------

    // draw borders normally
    // stretch or tile inner part

    virtual
    //void flexSurface(SRect ARect, SSurface* ASurface, int32 AMode, int32 ALeft, int32 ATop, int32 ARight, int32 ABottom) {
    void flexSurface(int32 AX1, int32 AY1, int32 AX2, int32 AY2, SSurface* ASurface, int32 AMode, int32 ALeft, int32 ATop, int32 ARight, int32 ABottom) {
      int32 dx1 = AX1;
      int32 dx2 = AX1 + ALeft;
      int32 dx3 = AX1 + (AX2-AX1+1) - ARight;
      //int32 dx4 = AX1 + /*ARect.w*/(AX2-AX1+1);
      int32 dy1 = AY1;
      int32 dy2 = AY1 + ATop;
      int32 dy3 = AY1 + (AY2-AY1+1) - ABottom;
      //int32 dy4 = AY1 + (AY2-AY1+1);
      int32 dw1 = ALeft;
      int32 dw2 = (AX2-AX1+1) - ALeft - ARight;
      int32 dw3 = ARight;
      int32 dh1 = ATop;
      int32 dh2 = (AY2-AY1+1) - ATop - ABottom;
      int32 dh3 = ABottom;
      int32 sx1 = 0;
      int32 sx2 = 0 + ALeft;
      int32 sx3 = 0 + ASurface->width() - ARight;
      //int32 sx4 = 0 + ASurface._width;
      int32 sy1 = 0;
      int32 sy2 = 0 + ATop;
      int32 sy3 = 0 + ASurface->height() - ABottom;
      //int32 sy4 = 0 + ASurface._height;
      int32 sw1 = ALeft;
      int32 sw2 = ASurface->width() - ALeft - ARight;
      int32 sw3 = ARight;
      int32 sh1 = ATop;
      int32 sh2 = ASurface->height() - ATop - ABottom;
      int32 sh3 = ABottom;
      //if ((sw1 + sw3) <= /*ARect.w*/(AX2-AX1+1)) sw2 = 0;
      //if ((sh1 + sh3) <= /*ARect.h*/(AY2-AY1+1)) sh2 = 0;
      /* top */
      if (sh1 > 0) {
        if (sw1 > 0) drawSurface( dx1,dy1, ASurface, sx1,sy1,sw1,sh1);                            // top left
        if (sw2 > 0) {
          switch(AMode) {
            case sfm_stretch: stretchSurface( dx2,dy1,dw2,dh1, ASurface, sx2,sy1,sw2,sh1); break; // top
            case sfm_tile:    tileSurface( dx2,dy1,dw2,dh1, ASurface, sx2,sy1,sw2,sh1); break;    // top
          }
        }
        if (sw3 > 0) drawSurface( dx3,dy1, ASurface, sx3,sy1,sw3,sh1);                            // top right
      }
      /* middle */
      if (sh2 > 0) {
        switch(AMode) {
          case sfm_stretch: {
            if (sw1 > 0) stretchSurface( dx1,dy2,dw1,dh2, ASurface, sx1,sy2,sw1,sh2);             // left
            if (sw2 > 0) stretchSurface( dx2,dy2,dw2,dh2, ASurface, sx2,sy2,sw2,sh2);             // center
            if (sw3 > 0) stretchSurface( dx3,dy2,dw3,dh2, ASurface, sx3,sy2,sw3,sh2);             // right
          }
          case sfm_tile: {
            if (sw1 > 0) tileSurface( dx1,dy2,dw1,dh2, ASurface, sx1,sy2,sw1,sh2);                // left
            if (sw2 > 0) tileSurface( dx2,dy2,dw2,dh2, ASurface, sx2,sy2,sw2,sh2);                // center
            if (sw3 > 0) tileSurface( dx3,dy2,dw3,dh2, ASurface, sx3,sy2,sw3,sh2);                // right
          }
        }
      }
      /* bottom */
      if (sh3 > 0) {
        if (sw1 > 0) drawSurface( dx1,dy3, ASurface, sx1,sy3,sw1,sh3);                            // bottom left
        if (sw2 > 0) {
          switch(AMode) {
            case sfm_stretch: stretchSurface( dx2,dy3,dw2,dh3, ASurface, sx2,sy3,sw2,sh3); break; // bottom
            case sfm_tile:    tileSurface( dx2,dy3,dw2,dh3, ASurface, sx2,sy3,sw2,sh3); break;       // bottom
          }
        }
        if (sw3 > 0) drawSurface( dx3,dy3, ASurface, sx3,sy3,sw3,sh3);                            // bottom right
      }
      /* */
    }

    //----------

    virtual
    void drawRoundedRect(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 ARound) {
      int32 r  = ARound - 1;
      int32 r2 = r*2;
      drawArc( AX2-r2-1,AY1,     AX2-1,   AY1+r2, 0.00, 0.25 ); // upper right
      drawArc( AX2-r2-1,AY2-r2-1,AX2-1,   AY2-1,  0.25, 0.25 ); // lower right
      drawArc( AX1,     AY2-r2-1,AX1+r2,  AY2-1,  0.50, 0.25 ); // lower left
      drawArc( AX1,     AY1,     AX1+r2,  AY1+r2, 0.75, 0.25 ); // upper left
      drawLine(AX1+r+1, AY1,     AX2-r-1, AY1     );  // top
      drawLine(AX1+r+1, AY2  ,   AX2-r-1, AY2     );  // bottom
      drawLine(AX1,     AY1+r+1, AX1,     AY2-r-1 );  // left
      drawLine(AX2  ,   AY1+r+1, AX2,     AY2-r-1 );  // right
    }

    //----------

    virtual
    void fillRoundedRect(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 ARound) {
      int32 r  = ARound - 1;
      int32 r2 = r * 2;
      fillArc( AX2-r2-1,AY1,     AX2-1,   AY1+r2, 0.00, 0.25 ); // upper right
      fillArc( AX2-r2-1,AY2-r2-1,AX2-1,   AY2-1,  0.25, 0.25 ); // lower right
      fillArc( AX1,     AY2-r2-1,AX1+r2,  AY2-1,  0.50, 0.25 ); // lower left
      fillArc( AX1,     AY1,     AX1+r2,  AY1+r2, 0.75, 0.25 ); // upper left
      fillRect(AX1+r+1, AY1,     AX2-r-1, AY1+r   );  // top
      fillRect(AX1,     AY1+r+1, AX2,     AY2-r-1 );  // mid
      fillRect(AX1+r+1, AY2-r,   AX2-r-1, AY2   );  // bot
    }

    //----------

    virtual
    void fillGradient(int32 AX1, int32 AY1, int32 AX2, int32 AY2, SColor AColor1, SColor AColor2, bool AVertical=true) {
      if (AVertical) {
        int32 h = AY2-AY1+1;
        if (h>0) {
          float v = 0;
          float vadd = 1.0f / (float)(h-1);
          for (int32 i=0; i<h; i++) {
            //STrace("v %.2f\n",v);
            SColor c = blendColor(v,AColor1,AColor2);
            v += vadd;
            setDrawColor(c);
            drawLine(AX1,AY1+i,AX2,AY1+i);
          } // for i
        } // h>0
      } // vertical
      else {
        int32 w = AX2-AX1+1;
        if (w>0) {
          float v = 0;
          float vadd = 1.0f / (float)(w-1);
          for (int32 i=0; i<w; i++) {
            //STrace("v %.2f\n",v);
            SColor c = blendColor(v,AColor1,AColor2);
            v += vadd;
            setDrawColor(c);
            drawLine(AX1+i,AY1,AX1+i,AY2);
          } // for i
        } // h>0
      }
    }

  //----------------------------------------
  // helpers
  //----------------------------------------

  public:

    inline
    void tileSurface(SRect ADst, SSurface* ASurface, SRect ASrc) {
      tileSurface(ADst.x, ADst.y, ADst.w, ADst.h, ASurface, ASrc.x, ASrc.y, ASrc.w, ASrc.h);
    }

    inline
    void flexSurface(SRect ARect, SSurface* ASurface, int32 AMode, int32 ALeft, int32 ATop, int32 ARight, int32 ABottom) {
      flexSurface(ARect.x, ARect.y, ARect.x2(), ARect.y2(), ASurface, AMode, ALeft, ATop, ARight, ABottom);
    }

    inline
    void drawRoundedRect(SRect ARect, int32 ARound) {
      drawRoundedRect(ARect.x,ARect.y,ARect.x2(),ARect.y2(),ARound);
    }

    inline
    void fillRoundedRect(SRect ARect, int32 ARound) {
      fillRoundedRect(ARect.x,ARect.y,ARect.x2(),ARect.y2(),ARound);
    }

};

//----------------------------------------------------------------------
#endif
