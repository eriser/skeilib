#ifndef midi_fibonacci_included
#define midi_fibonacci_included
//----------------------------------------------------------------------

/*
  changes notes based on the fibonacci series

  version history:
  0: initial release (preview)
  1: changed from note-creation to note-modification
  2: adapted to skeilib
*/

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//#define SKEI_PLUGIN_HAS_EDITOR
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_PER_SAMPLE
#define SKEI_PLUGIN_SEND_MIDI
#define SKEI_PLUGIN_RECEIVE_MIDI
#define SKEI_PLUGIN_AUTOSYNC

//#ifdef SKEI_PLUGIN_HAS_EDITOR
//  #define SKEI_XLIB
//  #define SKEI_XRENDER
//  #define SKEI_XFT
//#endif

//#define SKEI_DEBUG_MEM
#ifdef SKEI_VST
  //#define SKEI_DEBUG_VST
  #define SKEI_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"

//#include "skei_editor.h"
//#include "skei_widget_slider.h"

//----------------------------------------------------------------------

static const char* txt_dir[] = {"positive","negative"};

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin {

  private:
    int32 MNoteBuffer[128*16];
    int32 MChanBuffer[128*16];
    int32 MNum1;
    int32 MNum2;
  private:
    int32 p_num1;
    int32 p_num2;
    int32 p_mod;
    int32 p_dir;

  //----------------------------------------
  //
  //----------------------------------------

  private:

    void _fib_init(void) {
      MNum1 = p_num1;
      MNum2 = p_num2;
      for (int32 chan=0; chan<16; chan++) {
        for (int32 note=0; note<128; note++) {
          int32 index = (chan*128)+note;
          MNoteBuffer[index] = -1;
          MChanBuffer[index] = 0;
        }
      }
    }

    //----------

    /*
      aha!
      midi is processed before process(), and since we check the transport in the
      start of process, there is already a note playing, and it will be shut off
    */

    void _fib_reset(void) {
      MNum1 = p_num1;
      MNum2 = p_num2;
      for (int32 chan=0; chan<16; chan++) {
        for (int32 note=0; note<128; note++) {
          int32 index = (chan*128)+note;
          int32 n = MNoteBuffer[index];
          int32 c = MChanBuffer[index];
          if (n != -1) sendMidiToHost(0,0x80+c,n,0); // $80 = note off
          MNoteBuffer[index] = -1;
          MChanBuffer[index] = 0;
        }
      }
    }

    //----------

    int32 _fib_next(void) {
      int32 next;
      if (p_dir < 1) next = (MNum1 + MNum2) % p_mod;
      else next = (MNum1 - MNum2) % p_mod;
      int32 result = MNum1 % p_mod; // mod to catch 1-note length series.. 0,1,<next>
      MNum1 = MNum2; // 0 -> 1
      MNum2 = next;  // 1 -> 1
      return result;
    }

    //----------

    void _noteOn(uint32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      int32 chan = AMsg1 & 0x0f;
      int32 note = AMsg2 & 0x7f;
      int32 index = (chan*128)+note;
      int32 n = note + _fib_next();
      int32 c = chan;
      MNoteBuffer[index] = n;
      MChanBuffer[index] = c;
      sendMidiToHost(AOffset,0x90+c,n,AMsg3); // $90 = note on
    }

    //----------

    void _noteOff(uint32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      int32 chan = AMsg1 & 0x0f;
      int32 note = AMsg2 & 0x7f;
      int32 index = (chan*128)+note;
      int32 n = MNoteBuffer[index];
      int32 c = MChanBuffer[index]; // AMsg1 and 15;
      if (n != -1) sendMidiToHost(AOffset,0x80+c,n,AMsg3); // $80 = note off
      else sendMidiToHost(AOffset,AMsg1,AMsg2,AMsg3);
      MNoteBuffer[index] = -1;
      MChanBuffer[index] = 0;
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    void on_create(void) {
      MInfo.name       = "midi_fibonacci";
      MInfo.author     = "skei";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;

      //#ifdef SKEI_PLUGIN_HAS_EDITOR
      //MEditorRect.set(220+90+40,100);
      //#endif

      appendParameter( new SParamInt( "num1", 0,  -24, 24 ));
      appendParameter( new SParamInt( "num2", 1,  -24, 24 ));
      appendParameter( new SParamInt( "mod",  12,  1,  48 ));
      appendParameter( new SParamText("dir",  0,   2,  txt_dir ));

      _fib_init();

    }

    //----------

    /*
      test:
      do reset in stop..
    */

    //virtual
    void on_transportChange(uint32 ATransport) {
      if ((ATransport & spt_play) == 0) { _fib_reset(); }
    }

    //----------

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      float v = AValue;
      int32 i = STrunc(v);
      switch(AIndex) {
        case 0:
          p_num1 = i;
          MNum1 = i;
          break;
        case 1:
          p_num2 = i;
          MNum2 = i;
          break;
        case 2:
          p_mod = i;
          _fib_reset();
          break;
        case 3:
          p_dir = i;
          _fib_reset();
          break;
      }
    }

    //----------

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      switch(AMsg1 & 0xF0) {
        case 0x80:
          _noteOff(AOffset,AMsg1,AMsg2,AMsg3);
          break;
        case 0x90:
          _noteOn(AOffset,AMsg1,AMsg2,AMsg3);
          break;
        default:
          sendMidiToHost(AOffset,AMsg1,AMsg2,AMsg3);
          break;
      }
    }

    //------------------------------------------------------------
    // audio
    //------------------------------------------------------------

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
      SMemcpy(AOutputs[0],AInputs[0],ANumSamples*sizeof(SSample));
      SMemcpy(AOutputs[1],AInputs[1],ANumSamples*sizeof(SSample));
    }

    //------------------------------------------------------------
    // editor
    //------------------------------------------------------------

    #ifdef SKEI_PLUGIN_HAS_EDITOR

    /*

    //virtual
    void* on_openEditor(void* AParent) {
      SEditor* editor = new SEditor(this,MEditorRect.w,MEditorRect.h,AParent);
      editor->fillColor( SLightGrey );
      editor->fillBackground(true);

      for (int32 i=0; i<4; i++) {
        SWidget* widget = new SWidget_Slider( SRect(90+10,10+(i*20),200+40,18), 0 );
        editor->appendWidget( widget );
        editor->connect( widget, parameter(i) );
      }
      return editor;
    }

    //----------

    //virtual
    void on_closeEditor(void* AEditor) {
      SEditor* editor = (SEditor*)AEditor;
      delete editor;
    }

    */

    #endif // SKEI_PLUGIN_HAS_EDITOR

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif




