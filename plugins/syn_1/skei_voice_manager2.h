#ifndef skei_voice_manager2_included
#define skei_voice_manager2_included
//----------------------------------------------------------------------

template <typename V, int N>
class SVoiceManager2 {
  private:
    V MAllVoices[N];
  public:
    void on_create(void) {}
    void on_destroy(void) {}
    void on_parameterChange(int32 AIndex, float AValue) {}
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {}
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {}
    void on_postProcess(void) {}
};

//----------------------------------------------------------------------
#endif
