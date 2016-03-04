#ifndef skei_math_included
#define skei_math_included
//----------------------------------------------------------------------

#include <math.h>

float   SAverage(const unsigned int n, const float* ar);
float   SClamp(const float input, const float limit);
float   SClamp(const float input, const float minval, const float maxval);
float   SClip(float x, float l, float h);
float   SCurve(float x, float t);
float   SDbToAmp(float g);
float   SFract(const float value);
float   SFract(const float value, float* intpart);
float   SKillDenorm(float n);
float   SMax(const float a, const float b);
float   SMin(const float a, const float b);
float   SModulo(float x, float y);
float   SNoteToHz(float ANote);
float   SRms(const unsigned int n, const float* ar);
float   SRound(const float v);
float   SSigmoid(float x);
float   SSign(const float v);
double  SSinc(double x);
float   SSqr(float x);
float   STrunc(const float v);

float SWindowingRect(float pos);
float SWindowingHann(float pos);
float SWindowingHamming(float pos);
float SWindowingBlackman(float pos);
float SWindowingGaussian(float pos);
float SWindowingWelch(float pos);
float SWindowingBartlett(float pos);

//----------------------------------------------------------------------


/*
  calculate the average value of a set of floats
  example:
    float ar[] = { -1.f, 2.f, 3.f, 4.f, 5.f };
    float result = SAverage(5,ar); // result is 2.6f
  n - number of elements (n)
  ar - array of floats
*/

float SAverage(const unsigned int n, const float* ar) {
  float total = 0;
  unsigned int i=0;
  while (i<n) {
    total += ar[i];   i++;
  }
  return total/n;
}

//----------

// limits a floating point number to [-limit, limit]

float SClamp(const float input, const float limit) {
  const float _t = (input > -limit) ? input : -limit;
  //return (_t > limit) ? _t : limit;
  return (_t < limit) ? _t : limit;
}

//----------

// limits a floating point number to [-limit, limit]

float SClamp(const float input, const float minval, const float maxval ) {
  const float _t = (input > minval) ? input : minval;
  return (_t < maxval) ? _t : maxval;
}

//----------

float SClip(float x, float l, float h) {
  return  (abs(x-l) + (l+h) - abs(x-h)) * 0.5;
}

//----------

/*
  http://www.kvraudio.com/forum/viewtopic.php?p=2049788#p2049788

  't' is a scaling variable - 0.5 gives you a straight line output (for
  straight line input) > 0.5 and the curve gets logarithmic-like, < 0.5
  and it gets exponential-like.
*/

float SCurve(float x, float t) {
  float a = ( 1.0f - 1.0f/t );
  return x / (x+a * (x-1) );
}

//----------

/*
  db to amplitude and vice versa
  double atodb(double a) {
	  return 20*log10(a);
  }
  double dbtoa(double db) {
  	return pow(2, db/6);
  }

  ---

  there is no 10^(db/20) == 0
  BTW, 2^(db/6) is close, but not quite accurate.

*/

// Convert a value in dB's to a linear amplitude

float SDbToAmp(float g) {
  if (g > -144.0) return exp(g * 0.115129254);
  else return 0;
}

//----------

/*
  returns the fractional part of a floating point number
  (and stores the integer part in the second argument)

  example:
    value_pi = 3.141592;
    float integer;
    float fraction = SFract(value_pi, &integer);
    // fraction = 0.141592, integer = 3

  value - input variable
  intpart - pointer to integer part variable
  return fractional part
*/

/*
  SFract(0.1)   =  0.1
  SFract(0.9)   =  0.9
  SFract(1.1)   =  0.1
  SFract(1.9)   =  0.9
  SFract(-0.1)  = -0.1
  SFract(-0.9)  = -0.9
  SFract(-1.1)  = -0.1
  SFract(-1.9)  = -0.9
*/

float SFract(const float value) {
  float intpart = (float)(int)value;
  return (value - intpart);
}

//----------

float SFract(const float value, float* intpart) {
  *intpart = (float)(int)value;
  return (value - *intpart);
}

//----------

/*
  http://www.kvraudio.com/forum/viewtopic.php?p=4817458#p4817458
*/

/*
// skei_const.pas: SKEI_DENORM = 1.4012984e-45;
const double _denorm = 1.0e-18;//1.0e-56;

float SKillDenorm(float v) {
   v += _denorm;
   v -= _denorm;
   return v;
}
*/

//----------

// DelphiASIOVST/DAVDCommon.pas

/*
function SKillDenorm(value : Single) : Single;
begin
 if (abs(value) < 1.0e-20)
  then Result := 0.0
  else Result := value;
end;
*/

//----------

/*
  denormalize input value
*/

float SKillDenorm(float n) {
  /*register*/ union
  {
    float f;
    uint32 /*unsigned int*/ i;
  } u;
  u.f = n;
  if ( !(u.i & 0x7f800000) ) n = 0.f;
  return n;
}

//----------

float SMax(const float a, const float b) {
  return (a > b) ? a : b;
}

//----------

// returns the smaller of two floating point numbers

float SMin(const float a, const float b) {
  return (a < b) ? a : b;
}

//----------

float SModulo(float x, float y) {
  return x - (y * float(floor(x / y)));
}

//----------

float SNoteToHz(float ANote) {
  return 440 * pow(2.0,(ANote-69)*SKEI_INV12F);
}

//----------

/*
  calculate the RMS of a set (array) of float numbers

  example:
    float ar[] = { 1.f, 2.f, 3.f, 4.f, 5.f };
    float result = SRMS(5, ar);   // result = 3.31662

  n - size of the array
  ar - array of floats
*/

