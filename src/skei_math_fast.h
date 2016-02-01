#ifndef skei_math_fast_included
#define skei_math_fast_included
//----------------------------------------------------------------------

// optimizations
// approximations

#include <math.h> // expf, log2

float SAcosF(const float x);
float SAsinF(const float x);
float SAtanF(const float x);
float SCosF(const float x);
float SCoshF(const float x);
float SCoshF2(const float x);
float SExpF(const float v);
float SInverseF(float x);
float SInvSqrtF(const float x);
float SLog2F(const float val);
float SLog10F(const float x);
float SLogF(const float &val);
float SPowF(float x, unsigned long n);
float SSinF(float x);
float SSinhF(const float x);
float SSqrtF(const float x);
float STanF(const float x);
float STanhF(const float x);

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/// fast approximation of the arc-cosine function for range [-1, 1]

float SAcosF(const float x) {
  const float x2 = x*x;
  return x*(x2*(-0.55*x2 + 0.097) - 1.008) + 1.571;
}

//----------

/// fast approximation of the arc-sine function for range [-1, 1]

float SAsinF(const float x) {
  return SKEI_PI05 - SSqrtF(1 - x)*(1.5707288 - x*(0.2121144 + x*(0.0742610 -
  x*(0.0187293 + 0.395*x))));
}

//----------

/// fast approximation of the arc-tangens function for range [-2, 2]

float SAtanF(const float x) {
  const float x2 = x*x;
  return (x*(105 + 55*x2)) / (105 + x2*(90 + 9*x2));
}

//----------

/// fast approximation of the cosine function for range [-pi, pi]

float SCosF(const float x) {
  const float x2 = x*x;
  return (15120 + x2*(-6900 + 313*x2)) / (15120 + x2*(660 + 13*x2));
}

//----------

/// approximation of the hyperbolic-cosine function (fpu)

float SCoshF(const float x) {
  const float _e = expf(abs(x));
  return (_e + 1.0f/_e)*0.5f;
}

/// fast approximation of the hyperbolic-cosine function for range [-3.5, 3.5]

float SCoshF2(const float x) {
  const float x2 = x*x;
  return x2*(0.065*x2 + 0.428) + 1.025;
}

//----------

/**
  fast approximation of [e] to the power of a number
  based on http://theoval.sys.uea.ac.uk/publications/pdf/nc2000a.pdf
  note: original is for double precision (has a double to float cast)
*/

// is SKEI_LITTLE_ENDIAN defined?

float SExpF(const float v) {
  union {
    double d;
    struct {
      #ifdef SKEI_LITTLE_ENDIAN
      int i,j;
      #else
      int j,i;
      #endif
    } s;
  } u;
  u.s.i = (int)(1512775*(double)v) + 1072632447;
  u.s.j = 0;
  return (float)u.d;
}

//----------

/// invert of x: (1/x) - fast / inaccurate

float SInverseF(float x) {
  union {
    uint32 i;
    float j;
  } u;
  u.j = x;
  u.i = 0x7EEEEEEE - u.i;
  return u.j;
}

//----------

/**
  returns 1/x
  http://bits.stephan-brumme.com/inverse.html
*/

float SInverse2(float x) {
  unsigned int *i = (unsigned int*)&x; // re-interpret as a 32 bit integer
  // warning: dereferencing pointer \91i\92 does break strict-aliasing rules|
  *i = 0x7F000000 - *i; // adjust exponent
  // *i = 0x7EEEEEEE - *i;
  return x;
}

//----------

/**
  fast approximation of the invert squre root function
  based on code found in 'quake 3 arena' by 'id software'
*/

float SInvSqrtF(const float x) {
  // const float halfx = 0.5f*x;
  union {
    float j;
    int i;
  } u;
  u.j = x;
  u.i = 0x5f3759df - (u.i >> 1); // good initial guess
  // newton iteration <- enable for better result
  // return u.j*(1.5f - u.j*u.j*halfx) + 0.001f;
  return u.j;
}

//----------

/// http://bits.stephan-brumme.com/invSquareRoot.html

float SInvSqrtF2(float x) {
  float xHalf = 0.5f*x;                 // for Newton iteration
  unsigned int *i = (unsigned int*) &x; // same as above
  // warning: dereferencing pointer \91i\92 does break strict-aliasing rules|
  *i = 0x5F375A86 - (*i>>1);            // one Newton iteration, repeating further improves precision
  return x * (1.5f - xHalf*x*x);
}

//----------

float SInvSqrtF3(float x) {
  unsigned int *i = (unsigned int*) &x; // access float with integer logic
  // warning: dereferencing pointer \91i\92 does break strict-aliasing rules|
  *i = 0x5F375A86 - (*i>>1); // approximation with empirically found "magic number"
  return x;
}

//----------

/**
  fast approximation of the logarithm base 2 function
  based on code from http://www.flipcode.com/archives/Fast_log_Function.shtml
*/

float SLog2F(const float val) {
  if (val > 0.f) {
    union {
      int i;
      float j;
    } u;
    u.j = val;
    const int log_2 = ((u.i >> 23) & 255) - 128;
    u.i &= ~(255 << 23);
    u.i += 127 << 23;
    return (0.05f + u.j + (float)log_2);
  }
  else return 0.f;
}

//----------

/// calculates the logarithm base 10 of a floating point number

float SLog10F(const float x) {
  // log10(e) = 0.4342945239647
  // also: log10(x) = log2(x) - ln(x)
  const float y = (x - 1)/(x + 1);
  const float y2 = y*y;
  return (2.f*y*(1 + y2*0.3333333333f + y2*y2*0.2f))*0.4342945239647f;
}

//----------

/**
  fast approximation of the natural logarithm function
  based on code from http://www.flipcode.com/archives/Fast_log_Function.shtml
*/

float SLogF(const float &val) {
  return (log2(val)*0.69314718f);
}

//----------

/// performs fast pow(float, integer)

float SPowF(float x, unsigned long n) {
  float res = 1;
  while (n > 0) {
    if (n & 1) res *= x;
    n >>= 1;
    x *= x;
  }
  return res;
}

//----------

/// fast approximation of the sine function for range [-pi, pi]

float SSinF(float x) {
  x *= (1.2732395447f - 0.4052847345f * abs(x));
  return 0.225f * (x * abs(x) - x) + x;
}

//----------

/// fast approximation of the hyperbolic-sine function for range [-3.5, 3.5]

float SSinhF(const float x) {
  const float x2 = x*x;
  return x*(x2*(0.012*x2 + 0.156) + 1.004);
}

//----------

/**
  fast approximation of the squre root function
  based on: http://www.azillionmonkeys.com/qed/sqroot.html
*/

float SSqrtF(const float x) {
  const float halfx = x*0.5;
  union {
    int i;
    float j;
  } u;
  u.j = x;
  u.i = (0xbe6ec85f - u.i) >> 1;                // good initial guess
  return x*u.j*(1.5f - u.j*u.j*halfx) + 0.001f; // newton iteration
}

//----------

/// fast approximation of the tangens function for range [-pi, pi]

float STanF(const float x) {
  const float x2 = x*x;
  return (x*(105 - 10*x2)) / (105 - x2*(45 - x2));
}

//----------

/// fast approximation of the hyperbolic-tangens function for range [-4.2, 4.2]

float STanhF(const float x) {
  const float x2 = x*x;
  return x*(27 + x2) / (27 + 9*x2);
}

//----------------------------------------------------------------------
#endif
