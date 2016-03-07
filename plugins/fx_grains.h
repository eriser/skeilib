#ifndef fx_grains_included
#define fx_grains_included
//----------------------------------------------------------------------

/*

*/

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//----- plugins -----

#define SKEI_PLUGIN_PER_SAMPLE
//#define SKEI_PLUGIN_HAS_EDITOR
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_SEND_MIDI
#define SKEI_PLUGIN_RECEIVE_MIDI
//#define SKEI_PLUGIN_AUTOSYNC


//----- format -----

#ifdef SKEI_PLUGIN_HAS_EDITOR
  #define SKEI_XLIB
  #define SKEI_XRENDER
  #define SKEI_XFT
#endif

//----- debug -----

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
#include "skei_random.h"

#ifdef SKEI_PLUGIN_HAS_EDITOR
#include "skei_editor.h"
#endif

#define SKEI_MAX_GRAINS 1024
#define SKEI_BUFFERSIZE (1024*1024)

struct SGrain {
  bool  on;
  float pos;
  float sta;
  float end;
  float dur;
  float spd;
  float ph;
  float pa;
  float dh;
  float da;
};

const char* freeze_txt[2] = { "off", "on" };

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin
{

  private:

    SGrain  MGrains[SKEI_MAX_GRAINS];
    float   MBuffer[SKEI_BUFFERSIZE];

    float   master;
    int32   numgrains;
    int32   buffersize;
    bool    freeze;
    float   graindist;
    float   grainsize;
    float   graindur;
    float   grainpitch;
    float   grainenv;
    float   startjit;
    float   pitchjit;
    float   sizejit;
    float   durjit;
    int32   index;
    float   countdown;

  public:

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    //virtual
    void on_create(void) {
      MInfo.name       = "fx_grains";
      MInfo.author     = "skei";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      //
      #ifdef SKEI_PLUGIN_HAS_EDITOR
      MEditorRect = SRect(640,480);
      #endif
      //
      master      = 0.0f;
      numgrains   = 0;
      buffersize  = 0;
      freeze      = false;
      graindist   = 0.0f;
      grainsize   = 0.0f;
      graindur    = 0.0f;
      grainpitch  = 0.0f;
      grainenv    = 0.0f;
      startjit    = 0.0f;
      pitchjit    = 0.0f;
      sizejit     = 0.0f;
      durjit      = 0.0f;
      //
      appendParameter( new SParamFloat( "master (db)",    -6,   -60, 6     ));
      appendParameter( new SParamInt(   "num grains",      10,   1,  100   ));
      appendParameter( new SParamFloat( "buf size (ms)",   1000, 1,  1000  ));
      appendParameter( new SParamText(  "freeze",          0,    2,  freeze_txt )); // text
      appendParameter( new SParamFloat( "grain dist (ms)", 20,   0,  100   ));
      appendParameter( new SParamFloat( "grain size (ms)", 30,   1,  100   ));
      appendParameter( new SParamFloat( "grain dur (ms)",  300,  1,  1000  ));
      appendParameter( new SParamFloat( "grain pitch",     1,    0,  10    ));
      appendParameter( new SParamFloat( "grain env",       0               ));
      appendParameter( new SParamFloat( "dist jitter",     0.2             ));
      appendParameter( new SParamFloat( "pitch jitter",    0.2             ));
      appendParameter( new SParamFloat( "size jitter",     0.2             ));
      appendParameter( new SParamFloat( "dur jitter",      0.2             ));
      //
      index = 0;
      countdown = 0;

    }

    //----------

    //virtual
    void on_destroy(void) {
    }

    //------------------------------------------------------------
    // events
    //------------------------------------------------------------

    //virtual
    void on_stateChange(uint32 AState) {
      switch(AState) {
        case sps_open: break;
        case sps_close: break;
        case sps_sampleRate: break;
        case sps_blockSize: break;
        case sps_suspend: break;
        case sps_resume: break;
        case sps_start: break;
        case sps_stop: break;
        case sps_bypass: break;
        case sps_bypassOff: break;
      }
    }

    //virtual
    void on_transportChange(uint32 ATransport) {
      if (ATransport & spt_changed) {}  // play, cycle or record state has changed
      if (ATransport & spt_play) {}     // Host sequencer is currently playing
      if (ATransport & spt_cycle) {}    // Host sequencer is in cycle mode
      if (ATransport & spt_record) {}   // Host sequencer is in record mode
      if (ATransport & spt_awrite) {}   // automation write mode active (record parameter changes)
      if (ATransport & spt_aread) {}    // automation read mode active (play parameter changes)
    }

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      switch(AIndex) {
        case 0:  master      = pow(2,(AValue/6)); break;
        case 1:  numgrains   = trunc( AValue ); break;
        case 2:  buffersize  = trunc( (AValue/1000) * MState.sampleRate ); break;
        case 3:  AValue > 0.5 ? freeze=true : freeze=false; break;
        case 4:  graindist   = (AValue/1000) * MState.sampleRate; break;
        case 5:  grainsize   = (AValue/1000) * MState.sampleRate; break;
        case 6:  graindur    = (AValue/1000) * MState.sampleRate; break;
        case 7:  grainpitch  = AValue; break;
        case 8:  grainenv    = AValue; break;
        case 9:  startjit    = AValue; break;
        case 10: pitchjit    = AValue; break;
        case 11: sizejit     = AValue; break;
        case 12: durjit      = AValue; break;
      }
    }

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
    }

    //virtual
    void on_programChange(int32 AIndex) {
    }

    //virtual
    void on_preProgram(int32 AIndex) {
    }

    //virtual
    void on_postProgram(void) {
    }

    //------------------------------------------------------------
    // audio
    //------------------------------------------------------------

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
      SMemcpy(AOutputs[0],AInputs[0],ANumSamples*sizeof(SSample));
      SMemcpy(AOutputs[1],AInputs[1],ANumSamples*sizeof(SSample));
    }

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {

      float spl0 = *AInputs[0];
      float spl1 = *AInputs[1];

      float in0 = spl0;
      float in1 = spl1;
      float out0 = 0;
      float out1 = 0;
      int32 newgrain = -1;

      //if (numgrains > 0) {
        for (int32 i=0; i<numgrains; i++) {
          if (MGrains[i].on) {
            MGrains[i].pos += MGrains[i].spd;
            if (MGrains[i].pos >= MGrains[i].end) MGrains[i].pos = MGrains[i].sta;
            if (MGrains[i].pos >= buffersize) MGrains[i].pos -= buffersize;
            MGrains[i].ph += (MGrains[i].pa*2);
            if (MGrains[i].ph >= 2) MGrains[i].ph -= 2;
            MGrains[i].dh += (MGrains[i].da*2);
            if (MGrains[i].dh >=2) MGrains[i].dh -= 2;
            MGrains[i].dur -= 1;
            if (MGrains[i].dur <= 0) MGrains[i].on = false;
            float gvol = MGrains[i].ph * (2-abs(MGrains[i].ph)); // abs-neg ?
            float dvol = MGrains[i].dh * (2-abs(MGrains[i].dh));
            int32 temp = trunc( MGrains[i].pos );
            temp *= 2;
            out0 += MBuffer[ temp ] * dvol * gvol;
            out1 += MBuffer[ temp+1 ] * dvol * gvol;
          }
          else newgrain = i;
        }
      //}

      if (countdown <= 0) {
        countdown = graindist;
        if (newgrain > 0) {
          float startrnd = 1 + (startjit * SRandomSigned());
          float pitchrnd = 1 + (pitchjit * SRandomSigned());
          float sizernd  = 1 + (sizejit  * SRandomSigned());
          float durrnd   = 1 + (durjit   * SRandomSigned());
          float siz = (grainsize*sizernd);
          float st = index * startrnd;
          if (st >= buffersize) st -= buffersize;
          if (st <  0) st += buffersize;
          float en = st + siz;
          if (en >= buffersize) en = buffersize; // clamp
          if (en <  0) en = 0;
          float du = graindur*durrnd;
          MGrains[newgrain].on   = true;
          MGrains[newgrain].pos  = st;
          MGrains[newgrain].sta  = st;
          MGrains[newgrain].end  = en;
          MGrains[newgrain].dur  = du;
          MGrains[newgrain].spd  = grainpitch * pitchrnd;
          MGrains[newgrain].ph   = 0;
          MGrains[newgrain].pa   = 1 / siz;
          MGrains[newgrain].dh   = 0;
          MGrains[newgrain].da   = 1 / du;
        }
      }
      countdown -= 1;

      if (!freeze) {
        MBuffer[index*2] = in0;
        MBuffer[index*2+1] = in1;
      }

      index += 1;
      if (index >= buffersize) index -= buffersize;

      spl0 = out0 * master;
      spl1 = out1 * master;

      *AOutputs[0] = spl0;
      *AOutputs[1] = spl1;
    }

    //virtual
    void on_postProcess(void) {
    }

    //------------------------------------------------------------
    // editor
    //------------------------------------------------------------

    #ifdef SKEI_PLUGIN_HAS_EDITOR

    //virtual
    void* on_openEditor(void* AParent) {
      SEditor* editor = new SEditor(this,MEditorRect.w,MEditorRect.h,AParent);
      editor->fillColor( SLightGrey );
      editor->fillBackground(true);
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

    #endif // SKEI_PLUGIN_HAS_EDITOR

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif
