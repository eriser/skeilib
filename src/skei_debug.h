#ifndef skei_debug_included
#define skei_debug_included
//----------------------------------------------------------------------

/*
  this one is a mess, and should be rewritten,
  probably split up into multiple files:
  - mem tracer
  - socket
  - console
  - trace/dtrace
*/

// see end of this file..

/*
  socket:
  http://www.kvraudio.com/forum/viewtopic.php?p=5714875#p5714875
  nc -U -l -k /tmp/skei.sock
*/

//----------------------------------------------------------------------
// defines
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
// include
//----------------------------------------------------------------------

#include "skei_string.h"
#include "skei_convert.h" // SIntToStr

#include <iostream>
using namespace std;

#include <stdarg.h>     // va_list, va_start, va_arg, va_end
#include <stdio.h>      // printf

//----------

#ifdef SKEI_DEBUG_CONSOLE
  #include <Windows.h>
  //static HANDLE skei_console_out;
#endif

#ifdef SKEI_DEBUG_SOCKET
  #include <sys/socket.h>
  #include <sys/un.h>
  #include <unistd.h>
  #include <string.h>
  //static int skei_socket_out;
#endif

//----------------------------------------------------------------------
// node for mem tracer (double-lined list)
//----------------------------------------------------------------------

struct skei_debug_memnode {
  skei_debug_memnode* MPrev;
  skei_debug_memnode* MNext;
  void*     MAddr;
  int       MSize;
  char      MFile[SKEI_MAX_STRING_SIZE+1];
  char      MFunc[SKEI_MAX_STRING_SIZE+1];
  int       MLine;
  bool      MNew;
};

//----------------------------------------------------------------------

/*
  in case we want to change them...
  by not using SMalloc, etc, we avoid keeping track of the small allocs for
  the mem nodes, and also for the proper/real allocations
*/

#define memtrace_malloc   malloc
#define memtrace_calloc   calloc
#define memtrace_realloc  realloc
#define memtrace_free     free

//----------

//#define _trace    prefix(SStripPath(__FILE__),__FUNCTION__,__LINE__); trace
//#define _dtrace   dtrace

//----------------------------------------------------------------------
// SDebug
//----------------------------------------------------------------------

class SDebug {

  private:

    #ifdef SKEI_DEBUG
      char    MPrefix[256];
      int     MPrefixLen;
    #endif

    #ifdef SKEI_DEBUG_CONSOLE
      HANDLE  skei_console_out;
    #endif

    #ifdef SKEI_DEBUG_SOCKET
      int     skei_socket_out;
    #endif

    #ifdef SKEI_DEBUG_MEM
    //bool      MStarted;
      skei_debug_memnode* MHead;
      skei_debug_memnode* MTail;
      int       MAllocated;
      int       MAllocatedNum;
      int       MAllocatedMax;
      int       MNumAlloc;
      int       MNumFree;
    #endif

    //#ifdef SKEI_DEBUG_LOG
    //#endif

  //----------------------------------------

  #ifdef SKEI_DEBUG_MEM

  private:

    // append to tail...
    void _appendNode(skei_debug_memnode* ANode) {
      if (MTail) {
        // xx XX --
        ANode->MPrev = MTail;
        ANode->MNext = SKEI_NULL;
        MTail->MNext = ANode;
        MTail = ANode;
      }
      else {
        // -- XX --
        MHead = ANode;
        ANode->MPrev = SKEI_NULL;
        ANode->MNext = SKEI_NULL;
        MTail = ANode;
      }
      MAllocated += ANode->MSize;
      MAllocatedNum++;
      if (MAllocated > MAllocatedMax) MAllocatedMax = MAllocated;
    }

    // remove node
    void _removeNode(skei_debug_memnode* ANode) {
      skei_debug_memnode* next = ANode->MNext;
      skei_debug_memnode* prev = ANode->MPrev;
      if (prev) {
        if (next) {
          //  xx XX xx
          prev->MNext = next;
          next->MPrev = prev;
        }
        else {
          // xx XX -- (tail)
          prev->MNext = NULL;
          MTail = prev;
        }
      }
      else { // !head
        if (next) {
          // -- XX xx (head)
          MHead = next;
          next->MPrev = NULL;
        }
        else {
          // -- XX -- (single)
          MHead = NULL;
          MTail = NULL;
        }
      } // !next
      MAllocated -= ANode->MSize;
      MAllocatedNum--;
      //MFreedNum++;
    }

