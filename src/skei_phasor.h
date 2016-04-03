#ifndef skei_phasor_included
#define skei_phasor_included
//----------------------------------------------------------------------

// 0..1

//#include "skei_math.h"

//----------

class SPhasor {

  private:
    float srate;

  public:

    float t,dt;

  public:

    SPhasor() {
      t = 0;
      dt = 0;
      srate = 0;
    }

    void setSampleRate(float ASampleRate) {
      srate = ASampleRate;
    }

    // AFrreq = hz

    void setFreq(float AFreq/*, float ASampleRate*/) {
      dt = AFreq / srate;
    }

    void addFreq(float AFreq/*, float ASampleRate*/) {
      dt += AFreq / srate;
    }

    //void setPhase(float APhase) {
    //  t = APhase;
    //}

    //void setPhaseAdd(float APhaseAdd) {
    //  dt = 0;
    //}

    /*
      todo: negative frequency (t%1)
    */

    float process(void) {
      t += dt;
      // mod 1
      if (t >= 1) t -= 1;

      //t -= floor(t);
      //while (p>=1) p -= 1;
      //while (p<=0) p += 1;
      return t;
    }

};

//----------------------------------------------------------------------
#endif
