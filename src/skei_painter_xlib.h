#ifndef skei_painter_xlib_included
#define skei_painter_xlib_included
//----------------------------------------------------------------------

/*
  XSetFillStyle(display, gc, FillSolid);
  XSetLineAttributes(display, gc, 2, LineSolid, CapRound, JoinRound);

  XPoint points[] = { {0, 0}, {15, 15}, {0, 15}, {0, 0} };
  int npoints = sizeof(points)/sizeof(XPoint);
  XDrawLines(display, win, gc, points, npoints, CoordModeOrigin);

*/

//----------------------------------------------------------------------

/*

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <pthread.h>
#include <unistd.h> // sleep

// $ freetype-config --cflags
// -I/usr/include/freetype2
#include <X11/Xft/Xft.h>

#ifdef SKEI_XRENDER
#include <X11/extensions/Xrender.h>
#include "skei_xrender.h"
#endif

*/

#include "skei_color.h"
#include "skei_drawable.h"
#include "skei_math.h"
#include "skei_painter_base.h"
#include "skei_point.h"
#include "skei_rect.h"
#include "skei_stack.h"
#include "skei_string.h"
#include "skei_surface.h"

//----------------------------------------------------------------------

#ifdef SKEI_XRENDER
static XTransform skei_identity_matrix = {{
  { XDoubleToFixed(1), XDoubleToFixed(0), XDoubleToFixed(0) },
  { XDoubleToFixed(0), XDoubleToFixed(1), XDoubleToFixed(0) },
  { XDoubleToFixed(0), XDoubleToFixed(0), XDoubleToFixed(1) }
}};
#endif

typedef SStack<SRect,SKEI_CANVAS_CLIPSTACK_SIZE> SClipStack;

//----------------------------------------------------------------------

class SPainter_Xlib;
typedef SPainter_Xlib SPainter_Implementation;

//----------

