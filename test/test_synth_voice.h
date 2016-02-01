#ifndef test_synth_voice_included
#define test_synth_voice_included
//----------------------------------------------------------------------

#include "skei_voice_manager.h"

//----------------------------------------------------------------------


class myVoice : public SVoice {

  private:

    uint32                waveform;
    uint32                oversample;
    uint32                decimator;
    SPhase                phase;
    SEnvelope_ADSR        adsr;

    SFilter_Decimator5    decim5[8];
    SFilter_Decimator7    decim7[8];
    SFilter_Decimator9    decim9[8];
    SFilter_Decimator17   decim17[8];
    SFilter_Hiir2         hiir2[8];
    SFilter_Hiir4         hiir4[8];
    SFilter_Hiir6         hiir6[8];
    SFilter_Hiir8         hiir8[8];
    SFilter_Hiir10        hiir10[8];
    SFilter_Hiir12        hiir12[8];
    SFilter_Hiir2_Steep   hiir2s[8];
    SFilter_Hiir4_Steep   hiir4s[8];
    SFilter_Hiir6_Steep   hiir6s[8];
    SFilter_Hiir8_Steep   hiir8s[8];
    SFilter_Hiir10_Steep  hiir10s[8];
    SFilter_Hiir12_Steep  hiir12s[8];

    SWaveform_Saw_Naive        saw_naive;
    SWaveform_Saw_DPW          saw_dpw;
    SWaveform_Saw_DPW2         saw_dpw2;
    SWaveform_Saw_DPW3         saw_dpw3;
    SWaveform_Saw_PTR1         saw_ptr1;
    SWaveform_Saw_PTR2         saw_ptr2;
    SWaveform_Saw_PTR3         saw_ptr3;
    SWaveform_Saw_EPTR         saw_eptr;
    SWaveform_Saw_PolyBlep     saw_polyblep;
    SWaveform_Saw_PolyBlep2    saw_polyblep2;
    SWaveform_Saw_PolyBlep3    saw_polyblep3;

  public:

    myVoice(SVoiceManager* AManager)
    : SVoice(AManager) {
      waveform = 0;
      oversample = 1;
      decimator = 0;
      adsr.setDecay(0);
      adsr.setSustain(1);
    }

    //----------

    virtual ~myVoice() {
    }

  //------------------------------
  //
  //------------------------------

  private:

    float _getOsc(float t, float dt) {
      switch(waveform) {
        case 0:  return saw_naive.process(t,dt);      break;
        case 1:  return saw_dpw.process(t,dt);        break;
        case 2:  return saw_dpw2.process(t,dt);       break;
        case 3:  return saw_ptr1.process(t,dt);       break;
        case 4:  return saw_eptr.process(t,dt);       break;
        case 5:  return saw_dpw3.process(t,dt);       break;
        case 6:  return saw_ptr2.process(t,dt);       break;
        case 7:  return saw_polyblep.process(t,dt);   break;
        case 8:  return saw_ptr3.process(t,dt);       break;
        //case 10:  return saw_polyblep2.process(t,dt); break;
        //case 11: return saw_polyblep3.process(t,dt);  break;
      }
      return 0;
    }

    float _decimate(uint32 level, float i1, float i2) {
      switch(decimator) {
        case 0:  return i1;                               break;
        case 1:  return (i1 + i2) * 0.5;                  break;
        case 2:  return decim5[level].process(i1,i2);     break;
        case 3:  return decim7[level].process(i1,i2);     break;
        case 4:  return decim9[level].process(i1,i2);     break;
        case 5:  return decim17[level].process(i1,i2);    break;
        case 6:  return hiir2[level].downsample(i1,i2);   break;
        case 7:  return hiir4[level].downsample(i1,i2);   break;
        case 8:  return hiir6[level].downsample(i1,i2);   break;
        case 9:  return hiir8[level].downsample(i1,i2);   break;
        case 10: return hiir10[level].downsample(i1,i2);  break;
        case 11: return hiir12[level].downsample(i1,i2);  break;
        case 12: return hiir2s[level].downsample(i1,i2);  break;
        case 13: return hiir4s[level].downsample(i1,i2);  break;
        case 14: return hiir6s[level].downsample(i1,i2);  break;
        case 15: return hiir8s[level].downsample(i1,i2);  break;
        case 16: return hiir10s[level].downsample(i1,i2); break;
        case 17: return hiir12s[level].downsample(i1,i2); break;
      }
      return 0;//(i1 + i2) * 0.5; // linear interpolation
    }

  //------------------------------
  //
  //------------------------------

  public:

    void setOverSample(uint32 over) {
      switch(over) {
        case 0: over = 1; break;
        case 1: over = 2; break;
        case 2: over = 4; break;
        case 3: over = 8; break;
        case 4: over = 16; break;
        case 5: over = 32; break;
        case 6: over = 64; break;
        case 7: over = 128; break;
        case 8: over = 256; break;
      }
      float change = (float)oversample / (float)over;
      oversample = over;
      phase.dt *= change;
    }

  //------------------------------

    void setDecimator(uint32 deci) {
      decimator = deci;
    }

  //------------------------------
  //
  //------------------------------

  public:

    //virtual
    void on_noteOn(int ANote, int AVel) {
      SVoice::on_noteOn(ANote,AVel);
      float hz = SNoteToHz(ANote+MPitchBend);
      phase.setFreq(hz,MSampleRate*oversample);
      adsr.noteOn();
    }

    //----------

    //virtual
    void on_noteOff(int ANote, int AVel) {
      SVoice::on_noteOff(ANote,AVel);
      MState = svs_released;
      adsr.noteOff();
    }

    //----------

    //virtual
    void on_control(int AIndex, float AValue) {
      //float hz = 0;
      SVoice::on_control(AIndex,AValue);
      switch(AIndex) {
        case 0: on_pitchBend(AValue); break;
        case 1: waveform = AValue; break;
        case 2: setOverSample(AValue); break;
        case 3: setDecimator(AValue); break;
        case 4: adsr.setAttack(AValue); break;
        //case 5: adsr.setDecay(AValue); break;
        //case 6: adsr.setSustain(AValue); break;
        case 5: adsr.setRelease(AValue); break;
        //case 8: svf.setMode(AValue); break;
        //case 9: svf.setFreq(AValue*AValue); break;
        //case 10: svf.setBW(AValue); break;
      }
    }

    //----------

    //virtual
    void on_pitchBend(float ABend) {
      SVoice::on_pitchBend(ABend);
      float hz = SNoteToHz((float)MMidiNote + MPitchBend);
      phase.setFreq(hz, MSampleRate * oversample );

    }

    //----------

    /*
      oversample must be power of two, from 1 to 256
    */

    /*
      [upsample]
      osc
        phase
      decimate
      env
    */

    //virtual
    void on_process(float* outs) {
      //SVoice::on_process(outs);
      float out = 0.0f;

      // oscillators

      float buffer[256];

      for (uint32 i=0; i<oversample; i++) {
        phase.process();
        buffer[i] = _getOsc(phase.t,phase.dt);
      }

      // decimate

      // http://www.kvraudio.com/forum/viewtopic.php?p=4153463#p4153463
      // http://www.kvraudio.com/forum/viewtopic.php?p=4153694#p4153694

      // http://www.kvraudio.com/forum/viewtopic.php?t=337060

      //if (decimator==18) {
      //   out = downsample.process(buffer,oversample);
      //}
      //else {

        uint32 level = 0;//1;
        uint32 os = oversample;
        os >>= 1;
        while (os>=1) {
          for (uint32 i=0; i<os; i++) {
            float n = _decimate(level,buffer[i*2],buffer[i*2+1]);
            buffer[i] = n;
          }
          level++;
          os >>= 1;
        }
        out = buffer[0];

      //} // 18

//      // filter
//      out = svf.process(out);

      // todo: * midivel

      // envelope
      float env = adsr.process();
      if (adsr.stage() == ses_finished) MState = svs_off;
      out *= env;

      // out

      outs[0] = out;
      outs[1] = out;
    }
};

//----------------------------------------------------------------------
#endif
