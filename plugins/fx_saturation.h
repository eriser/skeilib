#ifndef fx_saturation_included
#define fx_saturation_included
//----------------------------------------------------------------------

/*
  based on the 'saturation' jesusonic effect by tale/loser
  http://forum.cockos.com/showpost.php?p=1537127&postcount=3
  see bottom of this file for original
*/

//----------------------------------------------------------------------

//#define SKEI_DEBUG_MEM

#ifdef SKEI_VST
  //#define SKEI_DEBUG_VST
  #define SKEI_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

#define scale ( 1.1 / 3.0 )

const char* txt_func[2] = { "sine", "cubic" };

class myPlugin
: public SPlugin
{

  private:

    float   p_saturation;
    uint32  p_stages;
    uint32  p_function;

    float   a, div, gain;
    //float   n; // num stages

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    void on_create(void) {
      MInfo.name       = "fx_saturation";
      MInfo.author     = "skeilib";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      appendParameter( new SParameter(  "saturation", 0 ));
      appendParameter( new SParamInt(   "stages",     1, 1, 10 ));
      appendParameter( new SParamText(  "function",   1, 2, txt_func ));
    }

  //----------------------------------------
  //
  //----------------------------------------

  private:

    void init(void) {
    }

    //----------

    void recalc(void) {
      a = SMax(0, SMin(1, p_saturation));
      if (a>0) {
        gain = 1 - (1 - 1/(float)p_stages) * a;
        float f = 1.0f;
        switch(p_function) {
          case 0: a*=SKEI_PI05; f = sin(a);  break;
          case 1: f = a - a*a*a * scale;        break;
        }
        div = 1/f;
      }
    }

  //----------------------------------------
  // events
  //----------------------------------------

  public:

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      switch(AIndex) {
        case 0: p_saturation = AValue; break;
        case 1: p_stages = AValue;     break;
        case 2: p_function = AValue;   break;
      }
      recalc();
    }

  //----------------------------------------
  // audio
  //----------------------------------------

  public:

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
      float* input0 = AInputs[0];
      float* input1 = AInputs[1];
      float* output0 = AOutputs[0];
      float* output1 = AOutputs[1];
      for (int32 i=0; i<ANumSamples; i++) {
        float spl0 = *input0++;
        float spl1 = *input1++;
        //----------
        if (a>0) {
          float x0 = SMax(-1, SMin(1, spl0));
          float x1 = SMax(-1, SMin(1, spl1));
          switch(p_function) {
            case 0:
              for (uint32 j=0; j<p_stages; j++) {
                x0 = sin(x0 * a) * div;
                x1 = sin(x1 * a) * div;
              }
              break;
            case 1:
              for (uint32 j=0; j<p_stages; j++) {
                x0 *= a; x0 = (x0 - x0*x0*x0 * scale) * div;
                x1 *= a; x1 = (x1 - x1*x1*x1 * scale) * div;
              }
              break;
          }
          spl0 = x0 * gain;
          spl1 = x1 * gain;
        } // a>0
        //----------
        *output0++ = spl0;
        *output1++ = spl1;
      }
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif





#if 0

desc:Another loser/Saturation rewrite, thanks to SaulT!
slider1:0<0,1,0.01>Saturation
slider2:1<1,10,1>Stages
slider3:1<0,1,1{Sine,Cubic}>Function

// Sine:  f(x) = sin(x * a * 0.5 * pi)/sin(a * 0.5 * pi)
// Cubic: f(x) = ((x*a)-((x*a)^3)/3)/(a-(a^3)/3)

@init

scale = 1.1 / 3;

@slider

a = max(0, min(1, slider1));
a > 0 ? (
  n = max(1, floor(slider2 + 0.5));
  f = slider3 < 0.5;

  gain = 1 - (1 - 1/n) * a;
  div = 1/(f ? sin(a *= 0.5*$pi) : a - a*a*a * scale);
);

@sample

a > 0 ? (
  x0 = max(-1, min(1, spl0));
  x1 = max(-1, min(1, spl1));

  f ? loop(n,
    x0 = sin(x0 * a) * div;
    x1 = sin(x1 * a) * div;
  ) : loop(n,
    x0 *= a; x0 = (x0 - x0*x0*x0 * scale) * div;
    x1 *= a; x1 = (x1 - x1*x1*x1 * scale) * div;
  );

  spl0 = x0 * gain;
  spl1 = x1 * gain;
);

#endif // 0