class SPainter_Xlib
: public SPainter_Base {

  private:
    SDrawable*    sdrawable;

  private:
    Display*      MDisplay;   // from drawable
    Drawable      MDrawable;  // from drawable

    Visual*       MVisual;
    Colormap      MColorMap;

    GC            MGC;
    XGCValues     MGCvalues;

    XFontStruct*  MFont;

    #ifdef SKEI_XFT
    XftDraw*      MXftDraw;
    XftColor      MXftColor;
    bool          xft_color_allocated;
    #endif

    #ifdef SKEI_XRENDER
    Picture       MPicture;
    #endif

    SColor  MDrawColor;
    SColor  MFillColor;
    SColor  MTextColor;
    SColor  MFontColor;
    int32   MTextSize;
    int32   MBlendMode;

  private:
    SRect         MClipRect;
    SClipStack    MClipStack;
    //FVisibleRect: SRect;

  public:
    SRect         cliprect(void)        { return MClipRect; }
    void          cliprect(SRect ARect) { MClipRect=ARect; }
    Display*      display(void)         { return MDisplay; }
    Drawable      drawable(void)        { return MDrawable; }
    GC            gc(void)              { return MGC; }
    XFontStruct*  font(void)            { return MFont; }

    //SRect visiblerect(void)         { return MVisibleRect; }
    //void  visiblerect(SRect ARect)  { MVisibleRect=ARect; }

  public:

    /*
      The XCreateGC() function creates a graphics context and returns a GC.
      The GC can be used with any destination drawable having the same root
      and depth as the specified drawable.

      ---

      todo:
        if a_Ptr=NULL: drawable = root window (desktop)
        NULL window-painter
        else surface-painter
    */

    SPainter_Xlib(SDrawable* ADrawable)
    /*: SPainter_Base(ADrawable)*/ {

      sdrawable = ADrawable;

      MDisplay  = ADrawable->display();
      MDrawable = ADrawable->drawable();

      MVisual = DefaultVisual(MDisplay,DefaultScreen(MDisplay));
      MColorMap = DefaultColormap(MDisplay,DefaultScreen(MDisplay));

      MGC       = XCreateGC(MDisplay,MDrawable,0,&MGCvalues);
      MFont     = XQueryFont(MDisplay,XGContextFromGC(MGC));
      #ifdef SKEI_XFT
      MXftDraw = XftDrawCreate(
        MDisplay,
        MDrawable,
        DefaultVisual(MDisplay,DefaultScreen(MDisplay)),
        DefaultColormap(MDisplay,DefaultScreen(MDisplay))
      );
      #endif
      MClipRect.set(0,0,ADrawable->width(),ADrawable->height());
      #ifdef SKEI_XRENDER
        MPicture = ADrawable->picture();
      #endif
      //MDrawColor  = SRed;
      //MFillColor  = SGreen;
      //MTextColor  = SBlue;
      //MTextSize   = 12;
      #ifdef SKEI_XRENDER
      MBlendMode = PictOpOver;
      #endif
      //font_color_allocated = false;
      #ifdef SKEI_XFT
      xft_color_allocated = false;
      #endif
    }

    //----------

    virtual ~SPainter_Xlib() {
      XFreeFontInfo(SKEI_NULL,MFont,1);   // 1??
      #ifdef SKEI_XFT
      if (xft_color_allocated) XftColorFree(MDisplay,MVisual,MColorMap,&MXftColor);
      XftDrawDestroy(MXftDraw);
      #endif
      XFreeGC(MDisplay,MGC);
      //TODO: delete picture etc...
      //self..noClip;
      //MClipStack.destroy;
      //inherited;



    }

  //----------------------------------------------------------------------
  // get/set
  //----------------------------------------------------------------------

  public:

    //virtual
    void setDrawColor(SColor AColor) {
      MDrawColor = AColor;
      XSetForeground(MDisplay,MGC,SRgb(AColor));
    }

    //----------

    //virtual
    void setFillColor(SColor AColor) {
      MFillColor = AColor;
      //XSetBackground(MDisplay,MGC,_rgb(AColor));
      XSetForeground(MDisplay,MGC,SRgb(AColor));
    }

    //----------

    //virtual
    void setTextColor(SColor AColor) {
      MTextColor = AColor;
      XSetForeground(MDisplay,MGC,SRgb(AColor));
    }

    //----------

    //virtual
    void setFontColor(SColor AColor) {
      MFontColor = AColor;
      #ifdef SKEI_XFT
      XRenderColor rcolor = xrender_make_color(AColor);
      if (xft_color_allocated) XftColorFree(MDisplay,MVisual,MColorMap,&MXftColor);
      XftColorAllocValue(MDisplay,MVisual,MColorMap,&rcolor,&MXftColor);
      xft_color_allocated = true;
      #endif
    }

    //----------

    //virtual
    void setTextSize(int32 ASize) {
      MTextSize = ASize;
    }

    //----------

    //virtual
    int32 getTextWidth(const char* AText) {
      return XTextWidth(MFont, AText, SStrlen(AText));
    }

    //----------

    //virtual
    int32 getTextHeight(const char* AText) {
      return MFont->ascent + MFont->descent;
    }

    //----------

    //virtual
    int32 getFontWidth(const char* AText) {
      //XGlyphInfo extents;
      //XftTextExtents8(MDisplay,(XftFont*)AFont,(XftChar8 *)text,SStrlen(text),&extents);
      //return extents.width;
      return 0;
    }

    //----------

    //virtual
    int32 getFontHeight(const char* AText) {
      //XGlyphInfo extents;
      //XftTextExtents8(MDisplay,(XftFont*)AFont,(XftChar8 *)text,SStrlen(text),&extents);
      //return extents.height;
      return 0;
    }

    //----------

    void setBlendMode(int32 AMode) {
      MBlendMode = AMode;
    }

    //----------------------------------------------------------------------
    // shapes
    //----------------------------------------------------------------------

    //virtual
    void drawPoint(int32 AX, int32 AY) {
      XDrawPoint(MDisplay,MDrawable,MGC,AX,AY);
    }

    //----------

    //virtual
    void drawLine(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      XDrawLine(MDisplay,MDrawable,MGC,AX1,AY1,AX2,AY2);
    }

    //----------

    //virtual
    void drawRect(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      XDrawRectangle(MDisplay,MDrawable,MGC,AX1,AY1,AX2-AX1,AY2-AY1);
    }

    //----------

    //virtual
    void drawEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      XDrawArc(MDisplay,MDrawable,MGC,AX1,AY1,AX2-AX1,AY2-AY1,0*64,360*64);
    }

    //----------

    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise

    //virtual
    void drawArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {
      //if (abs(AAngle2) >= 0.01) { /*EPSILON*/
        // start angle = 12 o'clock
        float a1 = -AAngle1 + 0.25;
        // positive = clockwise, negative = counter-clockwise
        float a2 = -AAngle2;
        XDrawArc(MDisplay, MDrawable,MGC, AX1,AY1,(AX2-AX1+1),(AY2-AY1+1),(a1*(360.0f*64.0f)),(a2*(360.0f*64.0f)));
      //}
    }

    //----------

    //virtual
    void drawTri(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {
      XPoint v[4];
      v[0].x = AX1;
      v[0].y = AY1;
      v[1].x = AX2;
      v[1].y = AY2;
      v[2].x = AX3;
      v[2].y = AY3;
      v[3].x = AX1;
      v[3].y = AY1;
      XDrawLines(
        MDisplay,
        MDrawable,
        MGC,
        v,
        4,
        CoordModeOrigin
      );
    }


    //----------------------------------------------------------------------
    // text
    //----------------------------------------------------------------------

    //virtual
    void drawText(int32 AX, int32 AY, const char* AText) {
      XDrawString(MDisplay,MDrawable,MGC,AX,AY+MFont->ascent,AText,SStrlen(AText));
    }

    //----------

    //virtual
    void drawText(int32 AX1, int32 AY1, int32 AX2, int32 AY2, const char* AText, uint32 AAlign) {
      int32 x,y,w;
      if (AAlign & sta_top) y = AY1 + MFont->ascent;
      else if (AAlign & sta_bottom) y = AY2 - MFont->descent;
      else y = AY1 + (MFont->ascent >> 1) + ((AY2-AY1) >> 1);
      w = getTextWidth(AText);
      if (AAlign & sta_left) x = AX1;
      else if (AAlign & sta_right) x = AX2 - w;
      else x = AX1 + ((AX2 - AX1) >> 1) - ( w >> 1);
      XDrawString(MDisplay,MDrawable,MGC,x,y,AText, SStrlen(AText));
    }

    //----------

    //virtual
    void drawFont(void* AFont, int32 x, int32 y,const char* text) {
      #ifdef SKEI_XFT
      //XftFont* font = (XftFont*)AFont;
      //int ascent = font->ascent;
      XGlyphInfo extents;
      XftTextExtents8(MDisplay,(XftFont*)AFont,(XftChar8 *)text,SStrlen(text),&extents);
      if (xft_color_allocated) {
        //XftDrawSetClip(MXftDraw,Region r);
        XftDrawString8(MXftDraw,&MXftColor,(XftFont*)AFont,x,y + extents.y ,(XftChar8 *)text,SStrlen(text));
      }
      #endif
    }

    //----------------------------------------------------------------------
    // filled shapes
    //----------------------------------------------------------------------

    //virtual
    void fillRect(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      XFillRectangle(MDisplay,MDrawable,MGC,AX1,AY1,AX2-AX1+1,AY2-AY1+1);
    }

    //----------

    //virtual
    void fillEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      XFillArc(MDisplay,MDrawable,MGC,AX1,AY1,AX2-AX1,AY2-AY1,0*64,360*64);
    }

    //----------

    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise

    //virtual
    void fillArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {
      //if (abs(AAngle2) >= 0.01) { //EPSILON
        // start angle = 12 o'clock
        float a1 = -AAngle1 + 0.25;
        // positive = clockwise, negative = counter-clockwise
        float a2 = -AAngle2;
        XFillArc(MDisplay, MDrawable,MGC, AX1-1,AY1-1,(AX2-AX1+3),(AY2-AY1+3),(int)(a1*(360*64)),(int)(a2*(360*64)));
      //}
    }

    //----------

    //virtual
    void fillTri(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {
      XPoint v[3];
      v[0].x = AX1;
      v[0].y = AY1;
      v[1].x = AX2;
      v[1].y = AY2;
      v[2].x = AX3;
      v[2].y = AY3;
      //v[3].x = AX1;
      //v[3].y = AY1;
      XFillPolygon(
        MDisplay,
        MDrawable,
        MGC,
        v,
        3, // 4
        Nonconvex,        // Complex, Convex, Nonconvex
        CoordModeOrigin   // coordModeOrigin, CoordModePrevious
      );
    }

    //----------------------------------------------------------------------
    // blend
    //----------------------------------------------------------------------

    //virtual
    void blendLine(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      /*
      Picture pen = xrender_make_brush(sdrawable,MDrawColor);
      XPointDouble poly[4];
      int dx, dy;
      double len, ddx, ddy;
      dx = AX2 - AX1;
      dy = AY2 - AY1;
      len = sqrt((double)(dx * dx) + (double)(dy * dy));
      ddx = (0.5 * dx) / len;
      ddy = (0.5 * dy) / len;
      if (ddx < 0) ddx = -0.5 - ddx;
      else ddx = 0.5 - ddx;
      if (ddy < 0) ddy = -0.5 - ddy;
      else ddy = 0.5 - ddy;
      poly[0].x =  (AX1 + ddx);
      poly[0].y =  (AY1 - ddy);
      poly[1].x =  (AX2 + ddx);
      poly[1].y =  (AY2 - ddy);
      poly[2].x =  (AX2 - ddx);
      poly[2].y =  (AY2 + ddy);
      poly[3].x =  (AX1 - ddx);
      poly[3].y =  (AY1 + ddy);
      XRenderPictFormat* fmt = xrender_find_format(MDisplay,sdrawable->depth());
      XRenderCompositeDoublePoly(
        MDisplay,   // display
        MBlendMode, // mode
        pen,        // src
        MPicture,   // dst
        fmt,        // maskFormat
        0,0,0,0,    // xsrc,ysrc,xdst,ydst
        poly,       // points
        4,          // numpoints
        EvenOddRule // winding
      );
      XRenderFreePicture(MDisplay,pen);
      */
    }

    //----------

    //virtual
    void blendRect(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      #ifdef SKEI_XRENDER
      //XRenderColor col = _makeXRenderColor(MFillColor);
      XRenderColor col = xrender_make_color(MFillColor);
      XRenderFillRectangle(
        MDisplay,
        MBlendMode,//PictOpOver,
        MPicture,
        &col,
        AX1,
        AY1,
        (AX2-AX1+1),
        (AY2-AY1+1)
      );
      #endif
    }

    //----------------------------------------------------------------------
    // bitmaps
    //----------------------------------------------------------------------

    /*
      XPutImage(display, d, gc, image, src_x, src_y, dest_x, dest_y, width, height)
              Display *display;
              Drawable d;
              GC gc;
              XImage *image;
              int src_x, src_y;
              int dest_x, dest_y;
              unsigned int width, height;

      display Specifies the connection to the X server.
      d       Specifies the drawable.
      gc      Specifies the GC.
      image   Specifies the image you want combined with the rectangle.
      src_x   Specifies the offset in X from the left edge of the image defined by the XImage structure.
      src_y   Specifies the offset in Y from the top edge of the image defined by the XImage structure.
      dest_x, dest_y  Specify the x and y coordinates, which are relative to the origin of the drawable and are the coordinates of the subimage.
      width, height 	Specify the width and height of the subimage, which define the dimensions of the rectangle.
    */

    //virtual
    void drawImage(int32 ADstX, int32 ADstY, SImage* AImage, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
      XPutImage(
        MDisplay,
        MDrawable,
        MGC,
        (XImage*)AImage->image(),
        ASrcX,
        ASrcY,
        ADstX,
        ADstY,
        ASrcW,
        ASrcH
      );
    }

    //----------

    /*
      XCopyArea(display, src, dest, gc, src_x, src_y, width, height,  dest_x, dest_y)
            Display *display;
            Drawable src, dest;
            GC gc;
            int src_x, src_y;
            unsigned int width, height;
            int dest_x, dest_y;

      display       Specifies the connection to the X server.
      src, dest     Specify the source and destination rectangles to be combined.
      gc            Specifies the GC.
      src_x, src_y 	Specify the x and y coordinates, which are relative to the origin of the source rectangle and specify its upper-left corner.
      width,height 	Specify the width and height, which are the dimensions of both the source and destination rectangles.
      dest_x,dest_y Specify the x and y coordinates, which are relative to the origin of the destination rectangle and specify its upper-left corner
    */

    //virtual
    void drawSurface(int32 ADstX, int32 ADstY, SSurface* ASurface, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
      XCopyArea(
        MDisplay,
        ASurface->drawable(),
        MDrawable,
        MGC,
        ASrcX,
        ASrcY,
        ASrcW,
        ASrcH,
        ADstX,
        ADstY
      ); // mWinHandle = dst
    }

    //----------

    /*
      void XRenderComposite(
        Display       *dpy,
        int           op,
        Picture       src,
        Picture       mask,
        Picture       dst,
        int           src_x,
        int           src_y,
        int           mask_x,
        int           mask_y,
        int           dst_x,
        int           dst_y,
        unsigned int  width,
        unsigned int  height
      );
    */

    //virtual
    void blendSurface(int32 ADstX, int32 ADstY, SSurface* ASurface, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
      #ifdef SKEI_XRENDER
        //STrace("MPicture: %i\n",MPicture);
        //STrace("picture: %i\n",ASurface->picture());
        XRenderComposite(
          MDisplay,
          MBlendMode,//PictOpOver,
          ASurface->picture(),
          None,
          MPicture,
          ASrcX,
          ASrcY,
          0,
          0,
          ADstX,
          ADstY,
          ASrcW,
          ASrcH
        );
      #else
        //drawImage(aImage,aX,aY,aSrcX,aSrcY,aSrcW,aSrcH);
      #endif
    }

    //----------

    //virtual
    void stretchSurface(int32 ADstX, int32 ADstY, int32 ADstW, int32 ADstH, SSurface* ASurface, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
      #ifdef SKEI_XRENDER
      //XTransform xform;
      double xs,ys,zs;
      int32 x,y; //float
      Picture pic;
      //int32 op;
      if (ADstW > 0) {
        if (ADstH > 0) {
          xs = (float)ASrcW / (float)ADstW;
          ys = (float)ASrcH / (float)ADstH;
          zs = 1;

          //TXTransform = array[0..2] {row} of array[0..2] {col} of TXFixed;

          XTransform xform = {{
            { XDoubleToFixed(xs), XDoubleToFixed(0 ), XDoubleToFixed(0 ) },
            { XDoubleToFixed(0 ), XDoubleToFixed(ys), XDoubleToFixed(0 ) },
            { XDoubleToFixed(0 ), XDoubleToFixed(0 ), XDoubleToFixed(zs) }
          }};

          //Picture pic = aImage->getPicture();
          pic = ASurface->picture();
          XRenderSetPictureTransform(MDisplay, pic, &xform );
          //op = PictOpOver;
          // hmmm.. is srcx, srcy transformed by the matrix too?
          x = STrunc( (float)ASrcX / xs );
          y = STrunc( (float)ASrcY / ys );
          XRenderComposite(
            MDisplay,
            MBlendMode,//op,
            pic,
            None,
            MPicture, // !!!
            x,
            y,
            0,
            0,
            ADstX,
            ADstY,
            ADstW,
            ADstH
          );
          XRenderSetPictureTransform(MDisplay,pic,&skei_identity_matrix);
        } //h>0
      } //w>0
      #endif
    }

    //----------------------------------------------------------------------
    // clipping
    //----------------------------------------------------------------------

    //virtual
    //void clip(SRect ARect) {
    void clip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      MClipRect = SRect(AX1,AY1,AX2-AX1+1,AY2-AY1+1);
      XRectangle r;
      r.x      = AX1;     //ARect.x;
      r.y      = AY1;     //ARect.y;
      r.width  = AX2-AX1+2; //ARect.w + 1; // xlib seems to cut off one pixel to the right & bottom... ?
      r.height = AY2-AY1+2; //ARect.h + 1; // so we compensate by adding another pixel
      XSetClipRectangles(MDisplay,MGC,0,0,&r,1,Unsorted);
    }

    //----------

    //virtual
    void noClip(void) {
      XSetClipMask(MDisplay,MGC,None);
    }

    //----------

    //virtual
    //void pushClip(SRect ARect) {
    void pushClip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      MClipStack.push(MClipRect);
      clip(AX1,AY1,AX2,AY2);
      MClipRect = SRect(AX1,AY1,AX2-AX1+1,AY2-AY1+1);
    }

    //----------

    //virtual
    void popClip(void) {
      SRect r = MClipStack.pop();
      /*this.*/clip(r.x,r.y,r.x2(),r.y2());
      MClipRect = r;
    }

    //----------

    //SRect clipIntersection(SRect ARect) {
    //  return MClipRect.intersection(ARect);
    //}

    //----------

    //function visibleIntersection(ARect:SRect) : SRect;
    //{
    //  result := FVisibleRect.intersection(ARect);
    //}

  //----------------------------------------
  // painter_base helpers
  //----------------------------------------

  /*
    these should be in skei_painter (the SPainter class),
    but i get an error when i tried to put them there :-/
  */

  public:

    inline
    void drawPoint(SPoint APos) {
      drawPoint(APos.x,APos.y);
    }

    inline
    void drawLine(SRect ARect) {
      drawLine(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    inline
    void drawRect(SRect ARect) {
      drawRect(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    inline
    void drawEllipse(SRect ARect) {
      drawEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    inline
    void drawArc(SRect ARect, float AAngle1, float AAngle2) {
      drawArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
    }

    inline
    void drawTri(SPoint APoint1, SPoint APoint2, SPoint APoint3) {
      drawTri(APoint1.x,APoint1.y,APoint2.x,APoint2.y,APoint3.x,APoint3.y);
    }

    //----------

    inline
    void drawText(SPoint APoint, const char* AText) {
      drawText(APoint.x,APoint.y,AText);
    }

    inline
    void drawText(SRect ARect, const char* AText, uint32 AAlign) {
      drawText(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AText,AAlign);
    }

    //----------

    inline
    void fillRect(SRect ARect) {
      fillRect( ARect.x, ARect.y, ARect.x2(), ARect.y2() );
    }

    inline
    void fillEllipse(SRect ARect) {
      fillEllipse( ARect.x, ARect.y, ARect.x2(), ARect.y2() );
    }

    inline
    void fillArc(SRect ARect, float AAngle1, float AAngle2) {
      fillArc( ARect.x, ARect.y, ARect.x2(), ARect.y2(), AAngle1, AAngle2 );
    }

    inline
    void fillTri(SPoint APoint1, SPoint APoint2, SPoint APoint3) {
      fillTri(APoint1.x,APoint1.y,APoint2.x,APoint2.y,APoint3.x,APoint3.y);
    }

    //----------

    inline
    void blendRect(SRect ARect) {
      blendRect( ARect.x, ARect.y, ARect.x2(), ARect.y2() );
    }

    //----------

    inline
    void drawImage(SPoint ADst, SImage* AImage, SRect ASrc) {
      drawImage(ADst.x, ADst.y, AImage, ASrc.x, ASrc.y, ASrc.w, ASrc.h);
    }

    inline
    void drawSurface(SPoint ADst, SSurface* ASurface, SRect ASrc) {
      drawSurface(ADst.x, ADst.y, ASurface, ASrc.x, ASrc.y, ASrc.w, ASrc.h);
    }

    inline
    void blendSurface(SPoint ADst, SSurface* ASurface, SRect ASrc) {
      blendSurface(ADst.x, ADst.y, ASurface, ASrc.x, ASrc.y, ASrc.w, ASrc.h);
    }

    inline
    void stretchSurface(SRect ADst, SSurface* ASurface, SRect ASrc) {
      stretchSurface(ADst.x, ADst.y, ADst.w, ADst.h, ASurface, ASrc.x, ASrc.y, ASrc.w, ASrc.h);
    }

    //----------

    inline
    void clip(SRect ARect) {
      clip(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    inline
    void pushClip(SRect ARect) {
      pushClip(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

};

//----------------------------------------------------------------------
#endif
