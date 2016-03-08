
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

class myPlugin
: public SPlugin
, public SHostSync_Listener
, public SMidiTiming_Listener {

  private:
    bool          MLoop;
    SHostSync*    MSync;
    SMidiTiming*  MTiming;

  public:

    //virtual
    void on_create(void) {
      //STrace("on_create\n");
      MInfo.name          = "skeilib test";
      MInfo.author        = "skei";
      MInfo.product       = MInfo.name;
      MInfo.uniqueId      = SKEI_MAGIC + 0x0000;
      MInfo.numInputs     = 2;
      MInfo.numOutputs    = 2;
      MEditorRect         = SRect(256,256);
      //
      SParameter* p = appendParameterP( new SParamFloat(  "slice size",  1, 0.125, 4, 0.125 ));
      p->label("beats");
      //
      MLoop = false;

      MSync = new SHostSync(this);
      MTiming = new SMidiTiming(this);

      MSync->appendInterval( 1.0f );
      MSync->appendInterval( 4.0f );

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
    }

    //----------

    //virtual
    void on_transportChange(uint32 ATransport) {
      uint32 flags = spt_changed + spt_play;
      if (ATransport & flags) MSync->restart();
      MLoop = (ATransport&spt_cycle);
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
      if (AIndex==0) MSync->setSliceSize(0,AValue);

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

      //MSync.setTempo(MState.tempo,MState.sampleRate);
      MSync->setTempo(MState.tempo);
      MSync->setSampleRate(MState.sampleRate);
      MSync->setBeatPos(MState.beatPos);
      MSync->setLoop( MLoop, MState.loopStart, MState.loopEnd );
      //
      MTiming->preProcess();
    }

    //----------

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {
      //float in0 = *AOutputs[0];
      //float in1 = *AOutputs[1];
      float out0 = 0;
      float out1 = 0;
      // #ifdef SKEI_PROCESS_WHEN_STOPPED:...
      if (MState.playState&spt_play) {
        MSync->process();
        MTiming->process();
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

      editor->appendWidget( new SWidget_Panel(    SRect( 10,10,100,18 ), swa_none ));
      editor->appendWidget( new SWidget_Switch(   SRect( 10,30,100,18 ), "on", "off", swa_none ));
      editor->appendWidget( new SWidget_Value(    SRect( 10,50,100,18 ), 0.3, swa_none ));
      editor->appendWidget( new SWidget_Button(   SRect( 10,70,100,18 ), "action!", "click me", swa_none ));

      SWidget_Waveform* wa = new SWidget_Waveform( SRect(40), swa_fillBottom );
      wa->setNumGrid(5);
      editor->appendWidget(wa);

      SWidget_Grid* gr = new SWidget_Grid( SRect(100), 10,10,swa_fillBottom );
      editor->appendWidget(gr);

      SWidget*    wdg = (SWidget*)editor->subWidget(2);
      SParameter* par = parameter(0);
      editor->connect( wdg,par );

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
      //STrace("on_idleEditor\n");
    }

    //----------------------------------------
    // listeners
    //----------------------------------------

    //virtual
    void on_hostsync_tick(uint32 AIndex) {
      STrace("hostsync %i : %i\n",AIndex,MSync->getSlice(AIndex));
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
