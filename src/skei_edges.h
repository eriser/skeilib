#ifndef skei_edges_included
#define skei_edges_included
//----------------------------------------------------------------------

struct SEdges {
  int32   left;
  int32   top;
  int32   right;
  int32   bottom;
  /* */
  SEdges() { left=0; top=0; right=0; bottom=0; }
  SEdges(int32 i) { left=i; top=i; right=i; bottom=i; }
  SEdges(int32 ALeft, int32 ATop, int32 ARight, int32 ABottom) { left=ALeft, top=ATop, right=ARight, bottom=ABottom; }

};

//----------------------------------------------------------------------
#endif
