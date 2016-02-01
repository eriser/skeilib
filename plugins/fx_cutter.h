#ifndef fx_cutter_included
#define fx_cutter_included
//----------------------------------------------------------------------

#define SKEI_XLIB
#define SKEI_XRENDER
#define SKEI_XFT

#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_MEM_PRINT
//#define SKEI_DEBUG_SOCKET
//#define SKEI_DEBUG_VST

#ifdef SKEI_VST
  #define SKEI_DEBUG_SOCKET
#endif

#define SKEI_PLUGIN_PER_SAMPLE
#define SKEI_PLUGIN_HAS_EDITOR
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_SEND_MIDI
//#define SKEI_PLUGIN_RECEIVE_MIDI
#define SKEI_PLUGIN_AUTOSYNC

//----------------------------------------------------------------------

//#include <math.h>

#include "skei.h"
#include "skei_editor.h"
#include "skei_plugin.h"
#include "skei_random.h"
#include "skei_widget.h"
#include "skei_widget_panel.h"
#include "skei_widget_slider.h"
#include "skei_widget_waveform.h"

//

//

/*

  this is getting messy...

  ----------------------------------------------------------------------
  ----------------------------------------------------------------------

  slice
  --------
  beat
  ----------------
  buffer
  ----------------------------------------------------------------

  :.......:.......|XXXXXoo:ooooooo|ooooooo:.......|.......:.......

                  ------------------------
                  cut
                  ------
                  loop


  beat_size = samplerate * 60 / tempo
  buffer_size = beat_size * num_beats
  slice_size = beat_size / num_slices

  cut_size = random(max_cut)
  loop_size = cut / random(max_loop)

  each slice, if not looping:
  - random(prob) ? start looping, random cut_size, randomloop_size

  each loop, if looping
  - random speedup : double_play_speed
  - random slowdown : halve play_speed
  - random double : double loop_size
  - random halve : halve loop_size

*/


  /*
    worst case:
    samplerate = 192.000, let's say 200 for simpler calc
    tempo = 30
    8 beats

    beat_size = FSampleRate * 60 / FTempo;
              = 200k * 60 / 30 = 200k * 2 = 400k
    buffer_size = trunc(beat_size) * par_beats;
                = 400k * 8 = 3.2m
    stereo * 2 = 6.4m


  */

