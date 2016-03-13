#ifndef skei_debug_included
#define skei_debug_included
//----------------------------------------------------------------------

#ifndef SKEI_DEBUG
  #undef SKEI_DEBUG_MEM
  #undef SKEI_DEBUG_MEM_PRINT
  #undef SKEI_DEBUG_CONSOLE
  #undef SKEI_DEBUG_SOCKET
  #undef SKEI_DEBUG_CALLSTACK
  #undef SKEI_DEBUG_CRASHHANDLER
  #undef SKEI_DEBUG_TEST
  #undef SKEI_DEBUG_XLIB
#endif

//----------

#ifndef SKEI_WIN32
  #undef SKEI_DEBUG_CONSOLE
#endif

#ifndef SKEI_LINUX
  #undef SKEI_DEBUG_SOCKET
  #undef SKEI_DEBUG_XLIB
#endif

#ifdef SKEI_DEBUG_MEM_PRINT
  #define SKEI_DEBUG_MEM
#endif

#ifdef SKEI_DEBUG_CRASHHANDLER
  #define SKEI_DEBUG_CALLSTACK
#endif

//----------------------------------------------------------------------

//#include "skei_debug_console.h"
#include "skei_debug_socket.h"
#include "skei_debug_trace.h"

#include "skei_debug_memtrace.h"

#include "skei_debug_assert.h"
#include "skei_debug_callstack.h"
#include "skei_debug_crashhandler.h"

#include "skei_debug_xlib.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#ifdef SKEI_DEBUG

  #define SKEI_BREAKPOINT \
    asm("int $0x3\n");

  //#define SKEI_ERROR(name, fmt, ...) fprintf( stderr, "SKEI_ERROR " #name ": " fmt "\n", __VA_ARGS__ )

#else // SKEI_DEBUG

  #define SKEI_BREAKPOINT {}
  //#define SKEI_ERROR(name, fmt, ...) {}

#endif // SKEI_DEBUG

//----------------------------------------------------------------------
#endif
