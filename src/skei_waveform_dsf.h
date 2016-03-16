#ifndef skei_waveform_dsf_included
#define skei_waveform_dsf_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Discrete Summation Formula (DSF)
// http://www.musicdsp.org/showArchiveComment.php?ArchiveID=68
//----------------------------------------------------------------------

class SWaveform_DSF {
  private:
  public:
    SWaveform_DSF() {
    }
    float process(float x, float a, float N, float fi) {
      float s1 = pow(a,N-1.0)*sin((N-1.0)*x+fi);
      float s2 = pow(a,N)*sin(N*x+fi);
      float s3 = a*sin(x+fi);
      float s4 =1.0 - (2*a*cos(x)) +(a*a);
      if (s4==0) return 0;
      else return (sin(fi) - s3 - s2 +s1)/s4;
    }
};

// According to Stilson + Smith, this should be
// float s1 = pow(a,N+1.0)*sin((N-1.0)*x+fi);
// Could be a typo though?

//----------------------------------------------------------------------
#endif

