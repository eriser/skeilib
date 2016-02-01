
/*

  - host sync is separated from buffer position & wraparound

*/

#include "skei_random.h"


//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//----- config -----

//#define SKEI_EXE
//#define SKEI_VST
#define SKEI_XLIB
#define SKEI_XRENDER

//----- debug -----

//#define SKEI_DEBUG
#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_MEM_PRINT

#ifdef SKEI_VST
  #define SKEI_DEBUG_SOCKET
#endif

//#define SKEI_DEBUG_VST

//----- plugin -----

#define SKEI_PLUGIN_PER_SAMPLE
#define SKEI_PLUGIN_HAS_EDITOR
//#define SKEI_PLUGIN_IS_SYNTH
#define SKEI_PLUGIN_SEND_MIDI
#define SKEI_PLUGIN_RECEIVE_MIDI
#define SKEI_PLUGIN_AUTOSYNC

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"

#include "skei_editor.h"
#include "skei_widget.h"
#include "skei_widget_panel.h"
#include "skei_widget_value.h"
#include "skei_widget_switch.h"
#include "skei_widget_button.h"
#include "skei_widget_waveform.h"
#include "skei_widget_grid.h"

#include "skei_host_sync.h"
#include "skei_midi_timing.h"

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

/*

30bpm

120 bpm 1 beat = ca 50k
120 bpm 1 beats = ca 100k
60 bpm = ca 200k
30 bpm = ca 400k

4 beats = ca 800k
8 beats = ca 1.6mb
16 beats = ca 3.2mb

*/

// 16mb
#define FX_CUTTER2_BUFFERSIZE 2*(1024*1024)*16

//----------

