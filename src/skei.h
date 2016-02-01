#ifndef skei_included
#define skei_included
//----------------------------------------------------------------------

/*
  some files need to be included in a specific order,
  (esp debugging, since it redefines new/delete, etc)
*/

//----------------------------------------------------------------------

#include "skei_doxygen.h"
#include "skei_config.h"

//------------------------------
// config / setup
//------------------------------

#include "skei_defines.h"
#include "skei_const.h"
#include "skei_types.h"
#include "skei_flags.h"

//------------------------------
// debug
//------------------------------

#include "skei_debug.h"

//

// from now on, we can use STrace

//------------------------------
// os
//------------------------------

#ifdef SKEI_LINUX
  #include "skei_linux.h"
#endif

#ifdef SKEI_WIN32
  #include "skei_win32.h"
#endif

//------------------------------
// framework
//------------------------------

//#include "skei_global.h"

#ifdef SKEI_MAIN
#else
//#ifndef SKEI_MAIN
  #include "skei_main.h"
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

//#ifdef SKEI_DOXYGEN
//  #include "skei_all.h"
//#endif

//----------------------------------------------------------------------
#endif