#define MAX_BUFFER_SIZE (1024*1024*8)

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class myPlugin
: public SPlugin {

  private:

    int32 par_beats;
    int32 par_slices;
    int32 par_maxcut;
    int32 par_maxloop;
    float par_cutprob;
    float par_speedup;
    float par_slowdown;
    float par_halve;
    float par_double;

  private:

    float buffer[MAX_BUFFER_SIZE*2];
    float beat_size;
    float slice_size;
    float buffer_size;
    float buffer_prev;
    float buffer_counter;
    bool  restart;
    bool  is_looping;
    float read_pos;
    float read_speed;
    float write_pos;
    //float slice_counter;
    int32 slice_counter;
    int32 slices_left;
    float cut_start;
    float cut_length;
    float cut_counter;
    float loop_start;
    float loop_length;
    float loop_counter;

  private:

    SWidget_Waveform* wdg_waveform;
    SEditor*          editor;

  private:

    void reset(void) {
      restart         = false;
      is_looping      = false;
      buffer_counter  = buffer_size;
      read_pos        = 0.0;
      read_speed      = 1.0;
      write_pos       = 0;
      slice_counter   = 0; //trunc(slice_size);
      // +1 because when slice_counter = 0 (in on_processSample),
      // we subtract one from slices_left, and do some
      // probabilities..
      slices_left     = par_beats * par_slices + 1;
      //xfade_active    = false;
      //xfade_pos       = 0;
      //xfade_speed     = 0;
      //xfade_val       = 0;
      //xfade_sub       = 0;
    }

  public:

    //virtual
    void on_create(void) {

    //var
    //  buf : array[0..255] of char;
      //KTrace(['KGetBaseName: "',KGetBaseName(buf),'"',KODE_CR]);
      //KTrace(['KGetBasePath: "',KGetBasePath(buf),'"',KODE_CR]);
      //KTrace(['hello world 64 bit',KODE_CR]);
      //KAssert(0=1,'blablabla');
      MInfo.name       = "fx_cutter";
      MInfo.author     = "skei.audio";
      MInfo.product    = MInfo.name;
      MInfo.version    = 0;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      //setFlag(spf_perSample );
      //KSetFlag(FFlags, kpf_autoUpdateSync );
      //KSetFlag(FFlags,kpf_hasEditor);
      MEditor = SKEI_NULL;
      MEditorRect = SRect(490,210);
      appendParameter( new SParamInt(  "beats",    4, 1,8 ));
      appendParameter( new SParamInt(  "slices",   2, 1,8 ));
      appendParameter( new SParamInt(  "max cut",  3, 1,8 ));
      appendParameter( new SParamInt(  "max loop", 4, 2,8 ));
      appendParameter( new SParamFloat("cut prob", 0.35 ));
      appendParameter( new SParamFloat("speedup",  0.25 ));
      appendParameter( new SParamFloat("slowdown", 0.25 ));
      appendParameter( new SParamFloat("halve",    0.25 ));
      appendParameter( new SParamFloat("double",   0.25 ));
      //appendParameter( new SParamFloat("xfade',    0.1 ));
      restart = true;
      //KBreakIf(restart);
    }

    //----------

    //virtual
    void on_transportChange(uint32 ATransport) {
      uint32 flags = spt_changed | spt_play;
      if ((ATransport & flags) == flags) {
        //KTrace('play'+KODE_CR);
        restart = true;
      }
    }

    //----------

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      switch (AIndex) {
        case 0: par_beats    = STrunc(AValue); /*DTrace("par_beats %i\n",par_beats);*/ break;
        case 1: par_slices   = STrunc(AValue); /*DTrace("par_slices %i\n",par_slices);*/ break;
        case 2: par_maxcut   = STrunc(AValue); break;
        case 3: par_maxloop  = STrunc(AValue); break;
        case 4: par_cutprob  = AValue; break;
        case 5: par_speedup  = AValue; break;
        case 6: par_slowdown = AValue; break;
        case 7: par_halve    = AValue; break;
        case 8: par_double   = AValue; break;
        //case 9: par_xfade    = AValue; break;
      }
    }

    //----------

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
      beat_size = MState.sampleRate * 60.0f / MState.tempo;
      slice_size = beat_size / par_slices;
      buffer_size = /*trunc*/(beat_size) * par_beats;
      //if buffer_size < buffer_prev then restart = true;
      if (abs( buffer_size - buffer_prev ) > 0.5) restart = true;
      buffer_prev = buffer_size;
      //while read_pos >= buffer_size do read_pos *= 0.5;
      //while write_pos >= buffer_size do write_pos = write_pos div 2;
    }

    //----------

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {
    //var
      //float out0,out1;
      //float xf0,xf1;
      int32 r,w,i;//,x;
      float spl0 = *AInputs[0];
      float spl1 = *AInputs[1];
      if ((MState.playState & spt_play) != 0) {
        /* restart */
        /* if play has been pressed, or the buffer size changed */
        if (restart) reset();
        /* cut */
        /*
          countdown cut (outer selection), and reset if finished
        */
        if (is_looping) {
          cut_counter -= 1;
          if (cut_counter <= 0) {
            is_looping = false;
            read_pos = write_pos;
            read_speed = 1;
          }
        }
        /* loop */
        /*
          loop the smalles bit, and check probabilities for each wraparound,
          possibly modifying loop size or speed
          also initialize xfade
        */
        if /*still*/ (is_looping) {
          loop_counter -= read_speed;
          if (loop_counter <= 0) {
            /*
              todo: some form of anti-click thing..
              crossfade between non-looping (a second 'fadeout' cursor?) and looping..
              we could have a second read_pos (fade_pos?), which just continues,
              and we crossfade between this and the 'new' position for a few
              samples?
              save relevant states into xfade_*, and initiate a cross-fade
            */
            //xfade_active = true;
            //xfade_pos = read_pos;
            //xfade_speed = read_speed;
            //xfade_val = 1;
            //xfade_sub = 1/XFADE_LENGTH;
            //
            if (SRandom() < par_speedup) read_speed *= 2;
            if (SRandom() < par_slowdown) read_speed *= 0.5;
            if (SRandom() < par_double) loop_length *= 2;
            if (SRandom() < par_halve) loop_length *= 0.5;
            read_speed = SClamp(read_speed,(1.0f/16.0f),16.0f);
            loop_length = SClamp(loop_length,1.0f,MAX_BUFFER_SIZE-1);
            //KTrace(['read_speed ',read_speed,' loop_length ',loop_length,KODE_CR]);
            read_pos = loop_start;
            loop_counter = loop_length;
          }
        }
        /* slice */
        /*
          countdown available slices
          check probabilities when a new slice starts
          and possibly start a new cut/loop
          (if we're not already looping)
        */
        slice_counter -= 1;
        if (slice_counter <= 0) {
          slices_left -= 1;
          slice_counter = STrunc(slice_size);
          if (!is_looping) {
            if (SRandom() < par_cutprob) {
              if (slices_left < par_maxcut) i = SRandomRangeInt(1,slices_left);
              else i = SRandomRangeInt(1,par_maxcut);
              cut_start     = write_pos;
              cut_length    = STrunc( i * slice_size );
              cut_counter   = cut_length;
              is_looping    = true;
              loop_start    = cut_start;
              loop_length   = cut_length / (float)(1 + SRandomRangeInt(1,par_maxloop-1) );
              loop_counter  = loop_length;
              read_pos      = write_pos;
              read_speed    = 1;
              //KTrace(['read_pos ',read_pos,' read_speed ',read_speed,KODE_CR]);
            }
          }
        }
        /* write */
        /*
          write to audio buffer
          increase write pos, wraparound
        */
        w = STrunc(write_pos);
        //if w < 0 then w += MAX_BUFFER_SIZE;
        //if w >= MAX_BUFFER_SIZE then w -= MAX_BUFFER_SIZE;
        w *= 2;
        buffer[w] = spl0;
        buffer[w+1] = spl1;
        write_pos += 1;
        if (write_pos < 0) write_pos += MAX_BUFFER_SIZE;
        if (write_pos >= MAX_BUFFER_SIZE) write_pos -= MAX_BUFFER_SIZE;
        /* read */
        /*
          reads from budio buffer 8read pos)
          increase read pos, wraparound
        */
        r = STrunc(read_pos);
        //if r < 0 then r += MAX_BUFFER_SIZE;
        //if r >= MAX_BUFFER_SIZE then r -= MAX_BUFFER_SIZE;
        r *= 2;
        spl0 = buffer[r];
        spl1 = buffer[r+1];
        read_pos += read_speed;
        if (read_pos < 0) read_pos += MAX_BUFFER_SIZE;
        if (read_pos >= MAX_BUFFER_SIZE) read_pos -= MAX_BUFFER_SIZE;
        /* anti-click */
        /*
          crossfade 16 samples when loop wraps around..
        */
        /*
        if xfade_active {
          x = trunc(xfade_pos);
          x *= 2;
          xfade_pos += xfade_speed;
          xfade_val -= xfade_sub;
          if xfade_val <= 0 {
            xfade_val = 0;
            xfade_active = false;
          }
          xf0 = buffer[x] * xfade_val;
          xf1 = buffer[x+1] * xfade_val;
          spl0 *= (1-xfade_val);
          spl1 *= (1-xfade_val);
          spl0 += xf0;
          spl1 += xf1;
        }
        */
        /* */
        buffer_counter -= 1;
        if (buffer_counter <= 0) restart = true;
      } // playstate
      /* */
      *AOutputs[0] = spl0;
      *AOutputs[1] = spl1;
    }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    virtual void* on_openEditor(void* AParent) {
      //KTrace(['myPlugin.on_openEditor',KODE_CR]);
      editor = new SEditor(this,MEditorRect.w,MEditorRect.h,AParent);
      SWidget* panel = editor->appendWidgetW( new SWidget_Panel( SRect(0), /*SColor(0.77),*/ swa_fillClient ));
      panel->appendWidget( new SWidget_Slider( SRect( 10,10,150,18), 0 ));
      panel->appendWidget( new SWidget_Slider( SRect( 10,30,150,18), 0 ));
      panel->appendWidget( new SWidget_Slider( SRect( 10,50,150,18), 0 ));
      panel->appendWidget( new SWidget_Slider( SRect( 10,70,150,18), 0 ));
      panel->appendWidget( new SWidget_Slider( SRect(170,10,150,18), 0 ));
      panel->appendWidget( new SWidget_Slider( SRect(330,10,150,18), 0 ));
      panel->appendWidget( new SWidget_Slider( SRect(330,30,150,18), 0 ));
      panel->appendWidget( new SWidget_Slider( SRect(330,50,150,18), 0 ));
      panel->appendWidget( new SWidget_Slider( SRect(330,70,150,18), 0 ));
      //panel->appendWidget( new SWidget_Slider( SRect(170,30,150,18), 0 ));
      for (int32 i=0; i<9; i++) editor->connect( panel->subWidget(i), MParameterArray[i] );

      /*

      wdg_waveform = (SWidget_Waveform*)panel->appendWidgetW( new SWidget_Waveform( SRect(10,100,470,100) ));
      wdg_waveform->drawBackground();
      wdg_waveform->setBackColor(SDarkGrey);
      wdg_waveform->setNumMarkers(2);
      wdg_waveform->setMarkerColor(0,SColor(1,0,0));      // write pos
      wdg_waveform->setMarkerColor(1,SColor(0,1,0));      // read pos
      wdg_waveform->setNumAreas(2);
      wdg_waveform->setAreaColor(0,SColor(0.0,0.5,0.0) ); // cut
      wdg_waveform->setAreaColor(1,SColor(0.5,0.5,0.0) ); // loop

      */

      return editor;
    }

    //----------

    //virtual
    void on_closeEditor(void* AEditor) {
      //KTrace(['myPlugin.on_closeEditor',KODE_CR]);
      //FEditor.hide;
      //FEditor.destroy;
      delete editor;
      //FEditor = nil;
    }

    //----------

    /*
      can we get a on_idleEditor call if we're still not finished
      with the previous call?
    */

    //virtual
    void on_idleEditor(void* AEditor) {

      if (MEditorIsOpen) {

        /*

        wdg_waveform->setBufferSize(buffer_size);
        wdg_waveform->setBuffer(buffer);
        //wdg_waveform->setAreaSize(0,0);
        //wdg_waveform->setAreaSize(1,0);
        wdg_waveform->setNumGrid( par_beats * par_slices );
        wdg_waveform->setMarkerPos(0,STrunc(write_pos));
        wdg_waveform->setMarkerPos(1,STrunc(read_pos));
        if (is_looping) {
          wdg_waveform->setAreaPosSize(0,STrunc(cut_start),STrunc(cut_length));
          wdg_waveform->setAreaPosSize(1,STrunc(loop_start),STrunc(loop_length));
        }
        else {
          wdg_waveform->setAreaSize(0,0);
          wdg_waveform->setAreaSize(1,0);
        }

        if (MEditor) MEditor->do_redraw(wdg_waveform, wdg_waveform->rect());

        */

      } // editorisopen

    }

};

#undef MAX_BUFFER_SIZE

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif
