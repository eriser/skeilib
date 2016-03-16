#ifndef skei_audio_channel_included
#define skei_audio_channel_included
//----------------------------------------------------------------------

#include "skei_array.h"
#include "skei_audio_utils.h"

//----------------------------------------------------------------------

//class SAudio_Channel;

//----------

class SAudio_Module {
  //private:
  //  SAudio_Channel* MChannel;
  public:
    //SAudio_Module(SAudio_Channel* AChannel) { MChannel = AChannel; }
    virtual void on_setSampleRate(float ARate) {}
    virtual void on_setBlockSize(uint32 ASize) {}
    virtual void on_setOverSample(uint32 AOver) {}
    virtual void on_control(uint32 AIndex, float AValue, int32 AChannel=-1) {}
    virtual void on_processBlock(SSample** AInputs, SSample** AOutputs, uint32 ALength) {}
};

//----------

typedef SArray<SAudio_Module*> SAudio_ModuleArray;

//----------------------------------------------------------------------

template <int CHAN>
class SAudio_Channel {

  //private:
  protected:

    bool                MActive;
    float               MSampleRate;
    uint32              MOverSample;
    uint32              MBlockSize;
    uint32              MBufferSize;
    SSample*            MBuffer;
    SSample*            MInputs[CHAN];
    SSample*            MOutputs[CHAN];

    SAudio_ModuleArray  MModules;

  public:

    SAudio_Channel() {
      MActive     = false;
      MSampleRate = 0.0f;
      MBlockSize  = 0;
      MOverSample = 1;
      MBufferSize = 0;
      MBuffer     = SKEI_NULL;
      SMemset(MInputs,0,sizeof(MInputs));
      SMemset(MOutputs,0,sizeof(MOutputs));
    }

    //----------

    virtual
    ~SAudio_Channel() {
      if  (MBuffer) SFree(MBuffer);
    }

  //----------------------------------------
  //
  //----------------------------------------

  private:

    void resizeBuffer(void) {
      uint32 size = MBlockSize * MOverSample * sizeof(SSample) * CHAN;
      if ((size>0) && (size != MBufferSize)) {
        //STrace("size %i\n",size);
        MBufferSize = size;
        if (MBuffer) SRealloc(MBuffer,MBufferSize);
        else MBuffer = (SSample*)SMalloc(MBufferSize);
      }
      SSample* ptr = MBuffer;
      for (uint32 i=0; i<CHAN; i++) {
        MInputs[i] = ptr;
        ptr += (MBlockSize * MOverSample);
      }
      for (uint32 i=0; i<CHAN; i++) {
        MOutputs[i] = ptr;
        ptr += (MBlockSize * MOverSample);
      }
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void      active(bool state)  { MActive = state; }
    bool      active(void)        { return MActive; }
    SSample*  buffer(void)        { return MBuffer; }

    void      activate(void)      { MActive = true; }
    void      deactivate(void)    { MActive = false; }


  //----------------------------------------
  //
  //----------------------------------------

  public:

    virtual
    void on_setSampleRate(float ARate) {
      MSampleRate = ARate * MOverSample;
    }

    //----------

    virtual
    void on_setBlockSize(uint32 ASize) {
      MBlockSize = ASize;
      resizeBuffer();
    }

    //----------

    virtual
    void on_setOverSample(uint32 AOver) {
      MOverSample = AOver;
      resizeBuffer();
      on_setSampleRate(MSampleRate*AOver);
    }

    //----------

    virtual
    void on_control(uint32 AIndex, float AValue, int32 AChannel=-1) {
    }

    //----------

    virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, uint32 ALength) {
      //copyStereoBuffer(AOutputs,AInputs,ALength);
      for (uint32 i=0; i<MModules.size(); i++) {
        MModules[i]->on_processBlock(AInputs,AOutputs,ALength);
      }
    }

};

//----------------------------------------------------------------------

typedef SAudio_Channel<1> SAudio_MonoChannel;
typedef SAudio_Channel<2> SAudio_StereoChannel;

typedef SArray<SAudio_MonoChannel*> SAudio_MonoChannelArray;
typedef SArray<SAudio_StereoChannel*> SAudio_StereoChannelArray;

//----------------------------------------------------------------------
#endif
