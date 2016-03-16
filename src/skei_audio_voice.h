#ifndef skei_audio_voice_included
#define skei_audio_voice_included
//----------------------------------------------------------------------

#include "skei_audio_channel.h"

//----------------------------------------------------------------------

class SAudio_Voice : public SAudio_MonoChannel {
  public:
    //virtual void on_control(uint32 AIndex, float AValue) {}
    //virtual void on_processBlock(SSample** AInputs, SSample** AOutputs, uint32 ALength) {}
  public:
    virtual void on_noteOn(float ANote, float AVelocity) {}
    virtual void on_noteOff(float ANote, float AVelocity) {}
    virtual void on_control(uint32 AIndex, float AValue, int32 AChannel=-1) {}
    virtual void on_pitchBend(float AValue) {}
};

//----------

typedef SArray<SAudio_Voice*>  SAudio_VoiceArray;


//----------------------------------------------------------------------
#endif
