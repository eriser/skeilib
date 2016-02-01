#ifndef skei_plugin_vst_included
#define skei_plugin_vst_included
//----------------------------------------------------------------------

/*
  SPlugin_Base
  SPlugin_Parameters
  SPlugin_Programs
  SPlugin_Editor
> SPlugin_Implementation
  SPlugin
*/

//----------

/*
  compiler options:
  -fPIC
  -rdynamic
  -Wl,-Bsymbolic

  linker options:
  -fPIC
  -Wl,-Bsymbolic
*/

//#include "skei_plugin_editor.h"
#include "skei_plugin_base.h"

#ifdef SKEI_PLUGIN_HAS_EDITOR
#include "skei_editor.h"
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

struct SVstEvents {
  VstInt32  numEvents;
  VstIntPtr reserved;
  VstEvent* events[SKEI_PLUGIN_MAX_MIDISEND];
};

//----------

VstIntPtr vst_dispatcher_callback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt);
void      vst_process_callback(AEffect* effect, float** inputs, float** outputs, VstInt32 sampleframes);
void      vst_processdouble_callback(AEffect* effect, double** inputs, double** outputs, VstInt32 sampleframes);
void      vst_setparameter_callback(AEffect* effect, VstInt32 index, float value);
float     vst_getparameter_callback(AEffect* effect, VstInt32 index);

//----------

