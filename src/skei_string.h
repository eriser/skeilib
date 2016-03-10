#ifndef skei_string_included
#define skei_string_included
//----------------------------------------------------------------------

#include <string.h>

//----------------------------------------------------------------------
//
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
//
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
//
//----------------------------------------------------------------------

// http://www.cprogramming.com/snippets/source-code/useful-c-string-functions

//----------

// remove specified characters from a string

void SRemoveChars(char *str, char c) {
	char *pos;
	while( ( pos = SStrchr(str,c) ) ) {
		SMemmove(pos, pos + 1, SStrlen(pos));
  }
}

// remove specified chunks from a string

void SRemoveChunks(char *str, char *cnk) {
	char *pos;
	int clen = SStrlen(cnk);
	while( ( pos = SStrstr(str, cnk) ) ) {
		SMemmove(pos, pos + clen, SStrlen(pos) - clen + 1);
  }
}

// replace specified characters in a string

void SReplaceChars(char *str, char c1, char c2) {
	char *pos;
	while( ( pos = SStrchr(str, c1) ) ) {
		*pos = c2;
  }
}

// replace specified chunks in a string
// (size-independent, just remember about memory)

void SReplaceChunks(char *str, char *cnk1, char *cnk2)
{
	char *pos;
	int clen1 = SStrlen(cnk1), clen2 = SStrlen(cnk2);
	while( ( pos = SStrstr(str,cnk1) ) )
	{
		SMemmove(pos + clen2, pos + clen1, SStrlen(pos) - clen1 + 1);
		SMemcpy(pos, cnk2, clen2);
	}
}

//reverse a string

void SReverse(char *str) {
	int i;
	int len = SStrlen(str) - 1;
	int mid = (len % 2) ? (len / 2) : ((len + 1) / 2);
	for (i=0; i<=mid; ++i) {
		char buf = str[i];
		str[i] = str[len - i];
		str[len - i] = buf;
	}
}

//----------------------------------------------------------------------
#endif

