#ifndef skei_string_included
#define skei_string_included
//----------------------------------------------------------------------

#include <string.h>

//----------------------------------------------------------------------

#define SStrcat   strcat
#define SStrchr   strchr
#define SStrcmp   strcmp
#define SStrcpy   strcpy
#define SStrlen   strlen
#define SStrncat  strncat
#define SStrncpy  strncpy
#define SStrrchr  strrchr
#define SStrrev   strrev
#define SStrstr   strstr

//----------------------------------------------------------------------

#include "skei_memory.h"

class SString {

  private:

    char MBuffer[SKEI_MAX_STRING_SIZE];

  public:

    SString() {
      clear();
    }

    SString(const char* AChars) {
      clear();
      SStrcpy(MBuffer,AChars);
    }

    ~SString() {
    }

    //------------------------------
    //
    //------------------------------

    char* ptr(void) {
      return MBuffer;
    }

    //------------------------------
    //
    //------------------------------

    void clear(bool AErase=false) {
      if (AErase) SMemset(MBuffer,0,SKEI_MAX_STRING_SIZE);
      else MBuffer[0] = 0;
    }

    //int32 size(void) {
    //  return SStrlen(MBuffer);
    //}

    int32 length(void) {
      return SStrlen(MBuffer);
    }

    void append(const char AChar) {
      MBuffer[length()] = AChar;
    }

    void remove(const unsigned int APos) {
      if (length()==1) clear();
      else { for (int32 i=APos; i<length()-1; i++ ) MBuffer[i] = MBuffer[i+1]; }
    }

    //------------------------------
    // operators
    //------------------------------

    /*
      todo:
        SString s1;
        SString s2 = "qwe" + s1;

    */

//    void     operator = (SString str)     { SText = str.mText; }
//    void     operator = (h_String* str)   { mText = str->mText; }
//    void     operator = (char* txt)       { mText = txt; }
//    void     operator = (const char* txt) { mText = (char*)txt; }

    SString& operator = (const char* AChars) {
      SStrcpy(MBuffer,AChars);
      return *this;
    }

    SString& operator + (const char* AChars) {
      SStrcat(MBuffer,AChars);
      return *this;
    }

    SString& operator += (const char* AChars) {
      SStrcat(MBuffer,AChars);
      return *this;
    }

    //----------

    SString& operator = (SString& AString) {
      if (this==&AString) return *this;
      SStrcpy(MBuffer,AString.ptr());
      return *this;
    }

    SString& operator + (SString& AString) {
      SStrcat(MBuffer,AString.ptr());
      return *this;
    }

    SString& operator += (SString& AString) {
      SStrcat(MBuffer,AString.ptr());
      return *this;
    }

    //------------------------------
    //
    //------------------------------

    char operator [] (const unsigned int AIndex) {
      return MBuffer[AIndex];
    }

};

//----------------------------------------------------------------------
#endif

