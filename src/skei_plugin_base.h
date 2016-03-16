#ifndef skei_plugin_base_included
#define skei_plugin_base_included
//----------------------------------------------------------------------

/*
> SPlugin_Base
  SPlugin_Parameters
  SPlugin_Programs
  SPlugin_Editor
  SPlugin_Implementation
  SPlugin
*/

//----------

#include "skei_parameter.h"
#include "skei_program.h"
#include "skei_queue.h"

#ifdef SKEI_PLUGIN_HAS_EDITOR
#include "skei_editor.h"
#endif
#include "skei_editor_listener.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  lock/wait-free queue to pass data from the plugin to the
  audio processing thread, and the (potential) editor
*/

typedef SQueue<int32,SKEI_PLUGIN_MAX_QUEUE_SIZE> SParamQueue;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

struct SPlugin_Info {
  const char* name;
  const char* author;
  const char* product;
  uint32      uniqueId;
  uint32      version;
  uint32      numInputs;
  uint32      numOutputs;
  uint32      latency;
  //
  SPlugin_Info() {
    name          = "plugin";
    author        = "author";
    product       = name;
    uniqueId      = SKEI_MAGIC + 0x0000;
    version       = 0;
    numInputs     = 0;
    numOutputs    = 0;
    latency       = 0;
  }
};

//----------------------------------------------------------------------

