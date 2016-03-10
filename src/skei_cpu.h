#ifndef skei_cpu_included
#define skei_cpu_included
//----------------------------------------------------------------------

#include "skei_defines.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// http://bits.stephan-brumme.com/endianess.html

bool SIsLittleEndian(void) {
  uint16 pattern = 0x0001;
  return *(char*)&pattern == 0x01;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#ifdef SKEI_WIN32
  #include <windows.h>
#endif

#ifdef SKEI_LINUX
  #include <unistd.h>
#endif

//#elif SKEI_MAC
//  #include <sys/param.h>
//  #include <sys/sysctl.h>
//#endif

//----------

int32 SNumCpuCores(void ) {
  #ifdef SKEI_WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
  #endif
  #ifdef SKEI_LINUX
    return sysconf(_SC_NPROCESSORS_ONLN);
  #endif
  //#ifdef SKEI_MAC
  //  int nm[2];
  //  size_t len = 4;
  //  uint32_t count;
  //  nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
  //  sysctl(nm, 2, &count, &len, NULL, 0);
  //  if(count < 1) {
  //    nm[1] = HW_NCPU;
  //    sysctl(nm, 2, &count, &len, NULL, 0);
  //    if(count < 1) { count = 1; }
  //  }
  //  return count;
  //#endif
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*

// fPIC compatible
#define SKEI_CPU_EBX_REG "r"

#define SKEI_CPU_EBX_STORE  \
  "pushl %%ebx;"            \
  "cpuid;"                  \
  "movl %%ebx, %1;"         \
  "popl %%ebx;"

//----------

class SCPU {

  private:

    unsigned char isCalled;
    char          cpustringbuf[SKEI_MAX_STRING_SIZE];
    unsigned int  _caps;
    unsigned char _SSE3, _SSSE3, _FPU, _CMOV, _SSE, _SSE2, _SSE4A, _SSE5, _MMX,
                  _MMXEXT, _3DNOW, _3DNOWEXT;

  public:

    SCPU(void) {
      isCalled=0;
      _caps=0;
    }

    //----------

    ~SCPU(void) {
    }

    //----------

    // get cpu specifiction

    void id(const int fcall=33139, int* eax=NULL, int* ebx=NULL, int* ecx=NULL, int* edx=NULL) {
      if (fcall == 33139)  { // 33139 = some default number
        isCalled = 1;
        int a, b, c, d;
        // -----------------
        // 0x00000001
        __asmv (
          SKEI_CPU_EBX_STORE
          : "=a" (a), "="SKEI_CPU_EBX_REG"" (b),
            "=c" (c), "=d" (d) : "a" (0x00000001) : "cc"
        );
        _SSE3   = SBitGet(c, 0);
        _SSSE3  = SBitGet(c, 9);
        _FPU    = SBitGet(d, 0);
        _CMOV   = SBitGet(d, 15);
        _SSE    = SBitGet(d, 25);
        _SSE2   = SBitGet(d, 26);
        // -----------------
        // 0x80000001
        __asmv (
          SKEI_CPU_EBX_STORE
          : "=a" (a), "="SKEI_CPU_EBX_REG"" (b),
            "=c" (c), "=d" (d) : "a" (0x80000001) : "cc"
        );
        _SSE4A    = SBitGet(c, 4);
        _SSE5     = SBitGet(c, 11);
        _MMX      = SBitGet(d, 23);
        _MMXEXT   = SBitGet(d, 22);
        _3DNOW    = SBitGet(d, 31);
        _3DNOWEXT = SBitGet(d, 30);
      }
      // user defined call
      // -----------------
      else
        __asmv (
          SKEI_CPU_EBX_STORE
          : "=a" (*eax), "="SKEI_CPU_EBX_REG"" (*ebx),
            "=c" (*ecx), "=d" (*edx) : "a" (fcall) : "cc"
        );
    }

    //----------

    // get cpu caps

    uint32 caps(void) {
      if (!isCalled) id();
      if (_SSE3)      _caps |= 0x0001;
      if (_SSSE3)     _caps |= 0x0002;
      if (_FPU)       _caps |= 0x0004;
      if (_CMOV)      _caps |= 0x0008;
      if (_SSE)       _caps |= 0x0010;
      if (_SSE2)      _caps |= 0x0020;
      if (_SSE4A)     _caps |= 0x0040;
      if (_SSE5)      _caps |= 0x0080;
      if (_MMX)       _caps |= 0x0100;
      if (_MMXEXT)    _caps |= 0x0200;
      if (_3DNOW)     _caps |= 0x0400;
      if (_3DNOWEXT)  _caps |= 0x0800;
      return _caps;
    }

    //----------

    // h_CpuCapsString
    char* capsString(void) {
      if (!isCalled) id();
      cpustringbuf[0] = 0;
      if (_SSE3)     SStrcat(cpustringbuf,(char*)"sse3 "); // space at the end make them easier to string together
      if (_SSSE3)    SStrcat(cpustringbuf,(char*)"ssse3 ");
      if (_FPU)      SStrcat(cpustringbuf,(char*)"fpu ");
      if (_CMOV)     SStrcat(cpustringbuf,(char*)"cmov ");
      if (_SSE)      SStrcat(cpustringbuf,(char*)"sse ");
      if (_SSE2)     SStrcat(cpustringbuf,(char*)"sse2 ");
      if (_SSE4A)    SStrcat(cpustringbuf,(char*)"sse4a ");
      if (_SSE5)     SStrcat(cpustringbuf,(char*)"sse5 ");
      if (_MMX)      SStrcat(cpustringbuf,(char*)"mmx ");
      if (_MMXEXT)   SStrcat(cpustringbuf,(char*)"mmxext ");
      if (_3DNOW)    SStrcat(cpustringbuf,(char*)"3dnow ");
      if (_3DNOWEXT) SStrcat(cpustringbuf,(char*)"3dnowext ");
      return cpustringbuf;
    }

    //----------

    // call rdtsc()

    #ifdef SKEI_64BIT
    inline unsigned long rdtsc(void) {

      //unsigned int low, high;
      //__asmv ( "rdtsc;" : "=a" (low), "=d" (high) );
      //return ( (low) | ( (unsigned long)(high) << 32 ) );

      unsigned long val;
      __asmv ( "rdtsc;" : "=A" (val) );
      return val;
      // ^ check if problems occur with 64bit return
    }
    #endif
    #ifdef SKEI_32BIT_
    inline unsigned long rdtsc(void) {
      unsigned long long val;
      __asmv (
        // ## serialization causes delayed segmentation faults !
        //"cpuid;"
        "rdtsc;"
        //"leal %0, %%ecx;"
        //"movl %%eax, (%%ecx);"
        //"movl %%edx, 4(%%ecx);"
        : "=A" (val)
        //:: "%eax", "%ebx", "%ecx", "%edx"
      );
      return (unsigned long)val;
    }
    #endif

    //----------

};

*/

//----------------------------------------------------------------------
#endif
