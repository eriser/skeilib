#ifndef skei_debug_crashhandler
#define skei_debug_crashhandler
//----------------------------------------------------------------------

#ifdef SKEI_DEBUG_CRASHHANDLER

//----------------------------------------------------------------------

// compile with: -g -rdynamic

#include <signal.h>

//----------------------------------------------------------------------

/*
  hmm..
  two first entries (0,1) = crash handler itself (and glib?)
  #2 = positiof of crash..
*/

void __skei_crash_handler(int sig) {
  SDumpCallStack;
  // You can't call exit() safely from a signal handler.
  // Use _exit() or _Exit()
  //exit(1);
  _exit(1);
}

//----------------------------------------------------------------------

class SDebug_CrashHandler {
  public:
    SDebug_CrashHandler() {
      signal(SIGSEGV,__skei_crash_handler);
    }
};

//----------

static SDebug_CrashHandler skei_crash_handler;

//----------

#endif // SKEI_DEBUG_CRASHHANDLER

//----------------------------------------------------------------------
#endif
