#ifndef test_buffering_included
#define test_buffering_included
//----------------------------------------------------------------------

/*
  preparations/development of fft analyzer..
  we need three things:
  * fft transform
  * buffer, audio-process -> editor/gui
  * fft data widget
*/

//#define FS sizeof(float)

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

#define SKEI_PLUGIN_PER_SAMPLE
#define SKEI_PLUGIN_HAS_EDITOR
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_SEND_MIDI
//#define SKEI_PLUGIN_RECEIVE_MIDI
//#define SKEI_PLUGIN_AUTOSYNC

#ifdef SKEI_PLUGIN_HAS_EDITOR
  #define SKEI_XLIB
  #define SKEI_XRENDER
  #define SKEI_XFT
#endif

#define SKEI_DEBUG_MEM
#ifdef SKEI_VST
  //#define SKEI_DEBUG_VST
  #define SKEI_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_buffer_queue.h"
#include "skei_fft.h"
#include "skei_math_fast.h"
#include "skei_plugin.h"
#include "skei_widget_slider.h"
#include "skei_widget_waveform.h"

#include "skei_complex.h"

//----------------------------------------------------------------------
// fft widget
//----------------------------------------------------------------------

class SWidget_FFT : public SWidget {

  private:

    float*  MBuffer;     // MFftSize * 2 (re,im)
    float   MSize;
    float   MInvSize;
    float   MSampleRate;
    float   MMinDb;
    float   MMaxDb;
    uint32  MScaleDb;
    float   MMinHz;
    float   MMaxHz;
    uint32  MScaleHz;

  public:

    SWidget_FFT(SRect ARect, /*float* ABuffer,*/ uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MBuffer     = SKEI_NULL;//ABuffer;
      MSize       = 0.0f;
      MInvSize    = 1.0f;
      MSampleRate = 0.0f;
      MMinDb      = 0.0f;
      MMaxDb      = 0.0f;
      MScaleDb    = 0;
      MMinHz      = 0.0f;
      MMaxHz      = 0.0f;
      MScaleHz    = 1;
    }

