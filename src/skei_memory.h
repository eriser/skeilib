#ifndef skei_memory_included
#define skei_memory_included
//----------------------------------------------------------------------

/*
  TODO:
  - memory manager (big buffer)
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

//----------------------------------------------------------------------

// AAlignment = power of 2

/*
void* SMalloc_Aligned(uint32 ASize, uint32 AAlignment=16) {
  // we can't assert here.. debug is ont oncluded yet (it needs to do some
  // #define tricks with memory stuff..
  //SAssert( SIsPowerOfTwo(AAlignment) );
  if (AAlignment < sizeof(intptr)*2) AAlignment = sizeof(intptr)*2;
  uint32 alignmask = AAlignment-1;
  void* mem = SMalloc(ASize + AAlignment*2 - 1 );
  intptr ma = (intptr)mem & alignmask;
  ma +=
  return (void*)ma;
}
*/

//----------------------------------------------------------------------
#endif




