#ifndef skei_color_included
#define skei_color_included
//----------------------------------------------------------------------

// float rgba <-> uint32

//----------

#include <math.h>
#include "skei_math.h"

struct SColor {

  float r,g,b,a;

  SColor() {
  }

  SColor(float ac) {
    r = ac;
    g = ac;
    b = ac;
    a = 1; // 0
  }

  SColor(float ar, float ag, float ab) {
    r = ar;
    g = ag;
    b = ab;
    a = 1; // 0
  }

  SColor(float ar, float ag, float ab, float aa) {
    r = ar;
    g = ag;
    b = ab;
    a = aa;
  }

  void set(float ac) {
    r = ac;
    g = ac;
    b = ac;
    a = 1; // 0
  }

  void set(float ar, float ag, float ab) {
    r = ar;
    g = ag;
    b = ab;
    a = 1; // 0
  }

  void set(float ar, float ag, float ab, float aa) {
    r = ar;
    g = ag;
    b = ab;
    a = aa;
  }

  // value = 0 = src
  // value = 1 = arg

  void blend(float value, float ar, float ag, float ab/*, float aa*/) {
    r = (r * (1-value)) + (ar * value);
    g = (g * (1-value)) + (ag * value);
    b = (b * (1-value)) + (ab * value);
    //a = (a * (1-value)) + (aa * value);
  }

  void blend(float value, SColor AColor) {
    blend(value,AColor.r,AColor.g,AColor.b/*,AColor.a*/);
  }

  /*
  todo:
    blend,mix,alpha,lerp
  */

};

//----------------------------------------------------------------------

//#define _L 0.8
//#define _M 0.6
//#define _D 0.4

//---

#define SWhite          SColor( 1.0, 1.0, 1.0 )

#define SBrightGrey     SColor( 1.0, 1.0, 1.0 )
#define SBrightRed      SColor( 1.0, 0.0, 0.0 )
#define SBrightOrange   SColor( 1.0, 0.5, 0.0 )
#define SBrightYellow   SColor( 1.0, 1.0, 0.0 )
#define SBrightGreen    SColor( 0.0, 1.0, 0.0 )
#define SBrightCyan     SColor( 0.0, 1.0, 1.0 )
#define SBrightBlue     SColor( 0.0, 0.0, 1.0 )
#define SBrightMagenta  SColor( 1.0, 0.0, 1.0 )

#define SLightGrey      SColor( 0.8, 0.8, 0.8 )
#define SLightRed       SColor( 0.8, 0.0, 0.0 )
#define SLightOrange    SColor( 0.8, 0.4, 0.0 )
#define SLightYellow    SColor( 0.8, 0.8, 0.0 )
#define SLightGreen     SColor( 0.0, 0.8, 0.0 )
#define SLightCyan      SColor( 0.0, 0.8, 0.8 )
#define SLightBlue      SColor( 0.0, 0.0, 0.8 )
#define SLightMagenta   SColor( 0.8, 0.0, 0.8 )

#define SGrey           SColor( 0.6, 0.6, 0.6 )
#define SRed            SColor( 0.6, 0.0, 0.0 )
#define SOrange         SColor( 0.6, 0.3, 0.0 )
#define SYellow         SColor( 0.6, 0.6, 0.0 )
#define SGreen          SColor( 0.0, 0.6, 0.0 )
#define SCyan           SColor( 0.0, 0.6, 0.6 )
#define SBlue           SColor( 0.0, 0.0, 0.6 )
#define SMagenta        SColor( 0.6, 0.0, 0.6 )

#define SDarkGrey       SColor( 0.4, 0.4, 0.4 )
#define SDarkRed        SColor( 0.4, 0.0, 0.0 )
#define SDarkOrange     SColor( 0.4, 0.2, 0.0 )
#define SDarkYellow     SColor( 0.4, 0.4, 0.0 )
#define SDarkGreen      SColor( 0.0, 0.4, 0.0 )
#define SDarkCyan       SColor( 0.0, 0.4, 0.4 )
#define SDarkBlue       SColor( 0.0, 0.0, 0.4 )
#define SDarkMagenta    SColor( 0.4, 0.0, 0.4 )

#define SBlack          SColor( 0.0, 0.0, 0.0 )

//---

//#undef L
//#undef M
//#undef D


//----------------------------------------------------------------------

/*
inline
uint32 rgba(KColor AColor) {
  float bb = AColor.b * 255.0;
  float gg = AColor.g * 255.0;
  float rr = AColor.r * 255.0;
  float aa = AColor.a * 255.0;
  uint32 b = (uint32)trunc(bb);
  uint32 g = (uint32)trunc(gg);
  uint32 r = (uint32)trunc(rr);
  uint32 a = (uint32)trunc(aa);
  uint32  res = (b) + (g<<8) + (r<<16) + (a<<24);
  //KTrace("rgba: %i,%i,%i,%i -> %x\n",r,g,b,a,res);
  //KTrace("RGB: %x\n",RGB(r,g,b));
  return res;
}
*/

/*
//inline
uint32 SRgb(SColor AColor) {
  return ((int)(AColor.r*255) << 16)
       + ((int)(AColor.g*255) << 8)
       +  (int)(AColor.b*255);
}
*/

//----------

//inline
uint32 SRgb(uint8 r, uint8 g, uint8 b) {
  return ((uint32)r<<16)
        + ((uint32)g<<8)
        + b;
}

//----------

uint32 SRgb(SColor AColor) {
  uint32 col = SRgb(AColor.r*255,AColor.g*255,AColor.b*255);
  //STrace("%8x\n",col);
  return col;
}

//----------

//inline
uint32 SRgba(uint8 r, uint8 g, uint8 b, uint8 a) {
  return   ((uint32)a<<24)
         + ((uint32)r<<16)
         + ((uint32)g<<8)
         + b;
}

//----------

uint32 SRgba(SColor AColor) {
  return SRgba(AColor.r*255,AColor.g*255,AColor.b*255,AColor.a*255);
}

//----------

//inline
uint8 alphaColor(uint32 c, uint32 a) {
  uint32 ret = (c*a) >> 8;
  return ret & 0xff;
}

//----------

//inline
uint8 scaleColor(uint8 c, float n ) {
  float nc = n * (float)c;
  return (uint8)SMin(255,nc);
}

//----------

// values = 0..1

SColor blendColor(float value, float r1, float g1, float b1, float r2, float g2, float b2) {
  SColor col;// = SColor(0);
  col.r = (r1 * (1-value)) + (r2 * value);
  col.g = (g1 * (1-value)) + (g2 * value);
  col.b = (b1 * (1-value)) + (b2 * value);
  col.a = 1;
  //a = (a * (1-value)) + (aa * value);
  return col;
}

//----------

SColor blendColor(float value, SColor AColor1, SColor AColor2) {
  return blendColor(value,AColor1.r,AColor1.g,AColor1.b,AColor2.r,AColor2.g,AColor2.b);
}


//----------------------------------------------------------------------
#endif


