#ifndef skei_debug_included
#define skei_debug_included
//----------------------------------------------------------------------

#ifndef SKEI_DEBUG
  #undef SKEI_DEBUG_MEM
  #undef SKEI_DEBUG_MEM_PRINT
  #undef SKEI_DEBUG_CONSOLE
  #undef SKEI_DEBUG_SOCKET
#endif

#ifndef SKEI_WIN32
  #undef SKEI_DEBUG_CONSOLE
#endif

#ifndef SKEI_LINUX
  #undef SKEI_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------

//#include "skei_debug_console.h"
#include "skei_debug_socket.h"
#include "skei_debug_trace.h"
#include "skei_debug_memtrace.h"
#include "skei_debug_assert.h"

//----------------------------------------------------------------------
#endif
