#ifndef skei_painter_base_included
#define skei_painter_base_included
//----------------------------------------------------------------------

#include "skei_drawable.h"
#include "skei_point.h"
#include "skei_rect.h"
#include "skei_surface.h"
#include "skei_image.h"

//----------

class SPainter_Base {

  public:

    virtual void    setDrawColor(SColor AColor) {}
    virtual void    setFillColor(SColor AColor) {}
    virtual void    setTextColor(SColor AColor) {}
    virtual void    setTextSize(int32 ASize) {}
    virtual void    setFontColor(SColor AColor) {}
    virtual void    setBlendMode(int32 AMode) {}

    virtual int32   getTextWidth(const char* AText) { return 0; }
    virtual int32   getTextHeight(const char* AText) { return 0; }
    virtual int32   getFontWidth(const char* AText) { return 0; }
    virtual int32   getFontHeight(const char* AText) { return 0; }

    virtual void    drawPoint(int32 AX, int32 AY) {}
    virtual void    drawLine(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void    drawRect(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void    drawEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void    drawArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {}
    virtual void    drawTri(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {}

    virtual void    drawText(int32 AX, int32 AY, const char* AText) {}
    virtual void    drawText(int32 AX1, int32 AY1, int32 AX2, int32 AY2, const char* AText, uint32 AAlign) {}
    virtual void    drawFont(void* AFont, int32 x, int32 y,const char* text) {}

    virtual void    fillRect(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void    fillEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void    fillArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {}
    virtual void    fillTri(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {}

    virtual void    blendRect(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}

    virtual void    drawImage(int32 ADstX, int32 ADstY, SImage* AImage, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {}
    virtual void    drawSurface(int32 ADstX, int32 ADstY, SSurface* ASurface, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {}
    virtual void    blendSurface(int32 ADstX, int32 ADstY, SSurface* ASurface, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {}
    virtual void    stretchSurface(int32 ADstX, int32 ADstY, int32 ADstW, int32 ADstH, SSurface* ASurface, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {}

    virtual void    clip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void    noClip(void) {}
    virtual void    pushClip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void    popClip(void) {}

};

//----------------------------------------------------------------------
#endif


