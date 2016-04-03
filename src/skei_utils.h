#ifndef skei_utils_included
#define skei_utils_included
//----------------------------------------------------------------------

#include <math.h> // sqrtf,

#include "skei_const.h"
#include "skei_memory.h"
#include "skei_string.h"

//----------------------------------------------------------------------

//----------------------------------------
// macros
//----------------------------------------

// SArraySize(x)
// SSwap(x,y)
// SBool(x)

// SBitClear(x, bit)
// SBitFlip(x, bit)
// SBitGet(x, bit)
// SBitSet(x, bit)

// SSquare(x)
// SCube(x)
// SDeg2Rad(x)
// SRad2Deg(x)
// SInverse(x)

//----------------------------------------
// strings
//----------------------------------------

/*
void        SCreateUniqueName(const char* ABuffer, const char* APrefix, void* APtr);
const char* SGetFileExt(char* APath);
const char* SGetPath(char* APath);
char*       SStripFileExt(char* APath);
const char* SStripPath(const char* APath);
*/

//----------------------------------------
// chars
//----------------------------------------

/*
int         SIsAlnum(int c);
int         SIsAlpha(int c);
int         SIsDigit(int c);
int         SIsPrint(int c);
int         SIsSpace(int c);
int         SIsUpper(int c);
int         SIsXdigit(int c);
int         SToLower(int c);
int         SToUpper(int c);
*/

//----------------------------------------
// buffer
//----------------------------------------

/*
void        SRadixSort(long *source, long *dest, uint32 N, int byte);
*/

//----------------------------------------------------------------------
// sizeof
//----------------------------------------------------------------------

/*
  returns the size of an array
    char a[15];
    int b[SArraySize(a) + 1];
    unsigned int j = SArraySize(b);    // j = 16
    // ----------------
    // NOTE: passing a pointer will not work
    int ar[21];
    int* ptr = ar;
    unsigned int j = SArraySize(ptr); // <- will not work
  param[in] x array type-unsafe
  return unsigned int
*/

template<class T, size_t N> T SDecayArrayToSubType(T (&a)[N]);

#define SArraySize(x) (sizeof(x)/sizeof(SDecayArrayToSubType(x)))

//

//#define lengthof(x) (sizeof(x)/sizeof(x[0]))

//----------------------------------------------------------------------

// clear bit
#define SBitClear(x, bit)  ((x) & ~(1 << (bit)))

// flip bit
#define SBitFlip(x, bit)  ((x) ^  (1 << (bit)))

/*
  returns a specific bit of an integer
    int x = 0x0f;
    for(unsigned int i=0; i<32; i++)
    { cout << axGetBit(x, i); }      // 11110000000000000000000000000000
    cout << axGetBit(x, 0);          // 1
    cout << axGetBit(x, 4);          // 0
  x long int - input value
  bit unsigned int - which bit
  return unsigned int - 0x0 / 0x1
*/

// get bit
#define SBitGet(x, bit)  ( 1  &  ((x) >> (bit)))

// set bit
#define SBitSet(x, bit)  ((x) |  (1 << (bit)))

// convert to boolean
#define SBool(x) (!(!(x)))

// cube of x: (x^3)
#define SCube(x) ((x)*(x)*(x))

/*
  converts decibel value to linear
    float amp_db = 20.f;         // 20 decibels
    signal *= axDB2Lin(amp_db);  // *= ~10.079
*/

//#define SDb2Lin(dB) ( SExpf( DB2LOG*(dB) ) )

// convert degrees to radians- rad = deg*(pi/180);
#define SDeg2Rad(x) ( (x)*0.01745329251994f )

// invert of x: (1/x)
#define SInverse(x) (1/(x))

/*
  converts linear value to decibel
  \code
  float dc_signal = 1.f;
  float amp_half = 0.5f;
  dc_signal *= amp_half;                  // half the amplitude
  float amp_db = axLin2DB(amp_half);      // amp_db = -6dbFS
  \endcode
  @param[in] lin float
  @return result float
*/

//#define SLin2DB(lin) ( LOG2DB*SLogf( (lin) ) )

//----------

// convert radians to degrees. deg = rad*(180/pi);
#define SRad2Deg(x) ( (x)*57.2957795130823f )

// square of x: (x^2)
#define SSquare(x) ((x)*(x))

/*
  origin: http://graphics.stanford.edu/~seander/bithacks.html
  note: type of first variable is used
    float x = 5.f;
    float y = 3.f;
    SSwap(x,y);
*/

// swap two values
#define SSwap(x,y) { typeof(x) tmp = (x); (x) = (y); (y) = (tmp); }


//----------------------------------------------------------------------

/*
  http://rosettacode.org/wiki/Binary_search#Pascal
  Iterative


  Usage:
  var
    list: array[0 .. 9] of real;
    // ...
    indexof := binary_search(123, list);
*/

/*
function binary_search(element: real; list: array of real): integer;
var
    l, m, h: integer;
begin
    l := 0;
    h := High(list) - 1;
    binary_search := -1;
    while l <= h do
    begin
        m := (l + h) div 2;
        if list[m] > element then
        begin
            h := m - 1;
        end
        else if list[m] < element then
        begin
            l := m + 1;
        end
        else
        begin
            binary_search := m;
            break;
        end;
    end;
end;
*/

