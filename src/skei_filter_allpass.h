#ifndef skei_filter_allpass_included
#define skei_filter_allpass_included
//----------------------------------------------------------------------

class SFilter_Allpass {

  private:

    float a;
    float x0,x1,x2;
    float y0,y1,y2;

  public:

    //SFilter_Allpass(float ACoefficient) {
    //  a   = ACoefficient;
    //  x0  = 0;
    //  x1  = 0;
    //  x2  = 0;
    //  y0  = 0;
    //  y1  = 0;
    //  y2  = 0;
    //}

    SFilter_Allpass() {
      setCoefficient(0);
    }

    SFilter_Allpass(float ACoefficient) {
      setCoefficient(ACoefficient);
    }

    void setCoefficient(float ACoefficient) {
      a   = ACoefficient;
      x0  = 0;
      x1  = 0;
      x2  = 0;
      y0  = 0;
      y1  = 0;
      y2  = 0;
    }

    float process(float AInput) {
      x2 = x1;
      x1 = x0;
      x0 = AInput;
      y2 = y1;
      y1 = y0;
      //output := x2 + ( (AInput-y2) * a );
      //output := KKillDenorm(output);
      //y0 := output;
      y0 = x2 + ( (AInput-y2) * a );

      y0 = SKillDenorm(y0);

      return y0;
    }

};

//----------------------------------------------------------------------
#endif
