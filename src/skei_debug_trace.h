#ifndef skei_debug_trace_included
#define skei_debug_trace_included
//----------------------------------------------------------------------

#ifdef SKEI_DEBUG

#include <stdarg.h>       // va_list, va_start, va_arg, va_end
#include <stdio.h>        // printf
#include "skei_string.h"  //
#include "skei_convert.h" // SIntToStr

//----------------------------------------------------------------------

class SDebug_Trace {

  private:

    char  MPrefix[256];
    int   MPrefixLen;

  public:

    SDebug_Trace() {
      MPrefix[0] = '\0';
      MPrefixLen = 0;
    }

    //----------

    ~SDebug_Trace() {
    }

    //----------

    void prefix(const char* file, const char* func, int line) {
      char linebuf[256];
      SStrcpy(MPrefix,"[");
      SStrcat(MPrefix,file);
      SStrcat(MPrefix,":");
      SStrcat(MPrefix,func);
      SStrcat(MPrefix,":");
      SStrcat(MPrefix,SIntToString(linebuf,line));
      SStrcat(MPrefix,"] ");
      MPrefixLen = SStrlen(MPrefix);
    }

    //----------

    void trace(const char* format, ...) {
        char buffer[256];
        SStrcpy(buffer,MPrefix);
        va_list args;
        va_start(args,format);
        vsprintf((buffer+MPrefixLen),format,args);
        #ifdef SKEI_DEBUG_CONSOLE
          SDebug_Console_Print(buffer);
        #elif defined SKEI_DEBUG_SOCKET
          SDebug_Socket_Print(buffer);
        #else
          printf("%s",buffer);
        #endif
        va_end(args);
    }

    //----------

    void dtrace(const char* format, ...) {
      char buffer[256];
      //SStrcpy(buffer,MPrefix);
      va_list args;
      va_start(args,format);
      //vsprintf((buffer+MPrefixLen),format,args);
      vsprintf(buffer,format,args);
      #ifdef SKEI_DEBUG_CONSOLE
        SDebug_Console_Print(buffer);
      #elif defined SKEI_DEBUG_SOCKET
        SDebug_Socket_Print(buffer);
      #else
        printf("%s",buffer);
      #endif
      va_end(args);
    }

};

//----------------------------------------------------------------------

static SDebug_Trace skei_debug_trace;

//----------------------------------------------------------------------

#define STrace \
  skei_debug_trace.prefix(SStripPath(__FILE__),__FUNCTION__,__LINE__); \
  skei_debug_trace.trace

//----------

#define DTrace \
  skei_debug_trace.dtrace

#else // SKEI_DEBUG

__SKEI_ALWAYS_INLINE void STrace(const char* format, ...) {}
__SKEI_ALWAYS_INLINE void DTrace(const char* format, ...) {}

#endif //SKEI_DEBUG

//----------------------------------------------------------------------
#endif
