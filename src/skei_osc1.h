#ifndef skei_osc1_included
#define skei_osc1_included
//----------------------------------------------------------------------

//#include "skei_phasor.h"
#include "skei_waveform_polyblep.h"
//#include "skei_filter_dc.h"

class SOsc1 {

  //public:
  //  float     p1;
  //  float prev_t1;

  private:
    //float                   MSampleRate;
    //uint32                  MOverSample;
    float                   MPulseWidth;
    float                   MSawSqu;
    float                   MSquTri;
    float                   MTriSin;
    float                   ph,phadd;
    float                   z1;
    //SWaveform_Saw_PolyBlep  MSaw1;
    //SWaveform_Saw_PolyBlep  MSaw2;

  public:

    SOsc1() {
      //MSampleRate = 44100.0f;
      //MOverSample = 1;
      MPulseWidth = 0.5f;
      MSawSqu     = 0.0f;
      MSquTri     = 0.0f;
      MTriSin     = 0.0f;
      z1          = 0;
      ph          = 0;
      phadd       = 0;
    }

    void setFreq(float hz,float srate) {
      phadd = hz / srate;
    }

    void setPulseWidth(float pw) {
      MPulseWidth = pw;
    }

    void setSawSqu(float sawsqu) {
      MSawSqu = sawsqu;
    }

    void setSquTri(float squtri) {
      MSquTri = squtri;
    }

    void setTriSin(float trisin) {
      MTriSin = trisin;
    }

    float process() {
      ph += phadd;
      if (ph >= 1.0f) ph -= 1.0f;
      float t1 = ph + 0.5f;
      if (t1 >= 1.0f) t1 -= 1.0f;
      float saw1 = 2.0f * t1 - 1.0f;
      saw1 -= SPolyBlep(t1,phadd);
      float t2 = t1 + MPulseWidth;
      if (t2 >= 1.0f) t2 -= 1.0f;
      float saw2 = 2.0f * t2 - 1.0f;
      saw2 -= SPolyBlep(t2,phadd);
      float squ = saw1 - (saw2*MSawSqu);
      z1 = (phadd * squ) + ((1.0f - phadd) * z1);

      // squ-tri
      float tri = squ * (1-MSquTri) + (z1*4.0f) * MSquTri;

      float sine = tri * (1-MTriSin) + sin(ph*SKEI_PI2) * MTriSin;

      //float tr = fabsf(ph - 0.5f) * 2.0f - 0.5f;
      //float q = tr*tr*tr;
      //float s = 3.0f / 0.9379f;
      //float sine = (tr - q*2.0f + q * fabsf(tr)) * s;

      // crossfade tri-sin
      float out = tri * (1-MTriSin) + sine * MTriSin;
      return out;

    }

};

//----------------------------------------------------------------------
#endif
