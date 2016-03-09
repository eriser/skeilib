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
#endif

#ifndef SKEI_WIN32
  #undef SKEI_DEBUG_CONSOLE
#endif

#ifndef SKEI_LINUX
  #undef SKEI_DEBUG_SOCKET
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

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define SKEI_BREAKPOINT \
  asm("int $0x3\n");

//----------

#define SKEI_ERROR(name, fmt, ...) \
  fprintf( stderr, "SKEI_ERROR " #name ": " fmt "\n", __VA_ARGS__ )

//----------


//----------------------------------------------------------------------
#endif
