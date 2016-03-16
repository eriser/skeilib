#ifndef skei_audio_effect_included
#define skei_audio_effect_included
//----------------------------------------------------------------------

#include "skei_audio_channel.h"

//----------------------------------------------------------------------

class SAudio_Effect
: public SAudio_StereoChannel {

  public:

    SAudio_Effect()
    : SAudio_Channel() {
    }

    ~SAudio_Effect() {
     }

};

//----------------------------------------------------------------------
#endif
