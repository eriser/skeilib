#ifndef skei_filter_svf_included
#define skei_filter_svf_included
//----------------------------------------------------------------------

/*
  http://www.kvraudio.com/forum/viewtopic.php?p=6371172#p6371172
  Take a look at the State Variable Filters described here:
  http://www.cytomic.com/technical-papers
  Those have the same phase/frequency response as Direct Form filters, but
  coefficients are easier to calculate, sweep without side effects and are
  generally much more useful for musical applications.
*/

//----------------------------------------------------------------------

#define svf_none  0
#define svf_lp    1
#define svf_hp    2
#define svf_bp    3
#define svf_n     4

class SFilter_SVF {

  private:

    uint32  MMode;
    float   MFreq;
    float   MBW;
    float   z1,z2;

  public:

    SFilter_SVF() {
      MMode = 0;
      MFreq = 1;
      MBW   = 0;
      z1    = 0;
      z2    = 0;
    }

    void setMode(uint32 AMode) {
      MMode = AMode;
      //z1 = 0;
      //z2 = 0;
    }

    void setFreq(float AFreq) {
      MFreq = AFreq;
      //z1 = 0;
      //z2 = 0;
    }

    void setBW(float ABW) {
      MBW = ABW;
      //z1 = 0;
      //z2 = 0;
    }

    float process(float AValue) {
      //result := 0;
      if (MMode==0) return AValue;
      else {
        float L   = z2 + MFreq * z1;
        float H   = AValue - L - MBW   * z1;
        float B   = MFreq * H + z1;
        float N   = H + L;
        z1 = B;
        z2 = L;
        switch(MMode) {
          case svf_lp:  return L; //break;
          case svf_hp:  return H; //break;
          case svf_bp:  return B; //break;
          case svf_n:   return N; //break;
        }
      }
      return 0;
    }

};


//----------------------------------------------------------------------
#endif
