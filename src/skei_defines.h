#ifndef skei_defines_included
#define skei_defines_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// platform
//----------------------------------------------------------------------

#ifdef linux
  #define SKEI_LINUX
#endif

#ifdef WIN32
  #define  SKEI_WIN32
  #define  WINVER 0x0501
  #undef  _WIN32_WINNT
  #define _WIN32_WINNT 0x0501
  #define  WIN32_LEAN_AND_MEAN /* exclude APIs such as Cryptography, DDE, RPC, Shell, and Windows Sockets */
#endif

//----------------------------------------------------------------------
// format
//----------------------------------------------------------------------

#ifdef SKEI_DSSI
  #define SKEI_LIB
#endif

#ifdef SKEI_EXE
  #undef SKEI_LIB
#endif

#ifdef SKEI_LV2
  #define SKEI_LIB
#endif

#ifdef SKEI_LADSPA
  #define SKEI_LIB
#endif

#ifdef SKEI_VST
  #define SKEI_LIB
#endif

//----------------------------------------------------------------------
// gui
//----------------------------------------------------------------------

#undef SKEI_GUI

//----------

#ifdef SKEI_XLIB
  #define SKEI_GUI
#else
  #undef SKEI_XRENDER
#endif

#ifdef SKEI_GDI
  #define SKEI_GUI
#endif

#ifdef SKEI_CAIRO
  #define SKEI_GUI
#endif

#ifdef SKEI_OPENGL
  #define SKEI_GUI
#endif

//----------------------------------------------------------------------
// architecture
//----------------------------------------------------------------------

#if defined  (__i386__) || defined (_X86_) || defined (__X86__)
  #define SKEI_32BIT
#endif

#if defined (__x86_64) || (__LP64__)
  #define SKEI_64BIT
#endif

//----------

//#if defined (__ORDER_BIG_ENDIAN__)
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  #define SKEI_BIG_ENDIAN
#endif

//#if defined (__ORDER_LITTLE_ENDIAN__)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  #define SKEI_LITTLE_ENDIAN
#endif

//----------------------------------------------------------------------
// dllexport, dllimport, cdecl, stdcall
//----------------------------------------------------------------------

/*

#undef  __dllexport
#undef  __dllimport
#undef  __cdecl
#undef  __stdcall

#ifdef SKEI_WIN32
  #define __dllexport __attribute__ ((dllexport))
  #define __dllimport __attribute__ ((dllimport))
  #define __cdecl     __attribute__ ((cdecl))
  #define __stdcall   __attribute__ ((stdcall))
#endif

#ifdef SKEI_LINUX
  #define __dllexport
  #define __dllimport
  #define __cdecl
  #define __stdcall
#endif

*/

#ifdef SKEI_WIN32
  #define __SKEI_DLLEXPORT __attribute__ ((dllexport))
  #define __SKEI_DLLIMPORT __attribute__ ((dllimport))
  #define __SKEI_CDECL     __attribute__ ((cdecl))
  #define __SKEI_STDCALL   __attribute__ ((stdcall))
#endif

#ifdef SKEI_LINUX
  #define __SKEI_DLLEXPORT
  #define __SKEI_DLLIMPORT
  #define __SKEI_CDECL
  #define __SKEI_STDCALL
#endif


//----------------------------------------------------------------------
// attributes
//----------------------------------------------------------------------

//#define __aligned_t(x)    __attribute__ ((aligned (x)))
//#define __may_alias_t     __attribute__ ((may_alias))

/*

#define __aligned(x)              __attribute__ ((aligned (x)))
#define __alignedmax              __attribute__ ((aligned))
#define __packed                  __attribute__ ((packed))
#define ___shared                 __attribute__ ((shared))
#define __section(name)           __attribute__ ((section (name)))
#define __used                    __attribute__ ((used))
#define __unused                  __attribute__ ((unused))
#define __vector(x)               __attribute__ ((vector_size (x)))
#define __selectany               __attribute__ ((selectany))
#define __transparent_union       __attribute__ ((transparent_union))
#define __may_alias               __attribute__ ((may_alias))
#define __deprecated              __attribute__ ((deprecated))

*/

