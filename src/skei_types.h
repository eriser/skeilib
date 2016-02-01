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

typedef double    __aligned(8) double_8;
typedef long long __aligned(8) longlong_8;

//----------------------------------------------------------------------
// aliasing types
//----------------------------------------------------------------------

typedef void*               __may_alias void_ptr_a;
typedef long double         __may_alias ldouble_a;
typedef double              __may_alias double_a;
typedef float               __may_alias float_a;
typedef long long           __may_alias longlong_a;
typedef unsigned long long  __may_alias ulonglong_a;
typedef long                __may_alias long_a;
typedef unsigned long       __may_alias ulong_a;
typedef int                 __may_alias int_a;
typedef unsigned int        __may_alias uint_a;
typedef short               __may_alias short_a;
typedef unsigned short      __may_alias ushort_a;
typedef char                __may_alias char_a;
typedef unsigned char       __may_alias uchar_a;

//----------------------------------------------------------------------
#endif
