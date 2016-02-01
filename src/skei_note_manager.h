#ifndef skei_note_manager_included
#ifndef skei_note_manager_included
//----------------------------------------------------------------------

class SNoteManager {
  public:
    void  noteOn(int32 ANote, float AVelocity) {}
    void  noteOff(int32 ANote, float AVelocity) {}
    bool  isOn(int32 ANote) { return false; }
    float afterTouch(int32 ANote) { return 0; }
};

//----------------------------------------------------------------------
#endif
