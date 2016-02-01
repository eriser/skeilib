#ifndef skei_basepath_included
#define skei_basepath_included
//----------------------------------------------------------------------

/*
  linux exe
  DTrace("SGetHomePath %s\n",SGetHomePath(buffer)); // '/home/skei/'
  DTrace("SGetBaseFile %s\n",SGetBaseFile(buffer)); // '/home/skei/Desktop/skeilib/bin/skeilib_debug'
  DTrace("SGetBasePath %s\n",SGetBasePath(buffer)); // '/home/skei/Desktop/skeilib/bin/skeilib_debug./' !!
*/

//----------

#include <stdio.h>
#include <stdlib.h>

#ifdef SKEI_LINUX
  #include <unistd.h> // readlink
  #include <dlfcn.h>
#endif

#include "skei_string.h"

//----------------------------------------------------------------------

char* SGetHomePath(char* ABuffer) {
  //SStrcat(ABuffer, getenv("HOME"));
  ABuffer[0] = '\0';
  #ifdef SKEI_LINUX
  char* env = getenv("HOME");
  if (env) SStrcat(ABuffer,env);
  //SStrcat(ABuffer, (char*)"\\");
  SStrcat(ABuffer, (char*)"/");
  #endif // SKEI_LINUX
  return ABuffer;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

const char* SGetBaseFile(char* ABuffer) {
  ABuffer[0] = '\0';
  #ifdef SKEI_LINUX
    //#include <dlfcn.h>
    #if defined SKEI_LIB
      Dl_info dli;
      dladdr(__func__, &dli);
      //DTrace("SGetBaseFile: '" << dli.dli_fname << "'");
      //int32 len = SStrlen(dli.dli_fname);
      SStrcpy(ABuffer,dli.dli_fname);
//      return ABuffer;//dli.dli_fname;
    #else //SKEI_LIB
      int32 len = readlink("/proc/self/exe",ABuffer,SKEI_MAX_PATH_SIZE-1);
      if (len != -1) {
        ABuffer[len] = '\0';
//        return ABuffer;
      }
    #endif //SKEI_LIB
  #endif // SKEI_LINUX
  return ABuffer;//SKEI_NULL;
}


//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#ifdef SKEI_LINUX

  #include <dlfcn.h>
  #include <unistd.h>

  const char* SGetBasePath(char* ABuffer) {
    #if defined SKEI_LIB
      Dl_info dli;
      dladdr(__func__, &dli);
      const char* slash = SStrrchr(dli.dli_fname, '/');
      if (slash) {
        int len = (slash + 1) - (char*)dli.dli_fname;
        SStrncpy(ABuffer, dli.dli_fname, len);
        ABuffer[len] = '\0';
      }
      else {
        SStrcat(ABuffer, (char*)"./");
      }
    #elif defined SKEI_EXE
      char filepath[SKEI_MAX_PATH_SIZE] = "";
      long rd_res = readlink("/proc/self/exe", filepath, SKEI_MAX_PATH_SIZE);
      filepath[SKEI_MAX_PATH_SIZE-1] = '\0';
      if (rd_res) {
        const char* slash = SStrrchr(filepath, '/');
        if (slash) {
          int len = (slash + 1) - (char*)filepath;
          SStrncpy(ABuffer, filepath, len);
          ABuffer[len] = '\0';
        }
        else {
          //SStrcat(ABuffer, (char*)"./");
          //SStrcat(ABuffer, (char*)"/"); // -> "/" -> root :-/
        }
      }
    #else
      SStrcat(ABuffer, (char*)"./");
    #endif
    return ABuffer;
  }

#endif // SKEI_LINUX

//------------------------------

#ifdef SKEI_WIN32

  #include <windows.h>

  char* SGetBasePath(char* ABuffer) {
    #if defined SKEI_LIB || defined SKEI_EXE
      char filepath[SKEI_MAX_PATH_SIZE] = "";
      GetModuleFileName(static_WinInstance, filepath, SKEI_MAX_PATH_SIZE-1);
      filepath[SKEI_MAX_PATH_SIZE-1] = '\0';
      const char* slash = SStrrchr(filepath,'\\');
      if (slash) {
        int len = (slash+1) - (char*)filepath;
        SStrncpy(ABuffer, filepath, len);
        ABuffer[len] = '\0';
      }
      else {
        SStrcat(ABuffer,(char*)".\\");
      }
    #else
      SStrcat(ABuffer,(char*)".\\");
    #endif
    return ABuffer;
  }

#endif // SKEI_WIN32

//----------------------------------------------------------------------
#endif
