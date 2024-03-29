#ifndef skei_const_included
#define skei_const_included
//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define SKEI_MAGIC            0x535f0000    /* 'S_' + 0x0000 */
#define SKEI_MAGIC_USER       0x735f0000    /* 's_' + 0x0000 */

#define SKEI_MAX_PATH_SIZE    512
#define SKEI_MAX_STRING_SIZE  256
#define SKEI_NULL             0
#define SKEI_EOF             -1
#define SKEI_OK               0
#define SKEI_ERROR           -1

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define SKEI_PI           3.14159265358f
#define SKEI_PI2          (SKEI_PI*2)
#define SKEI_PI05         (SKEI_PI*0.5)

#define SKEI_TINY         (0.001f)

#define SKEI_INV360       (1/360)
#define SKEI_INV12        (1/12)
#define SKEI_INV16        (1/16)
#define SKEI_INV32        (1/16)
#define SKEI_INV127       (1/127)
#define SKEI_INV256       (1/256)
#define SKEI_INV8192      (1/8192)

#define SKEI_INV360F      (1.0f/360.0f)
#define SKEI_INV12F       (1.0f/12.0f)
#define SKEI_INV16F       (1.0f/16.0f)
#define SKEI_INV32F       (1.0f/32.0f)
#define SKEI_INV127F      (1.0f/127.0f)
#define SKEI_INV256F      (1.0f/256.0f)
#define SKEI_INV8192F     (1.0f/8192.0f)

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define SKEI_CHAR_BIT       8

#define SKEI_CHAR_MAX       0x7f                /* 127 */
#define SKEI_SHORT_MAX      0x7fff              /* 32767 */
//#define SKEI_WCHAR_MAX      65535U
#define SKEI_INT32_MAX      0x7fffffff          /* 2147483647 */
#define SKEI_UINT32_MAX     0xffffffff          /* 8589934591U */
#define SKEI_IN64_MAX       0x7fffffffffffffff  /* 9223372036854775807LL */
#define SKEI_UINT64_MAX     0x7fffffffffffffff  /* 18446744073709551615ULL */

#define SKEI_FLOAT_MAX      3.40282347e+38F
#define SKEI_FLOAT_MIN      1.17549435E-38F
#define SKEI_FLOAT_EPSILON  1.19209290E-7F
#define SKEI_FLOAT_DENORM   1.40129846E-45F
#define SKEI_DOUBLE_MAX     1.7976931348623157e+308
#define SKEI_DOUBLE_MIN     2.2250738585072014E-308
#define SKEI_DOUBLE_EPSILON 2.2204460492503131E-16
#define SKEI_DOUBLE_DENORM  4.9406564584124654E-324
//#define SKEI_LDBL_MAX       1.18973149535723176502e+4932L
//#define SKEI_LDBL_MIN       3.36210314311209350626E-4932L
//#define SKEI_LDBL_EPSILON   1.08420217248550443401E-19L
//#define SKEI_LDBL_DENORM    3.64519953188247460253E-4951L

//----------

#ifdef SKEI_32BIT
  #define SKEI_INT_MAX     2147483647L
  #define SKEI_UINT_MAX    8589934591UL
#endif

#ifdef SKEI_64BIT
  #define SKEI_INT_MAX     9223372036854775807L
  #define SKEI_UINT_MAX    18446744073709551615UL
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

static const char SKEI_HEX_TABLE[17] = "0123456789ABCDEF";    // +'\0' = 17

//----------------------------------------------------------------------
#endif