class SPlugin_Vst;
typedef SPlugin_Vst SPlugin_Implementation;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class SPlugin_Vst
//: public SPlugin_Editor
: public SPlugin_Base
{

  friend VstIntPtr  vst_dispatcher_callback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt);
  friend void       vst_process_callback(AEffect* effect, float** inputs, float** outputs, VstInt32 sampleframes);
  friend void       vst_processdouble_callback(AEffect* effect, double** inputs, double** outputs, VstInt32 sampleframes);
  friend void       vst_setparameter_callback(AEffect* effect, VstInt32 index, float value);
  friend float      vst_getparameter_callback(AEffect* effect, VstInt32 index);

  private: // vst

    AEffect             MEffect;
    audioMasterCallback MMaster;
    SVstEvents          MVstEvents;
    VstMidiEvent        MMidiEvents[SKEI_PLUGIN_MAX_MIDISEND]; //VstEvent;
    ERect               MVstRect;


  protected: // host

    uint32              MHostCanDo;
    uint32              MHostVersion;
    uint32              MHostVstVer;
    uint32              MHostId;

  public:

    SPlugin_Vst()
    //: SPlugin_Editor() {
    : SPlugin_Base() {
      /* vst */
      SMemset(&MEffect,0,sizeof(AEffect));
      MMaster = SKEI_NULL;
      MVstEvents.numEvents = 0;
      MVstEvents.reserved  = 0;
      SMemset(&MMidiEvents,0,sizeof(MMidiEvents));
      for (uint32 i=0; i<SKEI_PLUGIN_MAX_MIDISEND; i++) MVstEvents.events[i] = (VstEvent*)&MMidiEvents[i];
      /* host */
      MHostCanDo    = shc_none;
      MHostVersion  = 0;
      MHostVstVer   = 0;
      MHostId       = shi_unknown;
    }

    //----------

    virtual ~SPlugin_Vst() {
    }

  //----------------------------------------
  //
  //----------------------------------------

  private:

    //virtual
    void midiFlush(void) {
      if (MVstEvents.numEvents > 0) {
        host_processEvents((VstEvents*)&MVstEvents);
        MVstEvents.numEvents = 0;
      }
    }

  //----------------------------------------
  // SPlugin_Base
  //----------------------------------------

  public:

    /*
      called from:
      - code_main.inc (build_win32.lpr)

      ---

      http://www.kvraudio.com/forum/viewtopic.php?p=5650970#p5650970
      There are cases where the host may not know the tempo and cases where it is
      impossible to communicate fully, such as in the constructor.
      Certain functions and their return values are only reliable to call from
      resume() or process().
      In general you may not use the host callback in any way from inside the
      constructor.
      Most data associated with the audio thread should only be reliable to access
      from inside the audio thread.
      note: you can't use the FMaster (call any audioMaster function) until
      you return from this function..

      ---

      can we call updateHostInfo from here?
      "this plugin needs: ..."
    */

    //virtual
    void* main(void* APointer=SKEI_NULL) {
      setDefaultParameterValues();
      //FHost := KHost.create(FMaster);
      MMaster = (audioMasterCallback)APointer;
      uint32 flags = effFlagsCanReplacing;
      //flags += effFlagsCanDoubleReplacing;
      //flags += effFlagsNoSoundInStop;
      //flags += effFlagsProgramChunks;
      #ifdef SKEI_PLUGIN_IS_SYNTH
      flags += effFlagsIsSynth;
      #endif
      #ifdef SKEI_PLUGIN_HAS_EDITOR
      flags += effFlagsHasEditor;
      #endif
      SMemset(&MEffect,0,sizeof(AEffect));
      MEffect.magic                   = kEffectMagic; //MFourChar('VstP');
      MEffect.uniqueID                = MInfo.uniqueId;
      MEffect.version                 = MInfo.version;
      MEffect.flags                   = flags;
      MEffect.numParams               = MParameterArray.size();
      MEffect.numPrograms             = MProgramArray.size();
      MEffect.numInputs               = MInfo.numInputs;
      MEffect.numOutputs              = MInfo.numOutputs;
      MEffect.initialDelay            = MInfo.latency;
      MEffect.dispatcher              = &vst_dispatcher_callback;
      MEffect.setParameter            = &vst_setparameter_callback;
      MEffect.getParameter            = &vst_getparameter_callback;
      MEffect.processReplacing        = &vst_process_callback;
      MEffect.processDoubleReplacing  = &vst_processdouble_callback;
      //MEffect.process               = SKEI_NULL; // (deprecated)
      MEffect.object                  = this;
      MEffect.user                    = SKEI_NULL;// self? &AEffect?
      return &MEffect;
    }

  //------------------------------
  // plugin implementation
  //------------------------------

  public:

    /*
      kVstTransportChanged     = 1;         // indicates that play, cycle or record state has changed
      kVstTransportPlaying     = 1 shl 1;   // set if Host sequencer is currently playing
      kVstTransportCycleActive = 1 shl 2;   // set if Host sequencer is in cycle mode
      kVstTransportRecording   = 1 shl 3;   // set if Host sequencer is in record mode
      kVstAutomationWriting    = 1 shl 6;   // set if automation write mode active (record parameter changes)
      kVstAutomationReading    = 1 shl 7;   // set if automation read mode active (play parameter changes)

      ---

      todo: check and double-check return values and flags!
      - calls on_transportChange
    */

    /*
    //virtual
    void updateSync(void) {
      // todo: check if we receive valid answer?
      MPlayState = getTime(&MSampleRate,&MSamplePos,&MTempo,&MBeatPos) & 0xff;
      if (MPlayState != MPrevState) on_transportChange(MPlayState);
      MPrevState = MPlayState;
    }
    */

    //virtual
    void updateHostTime(void) {
      //int32 result = 0;
      //todo: check hostCanDo (but some hosts return 0)
      if (MMaster) {
        uint32 flags = kVstPpqPosValid + kVstTempoValid + kVstCyclePosValid;
        VstTimeInfo* ti = host_getTime(flags);
        if (ti) {
          //todo: check flags?
          MState.playState    = ti->flags & 0xff;
          MState.samplePos    = ti->samplePos;
          MState.sampleRate   = ti->sampleRate;
          MState.beatPos      = ti->ppqPos;
          MState.tempo        = ti->tempo;
          MState.loopStart    = ti->cycleStartPos;
          MState.loopEnd      = ti->cycleEndPos;
          if (MState.playState != MState.prevState) on_transportChange(MState.playState);
          MState.prevState = MState.playState;
        }
      }
      //return result;
    }

    //----------

    //virtual
    void sendMidiToHost(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      int32 num = MVstEvents.numEvents;
      VstMidiEvent* event = &MMidiEvents[num];
      event->type         = kVstMidiType;
      event->deltaFrames  = AOffset;
      event->midiData[0]  = AMsg1;
      event->midiData[1]  = AMsg2;
      event->midiData[2]  = AMsg3;
      event->midiData[3]  = 0;
      event->byteSize     = sizeof(VstMidiEvent) - 2 * sizeof(int32);
      event->flags        = 0; // kVstMidiEventIsRealtime;
      event->noteLength   = 0;
      event->noteOffset   = 0;
      event->detune       = 0;
      num++;
      MVstEvents.numEvents = num;
    }

    //----------

    /*
      ext:        214
      reaper:     4255
      live:       10495
      fl studio:  4311
      bitwig:     6303
      carla:      14559
    */

    //TODO: iterate through table, with cando-string and flag

    //virtual
    void updateHostInfo(void) {
      MHostCanDo = shc_none;
      if (host_canDo("sendVstEvents"))                  MHostCanDo += shc_sendVstEvents;
      if (host_canDo("sendVstMidiEvent"))               MHostCanDo += shc_sendVstMidiEvent;
      if (host_canDo("sendVstTimeInfo"))                MHostCanDo += shc_sendVstTimeInfo;
      if (host_canDo("receiveVstEvents"))               MHostCanDo += shc_receiveVstEvents;
      if (host_canDo("receiveVstMidiEvent"))            MHostCanDo += shc_receiveVstMidiEvent;
      if (host_canDo("reportConnectionChanges"))        MHostCanDo += shc_reportConnectionChanges;
      if (host_canDo("acceptIOChanges"))                MHostCanDo += shc_acceptIOChanges;
      if (host_canDo("sizeWindow"))                     MHostCanDo += shc_sizeWindow;
      if (host_canDo("offline"))                        MHostCanDo += shc_offline;
      if (host_canDo("openFileSelector"))               MHostCanDo += shc_openFileSelector;
      if (host_canDo("closeFileSelector"))              MHostCanDo += shc_closeFileSelector;
      if (host_canDo("startStopProcess"))               MHostCanDo += shc_startStopProcess;
      if (host_canDo("shellCategory"))                  MHostCanDo += shc_shellCategory;
      if (host_canDo("sendVstMidiEventFlagIsRealtime")) MHostCanDo += shc_sendVstMidiEventFlagIsRealtime;
      MHostId = shi_unknown;
      char buffer[256];
      host_getProductString(buffer);
      #ifdef SKEI_DEBUG_VST
      DTrace("  ProductString: '%s'\n",buffer);
      #endif

      //TODO: iterate through table, with hostname-string and id

      if (SStrcmp("REAPER",buffer) == 0)              MHostId = shi_reaper;
      if (SStrcmp("energyXT",buffer) == 0)            MHostId = shi_energyxt;
      if (SStrcmp("Live",buffer) == 0)                MHostId = shi_live;
      if (SStrcmp("Fruity Wrapper",buffer) == 0)      MHostId = shi_fruity;
      if (SStrcmp("Qtractor",buffer) == 0)            MHostId = shi_qtractor;
      if (SStrcmp("Carla",buffer) == 0)               MHostId = shi_carla;      // 'Carla_Discovery' during scanning..
      if (SStrcmp("Bitwig Studio",buffer) == 0)       MHostId = shi_bitwig;
      if (SStrcmp("Multi Host 4 Linux",buffer) == 0)  MHostId = shi_jost;

      MHostVersion = host_getVendorVersion();
      MHostVstVer = host_version();

      #ifdef SKEI_DEBUG_VST
      DTrace("  MHostId: %i ('%s')\n",MHostId,buffer);
      DTrace("  MHostVersion: %i\n",MHostVersion);
      DTrace("  MHostVstVersion: %i\n",MHostVstVer);
      DTrace("  MHostCanDo: %i\n",MHostCanDo);
      #endif

      //TODO: writeout from table (indexed by flag)

      #ifdef SKEI_DEBUG_VST
      if ( MHostCanDo & shc_sendVstEvents )                   { DTrace("    'sendVstEvents'\n"); }
      if ( MHostCanDo & shc_sendVstMidiEvent )                { DTrace("    'sendVstMidiEvent'\n"); }
      if ( MHostCanDo & shc_sendVstTimeInfo )                 { DTrace("    'sendVstTimeInfo'\n"); }
      if ( MHostCanDo & shc_receiveVstEvents )                { DTrace("    'receiveVstEvents'\n"); }
      if ( MHostCanDo & shc_receiveVstMidiEvent )             { DTrace("    'receiveVstMidiEvent'\n"); }
      if ( MHostCanDo & shc_reportConnectionChanges )         { DTrace("    'reportConnectionChanges'\n"); }
      if ( MHostCanDo & shc_acceptIOChanges )                 { DTrace("    'acceptIOChanges'\n"); }
      if ( MHostCanDo & shc_sizeWindow )                      { DTrace("    'sizeWindow'\n"); }
      if ( MHostCanDo & shc_offline )                         { DTrace("    'offline'\n"); }
      if ( MHostCanDo & shc_openFileSelector )                { DTrace("    'openFileSelector'\n"); }
      if ( MHostCanDo & shc_closeFileSelector )               { DTrace("    'closeFileSelector'\n"); }
      if ( MHostCanDo & shc_startStopProcess )                { DTrace("    'startStopProcess'\n"); }
      if ( MHostCanDo & shc_shellCategory )                   { DTrace("    'shellCategory'\n"); }
      if ( MHostCanDo & shc_sendVstMidiEventFlagIsRealtime )  { DTrace("    'sendVstMidiEventFlagIsRealtime'\n"); }
      #endif
    }

    //----------

    //virtual
    void notifyHostParameterChanged(int32 AIndex, float AValue) {
      //STrace("notifyHostParameterChanged\n");
      host_beginEdit(AIndex);
      host_automate(AIndex,AValue);
      host_endEdit(AIndex);
    }

    //----------

    #ifdef SKEI_PLUGIN_HAS_EDITOR
    virtual bool resizeWindow(int32 AWidth, int32 AHeight) {
      //{$ifdef SKEI_GUI}
//      MEditor->setSize(AWidth,AHeight);
      bool res = host_sizeWindow(AWidth,AHeight);
      MEditorRect.w = AWidth;
      MEditorRect.h = AHeight;
      return res;
      //{$else}
      //result:= false;
      //{$endif}
    }
    #endif

    #ifdef SKEI_PLUGIN_HAS_EDITOR
    virtual void resizeWindowFromEditor(int32 AWidth, int32 AHeight) {
      resizeWindow(AWidth,AHeight);
    }
    #endif


  //----------------------------------------
  // AEffect -> callback -> this
  //----------------------------------------

  private:

    float vst_getparameter(VstInt32 AIndex) {
      return getParameterValue(AIndex);
    }

    //----------

    /*
      value will be read/distributed in process() and idleEditor()
    */

    void vst_setparameter(VstInt32 AIndex, float AValue) {
      setParameterValue(AIndex,AValue);
    }

    //----------

    /*
      TODO: multi-channel
    */

    void vst_process(float** AInputs, float** AOutputs, VstInt32 ASampleFrames) {
      updateParametersInProcess();
      #ifdef SKEI_PLUGIN_AUTOSYNC
      updateHostTime();
      #endif
      if (ASampleFrames > 0) {
        if (ASampleFrames > MState.blockSize) ASampleFrames = MState.blockSize;
        on_processBlock(AInputs,AOutputs,ASampleFrames);
        #ifdef SKEI_PLUGIN_PER_SAMPLE
        float* ins[2];
        float* outs[2];
        ins[0] = AInputs[0];
        ins[1] = AInputs[1];
        outs[0] = AOutputs[0];
        outs[1] = AOutputs[1];
        uint32 len = ASampleFrames;
        while (len > 0) {
          on_processSample(ins,outs);
          ins[0]++;
          ins[1]++;
          outs[0]++;
          outs[1]++;
          len--;
        }
        #endif
        on_postProcess();
      } // not 0/nil
      #ifdef SKEI_PLUGIN_SEND_MIDI
      midiFlush();
      #endif
    }

    //----------

    void vst_processdouble(double** AInputs, double** AOutputs, VstInt32 ASampleFrames) {
      // TODO
    }

  //----------------------------------------
  // vst dispatcher
  //----------------------------------------

  public:

    VstIntPtr vst_dispatcher(VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt) {
      //DTrace("dispatcher...\n");
      char* p;
      switch (opcode) {

        //----------

        /*
          called when plug-in is initialized
          initialise this plugin instance
        */

        case effOpen: // 00
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effOpen\n");
          #endif
          MState.isOpen = true;
          updateHostInfo();
          on_stateChange(sps_open);
          break;

        //----------

        /*
          called when plug-in will be released
          exit this plugin instance, release all memory and other resources
        */

        case effClose: // 01
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effClose\n");
          #endif
          MState.isOpen = false;
          on_stateChange(sps_close);
          //return 1;
          break;

        //----------

        /*
          host has changed the current program number
          host must call this inside a plugBeginSetProgram/plugEndSetProgram sequence..
          value: program number
        */

        case effSetProgram: // 02
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effSetProgram(%i)\n",value);
          #endif
          //MProgram = VstInt32(value);
          //on_programChange(MProgram);
          if ((VstInt32)value != MCurrentProgram) {
            on_preProgram(MCurrentProgram);
            saveProgram(MCurrentProgram);
            MCurrentProgram = (VstInt32)value;
            loadProgram(MCurrentProgram);
            on_programChange(MCurrentProgram);
            distributeParameterValues();
          }
          break;

        //----------

        /*
          host requests the current program number
          return: current program number

          reaper calls this repeatedly, 4-5 times per second
          (if editor open?)

          EnergyXt calls this before/after each effEditIdle,
          (even if we don't have an editor)
        */

        case effGetProgram: // 03
          #ifdef SKEI_DEBUG_VST
          //DTrace("vst dispatcher: effGetProgram() >> %i\n",MCurrentProgram); // too many!
          #endif
          return MCurrentProgram;
          break;

        //----------

        /*
          host has changed the current program name
          ptr: pointer to new program name (max 24 char + 0)

          stuff the name field of the current program with name.
          Limited to kVstMaxProgNameLen.
        */

        case effSetProgramName: // 04
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effSetProgramName(%s)\n",(char*)ptr);
          #endif
          if (MProgramArray.size() > 0) {
            SProgram* pr = MProgramArray[MCurrentProgram];
            pr->name((const char*)ptr);
            return 1;
          }
          break;

        //----------

        /*
          host requests the current program name
          ptr: returns string-pointer to program name (max 24 char + 0)

          stuff name with the name of the current program.
          Limited to kVstMaxProgNameLen.
        */

        case effGetProgramName: // 05
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetProgramName() >> ");
          #endif
          if (MProgramArray.size() > 0) {
            SProgram* pr = MProgramArray[MCurrentProgram];
            SStrcpy((char*)ptr,pr->name());
            #ifdef SKEI_DEBUG_VST
            DTrace("'%s'\n",pr->name());
            #endif
          }
          else {
            SStrcpy((char*)ptr,"default");
            #ifdef SKEI_DEBUG_VST
            DTrace("'default'\n");
            #endif
          }
          return 1;
          break;

        //----------

        /*
          host requests parameter label
          index: parameter index
          ptr: returns string-pointer to parameter label (max 8 char + 0),
               examples: sec, dB, type

          Stuff label with the units in which parameter index is displayed
          (i.e. "sec", "dB", "type", etc...).
          Limited to kVstMaxParamStrLen.
        */

        case effGetParamLabel: { // 06
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetParamLabel(%i) >> ",index);
          #endif
          SParameter* pa = MParameterArray[index];
          SStrcpy((char*)ptr,pa->label());
          #ifdef SKEI_DEBUG_VST
          DTrace("'%s'\n",pa->label());
          #endif
          return 1;
          break;
        }

        //----------

        /*
          stuff text with a string representation of the value of parameter index.
          ("0.5", "-3", "PLATE", etc...)
          Limited to kVstMaxParamStrLen.
        */

        case effGetParamDisplay: { // 07
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetParamDisplay(%i) >> ",index);
          #endif
          SParameter* pa = MParameterArray[index];
          float v = getParameterValue(index);
          SStrcpy((char*)ptr,pa->displayText(v));
          #ifdef SKEI_DEBUG_VST
          DTrace("'%s'\n",pa->displayText(v));
          #endif
          break;
        }

        //----------

        /*
          stuff text with the name of parameter index.
          ("Time", "Gain", "RoomType", etc...)
          Limited to kVstMaxParamStrLen.
        */

        case effGetParamName: { // 08
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetParamName(%i) >> ",index);
          #endif
          SParameter* pa = MParameterArray[index];
          SStrcpy((char*)ptr,pa->name());
          #ifdef SKEI_DEBUG_VST
          DTrace("'%s'\n",pa->name());
          #endif
          break;
        }

        //----------

        /*
          called if (flags and (effFlagsHasClip or effFlagsHasVu))
          return: 'VU' value; > 1.0 means clipped
        */

        //case effGetVu:  // 09 // deprecated
        //  DTrace("vst dispatcher: effGetVu");
        //  break;

        //----------

        /*
          called when the sample rate changes
          host sets SampleRate
          opt: float value in Hz, for example 44100.0Hz
          (always in a suspend state).
        */

        case effSetSampleRate: // 10
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effSetSampleRate: %f\n",opt);
          #endif
          MState.sampleRate = opt;
          on_stateChange(sps_sampleRate);
          break;

        //----------

        /*
          called when the maximun block size changes (always in a suspend state).
          note that the sampleFrames in process calls could be smaller than
          this block size, but NOT bigger.
        */

        case effSetBlockSize: // 11
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effSetBlockSize: %i\n",value);
          #endif
          MState.blockSize = VstInt32(value);
          on_stateChange(sps_blockSize);
          break;

        //----------

        /*
          the host has switched the 'power on' button. This only switches audio
          processing on/off.
          value: 0 = OFF, the plugin must call Resume() which in turn must flush
                     any delay buffers etc. here because otherwise pending data
                     would sound again when the effect is switched on next time.
                 1 = ON, the plug must call Suspend()

          ---

          This method is called when the effect is turned off by the user.
          ex: The buffer gets flushed here because otherwise pending data would
              sound again when the effect is switched on next time.

          ---

          called from non-rt thread..

          I believe resume() is called from a non-real-time thread, so you can
          allocate memory and stuff. startProcessing() is called from a real-time
          thread, so it's supposed to be "fast" in each plug-in. Thus, you can do
          things like taking initial timer measurements, spin up hardware buffers
          if needed, etc. startProcessing() gives you a chance to do the "last half"
          of your process() functionality, if you're interfacing with something that
          needs double-buffering, for example.
          -
          Ah, right, now that I checked it, the documentation says startProcess()/
          stopProcess() are supposed to be called in RT thread, and resume()/
          suspend() are not in the list. Ok fine, that makes sense.
          -
          flush event list(s) on resume?

          ---

          suspend: called when plug-in is switched to off
          resume:  called when plug-in is switched to on

        */

        case effMainsChanged: // 12
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effMainsChanged: %i\n",value);
          #endif
          if (value == 0) { // suspend
            MState.isProcessing = false;
            //clearEvents;
            on_stateChange(sps_suspend);
          }
          else { // resume
            //clearEvents;
            //if FFirstResume then
            //begin
            //  setDefaultParameters;
            //  FFirstResume := False;
            //  //updateHostInfo;
            //end
            //else
            //  //checkParametersInResume;
            //  //resetParameters;
            //begin
            //end;
            MState.isProcessing = true;
            //{$ifndef SPH_NORESETQUEUEINRESUME}
            //clearEvents;
            //{$endif}
            //// updateParametersInResume ??
            on_stateChange(sps_resume);
            //this.wantEvents(1); //deprecated as of vst2.4, but keep it --> backward compatibility!!!
          }
          break;

        //----------

        /*
          ptr: pointer to rect*(top, left, bottom, right)
          return: 1 = successful
          called repeatedly by reaper (4-5 times per second)
        */

        // ERect has 16 bit integers.. does it matter?

        case effEditGetRect: // 13
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effEditGetRect() >> ");
          #endif
          #ifdef SKEI_PLUGIN_HAS_EDITOR
          //ERect r;
          //r.left     = MEditorRect.x;
          //r.top      = MEditorRect.y;
          //r.right    = MEditorRect.x2(); // +1
          //r.bottom   = MEditorRect.y2(); // +1
          MVstRect.left     = 0;
          MVstRect.top      = 0;
          MVstRect.right    = MEditorRect.w; // +1
          MVstRect.bottom   = MEditorRect.h; // +1
          #ifdef SKEI_DEBUG_VST
          DTrace("[%i,%i,%i,%i]\n",MVstRect.left,MVstRect.top,MVstRect.right,MVstRect.bottom);
          #endif
          *(ERect**)ptr = &MVstRect;
          return 1;
          #endif // SKEI_PLUGIN_HAS_EDITOR
          break;

        //----------

        /*
          ptr: contains system dependant Window handle/pointer
          return: 1 = successful

          ---

          ext2/linux:  display ptr (32-bit?)
        */

        case effEditOpen: // 14
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effEditOpen(%0x)\n",ptr);
          #endif
          #ifdef SKEI_PLUGIN_HAS_EDITOR
          if (!MEditorIsOpen) {
            MEditor = (SEditor*)on_openEditor(ptr);
            if (MEditor) {
              //
              if ((MEditorStateBuffer) && (MEditorStateSize > 0)) {
                MEditor->on_restoreState(MEditorStateBuffer,MEditorStateSize);
              }
              //
              distributeParameterValues(); // this is called when setting program too...
              MEditor->on_align();
              MEditor->paintBuffer(MEditorRect);
              MEditor->open();
              MEditorIsOpen = true;
              return 1;
            } // editor
          } // !editor is open
          #endif
          break;

        //----------

        case effEditClose: // 15
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effEditClose()\n");
          #endif
          #ifdef SKEI_PLUGIN_HAS_EDITOR
          if (MEditor)  {
            if (MEditorIsOpen) {
              //
              if ((MEditorStateBuffer) && (MEditorStateSize > 0)) {
                SFree(MEditorStateBuffer);
                MEditorStateBuffer = SKEI_NULL;
                MEditorStateSize = 0;
              }
              MEditorStateSize = MEditor->on_getStateSize();
              if (MEditorStateSize > 0) {
                MEditorStateBuffer = SMalloc(MEditorStateSize);
                MEditor->on_saveState(MEditorStateBuffer,MEditorStateSize);
              }
              //
              MEditorRect = MEditor->rect();  // save editor size
              MEditor->close();               // stops event thread
              on_closeEditor(MEditor);        // deletes window
              MEditorIsOpen = false;
              MEditor = SKEI_NULL;
              return 1;
            } // editor is open
            //return 1;
          } // editor
          #endif
          break;

        //----------

        //case effEditDraw:  // 16 // deprecated
        //  DTrace("vst dispatcher: effEditDraw");
        //  break;

        //----------

        //case effEditMouse:  // 17 // deprecated
        //  DTrace("vst dispatcher: effEditMouse");
        //  break;

        //----------

        //case effEditKey:  // 18 // deprecated
        //  DTrace("vst dispatcher: effEditKey");
        //  break;

        //----------

        /*
          no arguments. Be gentle!
          (it would be better if this call could be avoided)
          don't push stuff or changes to editor,
          notify editor that we want to change, and do it in the editor

          ---

          EnergyXt calls this even if we don't have our own editor!
          FL studio --"--
          reaper, also in 'pause' state (when processing is not being called)
        */

        case effEditIdle: // 19
          #ifdef SKEI_DEBUG_VST
          //DTrace("vst dispatcher: effEditIdle()\n");
          #endif
          #ifndef SKEI_LINUX_EVENT_THREAD_SLEEP
          #ifdef SKEI_PLUGIN_HAS_EDITOR
          if (MEditorIsOpen) {
            if (MEditor) {
              on_idleEditor(MEditor);
              updateEditorInIdle();
              return 1;
            }
          }
          #endif // SKEI_PLUGIN_HAS_EDITOR
          #endif // SKEI_LINUX_EVENT_THREAD_SLEEP
          break;

        //----------

        //case effEditTop:  // 20 // deprecated
        //  DTrace("vst dispatcher: effEditTop");
        //  break;

        //----------

        //case effEditSleep:  // 21 // deprecated
        //  DTrace("vst dispatcher: effEditSleep");
        //  break;

        //----------

        case DECLARE_VST_DEPRECATED (effIdentify): // 22
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effIdentify() >> %0x\n",CCONST('N','v','E','f'));
          #endif
          return CCONST('N','v','E','f');
          break;

        //----------

        /*
          host requests pointer to a Bank/Program chunk, plugin is responsible for
          memory allocation/de-allocation of chunk
          index: 0 = isBank, 1 = isProgram
          ptr: pointer to chunk memory-block
          return: byteSize

          ---

          VstInt32 AudioEffect::getChunk (void** data, bool isPreset)
          data - should point to the newly allocated memory block containg state data. You can savely release it in next suspend/resume call.
          isPreset - true when saving a single program, false for all programs
          note: If your plug-in is configured to use chunks (see AudioEffect::programsAreChunks),
           the Host will ask for a block of memory describing the current plug-in state for saving.
          To restore the state at a later stage, the same data is passed back to AudioEffect::setChunk.
          Alternatively, when not using chunk, the Host will simply save all parameter values.

          ---

          host stores plug-in state. Returns the size in bytes of the chunk
          (plug-in allocates the data array)
          trace("axFormatVst.dispatcher :: effGetChunk");
          v = getChunk((void** )ptr, index ? true : false);

          ---

          SONAR is sending the setChunks on separated cores. (dual-core machine here)
          Or on separated threads.

          ---

          VstInt32 AudioEffect::getChunk (void** data, bool isPreset)
          data - should point to the newly allocated memory block containg state data. You can savely release it in next suspend/resume call.
          isPreset - true when saving a single program, false for all programs
          note: If your plug-in is configured to use chunks (see AudioEffect::programsAreChunks),
          the Host will ask for a block of memory describing the current plug-in state for saving.
          To restore the state at a later stage, the same data is passed back to AudioEffect::setChunk.
          Alternatively, when not using chunk, the Host will simply save all parameter values.

          ---

          host stores plug-in state. Returns the size in bytes of the chunk
          (plug-in allocates the data array)
          trace("axFormatVst.dispatcher :: effGetChunk");
          v = getChunk((void**)ptr, index ? true : false);
        */

        case effGetChunk: // 23
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetChunk()\n");
          #endif
          break;

        //----------

        /*
          plug-in receives a Bank/Program chunk, host is responsible for memory
          allocation/de-allocation of chunk
          index: 0 = isBank, 1 = isProgram
          value: byteSize
          ptr: pointer to chunk memory-block

          ---

          VstInt32 AudioEffect::setChunk (void* data, VstInt32 byteSize, bool isPreset)
          data - pointer to state data (owned by Host)
          byteSize - size of state data
          isPreset - true when restoring a single program, false for all programs

          ---

          host restores plug-in state
          trace("axFormatVst.dispatcher :: effSetChunk");
          v = setChunk(ptr, (VstInt32)value, index ? true : false);

          ---

          VstInt32 AudioEffect::setChunk (void* data, VstInt32 byteSize, bool isPreset)
          data - pointer to state data (owned by Host)
          byteSize - size of state data
          isPreset - true when restoring a single program, false for all programs

          ---

          host restores plug-in state
          trace("axFormatVst.dispatcher :: effSetChunk");
          v = setChunk(ptr, (VstInt32)value, index ? true : false);
        */

        case effSetChunk: // 24
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effSetChunk()\n");
          #endif
          break;

        //---------- aeffectx.h (vst 2)

        /*
          ptr: pointer to VstEvents*, host usually call ProcessEvents just before
               calling ProcessReplacing
          return: VST2.4: host should ignore return value, <=VST2.3: 0 = 'wants no
                  more', else return=1

          ---

          assumptions:
            called just before process(), from the same thread,
            non-overlapping (non-concurrent?)

          ---

          Are you taking deltaFrames offsets into account? (Including the fact that
          they can be negative, indicating the event should have happened in the
          last sample buffer. As will necessarily be the case with any real-time
          MIDI input.)

          ---

          called repeatedly! by fl studio, even if nothing happens???
          energyxt calls this repeatedly (with num = 0), before each block?
        */

        case effProcessEvents: { // 25
          #ifdef SKEI_DEBUG_VST
          //DTrace("vst dispatcher: effProcessEvents()\n"); // called before each processblock (too much, like processblock)
          #endif
          #if defined(SKEI_PLUGIN_IS_SYNTH ) || defined(SKEI_PLUGIN_RECEIVE_MIDI)
          //kode_socket.write("has flag kpf_receiveMidi\n");
          VstEvents* ev = (VstEvents*)ptr;
          int num = ev->numEvents;
          for (int32 i=0; i<num; i++) {
            VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
            if (event->type==kVstMidiType) {
              //kode_socket.write("event type is kVstMidiType\n");
              on_midiEvent(event->deltaFrames, event->midiData[0], event->midiData[1], event->midiData[2]);
            } //=miditype
          } //numevents
          #endif
          // sort?
        }
        return 1;
        break;

        //----------

        /*
          hosts asks if parameter can be automated, this should be called each time
          before attempting to call SetParameter()
          index: parameter index
          return: 1 = parameter can be automated
        */

        case effCanBeAutomated: // 26
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effCanBeAutomated(%i) >> ",index);
          #endif
          if (MParameterArray[index]->flags() & spf_automate) {
            #ifdef SKEI_DEBUG_VST
            DTrace("1\n");
            #endif
            return 1;
          }
          return 0;
          break;

        //----------

        /*
          host must call this two-fold:
          1) host checks the capability of plugin if it can convert a string into a parameter float value
              index: parameter index
              ptr: NULL
              return: 1 = plug CAN convert string2Parameter,
                      0 = plug CANNOT convert string2Parameter
          2) host requests plug to set a parameter from the parameter textual representation into a parameter
             float value (range 0.0 to 1.0), if this conversion is successful then internally the plugin must
             call setParameter as part of this call
              index: parameter index
              ptr: string-pointer to textual representation of parameter
              return: 1 = conversion successful,
                      0 = fail
        */

        case effString2Parameter: // 27
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effString2Parameter()\n");
          #endif
          break;

        //----------

        //case effGetNumProgramCategories: // 28 // deprecated
        //  DTrace("vst dispatcher: eff");
        //  break;

        //----------

        /*
          host requests program name
          index: program index
          value: deprecated in VST2.4
          ptr: returns string-pointer to program name (max 24 char + 0)
          return: 1 = OK, 0 = fail

          ---

          energy xt 2 calls this before effClose ??? (investigate)
        */

        case effGetProgramNameIndexed: // 29
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetProgramNameIndexed(%i)\n",index);
          #endif
          if (MProgramArray.size() > 0) {
            SProgram* pr = MProgramArray[index];
            SStrcpy((char*)ptr,pr->name());
            return 1;
          } else {
            // temp
            SStrcpy((char*)ptr,"default");
            return 1;
          }
          break;

        //----------

        //case effCopyProgram: // 30 // deprecated
        //  DTrace("vst dispatcher: eff");
        //  break;

        //case effConnectInput: // 31 // deprecated
        //  DTrace("vst dispatcher: eff");
        //  break;

        //case effConnectOutput: // 32 // deprecated
        //  DTrace("vst dispatcher: eff");
        //  break;

        //----------

        /*
          struct VstPinProperties {
            char label[kVstMaxLabelLen];	          // pin name
            VstInt32 flags;					                // @see VstPinPropertiesFlags
            VstInt32 arrangementType;		            // @see VstSpeakerArrangementType
            char shortLabel[kVstMaxShortLabelLen];  // short name (recommended: 6 + delimiter)
            char future[48];				                // reserved for future use
          };

          enum VstPinPropertiesFlags
          {
          	kVstPinIsActive   = 1 << 0,   // pin is active, ignored by Host
          	kVstPinIsStereo   = 1 << 1,   // pin is first of a stereo pair
          	kVstPinUseSpeaker = 1 << 2    // #VstPinProperties::arrangementType is valid and can be used to get the wanted arrangement
          };

          enum VstSpeakerArrangementType
          {
          	kSpeakerArrUserDefined = -2,  // user defined
          	kSpeakerArrEmpty = -1,        // empty arrangement
          	kSpeakerArrMono  =  0,        // M
          	kSpeakerArrStereo,            // L R
          	kSpeakerArrStereoSurround,    // Ls Rs
          	...
          };
        */

        case effGetInputProperties: { // 33
            #ifdef SKEI_DEBUG_VST
            DTrace("vst dispatcher: effGetInputProperties: %i\n",index);
            #endif
            //static char _int2hex[17] = "0123456789abcdef";    // +'\0' = 17
            VstPinProperties* pin = (VstPinProperties*)ptr;
            char* pc = SStrcpy(pin->label,"input "); // returns ptr to end of string? ptr to the '\0', or after the 0?
            *pc++ = SKEI_HEX_TABLE[index&0x0f];
            *pc = 0;
            //int l = SStrlen(pin->label);
            //pin->label[l] = _int2hex[index&0x0f];
            //pin->label[l+1] = 0;//'\0';
            pin->flags = 1; // active
            if ( (index&1) == 0) pin->flags = pin->flags | 2; // first of stereo pair
            return 1;
          }
          break;

        //----------

        case effGetOutputProperties: { // 34
            #ifdef SKEI_DEBUG_VST
            DTrace("vst dispatcher: effGetOutputProperties: %i\n",index);
            #endif
            //static char _int2hex[17] = "0123456789abcdef";    // +'\0' = 17
            VstPinProperties* pin = (VstPinProperties*)ptr;
            char* pc = SStrcpy(pin->label,"output ");
            *pc++ = SKEI_HEX_TABLE[index&0x0f];
            *pc = 0;
            //int l = SStrlen(pin->label);
            //pin->label[l] = _int2hex[index&0x0f];
            //pin->label[l+1] = 0;//'\0';
            pin->flags = 1; // active
            if ( (index&1) == 0) pin->flags = pin->flags | 2; // first of stereo pair
            return 1;
          }
          break;

        //----------

        /*
          host requests plugin category
          return: category number:
             0 = kPlugCategUnknown        Unknown, category not implemented
             1 = kPlugCategEffect         Simple Effect,
             2 = kPlugCategSynth          VST Instrument (Synths, samplers,...)
             3 = kPlugCategAnalysis       Analysis. Scope, Tuner, ...
             4 = kPlugCategMastering      Mastering, Dynamics, ...
             5 = kPlugCategSpacializer    Spacializer (panners),
             6 = kPlugCategRoomFx         RoomFx (delays and reverbs),
             7 = kPlugSurroundFx          SurroundFx (dedicated surround processors),
             8 = kPlugCategRestoration    Restoration, denoiser
             9 = kPlugCategOfflineProcess OfflineProcess,
            10 = kPlugCategShell          Shell (is only a container of plugins), @see effShellGetNextPlugin
            11 = kPlugCategGenerator      Generator, TonGenerator, ..
        */

        case effGetPlugCategory: // 35
          #ifdef SKEI_DEBUG_VST
          //DTrace("vst dispatcher: effGetPlugCategory >> %s\n", hasFlag(kpf_isSynth) ? "kPlugCategSynth" : "kPlugCategEffect" );
          #ifdef SKEI_PLUGIN_IS_SYNTH
            DTrace("vst dispatcher: effGetPlugCategory >> kPlugCategSynth\n");
          #else
            DTrace("vst dispatcher: effGetPlugCategory >> kPlugCategEffect\n");
          #endif
          #endif
          //if (hasFlag(kpf_isSynth)) return kPlugCategSynth;
          //else return kPlugCategEffect;
          #ifdef SKEI_PLUGIN_IS_SYNTH
          return kPlugCategSynth;
          #else
          return kPlugCategEffect;
          #endif
          break;

        //----------

        //case effGetCurrentPosition: // 36 // deprecated
        //  DTrace("vst dispatcher: eff");
        //  break;

        //----------

        //case effGetDestinationBuffer: // 37 // deprecated
        //  DTrace("vst dispatcher: eff");
        //  break;

        //----------

        /*
          index: start flag
          value: count
          ptr: pointer to VstAudioFile array
          return: 1 = successful
        */

        case effOfflineNotify: // 38
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effOfflineNotify\n");
          #endif
          break;

        //----------

        /*
          value: count
          ptr: pointer to VstOfflineTask array
          return: 1 = successful
        */

        case effOfflinePrepare: // 39
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effOfflinePrepare\n");
          #endif
          break;

        //----------

        /*
          value: count
          ptr: pointer to VstOfflineTask array
          return: 1 = successful
        */

        case effOfflineRun: // 40
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effOfflineRun\n");
          #endif
          break;

        //----------

        /*
          ptr: pointer to VariableIO*
          return: 1 = successful
        */

        case effProcessVarIo: // 41
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effProcessVarIo\n");
          #endif
          break;

        //----------

        /*
          value: pointer to VstSpeakerArrangement* pluginInput
          ptr: pointer to VstSpeakerArrangement* pluginOutput
          return: 1 = successful
        */

        case effSetSpeakerArrangement: // 42
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effSetSpeakerArrangement\n");
          #endif
          break;

        //----------

        //case effSetBlockSizeAndSampleRate: // 43 // deprecated
        //  DTrace("vst dispatcher: eff");
        //  break;

        //----------

        /*
          the host has switched the 'softBypass' state
          value: onOff (0 = off, 1 = On)
          return: 1 = successful
          ---
          returns true if the plug-in supports SoftBypass, which allows the process
          to be called, even if the plug-in was bypassed. This is extremely useful
          if the plug-in should be able to maintain a processing state even when
          turned off, e.g. surround decoders/encoders. Moreover, this feature may be
          automated by the host.
        */

        case effSetBypass: // 44
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effSetBypass: %i\n",value);
          #endif
          if (value == 0) on_stateChange(sps_bypassOff);
          else on_stateChange(sps_bypass);
          return 1;
          break;

        //----------

        /*
          ptr: returns string-pointer to a string with effect name (max 32 bytes)
          return: 1 = successful
        */

        case effGetEffectName: // 45
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetEffectName >> '%s'\n",MInfo.name);
          #endif
          SStrcpy((char*)ptr,MInfo.name);
          #ifdef SKEI_32BIT
            SStrcat((char*)ptr,"_32");
          #endif
          #ifdef SKEI_64BIT
            SStrcat((char*)ptr,"_64");
          #endif
          #ifdef SKEI_DEBUG
            SStrcat((char*)ptr,"_debug");
          #endif
          return 1;
          break;

        //----------

        //case effGetErrorText: // 46 // deprecated
        //  DTrace("vst dispatcher: eff");
        //  break;

        //----------

        /*
          ptr: returns string-pointer to a string identifying the vendor (max 64 ch)
          return: 1 = successful
        */

        case effGetVendorString: // 47
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetVendorString >> '%s'\n",MInfo.author);
          #endif
          SStrcpy((char*)ptr, MInfo.author);
          return 1;
          break;

        //----------

        /*
          ptr: returns string-pointer to a string with product name (max 64 char)
          return: 1 = successful
        */

        case effGetProductString: // 48
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetProductString >> '%s'\n",MInfo.product);
          #endif
          SStrcpy((char*)ptr, MInfo.product);
          //v = 1;
          break;

        //----------

        /*
          return: vendor-specific version
        */

        case effGetVendorVersion: // 49
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetVendorVersion >> %i\n",MInfo.version);
          #endif
          return MInfo.version;
          break;

        //----------

        /*
          no definition, vendor specific handling
          The MouseWheel handling is a bit of an after thought, and has been
          implemented by Stenberg via the effVendorSpecific call as follows:
          index: 'stCA' / $73744341
          value: 'Whee' / $57686565
          ptr: 1.0 = wheel-up, -1.0 = wheel-down
          return: 1 = MouseWheel used

          ---

          case effVendorSpecific:
             if (index == effGetParamDisplay && ptr)
             {
               if (value>=0 && value<NUM_PARAMS)
               {
                 sprintf(ptr,"%f",opt);
                 return 0xbeef;
               }
             }
        */

        case effVendorSpecific: // 50
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effVendorSpecific\n");
          #endif
          #ifdef SKEI_REAPER
          // reaper: http://www.reaper.fm/sdk/vst/vst_ext.php
          /*
            Gets the formatted display of a particular value for a parameter,
            without setting the parameter. REAPER uses this when displaying the
            value of a VST's automated parameter on an envelope tooltip, for
            example.
          */
          if ((index==effGetParamDisplay) && ptr) {
            if ((value >= 0) && (value < MParameters.size())) {
              return 0xbeef;
            }
          }
          #endif // REAPER
          break;

        //----------

        /*
          host queries a plugin 'cando' capability
          ptr: string-pointer to cando string
          return: 1='cando', 0='don't know' (default), -1='No'
        */

        //namespace PlugCanDos
        //{
        //	const char* canDoSendVstEvents = "sendVstEvents"; ///< plug-in will send Vst events to Host
        //	const char* canDoSendVstMidiEvent = "sendVstMidiEvent"; ///< plug-in will send MIDI events to Host
        //	const char* canDoReceiveVstEvents = "receiveVstEvents"; ///< plug-in can receive MIDI events from Host
        //	const char* canDoReceiveVstMidiEvent = "receiveVstMidiEvent"; ///< plug-in can receive MIDI events from Host
        //	const char* canDoReceiveVstTimeInfo = "receiveVstTimeInfo"; ///< plug-in can receive Time info from Host
        //	const char* canDoOffline = "offline"; ///< plug-in supports offline functions (#offlineNotify, #offlinePrepare, #offlineRun)
        //	const char* canDoMidiProgramNames = "midiProgramNames"; ///< plug-in supports function #getMidiProgramName ()
        //	const char* canDoBypass = "bypass"; ///< plug-in supports function #setBypass ()
        //}

        /*
          Bitwig asks for:
            midiSingleNoteTuningChange
            midiKeyBasedInstrumentControl

          ---


          see: MIDI specifications for Multidimensional Polyphonic Expression (MPE)
          https://docs.google.com/document/d/1-26r0pVtVBrZHM6VGA05hpF-ij5xT6aaXY9BfDzyTx8/edit?pli=1#
        */



        case effCanDo: // 51
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effCanDo '%s' >> ",(char*)ptr);
          #endif
          /*char**/ p = (char*)ptr;
          #ifdef SKEI_PLUGIN_SEND_MIDI
          if (!SStrcmp(p,"sendVstEvents")) {
            #ifdef SKEI_DEBUG_VST
            DTrace("1\n");
            #endif
            return 1;
          }
          //if (!SStrcmp(p,"sendVstMidiEvents")) {
          if (!SStrcmp(p,"sendVstMidiEvent")) {
            #ifdef SKEI_DEBUG_VST
            DTrace("1\n");
            #endif
            return 1;
          }
          #endif
          #ifdef SKEI_PLUGIN_RECEIVE_MIDI
          if (!SStrcmp(p,"receiveVstEvents")) {
            #ifdef SKEI_DEBUG_VST
            DTrace("1\n");
            #endif
            return 1;
          }
          //if (!SStrcmp(p,"receiveVstMidiEvents")) {
          if (!SStrcmp(p,"receiveVstMidiEvent")) {
            #ifdef SKEI_DEBUG_VST
            DTrace("1\n");
            #endif
            return 1;
          }
          #endif
          // plug-in can receive Time info from Host
          if (!SStrcmp(p,"receiveVstTimeInfo")) {
            #ifdef SKEI_DEBUG_VST
            DTrace("1\n");
            #endif
            return 1;
          }
          // plug-in supports function setBypass()
          if (!SStrcmp(p,"bypass")) {
            #ifdef SKEI_DEBUG_VST
            DTrace("1\n");
            #endif
            return 1;
          }
          // plug-in supports offline functions (#offlineNotify, #offlinePrepare, #offlineRun)
          //if (h_Strcmp(ptr,"offline")) return 0;
          // plug-in supports function #getMidiProgramName ()
          //if (h_Strcmp(ptr,"midiProgramNames")) return 0;
          /*
            http://www.reaper.fm/sdk/vst/vst_ext.php
            A REAPER aware VST plug-in can respond to effCanDo "hasCockosExtensions",
            with 0xbeef0000 (returning this value), which will inform the host that
            it can call certain extended calls. A plug-in that responds to
            "hasCockosExtensions" may choose to implement any subset of the extended
            calls.
          */
          //#ifdef SKEI_REAPER
          //if (MFlags&kpf_reaper) {
          //  // Warning: range check error while evaluating constants
          //  //and (Pos(ptr,'hasCockosExtensions') <> 0) then result := {%H-}$beef0000;
          //  if (!SStrcmp(p,"hasCockosExtensions")) return 0xbeef0000;
          //}
          //#endif
          #ifdef SKEI_DEBUG_VST
          DTrace("0\n");
          #endif
          return 0;
          break;

        //----------

        /*
          returns tail size; 0 is default (return 1 for 'no tail')

          ---

          called a lot by Bitwig
        */

        case effGetTailSize: // 52
          #ifdef SKEI_DEBUG_VST
          //DTrace("vst dispatcher: effGetTailSize\n");
          #endif
          return 1;
          break;

        //----------

        /*
          host sends this idle call in response to plugin's 'audioMasterneedIdle'
          request
          return: 1 must be returned to keep idle calls being issued by host

          ---

          energyXt2 calls this
          reaper too, if editor is open?
          reaper calls this even if we don't have an editor..
        */


        //case /*effIdle*/ 53: // deprecated
        case DECLARE_VST_DEPRECATED (effIdle): // 53

          #ifdef SKEI_DEBUG_VST
          //DTrace("vst dispatcher: effIdle\n");
          #endif
          break;

        //----------

        //case effGetIcon: // 54 // deprecated
        //  DTrace("vst dispatcher: eff");
        //  break;

        //----------

        //case effSetViewPosition: // 55 // deprecated
        //  DTrace("vst dispatcher: eff");
        //  break;

        //----------

        /*
          of parameter <index>, VstParameterProperties* in <ptr>
          return=1 when successful
        */

        case effGetParameterProperties: // 56
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetParameterProperties\n");
          #endif
          break;

        //----------

        //case effKeysRequired: // 57 // deprecated
        //  DTrace("vst dispatcher: eff");
        //  break;

        //----------

        /*
          returns 2; older versions return 0
        */

        case effGetVstVersion: // 58
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetVstVersion >> 2\n");
          #endif
          return 2;
          break;

        //---------- VST_2_1_EXTENSIONS

        /*
          index: character
          value: virtual key-code
          opt: key-modifiers
          return: 0 = KeyDown not used, 1 = KeyDown used
        */

        case effEditKeyDown: // 59
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effEditKeyDown\n");
          #endif
          break;


        //----------

        /*
          index: character
          value: virtual key-code
          opt: key-modifiers
          return: 0 = not KeyUp used, 1 = KeyUp used
        */

        case effEditKeyUp: // 60
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effEditKeyUp\n");
          #endif
          break;

        //----------

        /*
          value: knobmode 0=circular, 1=circular relativ, 2=linear
          return: 1 = successful
        */

        case effSetEditKnobMode: // 61
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effSetEditKnobMode\n");
          #endif
          break;

        //----------

        /*
          ptr: passed points to MidiProgramName struct.
          ptr: struct will be filled with information for 'thisProgramIndex'.
          return: number of used programIndexes.
                  if 0 is returned, no MidiProgramNames supported.
        */

        case effGetMidiProgramName: // 62
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetMidiProgramName\n");
          #endif
          break;

        //----------

        /*
          ptr: passed points to MidiProgramName struct.
          ptr: struct will be filled with information for the current program.
          return: the programIndex of the current program.
        */

        case effGetCurrentMidiProgram: // 63
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetCurrentMidiProgram\n");
          #endif
          break;

        //----------

        /*
          ptr: passed points to MidiProgramCategory struct.
          ptr: struct will be filled with information for 'thisCategoryIndex'.
          return: number of used categoryIndexes.
                  if 0 is returned, no MidiProgramCategories supported.
        */

        case effGetMidiProgramCategory: // 64
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetMidiprogramCategory\n");
          #endif
          break;

        //----------

        /*
          return 1 if the MidiProgramNames or MidiKeyNames had changed on this
          channel, 0 otherwise. <ptr> ignored.
        */

        case effHasMidiProgramsChanged: // 65
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effHasMidiProgramsChanged\n");
          #endif
          break;

        //----------

        /*
          ptr: passed points to MidiKeyName struct.
          ptr: struct will be filled with information for 'thisProgramIndex' and 'thisKeyNumber'.
        */

        case effGetMidiKeyName: // 66
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetMidiKeyName\n");
          #endif
          break;

        //----------

        /*
          called before all setParameter calls when changing program..
          host calls this before a new program (effSetProgram) is loaded
        */

        case effBeginSetProgram: // 67
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effBeginSetProgram\n");
          #endif
          break;

        //----------

        /*
          host calls this after the new program (effSetProgram) has been loaded
        */

        case effEndSetProgram: // 68
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effEndSetProgram\n");
          #endif
          break;

        //---------- VST_2_3_EXTENSIONS

        /*
          value: pointer to VstSpeakerArrangement** pluginInput
          ptr: pointer to VstSpeakerArrangement** pluginOutput
        */

        case effGetSpeakerArrangement: // 69
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetSpeakerArrangement\n");
          #endif
          break;

        //----------

        /*
          This opcode is only called, if plugin is of type kPlugCategShell.
          <ptr> points to a char buffer of size 64, which is to be filled with the
          name of the plugin including the terminating zero.
          return: the next plugin's uniqueID.
        */

        case effShellGetNextPlugin: // 70
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effShellGetNextPlugin\n");
          #endif
          break;

        //----------

        /*
          host calls this before the start of process call and after e.g. resume()

          http://ygrabit.steinberg.de/~ygrabit/public_html/vstsdk/OnlineDoc/vstsdk2.3/html/plug/2.0/AudioEffectX.html#startProcess
          Called one time before the start of process call.
          example:
            resume () // the user activates the plugin (User Interface Thread)
            startProcess () // Audio Thread
            process () // or ProcessReplacing ()
            process ()
            process ()
            stopProcess () // something has stopped the engine...or delayed the call
            startProcess ()
            process ()
            process ()
            process ()
            stopProcess () // yet the user has really stopped the plugin (Off)
            suspend ()

          ---

          pre vst 2.3 hosts don't call this, do they?
          should we handle it in suspend/resume?

          ext2 2.5 doesn't call this
          http://www.kvraudio.com/forum/viewtopic.php?t=277899
        */

        case effStartProcess: // 71
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effStartProcess\n");
          #endif
          MState.isProcessing = true;
          on_stateChange(sps_start);
        break;

        //----------

        /*
          host calls this after the stop of process call and before e.g. suspend()
          - not called in energy xt2 linux
        */

        case effStopProcess: // 72
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effStopProcess\n");
          #endif
          MState.isProcessing = false;
          on_stateChange(sps_stop);
          break;

        //----------

        /*
          Called in offline (non RealTime) Process before process is called,
          indicates how many sample will be processed
        */

        case effSetTotalSampleToProcess: // 73
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effSetTotalSampleToProcess\n");
          #endif
          break;

        //----------

        /*
          PanLaw:
            Type (Linear, Equal Power,.. see enum PanLaw Type) in <value>,
            Gain in <opt>: for Linear :
            [1.0 => 0dB PanLaw], [~0.58 => -4.5dB], [0.5 => -6.02dB]
        */

        case effSetPanLaw: // 74
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effSetPanLaw\n");
          #endif
          break;

        //----------

        /*
          Called before a Bank is loaded, <ptr> points to VstPatchChunkInfo structure
          return: -1 if the Bank cannot be loaded, 1 if it can be loaded, 0 (for compatibility)

          ala beginSetProgram
          called by: ableton live
        */

        case effBeginLoadBank: // 75
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effBeginLoadBank\n");
          #endif
          break;

        //----------

        /*
          Called before a Program is loaded, <ptr> points to VstPatchChunkInfo structure
          return: -1 if the Program cannot be loaded, 1 if it can be loaded, 0 (for compatibility)
        */

        case effBeginLoadProgram: // 76
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effBeginLoadProgram\n");
          #endif
          break;

        //---------- VST_2_4_EXTENSIONS

        /*
          value: @see VstProcessPrecision
        */

        case effSetProcessPrecision: // 77
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effSetProcessPrecision\n");
          #endif
          break;

        //----------

        /*
          return: number of used MIDI input channels (1-15)
        */

        case effGetNumMidiInputChannels: // 78
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetNumMidiInputChannels\n");
          #endif
          break;

        //----------

        /*
          return: number of used MIDI output channels (1-15)
        */

        case effGetNumMidiOutputChannels: // 79
          #ifdef SKEI_DEBUG_VST
          DTrace("vst dispatcher: effGetNumMidiOutputChannels\n");
          #endif
          break;

        //----------

        //default:
        //  DTrace("unhandled vst dispatcher: " << opcode << " : " << vst_opcodes[opcode]);
        //  break;

      } // switch

      return 0;//result;
    }

  //----------------------------------------
  // host
  //----------------------------------------

  private:

    /*
      An important thing to notice is that if the user changes a parameter in
      your editor (which is out of the host's control because we are not using
      the default string based interface), you should call...
      setParameterAutomated(index,newValue);
      This ensures that the host is notified of the parameter change, which
      allows it to record these changes for automation.
    */

    void host_automate(VstInt32 AIndex, float AValue) {
      if (MMaster) {
        MMaster(&MEffect,audioMasterAutomate,AIndex,0,SKEI_NULL,AValue);
      }
    }

    //----------

    /*
      To be called before setParameterAutomated (on Mouse Down). This will be
      used by the Host for specific Automation Recording.
      ---
      called before setParameterAutomated(), to tell the host that it should
      start recording automation data for the parameter specified byindex
    */

    bool host_beginEdit(VstInt32 AIndex) {
      if (MMaster) {
        if ( (VstInt32)MMaster(&MEffect,audioMasterBeginEdit,AIndex,0,SKEI_NULL,0) == 1) return true;
      }
      return false;
    }

    //----------

    /*
      sendVstEvents                   Host supports send of Vst events to plug-in
      sendVstMidiEvent                Host supports send of MIDI events to plug-in
      sendVstTimeInfo                 Host supports send of VstTimeInfo to plug-in
      receiveVstEvents                Host can receive Vst events from plug-in
      receiveVstMidiEvent             Host can receive MIDI events from plug-in
      reportConnectionChanges         Host will indicates the plug-in when something change in plug-in's routing/connections with suspend/resume/setSpeakerArrangement
      acceptIOChanges                 Host supports ioChanged
      sizeWindow                      used by VSTGUI
      offline                         Host supports offline feature
      openFileSelector                Host supports function openFileSelector
      closeFileSelector               Host supports function closeFileSelector
      startStopProcess                Host supports functions startProcess and stopProcess
      shellCategory                   'shell' handling via uniqueID. If supported by the Host and the Plug-in has the category kPlugCategShell
      sendVstMidiEventFlagIsRealtime  Host supports flags for VstMidiEvent
    */

    bool host_canDo(const char* AText) {
      if (MMaster) {
        if ( (VstInt32)MMaster(&MEffect,audioMasterCanDo,0,0,(void*)AText,0) == 1) return true;
      }
      return false;
    }

    //----------

    VstInt32 host_currentId(void) {
      if (MMaster) {
        return (VstInt32)MMaster(&MEffect,audioMasterCurrentId,0,0,SKEI_NULL,0);
      }
      return 0;
    }

    //----------

    /*
      To be called after setParameterAutomated (on Mouse Up).
      ---
      called after setParameterAutomated(), and tells the host that this parameter
      is no longer subject to changes by the user.
    */

    bool host_endEdit(VstInt32 AIndex) {
      if (MMaster) {
        if ((VstInt32)MMaster(&MEffect,audioMasterEndEdit,AIndex,0,SKEI_NULL,0) == 1) return true;
      }
      return false;
    }

    //----------

    bool host_getProductString(const char* ABuffer) {
      if (MMaster) {
        if ((VstInt32)MMaster(&MEffect,audioMasterGetProductString,0,0,(void*)ABuffer,0) == 1) return true;
      }
      return false;
    }

    //----------

    float host_getSampleRate(void) {
      if (MMaster) {
        return MMaster(&MEffect,audioMasterGetSampleRate,0,0,SKEI_NULL,0);
      }
      return 0;
    }

    //----------

    VstTimeInfo* host_getTime(VstInt32 AFilter) {
      if (MMaster) {
        // Hint: Conversion between ordinals and pointers is not portable
        return (VstTimeInfo*)MMaster(&MEffect,audioMasterGetTime,0,AFilter,SKEI_NULL,0);
      }
      return SKEI_NULL;
    }

    //----------

    bool host_getVendorString(const char* ABuffer) {
      if (MMaster) {
        if ((VstInt32)MMaster(&MEffect,audioMasterGetVendorString,0,0,(void*)ABuffer,0) == 1) return true;
      }
      return  false;
    }

    //----------

    int32 host_getVendorVersion(void) {
      if (MMaster) return (VstInt32)MMaster(&MEffect,audioMasterGetVendorVersion,0,0,SKEI_NULL,0);
      return 0;
    }

    //----------

    void host_idle(void) {
      if (MMaster) MMaster(&MEffect,audioMasterIdle,0,0,SKEI_NULL,0);
    }

    //----------

    bool host_ioChanged(void) {
      if (MMaster) {
        if ((VstInt32)MMaster(&MEffect,audioMasterIOChanged,0,0,SKEI_NULL,0) != 0) return true;
      }
      return false;
    }

    //----------

    bool host_processEvents(VstEvents* events) {
      if (MMaster) {
        //if VstInt32( FMaster(@FAEffect,audioMasterProcessEvents,0,0,events,0) ) = 1 then result := true;
        if ((VstInt32)MMaster(&MEffect,audioMasterProcessEvents,0,0,events,0) == 1) return true;
      }
      return false;
    }

    //----------

    bool host_sizeWindow(int32 AWidth, int32 AHeight) {
      if (MMaster) {
        if ((VstInt32)MMaster(&MEffect,audioMasterSizeWindow,AWidth,AHeight,SKEI_NULL,0) == 1) return true;
      }
      return false;
    }

    //----------

    VstInt32 host_version(void) {
      VstInt32 result = 1;
      if (MMaster) result = (VstInt32)MMaster(&MEffect,audioMasterVersion,0,0,SKEI_NULL,0);
      if (result==0) result = 1; // old (!version)
      return result;
    }

  //----------

};

