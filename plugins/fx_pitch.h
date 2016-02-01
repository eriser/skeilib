#ifndef fx_pitch_included
#define fx_pitch_included
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
//#define SKEI_PLUGIN_RECEIVE_MIDI
//#define SKEI_PLUGIN_AUTOSYNC

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin {

  private:

    float in_pos;
    float out_pos;
    float base;
    float old_base;
    float fade_base;
    float inc_;
    float len_;
    float fade_;
    float buffer[1024*1024*8];

  public:

    //virtual
    void on_create() {
      MInfo.name       = "fx_pitch";
      MInfo.author     = "skei";
      MInfo.product    = MInfo.name; //kode.cpp test";
      MInfo.uniqueId   = SKEI_MAGIC + 0x0003;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      //MFlags      |= kpf_perSample;
      appendParameter( new SParamFloat( "pitch",      0,  -1,  1 ));
      appendParameter( new SParamFloat( "window",     100, 1,  200, 1 ));
      appendParameter( new SParamFloat( "crossfade",  100, 0,  100, 1 ));
      inc_      = 0;
      len_      = 0;
      fade_     = 0;
      in_pos    = 0;
      out_pos   = 0;
      base      = 0;
      old_base  = 0;
      fade_base = 0;
      SMemset(buffer,0,sizeof(buffer));
    }

    //----------

    //virtual
    void on_destroy(void) {
    }

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    /*
      inc = slider1/100;
      // len must be multiple of 2 otherwise there will be drift due to floating point math in 1sdft93hgosdh
      len = floor(slider2/1000*srate/2)*2;
      fade = slider3/100 * len/2;
    */

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      //kode_socket.write("on_parameterChange\n");
      switch(AIndex) {
        case 0:
          //inc_  = AValue/100;
          if (AValue >= 0)      inc_ = 1 + AValue;
          else if (AValue < 0)  inc_ = 1 + (AValue*0.5); // -1 -> 1+(-1*0.5) = 1 + -0.5 = 0.5
          //else inc_ = 1;
          break;
        case 1: len_  = floorf(AValue/1000*MState.sampleRate/2)*2;
        case 2: fade_ = AValue/100 * len_/2;
      }
    }

    //----------

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {
      float spl0 = *AInputs[0];
      float spl1 = *AInputs[1];
      uint32 p = (uint32)floorf(in_pos);
      buffer[ p*2] = spl0;
      buffer[ p*2+1] = spl1;
      in_pos = (uint32)floorf(in_pos+1) % (uint32)floorf(len_);
      float gain = SMin(out_pos/fade_,1);
      uint32 p0 = (uint32)floorf(base+out_pos) % (uint32)floorf(len_);
      uint32 p1 = (uint32)floorf(fade_base+out_pos) % (uint32)floorf(len_);
      spl0 = buffer[ p0*2] * gain
           + buffer[ p1*2] * (1-gain);
      spl1 = buffer[ p0*2+1] * gain
           + buffer[ p1*2+1] * (1-gain);
      out_pos += inc_;
      if (out_pos >= (len_-1-fade_)) {
        fade_base = base + len_ - 1 - fade_;
        out_pos = 0;
        base = in_pos;
      }
      *AOutputs[0] = spl0;
      *AOutputs[1] = spl1;
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif
