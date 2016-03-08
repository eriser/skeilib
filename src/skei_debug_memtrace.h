#ifndef skei_debug_memtrace_included
#define skei_debug_memtrace_included
//----------------------------------------------------------------------

#ifdef SKEI_DEBUG_MEM

#include <iostream>
//using namespace std;

#include "skei_utils.h"   // SStripPath

//----------------------------------------------------------------------

/*
  in case we want to change them...
  by not using SMalloc, etc, we avoid keeping track of the small allocs for
  the mem nodes, and also for the proper/real allocations
*/

#define skei_memtrace_malloc   malloc
#define skei_memtrace_calloc   calloc
#define skei_memtrace_realloc  realloc
#define skei_memtrace_free     free

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

class SDebug_MemTrace {

  private:

    skei_debug_memnode* MHead;
    skei_debug_memnode* MTail;
    int                 MAllocated;
    int                 MAllocatedNum;
    int                 MAllocatedMax;
    int                 MNumAlloc;
    int                 MNumFree;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    SDebug_MemTrace() {
      MHead          = SKEI_NULL;
      MTail          = SKEI_NULL;
      MAllocated     = 0;
      MAllocatedNum  = 0;
      MAllocatedMax  = 0;
      MNumAlloc      = 0;
      MNumFree       = 0;
    }

    //----------

    ~SDebug_MemTrace() {
      dump();
    }

  //----------------------------------------
  //
  //----------------------------------------

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

    //----------

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

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void append(void* AAddr, unsigned int ASize, const char* AFile, const char* AFunc, unsigned int ALine, bool ANew) {
      //if (MStarted) {
        const char* file = SStripPath(AFile);
        skei_debug_memnode* node = (skei_debug_memnode*)skei_memtrace_malloc(sizeof(skei_debug_memnode)); // !!!
        MNumAlloc++;
        node->MAddr = AAddr;
        node->MSize = ASize;
        SStrncpy(node->MFile, file ,SKEI_MAX_STRING_SIZE);
        SStrncpy(node->MFunc, AFunc ,SKEI_MAX_STRING_SIZE);
        node->MLine = ALine;
        node->MNew = ANew;
        _appendNode(node);
        #ifdef SKEI_DEBUG_MEM_PRINT
        DTrace("+ [%s:%i:%s] %s, 0x%x, %i\n",file,ALine,AFunc,(ANew?"new":"malloc"),AAddr,ASize);
        #endif
      //} // MStarted
    }

    //----------