  #endif // SKEI_DEBUG_MEM

  //----------------------------------------

  #ifdef SKEI_DEBUG_MEM

  public:

    void append(void* AAddr, unsigned int ASize, const char* AFile, const char* AFunc, unsigned int ALine, bool ANew) {
      //if (MStarted) {
        const char* file = SStripPath(AFile);
        skei_debug_memnode* node = (skei_debug_memnode*)memtrace_malloc(sizeof(skei_debug_memnode)); // !!!
        MNumAlloc++;
        node->MAddr = AAddr;
        node->MSize = ASize;
        SStrncpy(node->MFile, file ,SKEI_MAX_STRING_SIZE);
        SStrncpy(node->MFunc, AFunc ,SKEI_MAX_STRING_SIZE);
        node->MLine = ALine;
        node->MNew = ANew;
        _appendNode(node);
        #ifdef SKEI_DEBUG_MEM_PRINT
        /*_dtrace*/ dtrace("+ [%s:%i:%s] %s, 0x%x, %i\n",file,ALine,AFunc,(ANew?"new":"malloc"),AAddr,ASize);
        #endif
      //} // MStarted
    }

    void remove(void* AAddr, __unused const char* AFile, __unused const char* AFunc, __unused unsigned int ALine, __unused bool ANew) {
      //if (MStarted) {
        #ifdef SKEI_DEBUG_MEM_PRINT
        const char* file = SStripPath(AFile);
        #endif
        skei_debug_memnode* node = MHead;
        while (node) {
          if (node->MAddr==AAddr) break;
          node = (skei_debug_memnode*)node->MNext;
        }
        if (node) {
          _removeNode(node);
          #ifdef SKEI_DEBUG_MEM_PRINT
          /*_dtrace*/ dtrace("- [%s:%i:%s] %s, 0x%x, %i\n",file,ALine,AFunc,(ANew?"delete":"free"),node->MAddr,node->MSize);
          if (node->MNew != ANew) {
            /*_dtrace*/ dtrace("! mismatch!!! [%s:%i:%s] %s\n",node->MFile,node->MLine,node->MFunc,(node->MNew?"new":"malloc"));
          }
          #endif
          memtrace_free(node); // !!!
          MNumFree++;
        }
        //else {
        //  DTrace("! node not found !");
        //  // and it will probably crash/segfault...
        //}
      //} // MStarted
    }

    void dump(void) {
      /*_dtrace*/ dtrace("----------------------------------------\n");
      /*_dtrace*/ dtrace("* num alloc     %i\n",MNumAlloc);
      /*_dtrace*/ dtrace("* num free      %i\n",MNumFree);
      /*_dtrace*/ dtrace("* num leaks     %i\n",MAllocatedNum);
      /*_dtrace*/ dtrace("* max allocated %i\n",MAllocatedMax);
      /*_dtrace*/ dtrace("* leaked memory %i\n",MAllocated);
      if (MAllocated > 0)
      {
        /*_dtrace*/ dtrace("----------------------------------------\n");
        skei_debug_memnode* node = (skei_debug_memnode*)MHead;
        while (node) {
          /*_dtrace*/ dtrace("* [%s:%s:%i] %s,0x%x,%i\n",node->MFile,node->MFunc,node->MLine,(node->MNew?"new":"malloc"),node->MAddr,node->MSize);
          // /*_dtrace*/ dtrace("* %i\n",node->MLine);
          skei_debug_memnode* next = (skei_debug_memnode*)node->MNext;
          //if (ADelete) { free(node); }
          node = next;
        }
      }
      /*_dtrace*/ dtrace("----------------------------------------\n");
    }

    void* trace_malloc(const size_t size, const char* file, const char* func, unsigned int line) {
      void* ptr = memtrace_malloc(size);
      append(ptr,size,file,func,line,false);
      return ptr;
    }

    void* trace_calloc(const int num, const size_t size, const char* file, const char* func, unsigned int line) {
      void* ptr = memtrace_calloc(num,size);
      append(ptr,num*size,file,func,line,false);
      return ptr;
    }

    void* trace_realloc(void* ptr, const size_t size, const char* file, const char* func, unsigned int line) {
      void* p2 = memtrace_realloc(ptr,size);
      remove(ptr,file,func,line,false);
      append(p2,size,file,func,line,false);
      return p2;
    }