class myPlugin
: public SPlugin
, public SHostSync_Listener
, public SMidiTiming_Listener {

  private:
    bool              MLoop;
    SHostSync*        MSync;
    SMidiTiming*      MTiming;
    SWidget_Waveform* w_waveform;
  private: // internal/state
    float   MBuffer[FX_CUTTER2_BUFFERSIZE];
    float   MBufferSize;
    float   MReadPos;
    float   MWritePos;
    uint32  MSelection;
    uint32  MSubSelection;
    uint32  MMainSlicesLeft;
  private: // parameter values
    uint32  MNumBeats;
    uint32  MBeatSubdiv;
    uint32  MMaxCut;
    uint32  MMaxSubdiv;
    float   MCutProb;
    float   MSpeedupProb;
    float   MSlowdownProb;
    float   MHalveProb;
    float   MDoubleProb;
  public:

    //virtual
    void on_create(void) {
      w_waveform = SKEI_NULL;
      // parameters
      MNumBeats = 1;
      MBeatSubdiv = 1;
      MMaxCut = 1;
      MMaxSubdiv = 1;
      MCutProb = 0;
      MSpeedupProb = 0;
      MSlowdownProb = 0;
      MHalveProb = 0;
      MDoubleProb = 0;
      // internal
      MReadPos = 0;
      MWritePos = 0;
      MBufferSize = 1;
      MSelection = 0;
      MSubSelection = 0;
      MMainSlicesLeft = MNumBeats * MBeatSubdiv;
      //STrace("on_create\n");
      MInfo.name          = "fx_cutter2";
      MInfo.author        = "skei";
      MInfo.product       = MInfo.name;
      MInfo.uniqueId      = SKEI_MAGIC + 0x0000;
      MInfo.numInputs     = 2;
      MInfo.numOutputs    = 2;
      MEditorRect         = SRect(490,165);
      //
      //SParameter* p = appendParameterP( new SParamFloat(  "slice size",  1, 0.125, 4, 0.125 ));
      //p->label("beats");
      //
      appendParameter( new SParamInt( "num beats",      4, 1,16 ));
      appendParameter( new SParamInt( "subdiv beats",   2, 1,8 ));
      appendParameter( new SParamInt( "max cut",        4, 1,8 ));
      appendParameter( new SParamInt( "max subdiv",     4, 1,8 ));
      appendParameter( new SParameter("cut prob",       0.25 ));
      appendParameter( new SParameter("speedup prob",   0.25 ));
      appendParameter( new SParameter("slowdown prob",  0.25 ));
      appendParameter( new SParameter("halve prob",     0.25 ));
      appendParameter( new SParameter("double prob",    0.25 ));
      //
      MLoop = false;
      MSync = new SHostSync(this);
      MSync->appendInterval( 1.0f );  // num beats
      MTiming = new SMidiTiming(this);
    }

    //----------

    //virtual
    void on_destroy(void) {
      //STrace("on_destroy\n");
      delete MSync;
      delete MTiming;
    }

    //----------------------------------------
    // events
    //----------------------------------------

    //virtual
    void on_stateChange(uint32 AState) {
      //STrace("on_stateChange %i\n",AState);
      switch(AState) {
        case sps_resume:
          MReadPos = 0;
          MWritePos = 0;
          MMainSlicesLeft = MNumBeats * MBeatSubdiv;
          break;
      }
    }

    //----------

    /*
      reset buffer read/write pos when starting playback..
      todo: check beatpos, and set read/write-pos accordingly..
    */

    //virtual
    void on_transportChange(uint32 ATransport) {
      uint32 flags = spt_changed + spt_play;
      if (ATransport & flags) {
        MSync->restart();
        MReadPos = 0;
        MWritePos = 0;
        MMainSlicesLeft = MNumBeats * MBeatSubdiv;
        /*
          TODO:
          read/writepos related to beatpos..
        */
      }
      MLoop = (ATransport&spt_cycle);
      //STrace("MLoop : %i\n",MLoop);
    }

    //----------

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      //STrace("on_midiEvent %i %i %i %i\n",AOffset,AMsg1,AMsg2,AMsg3);
      MTiming->event(AOffset,AMsg1,AMsg2,AMsg3);
    }

    //----------------------------------------
    // parameters/programs
    //----------------------------------------


    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      //STrace("on_parameterChange %i %f\n",AIndex,AValue);
      switch(AIndex) {
        case 0: // num beats
          MNumBeats = trunc(AValue);
          MBufferSize = MSync->getBeatSize() * AValue;
          break;
        case 1: // subdiv beats
          MBeatSubdiv = trunc(AValue);
          MSync->setSliceSize(0,1.0f/AValue);
          break;
        case 2: // max cut
          MMaxCut = trunc(AValue);
          break;
        case 3: // max subdiv
          MMaxSubdiv = trunc(AValue);
          break;
        case 4: // cut prob
          MCutProb = AValue;
          break;
        case 5: //speedup prob
          MSpeedupProb = AValue;
          break;
        case 6: // slowdown prob
          MSlowdownProb = AValue;
          break;
        case 7: // halve prob
          MHalveProb = AValue;
          break;
        case 8: // double prob
          MDoubleProb = AValue;
          break;
      };
    }

    //----------

    //virtual
    void on_programChange(int32 AIndex) {
      //STrace("on_programChange %i\n",AIndex);
    }

    //----------

    //virtual
    void on_preProgram(int32 AIndex) {
      //STrace("on_preProgram %i\n",AIndex);
    }

    //----------

    //virtual
    void on_postProgram(void) {
      //STrace("on_postProgram\n");
    }

    //----------------------------------------
    // process
    //----------------------------------------

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
      //STrace("on_processBlock\n");
      MSync->setTempo(MState.tempo);
      MSync->setSampleRate(MState.sampleRate);
      MSync->setBeatPos(MState.beatPos);
      MSync->setLoop( MLoop, MState.loopStart, MState.loopEnd );
      MTiming->preProcess();
    }

    //----------

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {
      float in0 = *AInputs[0];
      float in1 = *AInputs[1];
      float out0 = 0;
      float out1 = 0;
      // #ifdef SKEI_PROCESS_WHEN_STOPPED:...
      if (MState.playState&spt_play) {
        MSync->process();
        MTiming->process();
        // read from buffer
        uint32 rpos = (int)MReadPos * 2;
        out0 = MBuffer[rpos];
        out1 = MBuffer[rpos+1];
        MReadPos += 1;
        MReadPos = SModulo(MReadPos,MBufferSize);
        // write to buffer
        uint32 wpos = (int)MWritePos * 2;
        MBuffer[wpos] = in0;
        MBuffer[wpos+1] = in1;
        MWritePos += 1;
        MWritePos = SModulo(MWritePos,MBufferSize);

      } // play
      *AOutputs[0] = out0;
      *AOutputs[1] = out1;
    }

    //----------

    //virtual
    void on_postProcess(void) {
      //STrace("on_postProcess\n");
      MTiming->postProcess();
    }

    //----------------------------------------
    // editor
    //----------------------------------------

    //virtual
    void* on_openEditor(void* AParent) {
      //STrace("on_openEditor %x\n",AParent);
      SEditor* editor = new SEditor(this,MEditorRect.w,MEditorRect.h,AParent);
      editor->setTitle("fx_cutter2");
      editor->appendWidget( new SWidget_Value(  SRect( 10,60,   150,20 ), 0, swa_none ));
      editor->appendWidget( new SWidget_Value(  SRect( 10,85,   150,20 ), 0, swa_none ));
      editor->appendWidget( new SWidget_Value(  SRect( 10,110,  150,20 ), 0, swa_none ));
      editor->appendWidget( new SWidget_Value(  SRect( 10,135,  150,20 ), 0, swa_none ));
      editor->appendWidget( new SWidget_Value(  SRect( 170,60,  150,20 ), 0, swa_none ));
      editor->appendWidget( new SWidget_Value(  SRect( 330,60,  150,20 ), 0, swa_none ));
      editor->appendWidget( new SWidget_Value(  SRect( 330,85,  150,20 ), 0, swa_none ));
      editor->appendWidget( new SWidget_Value(  SRect( 330,110, 150,20 ), 0, swa_none ));
      editor->appendWidget( new SWidget_Value(  SRect( 330,135, 150,20 ), 0, swa_none ));
      w_waveform = new SWidget_Waveform( SRect(10,10,470,40), swa_none );
      w_waveform->setNumGrid(16);
      w_waveform->setNumMarkers(2);
      w_waveform->setMarkerColor(0,SLightRed);
      w_waveform->setMarkerColor(1,SLightGreen);
      //w_waveform->setNumAreas(2);
      editor->appendWidget(w_waveform);
      for (int i=0; i<9; i++)
        editor->connect( (SWidget*)editor->subWidget(i), parameter(i) );
      return editor;
    }

    //----------

    //virtual
    void on_closeEditor(void* AEditor) {
      //STrace("on_closeEditor\n");
      SEditor* editor = (SEditor*)AEditor;
      delete editor;
    }

    //----------

    //virtual
    void on_idleEditor(void* AEditor) {
      //STrace("MReadPos %i\n",(int32)MReadPos);
      w_waveform->setBuffer( MBuffer );
      w_waveform->setBufferSize( (int32)MBufferSize );
      w_waveform->setMarkerPos(0,(int32)MWritePos);
      w_waveform->setMarkerPos(1,(int32)MReadPos);
      w_waveform->setNumGrid( MNumBeats * MBeatSubdiv );
      w_waveform->do_redraw(w_waveform,w_waveform->rect());
    }

    //----------------------------------------
    // listeners
    //----------------------------------------

    // AIndex = slice index (in worldspace/hosttime)

    //virtual
    void on_hostsync_tick(uint32 AIndex) {
      STrace("MMainSlicesLeft %i\n",MMainSlicesLeft);
      if (MMainSlicesLeft>1) MMainSlicesLeft--;
      else MMainSlicesLeft = MNumBeats * MBeatSubdiv;
      if (MSelection > 0) MSelection--;
      else {
        if (SRandom() < MCutProb) {
          //STrace("new cut\n");
        }
      }
    }

    //----------

    //virtual
    void on_miditiming_event(uint32 msg, uint32 chn, uint32 idx, uint32 val) {
      STrace("miditiming: %i,%i,%i,%i\n",msg,chn,idx,val);
    }


};

//----------------------------------------------------------------------
// editor
//----------------------------------------------------------------------

//class myEditor
//: public SEditor {
//};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

