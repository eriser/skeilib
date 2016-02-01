#ifndef skei_waveform_naive_included
#define skei_waveform_naive_included
//----------------------------------------------------------------------


//float SWaveform_Sin_Naive(float t) {
//  return sin(t*SKEI_PI2);
//}

class SWaveform_Sin_Naive {
  public:
    float process(float t, float dt=0) {
      return sin(t*SKEI_PI2);
    }
};

//----------------------------------------------------------------------

//float SWaveform_Ramp_Naive(float t) {
//  return 1.0f - (t*2.0f);
//}

class SWaveform_Ramp_Naive {
  public:
    float process(float t, float dt=0) {
      return 1.0f - (t*2.0f);
    }
};

//----------------------------------------------------------------------

//float SWaveform_Saw_Naive(float t) {
//  return (t*2.0f)-1.0f;
//}

class SWaveform_Saw_Naive {
  public:
    float process(float t, float dt=0) {
      return (t*2.0f)-1.0f;
    }
};

//----------------------------------------------------------------------

//float SWaveform_Squ_Naive(float t) {
//  if (t < 0.5) return 1.0f;
//  else return -1.0f;
//}

class SWaveform_Squ_Naive {
  public:
    float process(float t, float dt=0) {
      if (t < 0.5) return 1.0f;
      else return -1.0f;
    }
};

//----------------------------------------------------------------------

//float SWaveform_Tri_Naive(float t) {
//  float a = t;    // 0..1
//  a *= 4.0f;         // 0..4
//  a -= 2.0f;         // -2..0..2
//  a = abs(a);    // 2..0..2
//  a -= 1.0f;         // 1..-1..1
//  return a;
//}

class SWaveform_Tri_Naive {
  public:
    float process(float t, float dt=0) {
      float a = t;    // 0..1
      a *= 4.0f;         // 0..4
      a -= 2.0f;         // -2..0..2
      a = fabs(a);    // 2..0..2
      a -= 1.0f;         // 1..-1..1
      return a;
    }
};

//----------------------------------------------------------------------
#endif
