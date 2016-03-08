#ifndef skei_debug_assert_included
#define skei_debug_assert_included
//----------------------------------------------------------------------

#include <iostream> // cout
//#include "skei_debug_trace.h"

//----------

// for c89 compat mainly if the header is used elsewhere
#ifdef __GNUC_STDC_INLINE__
  #define SKEI_STATIC_ASSERT_INLINE inline
#else
  #define SKEI_STATIC_ASSERT_INLINE
#endif

//----------------------------------------------------------------------

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

  //----------------------------------------
  //
  //----------------------------------------

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
#endif
