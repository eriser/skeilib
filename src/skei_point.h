#ifndef skei_point_included
#define skei_point_included
//----------------------------------------------------------------------

#include "skei_array.h"

//----------

struct SPoint
{
  //union
  //{
  //  //struct { int x,y,w,h; };
  //  //struct { int data[4]; };
  //  int x,y,w,h;
  //  int data[4];
  //};

  int x,y;

  //----------

  SPoint() { x=0; y=0; }
  SPoint(int i) { x=i; y=i; }
  SPoint(int ax, int ay) { x=ax; y=ay; }
  inline void set(int i) { x=i; y=i; }
  inline void set(int ax, int ay) { x=ax; y=ay; }
  inline void add(int ax, int ay) { x+=ax; y+=ay; }

};

//----------

typedef SArray<SPoint> SPointArray;

//----------------------------------------------------------------------
#endif

