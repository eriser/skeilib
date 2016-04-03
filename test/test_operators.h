#ifndef test_operator_included
#define test_operator_included
//----------------------------------------------------------------------
// nc -U -l -k /tmp/skeilib.socket



//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//#define SKEI_PLUGIN_HAS_EDITOR
#define SKEI_PLUGIN_IS_SYNTH
#define SKEI_PLUGIN_RECEIVE_MIDI

/*
#ifdef SKEI_PLUGIN_HAS_EDITOR
  #define SKEI_XLIB
  #define SKEI_XRENDER
  #define SKEI_XFT
#endif
*/

//#define SKEI_DEBUG_MEM
#define SKEI_DEBUG_CRASHHANDLER

#ifdef SKEI_VST
  //#define SKEI_DEBUG_VST
  #define SKEI_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
//#include "skei_channel.h"
#include "skei_graph.h"
#include "skei_plugin.h"
#include "skei_random.h"

/*
#ifdef SKEI_PLUGIN_HAS_EDITOR
  #include "skei_editor.h"
#endif
*/

//----------------------------------------------------------------------
// stereo noise operator
//----------------------------------------------------------------------

struct SOperator_Noise_Data {
  float*  buffer0;
  float*  buffer1;
};

//----------

static
void SOperator_Noise(void* AGlobal, void* AData, float* AParams) {
  SOperator_Noise_Data* data = (SOperator_Noise_Data*)AData;
  for (uint32 i=0; i<SKEI_OPERATOR_TICKSIZE; i++) data->buffer0[i] = SRandomSigned();
  for (uint32 i=0; i<SKEI_OPERATOR_TICKSIZE; i++) data->buffer1[i] = SRandomSigned();
}

//----------------------------------------------------------------------
// stereo scale operator
//----------------------------------------------------------------------

struct SOperator_Scale_Data {
  float*  buffer0;
  float*  buffer1;
  float   scale;
};

//----------

static
void SOperator_Scale(void* AGlobal, void* AData, float* AParams) {
  //SOperator_Scale_Data* data = (SOperator_Scale_Data*)AData;
  //for (uint32 i=0; i<SKEI_OPERATOR_TICKSIZE; i++) data->buffer0[i] *= AParams[0];
  //for (uint32 i=0; i<SKEI_OPERATOR_TICKSIZE; i++) data->buffer1[i] *= AParams[0];
}

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin
{

  private:

    //SChannel*             channel;
    SGraph*               graph;
    SOperatorGlobals      global;
    SOperator_Noise_Data  op_noise_data;
    SOperator_Scale_Data  op_scale_data;
    float                 op_scale_params[3];

  public:

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    /*
      we need to set keft/right (op_noise_state) = channel.MTickBuffer,
    */

    //virtual
    void on_create(void) {
      MInfo.name       = "test_operator";
      MInfo.author     = "skei";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;

      #ifdef SKEI_PLUGIN_HAS_EDITOR
      MEditorRect = SRect(640,480);
      #endif

      appendParameter( new SParameter("level",0.5) );

      /*
        global stuff, given to all operators
      */

      global.sampleRate     = 44100.0f;
      global.blockSize      = 128;
      global.beatPos        = 0.0f;
      global.pitchBend      = 0.0f;
      global.pitchBendRange = 2.0f;

      //channel = new SChannel(&global);
      graph = new SGraph(&global);
      SModule* module;

      // noise
      // could these be encapsulated into a SModule_Noise class?

      module = new SModule(&global);
        // noise
        //op_noise_data.buffer0 = channel->tickBuffer(0);
        //op_noise_data.buffer1 = channel->tickBuffer(1);
        op_noise_data.buffer0 = graph->tickBuffer(0);
        op_noise_data.buffer1 = graph->tickBuffer(1);
        module->appendOperator(SOperator_Noise,&op_noise_data);
        // scale
        //op_scale_data.buffer0 = channel->tickBuffer(0);
        //op_scale_data.buffer1 = channel->tickBuffer(1);
        op_scale_data.buffer0 = graph->tickBuffer(0);
        op_scale_data.buffer1 = graph->tickBuffer(1);
        op_scale_params[0]    = 0.1f;
        module->appendOperator(SOperator_Scale,&op_scale_data,op_scale_params);
        //module->connectParameter( 0, &op_scale_params[0] );

      //channel->appendModule(module);
      //channel->connectParameter( 0, &op_scale_params[0] );
      graph->appendModule(module);
      graph->connectParameter( 0, &op_scale_params[0] );

    }

    //----------

    //virtual
    void on_destroy(void) {
      //delete channel;
      delete graph;
    }

    //------------------------------------------------------------
    // events
    //------------------------------------------------------------

    //virtual
    void on_stateChange(uint32 AState) {
      switch(AState) {
        case sps_sampleRate:
          global.sampleRate = MState.sampleRate;
          break;
        case sps_blockSize:
          global.blockSize = MState.blockSize;
          break;
        case sps_resume:
          // connect?
          break;
      }
    }

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      //channel->parameterChange(AIndex,AValue);
      graph->parameterChange(AIndex,AValue);
    }

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      //channel->midiEvent(AOffset,AMsg1,AMsg2,AMsg3);
      graph->midiEvent(AOffset,AMsg1,AMsg2,AMsg3);
    }

    //------------------------------------------------------------
    // audio
    //------------------------------------------------------------

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {

      //graph->connectOutput(AOutputs[0],AOutputs[1]);
      graph->connectOutput(AOutputs[0],AOutputs[1]);

      //channel->processBlock(AInputs,AOutputs,ANumSamples);
      graph->processBlock(AInputs,AOutputs,ANumSamples);
    }

    //------------------------------------------------------------
    // editor
    //------------------------------------------------------------

    #ifdef SKEI_PLUGIN_HAS_EDITOR
    /*

    //virtual
    void* on_openEditor(void* AParent) {
      SEditor* editor = new SEditor(this,MEditorRect.w,MEditorRect.h,AParent);
      //editor->fillColor( SLightGrey );
      //editor->fillBackground(true);
      return editor;
    }

    //virtual
    void on_closeEditor(void* AEditor) {
      SEditor* editor = (SEditor*)AEditor;
      delete editor;
    }

    //virtual
    void on_idleEditor(void* AEditor) {
      SEditor* editor = (SEditor*)AEditor;
      if ((editor) && (MEditorIsOpen)) {
        //editor->do_redraw(..);
      }
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

