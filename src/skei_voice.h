#ifndef skei_voice_included
#define skei_voice_included
//----------------------------------------------------------------------

#if 0

#include "skei_channel.h"

class SVoice
: public SModule {

  public:

    //SVoice() : SModule() {}

  public:

    virtual void on_parameter(float AIndex, float AValue) {}
    virtual void on_control(float AIndex, float AValue) {}
    virtual void on_noteOn(uint32 ANote, float AVelocity) {}
    virtual void on_noteOff(uint32 ANote, float AVelocity) {}
    virtual void on_pitchBend(float ABend) {}

};


typedef SArray<SVoice*>  SVoiceArray;

#endif // 0

//----------------------------------------------------------------------
#endif
