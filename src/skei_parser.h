#ifndef skei_parser_included
#define skei_parser_included
//----------------------------------------------------------------------

#include "skei_array.h"
#include "skei_memory.h"

//typedef SArray<char*> s_parser_str_array;

//----------

class SParser {

  private:
    uint32        MBufLen;
    char*         MBuffer;
    //s_parser_str_array  MTokens;
    SStringArray  MTokens;

  public:

    SParser() {
      MBufLen = 0;
      MBuffer = SKEI_NULL;
    }

    //----------

    ~SParser() {
      if (MBuffer) SFree(MBuffer);
    }

  public:

    uint32  numTokens(void) { return MTokens.size(); }
    char*   token(uint32 i) { return MTokens[i]; }


  public:

    /*
      space is a separator! cr,lf,tab is too ( <= 32 )
      change <=32 to <32 to turn space into a character, not a separator

      copies buffer..
    */

    void parse(const char* ASource, uint32 ALength) {

      if (MBuffer) SFree(MBuffer);
      //MBufLen = 0;
      MTokens.clear(/*true*/);

      MBufLen = ALength;
      MBuffer = (char*)SMalloc(MBufLen+1);
      SMemcpy(MBuffer,(char*)ASource,MBufLen);
      MBuffer[MBufLen] = 0;
      // replace any non-visual chars with '\0'
      for (uint32 i=0; i<MBufLen; i++) {
        if (MBuffer[i] <= 32 ) MBuffer[i] = 0;
      }
      // new tokens after each (last) 0
      char prev_c = 0;
      for (uint32 i=0; i<MBufLen; i++) {
        char c = MBuffer[i];
        if ((prev_c == 0) && (c != 0)) {
          MTokens.append(&MBuffer[i]);
        }
        prev_c = c;
      }
    }

};

//----------------------------------------------------------------------
#endif
