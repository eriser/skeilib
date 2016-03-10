#ifndef skei_debug_crashhandler
#define skei_debug_crashhandler
//----------------------------------------------------------------------

/*
  based on:
  http://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes
*/

// compile with: -g -rdynamic

//----------------------------------------------------------------------

#ifdef SKEI_DEBUG_CRASHHANDLER

#include <signal.h>

//----------------------------------------------------------------------

/*
  hmm..
  two first entries (0,1) = crash handler itself (and glib?)
  #2 = positiof of crash..
*/

void __skei_crash_handler(int sig) {
  DTrace("\n...crash...\n");
  SDumpCallStackSkip(2);
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
    //~SDebug_CrashHandler() {
    //  // uninstall signal handler?
    //  // done automatically when program exits?
    //}
};

//----------------------------------------------------------------------

static SDebug_CrashHandler skei_crash_handler;

#endif // SKEI_DEBUG_CRASHHANDLER

//----------------------------------------------------------------------
#endif
