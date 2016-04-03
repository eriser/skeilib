#ifndef skei_types_included
#define skei_types_included
//----------------------------------------------------------------------

#include "skei_defines.h"
#include "skei_const.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define SSample  float
//#define SSample  double

#define SSampleSize sizeof(SSample)

//----------------------------------------------------------------------
// fixed size types
//----------------------------------------------------------------------

#define int8         char
#define uint8        unsigned char
#define int16        short
#define uint16       unsigned short

//----------------------------------------------------------------------
// architecture dependent
//----------------------------------------------------------------------

#ifdef SKEI_32BIT
  #define int32           long
  #define uint32          unsigned long
  #define int64           long long
  #define uint64          unsigned long long
  #define intptr          uint32
  //#define SKEI_LONG_MAX   2147483647L
  //#define SKEI_ULONG_MAX  8589934591UL
#endif

#ifdef SKEI_64BIT
  #define int32           int
  #define uint32          unsigned int
  #define int64           long long
  #define uint64          unsigned long long
  #define intptr          uint64
  //#define SKEI_LONG_MAX   9223372036854775807L
  //#define SKEI_ULONG_MAX  18446744073709551615UL
#endif

//----------------------------------------------------------------------
// aligned types
//----------------------------------------------------------------------

typedef double    __SKEI_ALIGNED(8) double_8;
typedef long long __SKEI_ALIGNED(8) longlong_8;

//----------------------------------------------------------------------
// aliasing types
//----------------------------------------------------------------------

typedef void*               __SKEI_MAY_ALIAS void_ptr_a;
typedef long double         __SKEI_MAY_ALIAS ldouble_a;
typedef double              __SKEI_MAY_ALIAS double_a;
typedef float               __SKEI_MAY_ALIAS float_a;
typedef long long           __SKEI_MAY_ALIAS longlong_a;
typedef unsigned long long  __SKEI_MAY_ALIAS ulonglong_a;
typedef long                __SKEI_MAY_ALIAS long_a;
typedef unsigned long       __SKEI_MAY_ALIAS ulong_a;
typedef int                 __SKEI_MAY_ALIAS int_a;
typedef unsigned int        __SKEI_MAY_ALIAS uint_a;
typedef short               __SKEI_MAY_ALIAS short_a;
typedef unsigned short      __SKEI_MAY_ALIAS ushort_a;
typedef char                __SKEI_MAY_ALIAS char_a;
typedef unsigned char       __SKEI_MAY_ALIAS uchar_a;

//----------------------------------------------------------------------
#endif