//----------

// ptr to (reversed) hex

static __SKEI_UNUSED
void SCreateUniqueName(const char* ABuffer, const char* APrefix, void* APtr) {
  static char _int2hex[17] = "0123456789abcdef";    // +'\0' = 17
  char* buf = (char*)ABuffer;
  while (*APrefix != '\0') /**ABuffer++*/*buf++ = *APrefix++; // memset + update pointers
  //uint32 iptr = (uint32)APtr;         // todo: 64-bit (long long?)
  uint64 iptr = (uint64)APtr;
  while (iptr>0) {
    int j = iptr & 0x0f;      // 0..f
    iptr >>= 4;               // /16 (0..f)
    /**ABuffer++*/*buf++ = _int2hex[j]; // to char
  }
  /**ABuffer++*/*buf++ = '\0';
}


//----------

// returns 'exe', not '.exe'

static __SKEI_UNUSED
const char* SGetFileExt(/*const*/ char* APath) {
  if (APath) {
    char *dot;
    dot = SStrrchr(APath,'.');
    if (dot) return dot+1;
  }
  return (char*)"NULL";
}

//----------

// /home/skei/test -> /home/skei/

static __SKEI_UNUSED
const char* SGetPath(/*const*/ char* APath) {
  if (APath) {
    char *slash, *backslash;
    slash     = SStrrchr(APath,'/');
    backslash = SStrrchr(APath,'\\');//+1;
    if (slash) *(slash+1) = '\0';
    else if (backslash) *(backslash+1) = '\0'; // see above
    return APath;
  }
  return SKEI_NULL;
}

//----------

static __SKEI_UNUSED  int SIsAlpha(int c)   {return (c>='A' && c<='Z') || (c>='a' && c<='z');}
static __SKEI_UNUSED  int SIsDigit(int c)   {return (c>='0' && c<='9');}
static __SKEI_UNUSED  int SIsPrint(int c)   {return c>=' ';}
static __SKEI_UNUSED  int SIsSpace(int c)   {return ((c>=0x09 && c<=0x0D) || (c==0x20));}
static __SKEI_UNUSED  int SIsUpper(int c)   {return (c>='A' && c<='Z');}
static __SKEI_UNUSED  int SIsXdigit(int c)  {return (c>='0' && c<='9') || (c>='A' && c<='F') || (c>='a' && c<='f');}

static __SKEI_UNUSED  int SIsAlnum(int c)   {return SIsAlpha(c) || SIsDigit(c);}

//----------

static __SKEI_UNUSED
void SRadixSort(long *source, long *dest, uint32 N, int byte) {
  uint32 i;
  long count[256];
  long index[256];
  SMemset(count, 0, sizeof(count));
  for (i=0; i<N; i++) count[ ((source[i]) >> (byte*8)) & 0xff ]++;
  index[0] = 0;
  for (i=1; i<256; i++) index[i] = index[i-1]+count[i-1];
  for (i=0; i<N; i++) dest[ index[ ((source[i])>>(byte*8))&0xff ]++ ] = source[i];
}

//----------

// -masm=intel

/*
uint32 SRdtsc(void) {
  uint32 cputime = 0;
  asm(".intel_syntax noprefix");
  asm ("fwait; rdtsc; mov [cputime], eax;");
  asm(".att_syntax prefix");
  return cputime;
}
*/

//----------

// problem if there is a dot in the path
// todo: find last dot..

static __SKEI_UNUSED
char* SStripFileExt(char* APath) {
  //DTrace("...stripping...");
  if (APath) {
    char *dot;
    dot = SStrrchr(APath,'.');
    //if (dot) return dot + 1;
    if (dot) {
      //DTrace("dot");
      *dot = '\0';//return dot + 1;
      return APath;
    }
  }
  return (char*)"NULL";
}

//----------

// /home/skei/test -> test

static __SKEI_UNUSED
const char* SStripPath(const char* APath) {
  if (APath) {
    const char *slash, *backslash;
    slash     = SStrrchr(APath,'/');
    backslash = SStrrchr(APath,'\\');//+1;
    if (slash) return slash + 1;
    else if (backslash) return backslash + 1; // see above
  }
  return (char*)"NULL";
}

//----------

static __SKEI_UNUSED
int SToLower(int c) {
  if (c < 'A' || c > 'Z') return c;
  return c+0x20;
}

//----------

static __SKEI_UNUSED
int SToUpper(int c) {
  if (c < 'a' || c > 'z') return c;
  return c-0x20;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  assume APos is valid
  // not tested.. (esp for-loop ranges..)
*/

//----------

static __SKEI_UNUSED
void SInsertChar(char* ABuffer, int32 APos, char AChar) {
//var
//  i,len : longint;
  if (!ABuffer) return;
  int32 len = SStrlen(ABuffer);
  for (int32 i=(len+1); i>=(APos+1);i--) ABuffer[i] = ABuffer[i-1];
  ABuffer[APos] = AChar;
}

//----------

static __SKEI_UNUSED
void SDeleteChar(char* ABuffer, int32 APos) {
  if (!ABuffer) return;
  int32 len = SStrlen(ABuffer);
  for (int32 i=APos; i<len; i++) ABuffer[i] = ABuffer[i+1];
}

//----------------------------------------------------------------------
#endif
