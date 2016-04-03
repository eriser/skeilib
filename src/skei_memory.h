#ifndef skei_memory_included
#define skei_memory_included
//----------------------------------------------------------------------

/*
  TODO:
  - memory manager (big buffer)
  - __builtin_alloca_with_align
*/

#include <stdlib.h>
#include <memory.h>

#include "skei_integer.h"

//----------

#define SMalloc   malloc
#define SCalloc   calloc
#define SRealloc  realloc
#define SFree     free

//----------

#define SMemset   memset
#define SMemcpy   memcpy
#define SMemmove  memmove

//----------------------------------------------------------------------

// we can't assert here.. debug is ont oncluded yet (it needs to do some
// #define tricks with memory stuff..
//SAssert( SIsPowerOfTwo(AAlignment) );

//----------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

void* SMalloc_Aligned(uint32 ASize, uint32 AAlignment) {
  void* p1; // original block
  void** p2; // aligned block
  int offset = AAlignment - 1 + sizeof(void*);
  if ((p1 = (void*)SMalloc(ASize + offset)) == SKEI_NULL) return SKEI_NULL;
  p2 = (void**)(((size_t)(p1) + offset) & ~(AAlignment - 1));
  p2[-1] = p1;
  return p2;
}

//----------

void SFree_Aligned(void *p)
{
  SFree(((void**)p)[-1]);
}
//----------------------------------------------------------------------
#endif




