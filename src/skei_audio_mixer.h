#ifndef skei_audio_mixer_included
#define skei_audio_mixer_included
//----------------------------------------------------------------------

#include "skei_audio_channel.h"

//----------------------------------------------------------------------

class SAudio_Mixer : public SAudio_StereoChannel {

  private:
    SAudio_StereoChannelArray  MChannels;

  private:

  public:

    //virtual
    void on_control(uint32 AIndex, float AValue, int32 AChannel=-1) {
      if (AChannel>=0) {
        SAssert( (uint32)AChannel < MChannels.size() );
        if (MChannels[AChannel]->active()) {
          MChannels[AChannel]->on_control(AIndex,AChannel);
        }
      }
      else {
        for (uint32 i=0; i<MChannels.size(); i++) {
          if (MChannels[i]->active()) {
            MChannels[i]->on_control(AIndex,AValue,AChannel);
          }
        }
      }
    }

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, uint32 ALength) {
      clearStereoBuffer(AOutputs,ALength);

      for (uint32 i=0; i<MChannels.size(); i++) {
        if (MChannels[i]->active()) {
          MChannels[i]->on_processBlock(AInputs,MOutputs,ALength);
          addStereoBuffer(AOutputs,MOutputs,ALength);
        }
      }
    }

};

//----------------------------------------------------------------------
#endif