    void trace_free(void* ptr, const char* file, const char* func, unsigned int line) {
      remove(ptr,file,func,line,false);
      memtrace_free(ptr);
    }

  #endif // SKEI_DEBUG_MEM

  //----------------------------------------

  public:

    void prefix(const char* file, const char* func, int line) {
      #ifdef SKEI_DEBUG
        char linebuf[256];
        SStrcpy(MPrefix,"[");
        SStrcat(MPrefix,file);
        SStrcat(MPrefix,":");
        SStrcat(MPrefix,func);
        SStrcat(MPrefix,":");
        SStrcat(MPrefix,SIntToString(linebuf,line));
        SStrcat(MPrefix,"] ");
        MPrefixLen = SStrlen(MPrefix);
      #endif // SKEI_DEBUG
    }

    /*
      printf   :
      vsprintf :
      vprintf  : print to stdout
      vdprintf : print to a file descriptor
    */

    void trace(const char* format, ...) {
      #ifdef SKEI_DEBUG
        char buffer[256];
        SStrcpy(buffer,MPrefix);
        va_list args;
        va_start(args,format);
        vsprintf((buffer+MPrefixLen),format,args);
        #ifdef SKEI_DEBUG_CONSOLE
          unsigned long written;
          WriteConsole(skei_console_out,buffer,SStrlen(buffer),&written,SKEI_NULL);
          //vsprintf(skei_console_out,format,args);
          //sprintf(skei_console_out,"%s\n",buffer);
        #elif defined SKEI_DEBUG_SOCKET
          //printf("printing to socket!\n");
          //vdprintf(skei_socket_out,format,args);
          dprintf(skei_socket_out,"%s",buffer);
        #else
          printf("%s",buffer);
        #endif
        va_end(args);
      #endif // SKEI_DEBUG
    }

    /*
    void dtrace(const char* format, ...) {
      #ifdef SKEI_DEBUG
        va_list args;
        va_start(args,format);
        #ifdef SKEI_DEBUG_CONSOLE
          //vsprintf(skei_console_out,format,args);
        #elif defined SKEI_DEBUG_SOCKET
          vdprintf(skei_socket_out,format,args);
        #else
          vprintf(format,args);
        #endif
        va_end(args);
      #endif
    }
    */