float SRms(const unsigned int n, const float* ar) {
  float numr = 0;
  unsigned int i=0;
  while (i<n) {
    numr += ar[i]*ar[i];
    i++;
  }
  return sqrtf(numr/n);
}

// rounds a floating point number

/*
  SRound(0.1)   =  0.0
  SRound(0.9)   =  1.0
  SRound(1.1)   =  1.0
  SRound(1.9)   =  2.0
  SRound(-0.1)  = -0.0
  SRound(-0.9)  = -1.0
  SRound(-1.1)  = -1.0
  SRound(-1.9)  = -2.0
*/

float SRound(const float v) {
  if (v < 0.f) return (float)(int)(v - 0.5f);
  else return (float)(int)(v + 0.5f);
}

//----------

/*
  SSigmoid(0.0)   //  0
  SSigmoid(0.1)   //  0.1495
  SSigmoid(0.3)   //  0.4365
  SSigmoid(0.5)   //  0.5875
  SSigmoid(0.7)   //  0.8785
  SSigmoid(0.9)   //  0.9855
  SSigmoid(1.0)   //  1
*/


float SSigmoid(float x) {
  if ( abs(x) < 1.0f ) return x * (1.5f - 0.5f * x * x);
  else {
    if (x < 0.0f) return -1.0f;
    else return 1.0f;
  }
}


//----------

/*
  SSign(0.5)  = 1
  SSign(1.5)  = 1
  SSign(-0.5) = -1
  SSign(-1.5) = -1
*/

float SSign(const float v) {
  union {
    int32 i;
    float f;
  } u;
  u.f = v;
  return (1 | (u.i >> 31));
}

//----------

/*
  0.0 = 1
  0.1 = 0.9836
  0.3 = 0.8584
  0.5 = 0.6366
  0.7 = 0.3678
  0.9 = 0.1093
  1.0 = 0
*/

double SSinc(double x) {
  if (x==0) return 1;
  else {
    double pix = SKEI_PI * x;
    return sin(pix) / pix;
  }
}

//----------

float SSqr(float x) {
  return x*x;
}

//----------

/*
  STrunc(0.1)   =  0.0
  STrunc(0.9)   =  0.0
  STrunc(1.1)   =  1.0
  STrunc(1.9)   =  1.0
  STrunc(-0.1)  =  0.0
  STrunc(-0.9)  =  0.0
  STrunc(-1.1)  = -1.0
  STrunc(-1.9)  = -1.0
*/

//----------

float STrunc(const float v) {
  return (float)(int)v;
}

//----------------------------------------------------------------------
// windowing
//----------------------------------------------------------------------

float SWindowingRect(float pos) {
  return 1.0;
}

float SWindowingHann(float pos) {
  float x = cos(pos*SKEI_PI/2.0);
  return x*x;
}

float SWindowingHamming(float pos) {
  return 0.54 + 0.46 * cos(SKEI_PI*pos);
}

float SWindowingBlackman(float pos) {
  return 0.42 + 0.5 * cos(SKEI_PI*pos) + 0.08 * cos(2.0*SKEI_PI*pos);
}

float SWindowingGaussian(float pos) {
  float a = 4.0;
  float ax = a * pos;
  return exp(-0.5 * ax*ax);
}

float SWindowingWelch(float pos) {
  return 1.0 - pos*pos;
}

float SWindowingBartlett(float pos) {
 if (pos == 0.0) return 1.0;
 else if (pos > -1.0 && pos < 0.0) return pos + 1.0;
 else if (pos > 0.0 && pos < 1.0) return 1.0 - pos;
 else return 0.0;
}

//----------------------------------------------------------------------
// https://github.com/Ixox/preenFM2/blob/master/src/third/wirish_math.h
//----------------------------------------------------------------------

/*
  Remap a number from one range to another.
  That is, a value equal to fromStart gets mapped to toStart, a value
  of fromEnd to toEnd, and other values are mapped proportionately.
  Does not constrain value to lie within [fromStart, fromEnd].
  If a "start" value is larger than its corresponding "end", the
  ranges are reversed, so map(n, 1, 10, 10, 1) would reverse the
  range [1,10].
  Negative numbers may appear as any argument.
  value     the value to map.
  fromStart the beginning of the value's current range.
  fromEnd   the end of the value's current range.
  toStart   the beginning of the value's mapped range.
  toEnd     the end of the value's mapped range.
  return    the mapped value.
*/

//int32 map(int32 value, int32 fromStart, int32 fromEnd, int32 toStart, int32 toEnd) {
//  return (value - fromStart) * (toEnd - toStart) / (fromEnd - fromStart) + toStart;
//}

//#define PI          3.1415926535897932384626433832795
//#define HALF_PI     1.5707963267948966192313216916398
//#define TWO_PI      6.283185307179586476925286766559
//#define DEG_TO_RAD  0.017453292519943295769236907684886
//#define RAD_TO_DEG 57.295779513082320876798154814105

//#define min(a,b)                ((a)<(b)?(a):(b))
//#define max(a,b)                ((a)>(b)?(a):(b))
//#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
//#define round(x)                ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
//#define radians(deg)            ((deg)*DEG_TO_RAD)
//#define degrees(rad)            ((rad)*RAD_TO_DEG)
//#define sq(x)                   ((x)*(x))

// undefine stdlib's abs
//#ifdef abs
//#undef abs
//#endif
//#define abs(x) (((x) > 0) ? (x) : -(x))

//----------------------------------------------------------------------
#endif


