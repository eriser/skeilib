#ifndef skei_envelope_included
#define skei_envelope_included
//----------------------------------------------------------------------

//#define env_numstages 5

#include <math.h>

#define env_rate_scale  30.0f
#define env_threshold   SKEI_TINY

//----------------------------------------------------------------------

struct SEnvStage {
  float target;
  float rate;
};

//----------------------------------------------------------------------

class SEnvelope_ADSR {

  private:

    //float     MScale;
    float     MValue;
    int32     MStage;
    SEnvStage MStages[5]; // -,a,d,s,r

  public:

    SEnvelope_ADSR() {
      //MScale = 50.0f;//6.0f;
      MStage = ses_off;
      MValue = 0.0f;
    }

    //----------

    float value(void) { return MValue; }
    int32 stage(void) { return MStage; }

    //----------

    void setAttack(float AValue) {
      float r1 = AValue * env_rate_scale;
      float r2 = (r1*r1*r1) + 1;
      MStages[ses_attack].target = 1.0f;
      MStages[ses_attack].rate   = 1.0f / r2;
      //DTrace("attack rate: %.5f\n",MStages[ses_attack].rate);
    }

    void setDecay(float AValue) {
      float r1 = AValue * env_rate_scale;
      float r2 = (r1*r1*r1) + 1;
      //MStages[ses_decay].target = ATarget; // set in setSustain
      MStages[ses_decay].rate = 1.0f / r2;
    }

    void setSustain(float AValue) {
      float r1 = AValue;// * env_rate_scale;
      float r2 = (r1*r1*r1);// + 1;
      MStages[ses_decay].target = r2; // set in setSustain
      MStages[ses_sustain].target = r2; // set in setSustain
      MStages[ses_sustain].rate = 1.0f;
      //DTrace("sustain target: %.5f\n",MStages[ses_sustain].target);
    }

    void setRelease(float AValue) {
      float r1 = AValue * env_rate_scale;
      float r2 = (r1*r1*r1) + 1;
      MStages[ses_release].target = 0.0f;
      MStages[ses_release].rate   = 1.0f / r2;
    }

    //----------

    //void setStage(int32 AStage, float ATarget, float ARate) {
    //  float r1 = ARate * env_rate_scale;
    //  float r2 = (r1*r1*r1) + 1;
    //  //float r2 = r1*r1 + 1;
    //  STrace("r2: %f\n",r2);
    //  MStages[AStage].target = ATarget;
    //  MStages[AStage].rate   = 1.0f / r2;
    //}

    //----------

    void setADSR(float a, float d, float s, float r) {
      //setStage(ses_attack,1.0f,a);
      //setStage(ses_decay,s,d);
      //setStage(ses_sustain,s,1,1);
      ////MStages[ses_sustain].target = s*s*s;
      ////MStages[ses_sustain].rate   = 1.0f;
      //setStage(ses_release,0.0f,r);
      setAttack(a);
      setDecay(d);
      setSustain(s);
      setRelease(r);
    }

    //----------

    void noteOn(void) {
      MStage = ses_attack;
      //DTrace("MStage %i\n",MStage);
//      MValue = 0.0f;
    }

    //----------

    void noteOff(void) {
      MStage = ses_release;
      //DTrace("MStage %i\n",MStage);
    }

    //----------

    float process(void) {
      if (MStage == ses_off) return 0.0f;
      if (MStage == ses_finished) return 0.0f;
      if (MStage == ses_sustain) return MValue;
      float target = MStages[MStage].target;
      float rate   = MStages[MStage].rate;
      MValue += ( (target-MValue) * rate );
      if (fabs(target-MValue) <= env_threshold) {
        MStage += 1;
//        DTrace("MStage %i\n",MStage);
      }
      return MValue;
    }

};

//----------------------------------------------------------------------
#endif