    //void remove(void* AAddr, __unused const char* AFile, __unused const char* AFunc, __unused unsigned int ALine, __unused bool ANew) {
    void remove(void* AAddr, __SKEI_UNUSED const char* AFile, __SKEI_UNUSED const char* AFunc, __SKEI_UNUSED unsigned int ALine, __SKEI_UNUSED bool ANew) {
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
          DTrace("- [%s:%i:%s] %s, 0x%x, %i\n",file,ALine,AFunc,(ANew?"delete":"free"),node->MAddr,node->MSize);
          if (node->MNew != ANew) {
            DTrace("! mismatch!!! [%s:%i:%s] %s\n",node->MFile,node->MLine,node->MFunc,(node->MNew?"new":"malloc"));
          }
          #endif
          skei_memtrace_free(node); // !!!
          MNumFree++;
        }
        //else {
        //  DTrace("! node not found !");
        //  // and it will probably crash/segfault...
        //}
      //} // MStarted
    }

    //----------

    void dump(void) {
      DTrace("----------------------------------------\n");
      DTrace("memtrace\n");
      DTrace("----------------------------------------\n");
      DTrace("* num alloc     %i\n",MNumAlloc);
      DTrace("* num free      %i\n",MNumFree);
      DTrace("* num leaks     %i\n",MAllocatedNum);
      DTrace("* max allocated %i\n",MAllocatedMax);
      DTrace("* leaked memory %i\n",MAllocated);
      if (MAllocated > 0)
      {
        DTrace("----------------------------------------\n");
        skei_debug_memnode* node = (skei_debug_memnode*)MHead;
        while (node) {
          DTrace("* [%s:%s:%i] %s,0x%x,%i\n",node->MFile,node->MFunc,node->MLine,(node->MNew?"new":"malloc"),node->MAddr,node->MSize);
          // dtrace("* %i\n",node->MLine);
          skei_debug_memnode* next = (skei_debug_memnode*)node->MNext;
          //if (ADelete) { free(node); }
          node = next;
        }
      }
      DTrace("----------------------------------------\n");
    }

    //----------

    void* trace_malloc(const size_t size, const char* file, const char* func, unsigned int line) {
      void* ptr = skei_memtrace_malloc(size);
      append(ptr,size,file,func,line,false);
      return ptr;
    }

    //----------

    void* trace_calloc(const int num, const size_t size, const char* file, const char* func, unsigned int line) {
      void* ptr = skei_memtrace_calloc(num,size);
      append(ptr,num*size,file,func,line,false);
      return ptr;
    }

    //----------

    void* trace_realloc(void* ptr, const size_t size, const char* file, const char* func, unsigned int line) {
      void* p2 = skei_memtrace_realloc(ptr,size);
      remove(ptr,file,func,line,false);
      append(p2,size,file,func,line,false);
      return p2;
    }

    //----------

    void trace_free(void* ptr, const char* file, const char* func, unsigned int line) {
      remove(ptr,file,func,line,false);
      skei_memtrace_free(ptr);
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

static SDebug_MemTrace skei_debug_memtrace;

//----------------------------------------------------------------------

// not thread-safe?
// unreliable (and dangerous?) if multiple plugin instances uses
// mem-tracing at the same time.. ??

// these should be defined as __thread
static __thread char*         skei_mem_del_file;
static __thread char*         skei_mem_del_func;
static __thread unsigned int  skei_mem_del_line;

//----------

bool SDebug_MemDelPrepare(const char* file, const char* func, const unsigned int line) {
  //DTrace("leaks_del_prepare:" /*<< SStripPath(file) << "," << func << "," << line*/);
  skei_mem_del_file = (char*)file;
  skei_mem_del_func = (char*)func;
  skei_mem_del_line = line;
  return true;
}

//----------

void* operator new (const size_t size, const char* file, const char* func, unsigned int line) throw (std::bad_alloc) {
  //DTrace("op new");
  void* ptr = skei_memtrace_malloc(size);
  skei_debug_memtrace.append(ptr,size,file,func,line,true);
  return ptr;
}

//----------

void* operator new[] (const size_t size, const char* file, const char* func, unsigned int line) throw (std::bad_alloc) {
  //DTrace("op new[]");
  void* ptr = skei_memtrace_malloc(size);
  skei_debug_memtrace.append(ptr,size,file,func,line,true);
  return ptr;
}

//----------

void operator delete (void* ptr) throw() {
  //DTrace("op delete");
  skei_memtrace_free(ptr);
  skei_debug_memtrace.remove(ptr,skei_mem_del_file,skei_mem_del_func,skei_mem_del_line,true);
}

//----------

void operator delete[] (void* ptr) throw() {
  //DTrace("op delete[]");
  skei_memtrace_free(ptr);
  skei_debug_memtrace.remove(ptr,skei_mem_del_file,skei_mem_del_func,skei_mem_del_line,true);
}

//----------------------------------------------------------------------

// catch malloc

#define malloc(size)      skei_debug_memtrace.trace_malloc(size,     __FILE__,__FUNCTION__,__LINE__)
#define calloc(num,size)  skei_debug_memtrace.trace_calloc(num,size, __FILE__,__FUNCTION__,__LINE__)
#define realloc(ptr,size) skei_debug_memtrace.trace_realloc(ptr,size,__FILE__,__FUNCTION__,__LINE__)
#define free(ptr)         skei_debug_memtrace.trace_free(ptr,        __FILE__,__FUNCTION__,__LINE__)

// catch op. new / delete

#define debug_new         new(__FILE__, __FUNCTION__, __LINE__)
#define debug_delete      if (SDebug_MemDelPrepare(__FILE__, __FUNCTION__, __LINE__)) delete

#define new               debug_new
#define delete            debug_delete

// http://wyw.dcweb.cn/leakage.htm
//void* operator new(size_t size, const char* file, int line);
//void* operator new[](size_t size, const char* file, int line);
//#define DEBUG_NEW new(__FILE__, __LINE__)
//#define new DEBUG_NEW

//----------------------------------------------------------------------

#undef skei_memtrace_malloc
#undef skei_memtrace_calloc
#undef skei_memtrace_realloc
#undef skei_memtrace_free

#endif // SKEI_DEBUG_MEM

//----------------------------------------------------------------------
#endif
