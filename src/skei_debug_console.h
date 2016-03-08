#ifndef skei_debug_console_included
#define skei_debug_console_included
//----------------------------------------------------------------------

#ifdef SKEI_DEBUG_CONSOLE

#include <Windows.h>

//----------------------------------------------------------------------

class SDebug_Console {

  private:

    HANDLE  skei_console_out;

  public:

    SDebug_Console() {
      AllocConsole();
      SetConsoleTitle("skei_console");
      skei_console_out = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    //----------

    ~SDebug_Console() {
      FreeConsole();
    }

    //----------

    void print(const char* ABuffer) {
      unsigned long written;
      WriteConsole(skei_console_out,ABuffer,SStrlen(ABuffer),&written,SKEI_NULL);
    }

};

//----------------------------------------------------------------------

static SDebug_Console skei_debug_console;

//----------------------------------------------------------------------

#define SDebug_Console_Print  skei_debug_console.print

#endif // SKEI_DEBUG_CONSOLE

//----------------------------------------------------------------------
#endif
