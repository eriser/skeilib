#ifndef skei_parameter_included
#define skei_parameter_included
//----------------------------------------------------------------------

/*
  TODO: split up into separate SParameter_Float, etc ??
*/

#include "skei_array.h"
#include "skei_convert.h"
#include "skei_math.h"

//----------------------------------------------------------------------
// parameter
//----------------------------------------------------------------------

class SParameter;
typedef SArray<SParameter*> SParameterArray;

//----------

class SParameter {

  protected:
    char        MDisplay[256];

  protected:

    int32       MType;          /// parameter type, spt_
    const char* MName;          /// parameter name, "volume"
    const char* MLabel;         /// parameter name, 'db', '%', ..
    float       MDefault;       /// default (initial) value
    uint32      MFlags;         /// parameter flags, spf_
    int32       MIndex;         /// parameter index (in plugin)
    void*       MWidget;        /// widget link

  public:

    SParameter() {
      MType       = spt_parameter;
      MName       = "param";
      MLabel      = "";
      MDisplay[0] = 0;
      MDefault    = 0;
      MFlags      = spf_automate;
      MIndex      = -1;
      MWidget     = SKEI_NULL;
    }

    SParameter(const char* AName, float AValue=0) {
      MType       = spt_parameter;
      MName       = AName;
      MLabel      = "";
      MDisplay[0] = 0;
      MDefault    = AValue;//to01(AValue);
      MFlags      = spf_automate;
      MIndex      = -1;
      MWidget     = SKEI_NULL;
    }

    virtual ~SParameter() {
    }

    //----------

    virtual const char* name(void)                { return MName; }
    virtual const char* label(void)               { return MLabel; }
    virtual void        label(const char* ALabel) { MLabel = ALabel; }
    virtual const char* displayText(float AValue) { SFloatToString(MDisplay,from01(AValue)); return MDisplay; }
    virtual float       defaultValue(void)        { return MDefault; }
    virtual uint32      flags(void)               { return MFlags; }
    virtual int32       index(void)               { return MIndex; }
    virtual void        index(int32 AIndex)       { MIndex = AIndex; }
    virtual float       from01(float AValue)      { return AValue; }
    virtual float       to01(float AValue)        { return AValue; }
    virtual void*       widget(void)              { return MWidget; }
    virtual void        widget(void* AWidget)     { MWidget = AWidget; }

};

//----------------------------------------------------------------------
// float
//----------------------------------------------------------------------

class SParamFloat : public SParameter {

  protected:

    float MMin;
    float MMax;
    float MStep;
    float MRange;
    float MInvRange;
    bool  MStepped;
    int32 MNumSteps;
    float MStepSize;
    float MHalfStep;

  private:

    void setup(const char* AName, const char* ALabel, float AVal, float AMin, float AMax, float AStep) {
      MIndex    = -1;
      MName     = AName;
      MLabel    = ALabel;
      //FDefault  = AVal;
      MMin      = AMin;
      MMax      = AMax;
      MStep     = AStep;
      MRange    = MMax - MMin;
      MInvRange = 1 / MRange;
      if (MStep > 0) {
        MStepped  = true;
        MNumSteps = 1 + SRound( MRange / MStep );
        MStepSize = 1 / (MNumSteps - 1);
        MHalfStep = 0.5 * MStepSize;
      } else {
        MStepped  = false;
        MNumSteps = 1;
        MStepSize = 1;
        MHalfStep = 0;
      }
      //MDefault = to01(AVal); // ... oops, calling virtual function in constructor :-/
      MDefault = ((AVal-MMin)*MInvRange);
      //KTrace( MDefault ); // 0.5
    }

  public:

    SParamFloat(const char* AName, float AVal, float AMin=0, float AMax=1, float AStep=0)
    //: SParameter(AName,AValue) {
    : SParameter() {
      MType = spt_paramFloat;
      setup(AName,"",AVal,AMin,AMax,AStep);
    }

    //----------

    virtual ~SParamFloat() {
    }

    //----------

    // user scale to 0..1

    virtual float to01(float AValue) {
      return ((AValue-MMin)*MInvRange);
    }

    //----------

    // 0..1 to user scale

    virtual float from01(float AValue) {
      if (MStepped) {
        float n = AValue * MNumSteps;
        int32 st = floorf(n);
        if (st > (MNumSteps-1)) st = (MNumSteps-1);
        return  MMin + (st * MStep);
      } else {
        return MMin + (AValue*MRange);
      }
    }

};

//----------------------------------------------------------------------
// int
//----------------------------------------------------------------------

class SParamInt : public SParamFloat {

  public:

    SParamInt(const char* AName, float AVal, float AMin=0, float AMax=1)
    : SParamFloat(AName,AVal,AMin,AMax,1) {
      MType = spt_paramInt;
    }

    //----------

    virtual ~SParamInt() {
    }

    //----------

    virtual const char* displayText(float AValue) {
      SIntToString(MDisplay,floorf(from01(AValue)));
      return MDisplay;
    }

};

//----------------------------------------------------------------------
// text
//----------------------------------------------------------------------

class SParamText : public SParamInt {

  private:

    const char** MStrings;

  public:

    SParamText(const char* AName, uint32 AIndex, uint32 ANum, const char** AText)
    : SParamInt(AName,AIndex,0,ANum-1) {
      MType = spt_paramText;
      MStrings = AText;
    }

    //----------

    // TODO: fix

    virtual const char* displayText(float AValue) {
      int32 i = floorf(from01(AValue));
      SStrcpy(MDisplay, MStrings[i] );
      return MDisplay;
    }

};

//----------------------------------------------------------------------
#endif
