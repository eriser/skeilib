#ifndef skei_program_included
#define skei_program_included
//----------------------------------------------------------------------

#include "skei_array.h"

//----------

//#define SKEI_MAX_PARAMETERS 256
//SKEI_MAX_PLUGIN_PARAMETERS

//----------

class SProgram;
typedef SArray<SProgram*> SProgramArray;

//----------

class SProgram {

  protected:
    char    MName[256];
    int32   MIndex;
    int32   MNumValues;
    float   MValues[SKEI_PLUGIN_MAX_PARAMETERS];

  public:

    SProgram() {
      SStrncpy(MName,"program",255);
      MIndex = -1;
      MNumValues = 0;
      SMemset(MValues,0,SKEI_PLUGIN_MAX_PARAMETERS*sizeof(float));
    }

    SProgram(const char* AName, int32 ANumValues=0, float* AValues=SKEI_NULL) {
      SStrcpy(MName,"program");
      MIndex = -1;
      MNumValues = ANumValues;
      SMemset(MValues,0,SKEI_PLUGIN_MAX_PARAMETERS*sizeof(float));
      if (ANumValues > 0) SMemcpy(MValues,AValues,ANumValues*sizeof(float));
    }

    virtual ~SProgram() {
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    virtual void name(const char* AName) {
      SStrcpy(MName,AName);
    }

    virtual const char* name(void) {
      return MName;
    }

    virtual void index(int32 AIndex) {
      MIndex = AIndex;
    }

    virtual int32 index(void) {
      return MIndex;
    }

    virtual float value(int32 AIndex) {
      return MValues[AIndex];
    }

    virtual void value(int32 AIndex, float AValue) {
      if (AIndex >= MNumValues) MNumValues = AIndex+1;
      MValues[AIndex] = AValue;
    }

};

//----------

#undef SKEI_MAX_PARAMETERS

//----------------------------------------------------------------------
#endif
