#ifndef skei_lfo_included
#ifndef skei_lfo_included
//----------------------------------------------------------------------

#include "skei_phasor.h"

class SLfo {
  private:
    uint32    MShape;
    SPhasor   MPhasor;
  public:
    float     process(void)     { return 0; }
    float     process(float t)  { return 0; }
};

//----------------------------------------------------------------------
#endif
