#ifndef skei_waveform_blamp
#define skei_waveform_blamp
//----------------------------------------------------------------------

/*
  see also:

  Triangle OSC with BLAMP, minBlep:
  http://www.kvraudio.com/forum/viewtopic.php?t=287993
*/

//----------------------------------------------------------------------

// http://www.kvraudio.com/forum/viewtopic.php?p=6107378#p6107378 // tale

static __SKEI_UNUSED
float SPolyBlamp(float t, float dt) {
  if (t < dt) {
    t = t/dt - 1;
    return (float)-1/3 * t*t*t;
  }
  else if (t > 1 - dt) {
    t = (t - 1)/dt + 1;
    return (double)1/3 * t*t*t;
  }
  else {
    return 0;
  }
}

//----------------------------------------------------------------------

class SWaveform_Tri_PolyBlamp {
  public:
    float process(float t, float dt) {
      // Start with naive triangle.
      float tri = 4 * t;
      if (tri >= 3) {
        tri = tri - 4;
      }
      else if (tri > 1) {
        tri = 2 - tri;
      }
      // Correct falling discontinuity.
      float scale = 4 * dt;
      float t2 = t + 0.25;
      t2 -= floor(t2);
      tri += scale * SPolyBlamp(t2, dt);
      // Correct rising discontinuity.
      t2 = t2 + 0.5;
      t2 = t2 - floor(t2);
      tri -= scale * SPolyBlamp(t2,dt);
      return tri;
    }
};

//----------------------------------------------------------------------
#endif