struct SPlugin_State {
  bool        isOpen;
  bool        isProcessing;
  float       sampleRate;
  int32       blockSize;
  int32       playState;
  int32       prevState;
  float       samplePos;
  float       tempo;
  float       beatPos;
  float       loopStart;
  float       loopEnd;
  //
  SPlugin_State() {
    isOpen       = false;
    isProcessing = false;
    sampleRate   = 0;
    blockSize    = 0;
    playState    = 0;
    prevState    = 0;
    samplePos    = 0;
    tempo        = 0;
    beatPos      = 0;
    loopStart    = 0;
    loopEnd      = 0;
  }
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class SPlugin_Base
: public SEditor_Listener

{

  protected:

    //SPlugin_Listener* MListener;
    SPlugin_Info  MInfo;
    SPlugin_State MState;

  protected:

    // parameters

    SParameterArray MParameterArray;
    float           MParameterValues[SKEI_PLUGIN_MAX_PARAMETERS];
    float           MProcessValues[SKEI_PLUGIN_MAX_PARAMETERS];
    bool            MProcessValuesDirty;
    SParamQueue     MProcessValueQueue;
    #ifdef SKEI_PLUGIN_HAS_EDITOR
    float           MEditorValues[SKEI_PLUGIN_MAX_PARAMETERS];
    bool            MEditorValuesDirty;
    SParamQueue     MEditorValueQueue;
    #endif

    // programs

    int32           MCurrentProgram;
    SProgramArray   MProgramArray;

    // editor

    #ifdef SKEI_PLUGIN_HAS_EDITOR
    SEditor*    MEditor;
    bool        MEditorIsOpen;
    SRect       MEditorRect;
    uint32      MEditorStateSize;
    void*       MEditorStateBuffer;
    #endif

  //----------------------------------------
  //
  //----------------------------------------

  public:

    SPlugin_Base() {

      // parameters

      SMemset( &MParameterValues, 0, SKEI_PLUGIN_PARAMETER_ARRAY_SIZE );
      SMemset( &MProcessValues,   0, SKEI_PLUGIN_PARAMETER_ARRAY_SIZE );
      MProcessValuesDirty  = false;
      #ifdef SKEI_PLUGIN_HAS_EDITOR
      SMemset( &MEditorValues, 0, SKEI_PLUGIN_PARAMETER_ARRAY_SIZE );
      MEditorValuesDirty   = false;
      #endif

      // programs

      MCurrentProgram = 0;

      // editor

      #ifdef SKEI_PLUGIN_HAS_EDITOR
      MEditor              = SKEI_NULL;
      MEditorIsOpen        = false;
      MEditorRect          = SRect(0);
      MEditorStateSize     = 0;
      MEditorStateBuffer   = SKEI_NULL;
      #endif

    }

    //----------

    virtual ~SPlugin_Base() {

      // parameters

      #ifndef SKEI_NO_AUTODELETE
      deleteParameters();
      #endif

      // programs

      #ifndef skei_NO_AUTODELETE
      deletePrograms();
      #endif

      // editor

      //#ifdef SKEI_PLUGIN_HAS_EDITOR
      //if (MEditorIsOpen && MEditor) on_closeEditor(MEditor);
      //if (MEditorStateBuffer) SFree(MEditorStateBuffer);
      //#endif

    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    SPlugin_Info*   info(void)  { return &MInfo; }
    SPlugin_State*  state(void) { return &MState; }

  //----------------------------------------
  //
  //----------------------------------------

  /*
    implemented by plugin format (exe/vst)
  */

  public:

    virtual void* main(void* APointer=SKEI_NULL) { return SKEI_NULL; }
    virtual void  updateHostInfo(void) {}
    virtual void  updateHostTime(void) {}
    virtual void  notifyHostParameterChanged(int32 AIndex, float AValue) {}
    virtual void  sendMidiToHost(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {}
    virtual void  resizeEditor(int32 AWidth, int32 AHeight) {}

  //----------------------------------------
  //
  //----------------------------------------

  /*
    (optionally) implemented by your plugin
    aka host_listener
  */

  public:

    virtual void  on_create(void) {}
    virtual void  on_destroy(void) {}
    virtual void  on_stateChange(uint32 AState) {}
    virtual void  on_transportChange(uint32 ATransport) {}
    virtual void  on_parameterChange(int32 AIndex, float AValue) {}
    virtual void  on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {}
    virtual void  on_programChange(int32 AIndex) {}
    virtual void  on_preProgram(int32 AIndex) {}
    virtual void  on_postProgram(void) {}
    virtual void  on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {}
    virtual void  on_processSample(SSample** AInputs, SSample** AOutputs) {}
    virtual void  on_postProcess(void) {}
    virtual void* on_openEditor(void* AParent) { return SKEI_NULL; }
    virtual void  on_closeEditor(void* AEditor) {}
    virtual void  on_idleEditor(void* AEditor) {}

  //----------------------------------------
  // parameters
  //----------------------------------------

  public:

    virtual
    int32 appendParameter(SParameter* AParameter) {
      int32 index = MParameterArray.size();
      AParameter->index(index);
      MParameterArray.append(AParameter);
      return index;
    }

    //----------

    virtual
    SParameter* appendParameterP(SParameter* AParameter) {
      appendParameter(AParameter);
      return AParameter;
    }

    //----------

    virtual
    void deleteParameters(void) {
      for (uint32 i=0; i<MParameterArray.size(); i++) {
        delete MParameterArray[i];
      }
    }

    //----------

    virtual
    int32 numParameters(void) {
      return MParameterArray.size();
    }

    //----------

    virtual
    SParameter* getParameter(int32 AIndex) {
      return MParameterArray[AIndex];
    }

    virtual SParameter* parameter(int32 AIndex) {
      return MParameterArray[AIndex];
    }

    //----------

    virtual
    float getParameterValue(int32 AIndex) {
      return MParameterValues[AIndex];
    }

    //----------

    /*
      be sure to call this instead of setting the MParameterValues directly,
      since we also want to update MProcess/EditorValues..
    */

    virtual
    void setParameterValue(int32 AIndex, float AValue) {
      MParameterValues[AIndex] = AValue;
      //MProcessValuesDirty = true;
      //MEditorValuesDirty = true;
      MProcessValueQueue.write(AIndex);
      #ifdef SKEI_PLUGIN_HAS_EDITOR
      MEditorValueQueue.write(AIndex);
      #endif
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    /*
      set parameterValues to default values (from the parameter),
      and set processValues & editorValues different from this,
      so that they will be seen as different, and action taken in:
        - SPlugin.updateEditorInIdle
        - SPlugin.updateParametersInProcess

      called from:
      - SPlugin.main
    */

    //virtual
    void setDefaultParameterValues(void) {
      for (uint32 i=0; i<MParameterArray.size(); i++) {
        float v = MParameterArray[i]->defaultValue();
        //STrace("%i = %f\n",i,v);
        MParameterValues[i] = v;       // FParameters[i].to01(v);
        MProcessValues[i]   = v + 1;    // force on_ParameterChange
        #ifdef SKEI_PLUGIN_HAS_EDITOR
        MEditorValues[i] = v + 1;       // force redraw
        #endif
      }
      MProcessValuesDirty = true;
      #ifdef SKEI_PLUGIN_HAS_EDITOR
      MEditorValuesDirty = true;
      #endif
    }

    //----------

    /*
      when we open an editor, we need to distribute (spread) the parameter
      values to both the process() thread, and the editor..
    */

    /*
      distribute/spread parameter values from plugin (FParamValues)
      to audio process (FProcValues) and editor (FEditorValues)

      set FProcValues & FEditorValues different from FParamValues,
      so that they will be seen as different, and action taken in:
      - SPlugin.updateEditorInIdle
      - SPlugin.updateParametersInProcess

      called from:
        SPlugin_Vst.vst_dispatcher/effEditOpen
    */

    //virtual
    void distributeParameterValues(void) {
      for (uint32 i=0; i<MParameterArray.size(); i++) {
        float v = MParameterValues[i];

        //STrace("%i = %f\n",i,v);

        MProcessValues[i] = v + 1;  // force on_ParameterChange
        #ifdef SKEI_PLUGIN_HAS_EDITOR
        MEditorValues[i]  = v + 1;  // force redraw
        #endif
      }
      MProcessValuesDirty = true;
      #ifdef SKEI_PLUGIN_HAS_EDITOR
      MEditorValuesDirty = true;
      #endif
    }

    //----------

    /*
      to be as thread-safe (and no wait/lock) as possible, communication
      between the different (possible) threads are kept to a minimum,
      and only in known/safe places..
      parameter values are checked for changes at the start of
      process() only..
    */

    /*
      called from:
      - SPlugin.vst_process

      todo/cando:
      - set a flag or something for modified parameters, so we can check for
        things like freq _or_ bw changed, so we need to recalc coefficients
        (avoid doing it twice)
      - preParameterChanges, postParameterChanges , ??
    */

    //virtual
    void updateParametersInProcess(void) {
      // if dirty flag is set, we compare and possibly update all parameters..
      if (MProcessValuesDirty) {
        //STrace("dirty (all)\n");
        // we are going to check all parameters, including those possibly in the queue, so we just clear it..
        MProcessValueQueue.clear();
        // if items are pushed onto the queue after this, they will be handled next round
        MProcessValuesDirty = false;
        // if flag is set to true after this (from  another thread?), we will do this again next round
        for (uint32 i=0; i<MParameterArray.size(); i++) {
          float v = MParameterValues[i];
          if (v != MProcessValues[i]) {
            MProcessValues[i] = v;
            on_parameterChange(i, MParameterArray[i]->from01(v));
          }
        }
      } // processValuesDirty
      else {
        /*
        else we pop items off the event queue, and check only these..
        consider: make sure queue and process() calls are 'n sync'
        reaper just stops calling process when pause is on, but
        doesn't let us know via any other call, so, if you tweak a
        knob in pause mode, the queue fills up..
        maybe: if we can't write item to queue, set dirty flag, and
        clear the queue..
        */
        int32 i;
        while (MProcessValueQueue.read(&i)) {
          //STrace("queue (1)\n");
          float v = MParameterValues[i];
          if (v != MProcessValues[i]) {
            MProcessValues[i] = v;
            on_parameterChange(i, MParameterArray[i]->from01(v));
          }
        }
      }  // !processValuesDirty

    }

  //----------------------------------------
  // programs
  //----------------------------------------

  public:

    virtual
    int32 appendProgram(SProgram* AProgram) {
      int32 index = MProgramArray.size();
      AProgram->index(index);
      MProgramArray.append(AProgram);
      return index;
    }

    //----------

    virtual
    void deletePrograms(void) {
      for (uint32 i=0; i<MProgramArray.size(); i++) {
        delete MProgramArray[i];
      }
    }

    //----------

    virtual
    SProgram* getProgram(int32 AIndex) {
      return MProgramArray[AIndex];
    }

    //----------

    virtual
    SProgram* createDefaultProgram(void) {
      int32 num = MParameterArray.size();
      if (num > 0) {
        //prog := SProgram.create('default',0,nil);
        SProgram* prog = new SProgram("default");
        for (int32 i=0; i<num; i++) {
          float v = MParameterArray[i]->defaultValue();
          prog->value(i,v);
        }
        return prog;
      }
      else return SKEI_NULL;
    }

    //----------

    virtual
    void saveProgram(int32 AIndex) {
      int32 num = MParameterArray.size();
      SProgram* prog = MProgramArray[AIndex];
      for (int32 i=0; i<num; i++) {
        float v = MParameterValues[i];
        prog->value(i,v);
      }
    }

    //----------

    virtual
    void loadProgram(uint32 AIndex) {
      if (AIndex < MProgramArray.size()) {
        int32 num = MParameterArray.size();
        SProgram* prog = MProgramArray[AIndex];
        for (int32 i=0; i<num; i++) {
          float v = prog->value(i);
          MParameterValues[i] = v;
        }
        MProcessValuesDirty = true;
        #ifdef SKEI_PLUGIN_HAS_EDITOR
        MEditorValuesDirty = true;
        #endif
      }
    }

  //----------------------------------------
  // editor listener
  //----------------------------------------

  public:

    /*
      when we tweak a widget in the gui, we need to let the plugin
      know that it has changed, so that it can tell the host..
      and the process() thread.,.

      ---

      called from:
      - KEditor.do_update

      ---

      hmmm.. we change the FParamValues..
      next time we're in editor-idle, if this value is different from editor
      value, the widget is updated.. again..
      so, we also set the FEditorValue to avoid redrawing it..
      we already painted it during the widget tweaking (gui thread)
    */

    #ifdef SKEI_PLUGIN_HAS_EDITOR

    //virtual
    void updateParameterFromEditor(int32 AIndex, float AValue) {
      //STrace("...\n");
      MParameterValues[AIndex] = AValue;
      MEditorValues[AIndex] = AValue; // hack? so we don't redraw it (again) in idle..
      //MProcessValuesDirty = true;
      MProcessValueQueue.write(AIndex);
      notifyHostParameterChanged(AIndex,AValue);
    }

    #endif

  //----------------------------------------
  // editor
  //----------------------------------------

  public:

    /*
      called from:
        SPlugin.vst_dispatcher/effEditOpen

      update editor/controls
      (same as setDefault.. but just for 'forcing' distribution, not set paramvalues)
    */

    #ifdef SKEI_PLUGIN_HAS_EDITOR

    ////virtual
    //void initializeEditorValues(void) {
    //  for (int32 i=0; i<MParameterArray.size(); i++) {
    //    float v = MParameterValues[i];
    //    //MProcValues[i] = v + 1;         // force on_ParameterChange
    //    MEditorValues[i] = v + 1;   // force redraw
    //  }
    //}

    #endif

    //----------

    /*
      called during idle (called either from the plugin host (effEditIdle), or
      our own (timer) thread.. we check if any parameters have changed, and if
      so, let the editor know, so it can redraw the widget..
    */

    /*
      called from:
        SPlugin_Vst.vst_dispatcher/effEditIdle
    */

    #ifdef SKEI_PLUGIN_HAS_EDITOR

    //virtual
    void updateEditorInIdle(void) {
      //STrace("SPlugin_Editor.updateEditorInIdle\n");

      if (MEditorValuesDirty) {
        MEditorValueQueue.clear();
        MEditorValuesDirty = false;
        for (uint32 i=0; i<MParameterArray.size(); i++) {
          float v = MParameterValues[i];
          if (v != MEditorValues[i]) {
            MEditorValues[i] = v;
            SParameter* p = MParameterArray[i];
            MEditor->updateParameterFromPlugin(p,v);
          }
        }
      } // editorValuesDirty
      else {
        int32 i;
        while (MEditorValueQueue.read(&i)) {
          float v = MParameterValues[i];
          if (v != MEditorValues[i]) {
            MEditorValues[i] = v;
            SParameter* p = MParameterArray[i];
            MEditor->updateParameterFromPlugin(p,v);
          }
        } // read
      } // !editorValuesDirty

    }

    #endif

    #ifdef SKEI_PLUGIN_HAS_EDITOR
    //#ifdef SKEI_LINUX_IDLE_THREAD

    //virtual
    void idleFromEditor(void) {
      //STrace("on_idle\n");
      on_idleEditor(MEditor);
      updateEditorInIdle();
    }

    //#endif
    #endif

};

//----------------------------------------------------------------------
#endif

