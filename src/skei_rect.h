#ifndef skei_rect_included
#define skei_rect_included
//----------------------------------------------------------------------

#include "skei_array.h"

/*
  union {
    int32 x,y,w,h;
    int32 data[4];
  };
*/

//----------------------------------------------------------------------

//class SRect;
struct SRect;
typedef SArray<SRect> SRectArray;

//----------------------------------------------------------------------

struct SRect {
//class SRect {

  public:
    int32 x,y,w,h;

  public:

  //----------

  SRect() { x=0; y=0; w=0; h=0; }
  SRect(int32 i) { x=i; y=i; w=i; h=i; }
  SRect(int32 AW, int32 AH) { x=0; y=0; w=AW; h=AH; }
  SRect(int32 AX, int32 AY, int32 AW, int32 AH) { x=AX; y=AY; w=AW; h=AH; }
  inline void set(int32 i) { x=i; y=i; w=i; h=i; }
  inline void set(int32 AW, int32 AH) { x=0; y=0; w=AW; h=AH; }
  inline void set(int32 AX, int32 AY, int32 AW, int32 AH) { x=AX; y=AY; w=AW; h=AH; }
  inline void set(SRect ARect) { x=ARect.x; y=ARect.y; w=ARect.w; h=ARect.h; }
  inline void setPos(int32 AX, int32 AY) { x=AX; y=AY; }
  inline void setSize(int32 AW, int32 AH) { w=AW; h=AH; }
  inline int32  x2(void)  { return x+w-1; }
  inline int32  y2(void)  { return y+h-1; }
  inline void x2(int32 i) { w=i-x+1; }
  inline void y2(int32 i) { h=i-y+1; }
  inline void add(int32 AX, int32 AY) { x+=AX; y+=AY; }
  inline void add(int32 AX, int32 AY, int32 AW, int32 AH) { x+=AX; y+=AY; w+=AW; h+=AH; }

  inline SPoint pos(void) { return SPoint(x,y); }

  //----------

  bool isEmpty(void) {
    if ((w>0) && (h>0)) return false;
    else return true;
  }

  bool isNotEmpty(void) {
    if ((w>0) && (h>0)) return true;
    else return false;
  }

  //----------


  // return true if rect contains the specified point32

  bool contains(int32 AX, int32 AY) {
    if( AX < x ) return false;
    if( AY < y ) return false;
    if( AX >= (x+w) ) return false;
    if( AY >= (y+h) ) return false;
    return true;
  }

  // return true if rect overlap the specified rect

  bool intersects(int32 AX, int32 AY, int32 AW, int32 AH) {
    if( (AX+AW) < x ) return false;
    if( (AY+AH) < y ) return false;
    if( AX >= (x+w) ) return false;
    if( AY >= (y+h) ) return false;
    return true;
  }

  //  __            _____
  // |  |          :  :  |
  // |__|          :..:  |
  //     __    ->  |   ..|
  //    |  |       |  :  :
  //    |__|       |__:__:
  //

  // create a rect big enough to hold itself and specified rect

  void combine(int32 AX, int32 AY, int32 AW, int32 AH) {
    if( AX < x ) { w += (x-AX); x=AX; }
    if( AY < y ) { h += (y-AY); y=AY; }
    if( (AX+AW) >= (x+w) ) w = (AX+AW) - x;
    if( (AY+AH) >= (y+h) ) h = (AY+AH) - y;
  }

  //    ___
  //   |  _|_          _:.
  //   | | | |   ->   | | :
  //   | |_|_|        |_|.:
  //   |___|            :
  //

  // keep only int32ersecting area

  void crop(int32 AX, int32 AY, int32 AW, int32 AH) {
    if( AX > x ) x = AX;
    if( AY > y ) y = AY;
    if( (AX+AW) <= (x+w) ) w = (AX+AW) - x;
    if( (AY+AH) <= (y+h) ) h = (AY+AH) - y;
  }

  //----------

  // shortcuts

  inline bool intersects(SRect ARect) {
    return intersects( ARect.x, ARect.y, ARect.w, ARect.h );
  }

  inline void combine(SRect ARect) {
    return combine( ARect.x, ARect.y, ARect.w, ARect.h );
  }

};

//----------


//----------------------------------------------------------------------
#endif

