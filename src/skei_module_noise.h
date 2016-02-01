#ifndef skei_module_noise_included
#define skei_module_noise_included
//----------------------------------------------------------------------

#include "skei_module.h"
#include "skei_random.h"

//----------------------------------------------------------------------

class SModule_Noise : public SModule {
  public:
    SModule_Noise() : SModule() {
      MName = "Noise";
    }
    virtual void processBlock() {
      float* out0 = MOutputs[0];
      int32 len = MBlockSize * MOverSample;
      for (int32 i=0; i<len; i++) {
        out0[i] = SRandomSigned();
      }
    }
};

//----------------------------------------------------------------------

class SModule_Mul : public SModule {
  private:
    float MValue;
  public:
    SModule_Mul() : SModule() {
      MName = "Mul";
      MValue = 0.0f;
    }
    virtual void handleControl(uint32 AIndex, float AValue) {
      if (AIndex==0) MValue = AValue;
    }
    virtual void processBlock() {
      float* in0   = MInputs[0];
      float* out0  = MOutputs[0];
      int32 len   = MBlockSize * MOverSample;
      for (int32 i=0; i<len; i++) {
        out0[i] = in0[i] * MValue ;
      }
    }
};

//----------------------------------------------------------------------



//----------------------------------------------------------------------
#endif

