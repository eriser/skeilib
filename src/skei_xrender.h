#ifndef skei_xrender_included
#define skei_xrender_included
//----------------------------------------------------------------------

#include <X11/extensions/Xrender.h>

#include "skei_color.h"
#include "skei_drawable.h"

//----------------------------------------------------------------------
// should these really be here?
//----------------------------------------------------------------------

static XRenderPictFormat xrender_argb_templ = {
  0,              // id
  PictTypeDirect, // type
  32,             // depth
  {               // direct
    16,           // direct.red
    0xff,         // direct.redMask
    8,            // direct.green
    0xff,         // direct.greenMask
    0,            // direct.blue
    0xff,         // direct.blueMask
    24,           // direct.alpha
    0xff,         // direct.alphaMask
  },
  0,              // colormap
};

//----------

static uint32 xrender_argb_templ_mask =
  PictFormatType      |
  PictFormatDepth     |
  PictFormatRed       |
  PictFormatRedMask   |
  PictFormatGreen     |
  PictFormatGreenMask |
  PictFormatBlue      |
  PictFormatBlueMask  |
  PictFormatAlpha     |
  PictFormatAlphaMask;

//----------------------------------------------------------------------

/*
  http://www.winehq.org/pipermail/wine-patches/2005-August/020119.html
  Avoid using XRenderFindStandardFormat as older libraries don't have it
*/

static __SKEI_UNUSED
XRenderPictFormat* xrender_find_format(Display* ADisplay, int32 ADepth) {
  //case ADepth of
  //  24: result := XRenderFindStandardFormat(FDisplay,PictStandardRGB24);
  //  32: result := XRenderFindStandardFormat(FDisplay,PictStandardRGB32);
  //end;
  switch(ADepth) {
    case 24: xrender_argb_templ.depth = 24; break;
    case 32: xrender_argb_templ.depth = 32; break;
  }
  return XRenderFindFormat(ADisplay, xrender_argb_templ_mask, &xrender_argb_templ, 0);
}

//----------------------------------------------------------------------

static __SKEI_UNUSED
//void create_picture(Drawable ADrawable) {
Picture xrender_create_picture(Display* ADisplay, Drawable ADrawable, int32 ADepth) {
  XRenderPictFormat*        fmt;
  XRenderPictureAttributes  pict_attr;
  int32                     pict_bits;
  /*
  if FDepth = 24 then  fmt := find_format(FDisplay,24{PictStandardRGB24})
  else fmt := find_format(FDisplay,32{PictStandardARGB32});
  */
  if (ADepth == 24) fmt = XRenderFindStandardFormat(ADisplay,PictStandardRGB24);
  else fmt = XRenderFindStandardFormat(ADisplay,PictStandardARGB32);
  pict_attr.poly_edge = PolyEdgeSmooth;
  pict_attr.poly_mode = PolyModeImprecise;
  /*
  pict_attr.component_alpha = true;
  */
  pict_bits = /*CPComponentAlpha |*/ CPPolyEdge | CPPolyMode;
  return  XRenderCreatePicture(
    ADisplay,
    ADrawable,//MPixmap,
    fmt,
    pict_bits,
    &pict_attr
  );
}

//----------

static __SKEI_UNUSED
void xrender_delete_picture(Display* ADisplay, Picture APicture) {
  XRenderFreePicture(ADisplay,APicture);
}

//----------------------------------------------------------------------

static __SKEI_UNUSED
XRenderColor xrender_make_color(SColor AColor) {
  XRenderColor col;
  col.red   = (int)((AColor.r*AColor.a) * 255) << 8;
  col.green = (int)((AColor.g*AColor.a) * 255) << 8;
  col.blue  = (int)((AColor.b*AColor.a) * 255) << 8;
  col.alpha = (int)((AColor.a) * 255) << 8;
  //col.red   = (uint16)(AColor.r * 65535);
  //col.green = (uint16)(AColor.g * 65535);
  //col.blue  = (uint16)(AColor.b * 65535);
  //col.alpha = (uint16)(AColor.a * 65535);
  //col.flags = (DoRed | DoGreen | DoBlue);
  //XAllocColor(MDisplay,XDefaultColormap(MDisplay,0),&col);
  return col;
}

//----------

static __SKEI_UNUSED
Picture xrender_make_brush(SDrawable* ADrawable, SColor AColor)
{
  Display* display  = ADrawable->display();
  Drawable drawable = ADrawable->drawable();
  int32    depth    = ADrawable->depth();
  int32    screen   = DefaultScreen(display);
  Visual*  visual   = DefaultVisual(display,screen);
  XRenderPictFormat *fmt = XRenderFindVisualFormat(display,visual);
  XRenderColor color = xrender_make_color(AColor);
  Pixmap pm = XCreatePixmap(display,drawable,1,1,depth);
  XRenderPictureAttributes pict_attr;
  pict_attr.repeat = 1;
  Picture picture = XRenderCreatePicture(display,pm,fmt,CPRepeat,&pict_attr);
  XRenderFillRectangle(display,PictOpOver,picture,&color,0,0,1,1);
  XFreePixmap(display,pm);
  return picture;
}

//----------

/*

Picture white_brush;
white_brush = create_brush(0xffff, 0xffff, 0xffff, 0xffff);

int make_circle(int cx, int cy, int radius, int max_ntraps, XTrapezoid traps[])
{
  int n=0, k=0, y1, y2;
  double w;
  while(k<max_ntraps) {
    y1=(int)(-radius*cos(M_PI*k/max_ntraps));
    traps[n].top=(cy+y1)<<16;
    traps[n].left.p1.y=(cy+y1)<<16;
    traps[n].right.p1.y=(cy+y1)<<16;
    w=sqrt(radius*radius-y1*y1)*65536;
    traps[n].left.p1.x=(int)((cx<<16)-w);
    traps[n].right.p1.x=(int)((cx<<16)+w);
    do {
      k++;
      y2=(int)(-radius*cos(M_PI*k/max_ntraps));
    } while(y1==y2);
    traps[n].bottom=(cy+y2)<<16;
    traps[n].left.p2.y=(cy+y2)<<16;
    traps[n].right.p2.y=(cy+y2)<<16;
    w=sqrt(radius*radius-y2*y2)*65536;
    traps[n].left.p2.x=(int)((cx<<16)-w);
    traps[n].right.p2.x=(int)((cx<<16)+w);
    n++;
  }
  return n;
}

Picture drawingarea = XRenderCreatePicture()
XTrapezoid traps[30];
int ntraps=make_circle(x,y,r,30,traps);
XRenderCompositeTrapezoids(display,PictOpOver,white_brush,drawingarea,0,0,0,traps,ntraps);

*/

//----------------------------------------------------------------------
#endif


