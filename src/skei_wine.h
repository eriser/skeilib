#ifndef skei_wine_included
#define skei_wine_included
//----------------------------------------------------------------------

#ifdef SKEI_WIN32

#include "skei_defines.h" // sets winver etc..
#include <windows.h>


static __SKEI_UNUSED
BOOL SDetectWine(void) {
    BOOL ret = 0;
    HINSTANCE hLib = LoadLibrary(TEXT("ntdll.dll"));
    if (hLib) {
      ret = GetProcAddress(hLib, "wine_get_version") != NULL ||
            GetProcAddress(hLib, "wine_nt_to_unix_file_name") != NULL;
      /*BOOL unload =*/ FreeLibrary(hLib);
      //if (!unload)
      //  printf("[h_DetectWine.h] wine detect: # cannot release ntdll.dll\n");
    }
    //else
    //  printf("[h_DetectWine.h] wine detect: # cannot load ntdll.dll\n");
    //printf("[h_DetectWine.h] wine detect: %d\n", ret);
    return ret;
  }

#endif // SKEI_WIN32

//----------------------------------------------------------------------

#ifdef SKEI_LINUX

static __SKEI_UNUSED
bool SDetectWine(void) {
  return false;
}

#endif // SKEI_LINUX

//----------------------------------------------------------------------
#endif

