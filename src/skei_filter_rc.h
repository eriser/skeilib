#ifndef skei_filter_rc_included
#define skei_filter_rc_included
//----------------------------------------------------------------------

//#include "skei_math.h"
#include <math.h> // expf

//----------

class SFilter_RC {

  private:

    float MValue;
    float MTarget;
    float MWeight;

  public:

    void  value(float AValue)   { MValue = AValue; }
    void  target(float AValue)  { MTarget = AValue; }
    void  weight(float AValue)  { MWeight = AValue; }
    float value(void)   { return MValue; }
    float target(void)  { return MTarget; }
    float weight(void)  { return MWeight; }

  public:

    SFilter_RC() {
      MValue  = 0;
      MTarget = 0;  // 1
      MWeight = 0;  // 1
    }

    ~SFilter_RC() {
    }

    void setup(float AValue=0, float ATarget=0, float AWeight=0) {
      MValue  = AValue;
      MTarget = ATarget;
      MWeight = AWeight;
    }

    void setFrequency(float AFrequency, float ASampleRate) {
      if (ASampleRate > 0) MWeight = 1 - expf(-SKEI_PI2 * AFrequency / ASampleRate );
      else MWeight = 0;
    }

    /*
      ATime is the time it takes the filter to decay to
      36.8% of its initial input or reach 63.2% of its final output.
    */

    void setTime(float ATime) {
      if (ATime > 0) MWeight = 1 - expf(-1 / ATime);
      else MWeight = 0;
    }

    float process(void) {
      MValue += (MTarget-MValue) * MWeight;
      return MValue;
    }

    float process(float AValue) {
      MTarget = AValue;
      return this->process();
    }

};

//----------------------------------------------------------------------
#endif