#define __SKEI_ALIGNED(x)           __attribute__ ((aligned (x)))
#define __SKEI_ALIGNEDMAX           __attribute__ ((aligned))
#define __SKEI_PACKED               __attribute__ ((packed))
#define __SKEI_SHARED               __attribute__ ((shared))
#define __SKEI_SECTION(name)        __attribute__ ((section (name)))
#define __SKEI_USED                 __attribute__ ((used))
#define __SKEI_UNUSED               __attribute__ ((unused))
#define __SKEI_VECTOR(x)            __attribute__ ((vector_size (x)))
#define __SKEI_SELECTANY            __attribute__ ((selectany))
#define __SKEI_TRANSPARENT_UNION    __attribute__ ((transparent_union))
#define __SKEI_MAY_ALIAS            __attribute__ ((may_alias))
#define __SKEI_DEPRECATED           __attribute__ ((deprecated))

/*

#define __constructor             __attribute__ ((constructor))
#define __destructor              __attribute__ ((destructor))
#define __alloc_size1(x)          __attribute__ ((alloc_size(x)))
#define __alloc_size2(x, y)       __attribute__ ((alloc_size(x, y)))
#define __alias(name)             __attribute__ ((alias (name)))
#define ___always_inline   inline __attribute__ ((always_inline))
#define __malloc                  __attribute__ ((malloc))
#define __noinline                __attribute__ ((noinline))
#define __noreturn                __attribute__ ((noreturn))
#define __pure                    __attribute__ ((pure))
#define __target(value)           __attribute__ ((target (value)))
#define __weak                    __attribute__ ((weak))
#define __weakref(name)           __attribute__ ((weakref (name)))

*/

#define __SKEI_CONSTRUCTOR             __attribute__ ((constructor))
#define __SKEI_DESTRUCTOR              __attribute__ ((destructor))
#define __SKEI_ALLOC_SIZE1(x)          __attribute__ ((alloc_size(x)))
#define __SKEI_ALLOC_SIZE2(x, y)       __attribute__ ((alloc_size(x, y)))
#define __SKEI_ALIAS(name)             __attribute__ ((alias (name)))
#define __SKEI_ALWAYS_INLINE    inline __attribute__ ((always_inline))
#define __SKEI_MALLOC                  __attribute__ ((malloc))
#define __SKEI_NOINLINE                __attribute__ ((noinline))
#define __SKEI_NORETURN                __attribute__ ((noreturn))
#define __SKEI_PURE                    __attribute__ ((pure))
#define __SKEI_TARGET(value)           __attribute__ ((target (value)))
#define __SKEI_WEAK                    __attribute__ ((weak))
#define __SKEI_WEAKREF(name)           __attribute__ ((weakref (name)))

/*

#define __externc     extern "C"
#define __may_alias   __attribute__ ((may_alias))
#define __hotinline   ___always_inline __hot __optimize(3)
#define __asmv        __asm__ __volatile__
#define __vlt         __volatile__

*/

#define __SKEI_EXTERNC      extern "C"
#define __SKEI_MAY_ALIAS    __attribute__ ((may_alias))
#define __SKEI_HOTINLINE    ___always_inline __hot __optimize(3)
#define __SKEI_ASMV         __asm__ __volatile__
#define __SKEI_VLT          __volatile__


//----------------------------------------------------------------------
// newer version of GCC required
//----------------------------------------------------------------------

/*
#if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 4)
  #define __optimize(level)         __attribute__ ((optimize (level)))
  #define __hot                     __attribute__ ((hot))
  #define __cold                    __attribute__ ((cold))
  #define __error(message)          __attribute__ ((error(message)))
  #define __warning(message)        __attribute__ ((warning(message)))
#else
  #define __optimize(level)
  #define __hot
  #define __cold
  #define __error
  #define __warning
  // disable TLS & show warning
  #undef __thread
  #define __thread
  #warning "### Thread-local storage requires GCC 4.4.x"
#endif
*/

#if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 4)
  #define __SKEI_OPTIMIZE(level)  __attribute__ ((optimize (level)))
  #define __SKEI_HOT              __attribute__ ((hot))
  #define __SKEI_COLD             __attribute__ ((cold))
  #define __SKEI_ERROR(message)   __attribute__ ((error(message)))
  #define __SKEI_WARNING(message) __attribute__ ((warning(message)))
#else
  #define __SKEI_OPTIMIZE(level)
  #define __SKEI_HOT
  #define __SKEI_COLD
  #define __SKEI_ERROR
  #define __SKEI_WARNING
  // disable TLS & show warning
//  #undef __thread
  #define __SKEI_THREAD
  #warning "### Thread-local storage requires GCC 4.4.x"
#endif


//----------------------------------------------------------------------
#endif