    virtual ~SWidget_FFT() {
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void setBuffer(float* ABuffer) { MBuffer = ABuffer; }
    void setSize(float v) { MSize = v; MInvSize = 1.0f / MSize; }
    void setSampleRate(float ARate) { MSampleRate = ARate; }
    void setMinDb(float v) { MMinDb = v; }
    void setMaxDb(float v) { MMaxDb = v; }
    void setScaleDb(uint32 s) { MScaleDb = s; }
    void setMinHz(float v) { MMinHz = v; }
    void setMaxHz(float v) { MMaxHz = v; }
    void setScaleHz(uint32 s) { MScaleHz = s; }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //bin: k/2
    //freq (Hz): sampleRate*.5*(double)k/FFT_SIZE
    //mag: normBinMag
    //ampl (dB): amplitude

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {

      APainter->setFillColor( SDarkGrey );
      APainter->fillRect( MRect );
      APainter->setDrawColor( SLightGrey );

      if ((MRect.w > 0) && (MRect.h>0)) {
        //float h2 = (float)MRect.h / 2.0f;
        int32 prevx = MRect.x;
        int32 prevy = MRect.y2() - 1; //MRect.y + h2;
        //float range_db = (float)(MAX_DB - MIN_DB);
        float range_db = (float)(MMaxDb - MMinDb);
        //SAssert( range_db > 0 );
        if (range_db > 0) {

          float db_scale = (float)MRect.h / range_db;
          float pos = 0.0f; // 1?
          float pos_add = (float)(MSize * 0.5f) / (float)(MRect.w);

          // https://github.com/automatl/dsp-utility/blob/master/Scaling.h
          float octaveCount = log2(MMaxHz) - log2(MMinHz);                           // 10.1
          SAssert( octaveCount>0 );
          float pixelsPerOctave = (float)MSize / octaveCount;                // 405.3
          SAssert( pixelsPerOctave>0 );
          float offset = round(log2(MMinHz / pixelsPerOctave) * pixelsPerOctave);  // -1759

          for (int32 x=1; x<MRect.w; x++) {
            int32 idx = 0;
            //float value;
            switch(MScaleHz) {
              case 0:
                // linear
                idx = (floor)(pos);
                break;
              case 1:
                // logarithmic
                float value = pixelsPerOctave * pow(2.0f, ((pos + offset) / pixelsPerOctave));
                idx = (floor)(value);
                break;
            }
            pos += pos_add;
            SAssert( idx>=0 );
            SAssert( idx < MSize );
            //if (idx<0) idx = 0;
            //if (idx>=MFFTSize) idx = MFFTSize-1;
            float re = MBuffer[idx*2];
            float im = MBuffer[idx*2+1];
            float re_im = re*re + im*im;
            //SAssert( im == 0 );
            SAssert( re_im >= 0 );
            //float magnitude = 2.0f * sqrt(re_im) * MInvFftSize;   //  normalized bin magnitude

            float magnitude = 2.0f * SSqrtF(re_im) * MInvSize;      //  normalized bin magnitude
            //float magnitude = 2.0f * SSqrtF(re_im) * MSampleRate * MInvSize;      //  normalized bin magnitude

            SAssert( magnitude >= 0 );
            //float amplitude = 20.0f * log10( magnitude );         // convert to dB
            float amplitude = 20.0f * SLog10F( magnitude );         // convert to dB
            float y = amplitude * db_scale;                         // -32 * 6.49 = -207
            //int mh2 = (floor)(MRect.h / 2);
            //if (y < -mh2) y = -mh2;
            //if (y > mh2)  y = mh2;
            int32 xx = MRect.x + x;
            int32 yy = MRect.y + MMaxDb - y;
            //APainter->drawPoint(xx,yy);
            APainter->drawLine( prevx,prevy, xx, yy );
            prevx = xx;
            prevy = yy;
          } // for
        } // range_db > 0
      } //w/h > 0
      //  MIsPaiting = false;
      //} // MIsPainting
    }

};


//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

typedef SBufferQueue<float,32> myBufferQueue;

const uint32 int_fftsize[11]  = { 16,  32,  64,  128,  256,  512,  1024,  2048,  4096,  8192,  16384 };
const char*  txt_fftsize[11]  = {"16","32","64","128","256","512","1024","2048","4096","8192","16384"};

#define swt_rect      0
#define swt_hann      1
#define swt_hamming   2
#define swt_blackman  3
#define swt_bartlett  4
#define swt_gaussian  5
#define swt_welch     6

//----------

class myPlugin
: public SPlugin
{

  private:
    SFFT              MFFT;
    myBufferQueue     MBuffers;
    //SWidget_Waveform* wdg_waveform;
    SWidget_FFT*      wdg_fft;

    uint32  wavsize;
    uint32  fftsize;
    float   tempbuffer[1024*1024];

  private:

    /*
      AFunc   = 0..6 windowing function
      AInput  = value
      APos    = 0..1
    */

    float window_fft(float AInput, float APos, uint32 AFunc) {
      float w = 1.0f;
      switch (AFunc) {
        case 0: w = SWindowingRect(APos);      break;
        case 1: w = SWindowingHann(APos);      break;
        case 2: w = SWindowingHamming(APos);   break;
        case 3: w = SWindowingBlackman(APos);  break;
        case 4: w = SWindowingBartlett(APos);  break;
        case 5: w = SWindowingGaussian(APos);  break;
        case 6: w = SWindowingWelch(APos);     break;
      }
      //MTempBuffer[i*2+1] = 0;//*= SWindowingHann(pos); // skip imaginary.. always 0
      return AInput * w;
    }

  public:

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    //virtual
    void on_create(void) {
      MInfo.name       = "";
      MInfo.author     = "";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      appendParameter( new SParamText( "fft size", 6, 11, txt_fftsize) );
      #ifdef SKEI_PLUGIN_HAS_EDITOR
      MEditorRect = SRect(640,480);
      #endif
      wavsize = 1024;
      MBuffers.setSize(wavsize,16);
      fftsize = 1024;
      SMemset(tempbuffer,0,sizeof(tempbuffer));

    }

    //----------

    //virtual
    //void on_destroy(void) {
    //}

    //------------------------------------------------------------
    // events
    //------------------------------------------------------------

    //virtual
    //void on_stateChange(uint32 AState) {
    //  switch(AState) {
    //    case sps_open: break;
    //    case sps_close: break;
    //    case sps_sampleRate: break;
    //    case sps_blockSize: break;
    //    case sps_suspend: break;
    //    case sps_resume: break;
    //    case sps_start: break;
    //    case sps_stop: break;
    //    case sps_bypass: break;
    //    case sps_bypassOff: break;
    //  }
    //}

    //----------

    //virtual
    //void on_transportChange(uint32 ATransport) {
    //  if (ATransport & spt_changed) {}  // play, cycle or record state has changed
    //  if (ATransport & spt_play) {}     // Host sequencer is currently playing
    //  if (ATransport & spt_cycle) {}    // Host sequencer is in cycle mode
    //  if (ATransport & spt_record) {}   // Host sequencer is in record mode
    //  if (ATransport & spt_awrite) {}   // automation write mode active (record parameter changes)
    //  if (ATransport & spt_aread) {}    // automation read mode active (play parameter changes)
    //}

    //----------

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      int32 vi = floor(AValue);
      switch(AIndex) {
        case 0:
          fftsize = int_fftsize[ vi ];
          //wavsize = int_fftsize[ vi ];
          break;
      }
    }

    //----------

    //virtual
    //void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
    //}

    //----------

    //virtual
    //void on_programChange(int32 AIndex) {
    //}

    //----------

    //virtual
    //void on_preProgram(int32 AIndex) {
    //}

    //----------

    //virtual
    //void on_postProgram(void) {
    //}

    //------------------------------------------------------------
    // audio
    //------------------------------------------------------------

    //virtual
    //void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
    //  SMemcpy(AOutputs[0],AInputs[0],ANumSamples*sizeof(SSample));
    //  SMemcpy(AOutputs[1],AInputs[1],ANumSamples*sizeof(SSample));
    //}

    //----------

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {
      float spl0 = *AInputs[0];
      float spl1 = *AInputs[1];
      MBuffers.write( (spl0 + spl1) * 0.5f);
      *AOutputs[0] = spl0;
      *AOutputs[1] = spl1;
    }

    //----------

    //virtual
    //void on_postProcess(void) {
    //}

    //------------------------------------------------------------
    // editor
    //------------------------------------------------------------

    #ifdef SKEI_PLUGIN_HAS_EDITOR

    //virtual
    void* on_openEditor(void* AParent) {
      SEditor* editor = new SEditor(this,MEditorRect.w,MEditorRect.h,AParent);
      editor->fillColor( SLightGrey );
      editor->fillBackground(true);
      //wdg_waveform = (SWidget_Waveform*)editor->appendWidgetW( new SWidget_Waveform(SRect(0), swa_fillClient ));
      SWidget* left = editor->appendWidgetW( new SWidget(SRect(200), swa_fillLeft ));
      left->margins(2);
      left->padding(2);
      left->appendWidget( new SWidget_Slider( SRect(18), 0, swa_fillTop ) );
      wdg_fft = (SWidget_FFT*)editor->appendWidgetW( new SWidget_FFT(SRect(0), swa_fillClient ));

      for (uint32 i=0; i<left->numSubWidgets(); i++) {
        editor->connect( left->subWidget(i), parameter(i) );
      }
      return editor;
    }

    //----------

    //virtual
    void on_closeEditor(void* AEditor) {
      SEditor* editor = (SEditor*)AEditor;
      delete editor;
    }

    //----------

    //virtual
    void on_idleEditor(void* AEditor) {
      SEditor* editor = (SEditor*)AEditor;
      if ((editor) && (MEditorIsOpen)) {
        float* wavbuffer;
        if ( MBuffers.getBuffer(&wavbuffer) ) {

          // waveform

          //wdg_waveform->setBuffer(wavbuffer);
          //wdg_waveform->setBufferSize(wavsize);
          //wdg_waveform->setMono(true);
          //editor->do_redraw(wdg_waveform,wdg_waveform->rect());

          // fft

          for (uint32 i=0; i<wavsize; i++) {
            float n = wavbuffer[i];
            float t = (float)i / (float)wavsize;
            n = window_fft(n,t,swt_hamming);
            tempbuffer[i*2]   = window_fft(n,t,swt_hamming);
            tempbuffer[i*2+1] = 0;
          }

          MFFT.setSize(fftsize);
          MFFT.process(tempbuffer);

          //MFFT.scale( /*MState.sampleRate*/ 1 / (float)fftsize );

          wdg_fft->setBuffer( MFFT.buffer() );
          wdg_fft->setSize(fftsize);
          wdg_fft->setSampleRate(MState.sampleRate);

          wdg_fft->setMinHz(20);
          wdg_fft->setMaxHz(20000);
          wdg_fft->setScaleDb(1);

          wdg_fft->setMinDb(-96);
          wdg_fft->setMaxDb(6);
          wdg_fft->setScaleHz(1);

          editor->do_redraw(wdg_fft,wdg_fft->rect());

        }
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
