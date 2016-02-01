#ifndef test_synth_included
#define test_synth_included
//----------------------------------------------------------------------

#define MAX_VOICES 8

/*
  todo:
  - rename all osc -> waveform
  - create new osc classes that combine low level elements we need..
    - crossfading, filtering, interpolation, oversampling, etc, etc
*/

//----------

// http://www.kvraudio.com/forum/viewtopic.php?p=976301#p976301
// Using zeroes and filtering does reduce the level of the signal but it is
// easily regained by multiplying by the same number of times you're
// oversampling. (eg. if you're doing 4x oversampling, the signal is reduced by
// four times. So to get it back up to the original level, just multiply by 4.
// This is because the overall power in the signal is more spread out, i think).

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

#ifdef SKEI_VST
  #define SKEI_DEBUG_SOCKET
  #define SKEI_DEBUG_VST
#endif

#define SKEI_PLUGIN_PER_SAMPLE
#define SKEI_PLUGIN_IS_SYNTH
#define SKEI_PLUGIN_RECEIVE_MIDI

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"
#include "skei_voice_manager.h"
//#include "skei_voice.h"

#include "skei_phase.h"
#include "skei_envelope.h"

#include "skei_waveform_naive.h"
#include "skei_waveform_dpw.h"
#include "skei_waveform_ptr.h"
#include "skei_waveform_eptr.h"
#include "skei_waveform_polyblep.h"
#include "skei_waveform_polyblamp.h"

#include "skei_filter_decimator.h"
#include "skei_filter_hiir.h"
#include "skei_filter_downsample.h"

//#include "skei_filter_svf.h"

#include "test_synth_voice.h"


//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

const char* waveform_txt[9] = {
  "saw (naive)",
  "saw (dpw)",  "saw (dpw2)", "saw (ptr1)", "saw (eptr)",
  "saw (dpw3)", "saw (ptr2)", "saw (polyblep1)", "saw (ptr3)"
};

const char* oversample_txt[9] = { "1 (off)", "2", "4", "8", "16", "32", "64", "128", "256" };

const char* decimator_txt[18] = {
  "none",         "linear",
  "decimator 5",  "decimator 7",      "decimator 9",  "decimator 17",
  "hiir 2",       "hiir 4", "hiir 6", "hiir 8",       "hiir 10",      "hiir 12",
  "hiir 2 steep", "hiir 4 steep",     "hiir 6 steep", "hiir 8 steep", "hiir 10 steep", "hiir 12 steep"
};

const char* filter_txt[5] = { "off", "lowpass", "highpass", "bandpass", "notch" };

//----------------------------------------

class myPlugin
: public SPlugin
{

  private:

    SVoiceManager MVoiceManager;
    float         master;

  public:

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    //virtual
    void on_create(void) {
      MInfo.name       = "test synth";
      MInfo.author     = "skei";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      appendParameter( new SParameter(  "master",     0.5   ));
      appendParameter( new SParamFloat( "detune",     0,   -2, 2 ));
      appendParameter( new SParamText(  "waveform",   0,    9,  waveform_txt ));
      appendParameter( new SParamText(  "oversample", 0,    9,  oversample_txt ));
      appendParameter( new SParamText(  "decimator",  0,    18, decimator_txt ));
      appendParameter( new SParameter(  "attack",     0.1   ));
      //appendParameter( new SParameter(  "decay",      0     ));
      //appendParameter( new SParameter(  "sustain",    1     ));
      appendParameter( new SParameter(  "release",    0.5   ));
      //appendParameter( new SParamText(  "filter",     0,    5, filter_txt ));
      //appendParameter( new SParameter(  "freq",       1     ));
      //appendParameter( new SParamFloat( "bw",         1,    0.01, 1 ));
      for (uint32 i=0; i<MAX_VOICES; i++) {
        myVoice* v = new myVoice(&MVoiceManager);
        //v->setOverSample(8);
        MVoiceManager.appendVoice(v);
      }
      master = 0;

      //----------



      //----------


    }

    //------------------------------------------------------------
    // events
    //------------------------------------------------------------

    //virtual
    void on_stateChange(uint32 AState) {
      switch(AState) {
        case sps_sampleRate:
          MVoiceManager.setSampleRate(MState.sampleRate);
          break;
      }
    }

    //----------

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      if (AIndex==0) master = AValue*AValue*AValue;
      else MVoiceManager.control(AIndex-1,AValue);
    }

    //----------

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      MVoiceManager.midi(AOffset,AMsg1,AMsg2,AMsg3);
    }

    //------------------------------------------------------------
    // audio
    //------------------------------------------------------------

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
      MVoiceManager.preProcess();
      //MVoiceManager.process(AInputs,AOutputs,ANumSamples);
    }

    //----------

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {
      float synth[2];
      MVoiceManager.process(synth);
      *AOutputs[0] = synth[0] * master;
      *AOutputs[1] = synth[1] * master;
    }

    //----------

    //virtual
    void on_postProcess(void) {
      MVoiceManager.postProcess();
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

#undef MAX_VOICES

//----------------------------------------------------------------------
#endif