    void dtrace(const char* format, ...) {
      #ifdef SKEI_DEBUG
        char buffer[256];
        //SStrcpy(buffer,MPrefix);
        va_list args;
        va_start(args,format);
        //vsprintf((buffer+MPrefixLen),format,args);
        vsprintf(buffer,format,args);
        #ifdef SKEI_DEBUG_CONSOLE
          unsigned long written;
          WriteConsole(skei_console_out,buffer,SStrlen(buffer),&written,SKEI_NULL);
          //vsprintf(skei_console_out,format,args);
          //sprintf(skei_console_out,"%s\n",buffer);
        #elif defined SKEI_DEBUG_SOCKET
          //printf("printing to socket!\n");
          //vdprintf(skei_socket_out,format,args);
          dprintf(skei_socket_out,"%s",buffer);
        #else
          printf("%s",buffer);
        #endif
        va_end(args);
      #endif // SKEI_DEBUG
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    SDebug() {

      #ifdef SKEI_DEBUG
        MPrefix[0] = '\0';
        MPrefixLen = 0;
      #endif

      #ifdef SKEI_DEBUG_CONSOLE
        AllocConsole();
        SetConsoleTitle("skei_console");
        skei_console_out = GetStdHandle(STD_OUTPUT_HANDLE);
      #endif

      #ifdef SKEI_DEBUG_SOCKET
        skei_socket_out = socket(PF_UNIX,SOCK_STREAM,0);
        sockaddr_un address;
        memset(&address, 0, sizeof(sockaddr_un));
        address.sun_family = AF_UNIX;
        snprintf(address.sun_path,108,"/tmp/skei.sock");
        connect(skei_socket_out,reinterpret_cast<sockaddr*>(&address),sizeof(sockaddr_un));
      #endif

      //#ifdef SKEI_DEBUG_LOG
      //#endif

      #ifdef SKEI_DEBUG_MEM
        //MStarted       = false;
        MHead          = SKEI_NULL;
        MTail          = SKEI_NULL;
        MAllocated     = 0;
        MAllocatedNum  = 0;
        MAllocatedMax  = 0;
        MNumAlloc      = 0;
        MNumFree       = 0;
      #endif

    }

    ~SDebug() {

      #ifdef SKEI_DEBUG_MEM
        dump();
      #endif

      #ifdef SKEI_DEBUG_CONSOLE
        FreeConsole();
      #endif

      #ifdef SKEI_DEBUG_SOCKET
        close(skei_socket_out);
      #endif

      //#ifdef SKEI_DEBUG_LOG
      //#endif

    }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  being static means that multiple instances uses the same
  static_MemTrace object.. it's shared for all instances/threads..

  needs to be static, so our 'overloaded' new/delete macros can use it.
  would be better if it were part of the SDebug class, but i haven't found
  a nice/clean way to replace the new/delete with macros that call a
  non-static class method. if this were part of a class, this class must be
  available/accessible during our new (pun..) new/delete macros. everywhere.

  __thread ??

  multiple instances can be in a single thread, or in multiple threads.
  our problem above is not related to threads, but that many plugin instances
  have access to the same static/global class-instance.
*/

static SDebug skei_debug;

//----------------------------------------------------------------------
//
// mem trace
//
//----------------------------------------------------------------------

#ifdef SKEI_DEBUG_MEM

  // not thread-safe?
  // unreliable (and dangerous?) if multiple plugin instances uses
  // mem-tracing at the same time.. ??

  // these should be defined as __thread
  static __thread char*         mem_del_file;
  static __thread char*         mem_del_func;
  static __thread unsigned int  mem_del_line;

  //----------

  bool SDebug_MemDelPrepare(const char* file, const char* func, const unsigned int line) {
    //DTrace("leaks_del_prepare:" /*<< SStripPath(file) << "," << func << "," << line*/);
    mem_del_file = (char*)file;
    mem_del_func = (char*)func;
    mem_del_line = line;
    return true;
  }

  void* operator new (const size_t size, const char* file, const char* func, unsigned int line) throw (std::bad_alloc) {
    //DTrace("op new");
    void* ptr = memtrace_malloc(size);
    skei_debug.append(ptr,size,file,func,line,true);
    return ptr;
  }

  void* operator new[] (const size_t size, const char* file, const char* func, unsigned int line) throw (std::bad_alloc) {
    //DTrace("op new[]");
    void* ptr = memtrace_malloc(size);
    skei_debug.append(ptr,size,file,func,line,true);
    return ptr;
  }

  void operator delete (void* ptr) throw() {
    //DTrace("op delete");
    memtrace_free(ptr);
    skei_debug.remove(ptr,mem_del_file,mem_del_func,mem_del_line,true);
  }

  void operator delete[] (void* ptr) throw() {
    //DTrace("op delete[]");
    memtrace_free(ptr);
    skei_debug.remove(ptr,mem_del_file,mem_del_func,mem_del_line,true);
  }

  //----------

  // catch malloc

  #define malloc(size)      skei_debug.trace_malloc(size,     __FILE__,__FUNCTION__,__LINE__)
  #define calloc(num,size)  skei_debug.trace_calloc(num,size, __FILE__,__FUNCTION__,__LINE__)
  #define realloc(ptr,size) skei_debug.trace_realloc(ptr,size,__FILE__,__FUNCTION__,__LINE__)
  #define free(ptr)         skei_debug.trace_free(ptr,        __FILE__,__FUNCTION__,__LINE__)

  // catch op. new / delete

  //TEST

  //#define new     new(__FILE__, __FUNCTION__, __LINE__)
  //#define delete  if (SDebug_MemDelPrepare(__FILE__, __FUNCTION__, __LINE__)) delete

  #define debug_new     new(__FILE__, __FUNCTION__, __LINE__)
  #define debug_delete  if (SDebug_MemDelPrepare(__FILE__, __FUNCTION__, __LINE__)) delete

  #define new     debug_new
  #define delete  debug_delete

  // http://wyw.dcweb.cn/leakage.htm
  //void* operator new(size_t size, const char* file, int line);
  //void* operator new[](size_t size, const char* file, int line);
  //#define DEBUG_NEW new(__FILE__, __LINE__)
  //#define new DEBUG_NEW


#endif // SKEI_DEBUG_MEM

//----------------------------------------------------------------------
//
// trace
//
//----------------------------------------------------------------------

#define STrace \
  skei_debug.prefix(SStripPath(__FILE__),__FUNCTION__,__LINE__); \
  skei_debug.trace

#define DTrace \
  skei_debug.dtrace

//----------------------------------------------------------------------
//
// assert
//
//----------------------------------------------------------------------

/*
  ISO C/C++ compatible runtime and static assertion for GCC (4.4.x)
  no external dependencies.
*/

//#include "skei_trace.h"
#include <iostream> // cout

// for c89 compat mainly if the header is used elsewhere
#ifdef __GNUC_STDC_INLINE__
  #define SKEI_STATIC_ASSERT_INLINE inline
#else
  #define SKEI_STATIC_ASSERT_INLINE
#endif

#ifdef SKEI_DEBUG

  /*
    SAssert()
    runtime assertion
  */

  unsigned int _SAssert () __attribute__ (( noinline ));
  unsigned int _SAssert (const unsigned int e, const char* file, const unsigned int line, const char* e_str) {
    if (!e) {
      //DTrace("SKEI_ASSERT: " << file << ", " << line << ", (" << e_str << ")");
      DTrace("SKEI_ASSERT: %s, %i, %s\n",file,line,e_str);
      __builtin_exit(0);
    }
    return 1;
  }

  #define SAssert(e) _SAssert((e), __FILE__, __LINE__, #e)

  //----------

  /*
    SStaticAssert()
    compile time assertion ( requires GCC 4.4.x )
  */

  #if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 4)
    SKEI_STATIC_ASSERT_INLINE unsigned int SKEI_STATIC_ASSERT_TRUE (void) { return 1; }
    unsigned int SKEI_STATIC_ASSERT (void)
    __attribute__ (( error("### SKEI_STATIC_ASSERT at this point!") ))
    __attribute__ (( optimize(0) ));
    #define SStaticAssert(e) ( (!(e)) ? SKEI_STATIC_ASSERT() : SKEI_STATIC_ASSERT_TRUE() )
  #else
    /*
      switch can also be used. it does not generate extra code and is much
      more portable, but not very useful at the same time.
    */
    #define SStaticAssert(e) switch(0) { case 0: case (e): ; }
  #endif

#else // SKEI_DEBUG

  SKEI_STATIC_ASSERT_INLINE unsigned int _SKEI_STATIC_ASSERT (void) { return 0; }
  #define SAssert(e)       _SKEI_STATIC_ASSERT()
  #define SStaticAssert(e) _SKEI_STATIC_ASSERT()

#endif // SKEI_DEBUG

//----------------------------------------------------------------------
//
// cleanup
//
//----------------------------------------------------------------------

#undef memtrace_malloc
#undef memtrace_calloc
#undef memtrace_realloc
#undef memtrace_free

//----------------------------------------------------------------------
#endif

//----------

/*

  http://stackoverflow.com/questions/1054697/why-isnt-my-new-operator-called

  The problem is that on most UNIX platforms (unlike on Win32 and AIX) all
  symbol references by default bind to the first definition of the symbol
  visible to the runtime loader.
  If you define 'operator new' in the main a.out, everything will bind to that
  definition (as Neil Butterworth's example shows), because a.out is the very
  first image runtime loader searches.
  If you define it in a library which is loaded after libC.so (or libstdc++.so
  in case you are using GCC), then your definition will never be used. Since you
  are dlopen()ing your library after the program has started, libC is already
  loaded by that point, and your library is the very last one the runtime loader
  will search; so you lose.
  On ELF platforms, you may be able to change the default behavior by using
  -Bsymbolic. From man ld on Linux:
  -Bsymbolic
    When creating a shared library, bind references to global symbols
    to the definition within the shared library, if any. Normally, it
    is possible for a program linked against a shared library to override
    the  definition within the shared library. This option is only meaningful
    on ELF platforms which support shared libraries.
  Note that -Bsymbolic is a linker flag, not a compiler flag. If using g++, you
  must pass the flag to the linker like this:
  g++ -fPIC -shared library.cpp -o library.so -Wl,-Bsymbolic

*/

//----------

/*

code::blocks

compiler settings/other options
  -fPIC
  -rdynamic
  -Wl,-Bsynbolic

linker settings/other linker options
  -Wl,-Bsymbolic

removed from linker options, and it looks like it works anyway:
  -fPIC
  -export-dynamic


*/

//----------

/*
  some interesting things:
  http://eli.thegreenplace.net/2011/02/17/the-many-faces-of-operator-new-in-c/
  http://oroboro.com/overloading-operator-new/

  http://wyw.dcweb.cn/leakage.htm
*/