//----------------------------------------------------------------------
// vst callbacks
//----------------------------------------------------------------------

/*
  these are the callbacks given to the host via the aeffect structure,
  in main()
*/

// cdecl
VstIntPtr vst_dispatcher_callback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt) {
  //VstIntPtr result = 0;
  SPlugin_Vst* plugin = (SPlugin_Vst*)effect->object;
  if (plugin) {
    if (opcode==effClose) {
      //#ifdef SKEI_DEBUG_VST
      //DTrace("vst_dispatcher_callback: effClose\n");
      //#endif
      plugin->vst_dispatcher(opcode,index,value,ptr,opt);
      plugin->on_destroy();
      delete plugin;
      return 1; // exit(1);
    }
    else return plugin->vst_dispatcher(opcode,index,value,ptr,opt);
  }
  return 0;
}

//----------

// cdecl
void vst_process_callback(AEffect* effect, float** inputs, float** outputs, VstInt32 sampleframes) {
  SPlugin_Vst* plugin = (SPlugin_Vst*)effect->object;
  if (plugin) plugin->vst_process(inputs,outputs,sampleframes);
}

//----------

// cdecl
void vst_processdouble_callback(AEffect* effect, double** inputs, double** outputs, VstInt32 sampleframes) {
  SPlugin_Vst* plugin = (SPlugin_Vst*)effect->object;
  if (plugin) plugin->vst_processdouble(inputs,outputs,sampleframes);
}

//----------

// cdecl
void vst_setparameter_callback(AEffect* effect, VstInt32 index, float value) {
  #ifdef SKEI_DEBUG_VST
  DTrace("vst_setparameter_callback(%i,%f)\n",index,value);
  #endif
  SPlugin_Vst* plugin = (SPlugin_Vst*)effect->object;
  if (plugin) plugin->vst_setparameter(index,value);
}

//----------

// cdecl
float vst_getparameter_callback(AEffect* effect, VstInt32 index) {
  #ifdef SKEI_DEBUG_VST
  DTrace("vst_getparameter_callback(%i)\n",index);
  #endif
  SPlugin_Vst* plugin = (SPlugin_Vst*)effect->object;
  if (plugin) return plugin->vst_getparameter(index);
  return 0;
}

//----------------------------------------------------------------------
#endif
