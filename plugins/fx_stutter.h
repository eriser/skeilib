
//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//#define SKEI_EXE
//#define SKEI_XLIB

//#define SKEI_DEBUG
//#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_MEM_PRINT
#define SKEI_DEBUG_SOCKET
//#define SKEI_DEBUG_VST

//#define SKEI_PLUGIN_PER_SAMPLE
//#define SKEI_PLUGIN_HAS_EDITOR
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_SEND_MIDI
//#define SKEI_PLUGIN_RECEIVE_MIDI
//#define SKEI_PLUGIN_AUTOSYNC

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

const char* txt_division[16] = {
  "4 Beats",
  "3 Beats",
  "2 Beats",
  "1 Beat",
  "1/2 Beat",
  "1/3 Beat",
  "1/4 Beat",
  "1/6 Beat",
  "1/8 Beat",
  "1/9 Beat",
  "1/12 Beat",
  "1/16 Beat",
  "1/24 Beat",
  "1/32 Beat",
  "1/48 Beat",
  "1/64 Beat"

};

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin
//, public SHostSync_Listener
//, public SMidiTiming_Listener
{

  private:
    float   p_mix;
    float   p_bpm;
    float   p_division;
  private:
    float   mix;
    double  bpm;
    //unsigned int  division;
    //uint32  division;
    //double  beatlength;
    //double  divisor;
    //double  stutterlength;
  private:
    float   coef;
    double  phase;
    double  s_rate;

  public:

    //virtual
    void on_create(void) {

      MInfo.name          = "fx_stutter";
      MInfo.author        = "skei";
      MInfo.product       = MInfo.name;
      MInfo.uniqueId      = SKEI_MAGIC + 0x0000;
      MInfo.numInputs     = 2;
      MInfo.numOutputs    = 2;

      appendParameter( new SParameter(  "mix",      1 ));
      appendParameter( new SParamFloat( "bpm",      90, 1,400 ));
      appendParameter( new SParamText(  "division", 4, 16, txt_division ));

      coef = 0;
      phase = 0;
      s_rate = 0;

    }

    //----------

    //virtual
    void on_destroy(void) {
    }

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    /*
    //virtual
    void on_transportChange(uint32 ATransport) {
      if (ATransport & (spt_changed|spt_play)) {
        MHostSync->restart();
        MReadPos = 0;
        MWritePos = 0;
      }
      MHostLoop = (ATransport&spt_cycle);
    }
    */

    //----------

    /*
    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      MMidiTiming->event(AOffset,AMsg1,AMsg2,AMsg3);
    }
    */

    //----------------------------------------
    // parameters/programs
    //----------------------------------------

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      switch(AIndex) {
        case 0: p_mix       = AValue; break;
        case 1: p_bpm       = AValue; break;
        case 2: p_division  = AValue; break;
      }
    }

    /*

    +1      2
    +0-5    1.5
    0       1

    0       1
    -0.5    1/(1--0.5) = 1/1.5 = 0.66
    -1      1/(1--1)   = 1/2   = 0.5

    */


    //----------------------------------------
    // process
    //----------------------------------------

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {

      s_rate      = MState.sampleRate;
      mix         = 1 - p_mix;
      bpm         = p_bpm;
      uint32 division    = p_division;
      double beatlength  = ( s_rate * 60 ) / bpm;

      double divisor;
      switch(division) {
        case 0:  divisor = 1.0f/4.0f; break;
        case 1:  divisor = 1.0f/3.0f; break;
        case 2:  divisor = 1.0f/2.0f; break;
        case 3:  divisor = 1.0f;      break;
        case 4:  divisor = 2.0f;      break;
        case 5:  divisor = 3.0f;      break;
        case 6:  divisor = 4.0f;      break;
        case 7:  divisor = 6.0f;      break;
        case 8:  divisor = 8.0f;      break;
        case 9:  divisor = 9.0f;      break;
        case 10: divisor = 12.0f;     break;
        case 11: divisor = 16.0f;     break;
        case 12: divisor = 24.0f;     break;
        case 13: divisor = 32.0f;     break;
        case 14: divisor = 48.0f;     break;
        case 15: divisor = 64.0f;     break;
        default: divisor = 1.0f;      break;
      }
      double stutterlength = beatlength / divisor;
      float* in0 = AInputs[0];
      float* in1 = AInputs[1];
      float* out0 = AOutputs[0];
      float* out1 = AOutputs[1];
      //float coef = plugin_data->coef;
      if (coef < 1) { coef = mix; }
      //To satisfy a*x+b=1 and a*1+b=x, we get a=-1, b=1+x
      float twiddle = 1 + mix;
      //double phase = plugin_data->phase;
      /*unsigned int*/ int32 pos  = 0;
      while (phase >= stutterlength) {
        phase -= stutterlength;
        coef = twiddle - coef;
      }
      while (1) {
        /*unsigned int*/ int32 rest = stutterlength + ceil(-phase);
        if (pos+rest>/*sample_count*/ANumSamples) { break; }
        //Pulling out these variables to make GCC vectorize on x86_64

        float* inbuf  = in0 + pos;
        float* outbuf       = out0 + pos;
        for (/*unsigned int*/int32 i=0; i<rest; i++) {
          outbuf[i] = inbuf[i] * coef;
        }

        coef = twiddle-coef;
        pos += rest;
        phase += rest-stutterlength;
      }

      //To make GCC auto-vectorize
      float* inbuf  = in0 + pos;
      float* outbuf = out0 + pos;
      for (/*unsigned int*/ int32 i=0; i</*sample_count*/ANumSamples-pos; i++) {
        outbuf[i] = inbuf[i] * coef;
      }

      //plugin_data->phase = phase + (/*sample_count*/ANumSamples-pos);     // !!!
      //plugin_data->coef = coef;     // !!!

      phase += (ANumSamples-pos);


    }

    //----------

    /*
    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {
      float in0 = *AInputs[0];
      float in1 = *AInputs[1];
      float out0 = 0.0f;
      float out1 = 0.0f;
      *AOutputs[0] = out0;
      *AOutputs[1] = out1;
    }
    */

    //----------

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

