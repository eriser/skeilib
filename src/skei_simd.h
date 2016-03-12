#ifndef skei_simd_included
#define skei_simd_included
//----------------------------------------------------------------------

/*
  https://gcc.gnu.org/onlinedocs/gcc/Vector-Extensions.html#Vector-Extensions

  //https://gcc.gnu.org/onlinedocs/gcc/x86-Built-in-Functions.html#x86-Built-in-Functions

  On some targets, the instruction set contains SIMD vector instructions which
  operate on multiple values contained in one large register at the same time.
  For example, on the x86 the MMX, 3DNow! and SSE extensions can be used this
  way.

  All the basic integer types can be used as base types, both as signed and as
  unsigned: char, short, int, long, long long. In addition, float and double
  can be used to build floating-point vector types.

  GCC allows using the following operators on these types:
  +, -, *, /, unary minus, ^, |, &, ~, %

  v4si a,b,c;
  c = a + b;

  v4si a,b,c;
  long l;
  a = b + 1;    // a = b + {1,1,1,1};
  a = 2 * b;    // a = {2,2,2,2} * b;
  a = l + a;    // Error, cannot convert long to int.

  v4si a = {1,2,3,4};
  v4si b = {3,2,1,4};
  v4si c;
  c = a >  b;   // The result would be {0, 0,-1, 0}
  c = a == b;   // The result would be {0,-1, 0,-1}

  v4si a = {1,2,3,4};
  v4si b = {5,6,7,8};
  v4si mask1 = {0,1,1,3};
  v4si mask2 = {0,4,2,5};
  v4si res;
  res = __builtin_shuffle (a, mask1);       // res is {1,2,2,4}
  res = __builtin_shuffle (a, b, mask2);    // res is {1,5,3,6}

*/

//----------------------------------------------------------------------

/*
typedef int8    v4s8  __SKEI_VECTOR(16);
typedef int16   v4s16 __SKEI_VECTOR(16);
typedef int32   v4s32 __SKEI_VECTOR(16);
typedef int64   v4s64 __SKEI_VECTOR(16);

typedef uint8   v4u8 __SKEI_VECTOR(16);
typedef uint16  v4u16 __SKEI_VECTOR(16);
typedef uint32  v4u32 __SKEI_VECTOR(16);
typedef uint64  v4u64 __SKEI_VECTOR(16);

typedef float   v4f32 __SKEI_VECTOR(16);
typedef double  v4f64 __SKEI_VECTOR(16);
*/

//----------------------------------------------------------------------


//----------------------------------------------------------------------
#endif
