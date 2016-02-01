#ifndef fx_fracdelay_included
#define fx_fracdelay_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//#define SKEI_EXE
//#define SKEI_XLIB

//#define SKEI_DEBUG
//#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_MEM_PRINT
//#define SKEI_DEBUG_SOCKET
//#define SKEI_DEBUG_VST

#define SKEI_PLUGIN_PER_SAMPLE
//#define SKEI_PLUGIN_HAS_EDITOR
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_SEND_MIDI
#define SKEI_PLUGIN_RECEIVE_MIDI
//#define SKEI_PLUGIN_AUTOSYNC

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"

// 20 hz = 44100/20 = 2205
#define SKEI_MAX_SAMPLES 4096

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin
{

  private:

    float buf[SKEI_MAX_SAMPLES*2*2];
    int32 pos;
    float size;
    float frac;
    float z0;
    float z00;
    float z01;
    //
    float dly;
    float fbk;
    float wet;
    float dry;

  public:

    //virtual
    void on_create(void) {
      MInfo.name       = "fx_fracdelay";
      MInfo.author     = "skei";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0001;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      appendParameter( new SParamFloat( "delay",     100, 0, SKEI_MAX_SAMPLES ));
      appendParameter( new SParamFloat( "feedback",  0.9, 0, 0.99 ));
      appendParameter( new SParamFloat( "wet",      -6, -60, 0 ));
      appendParameter( new SParamFloat( "dry",      -6, -60, 0 ));
      pos  = 0;
      size = 0;
      frac = 0;
      z00  = 0;
      z01  = 0;
    }

    //----------

    //virtual
    void on_destroy(void) {
    }

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      switch(AIndex) {
        case 0: dly = AValue; size = floorf(dly); frac = dly-size; break;
        case 1: fbk = AValue;
        case 2: wet = powf(2,AValue/6);
        case 3: dry = powf(2,AValue/6);
      }
    }

    //----------

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      uint32 m = AMsg1 & 0xF0/*240*/;
      if (m == 0x90/*(9*16)*/) {
        uint32 note = AMsg2 & 127;
        float freq = 440.0 * powf(2,(note-69.0)/12.0);
        //uint32 vel = AMsg3 & 127;
        //wet = vel / 127;
        dly = MState.sampleRate / freq;
        //slider1 := dly;
        //slider3 := wet;
        //sliderchange(slider1+slider3);
        size = floorf(dly);
        frac = dly - size;
        pos  = 0;
        z0   = 0;
      }
    }

    //----------

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {
      float spl0 = *AInputs[0];
      float spl1 = *AInputs[1];
      int32 pos2 = pos*2;
      float in0 = spl0;
      float in1 = spl1;
      float d00 = buf[pos2];
      float d01 = buf[pos2+1];
      float i00,i01;
      if (dly<1) {
        i00 = (in0*(1-frac)) + (d00*frac);
        i01 = (in1*(1-frac)) + (d01*frac);
      } else {
        i00 = (d00*(1-frac)) + (z00*frac);
        i01 = (d01*(1-frac)) + (z01*frac);
      }
      buf[pos2]   = in0 + (fbk*i00);
      buf[pos2+1] = in1 + (fbk*i01);
      z00 = d00;
      z01 = d01;
      pos += 1;
      if (pos>=size) pos = 0; //-=size
      spl0 = (in0*dry) + (i00*wet);
      spl1 = (in1*dry) + (i01*wet);
      *AOutputs[0] = spl0;
      *AOutputs[1] = spl1;
    }

};

//----------

#undef SKEI_MAX_SAMPLES

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif
