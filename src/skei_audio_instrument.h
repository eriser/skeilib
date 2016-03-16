#ifndef skei_audio__included
#define skei_audio__included
//----------------------------------------------------------------------

#include "skei_audio_channel.h"

//----------------------------------------------------------------------

class SAudio_Instrument : public SAudio_StereoChannel {

  private:

    SAudio_VoiceArray  MAllVoices;

  public:

    //virtual
    void on_control(uint32 AIndex, float AValue, int32 AChannel=-1) {
    }

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, uint32 ALength) {
    }

    //virtual
    void on_noteOn(float ANote, float AVelocity, int32 AChannel=-1) {
    }

    //virtual
    void on_noteOff(float ANote, float AVelocity, int32 AChannel=-1) {
    }

    //virtual
    void on_pitchBend(float AValue, int32 AChannel=-1) {
    }

};

//----------------------------------------------------------------------
#endif
