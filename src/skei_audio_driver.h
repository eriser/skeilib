#ifndef skei_audio_driver_included
#define skei_audio_driver_included
//----------------------------------------------------------------------

class SAudioDriver {

  private:

    SChannel*  MChannel;

  public:

    SAudioDriver() {
    }

    virtual ~SAudioDriver() {
    }

  public:

    SChannel*  channel(void)                     { return MChannel; }
    void            channel(SChannel* AChannel)  { MChannel = AChannel; }

  public:

    virtual void processBlock(SSample** AInputs, SSample** AOutputs, uint32 ALength) {
      MChannel->processBlock(AInputs,AOutputs,ALength);
    }

};

//----------------------------------------------------------------------
#endif
