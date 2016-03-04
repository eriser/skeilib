#ifndef ana_fft_included
#define ana_fft_included
//----------------------------------------------------------------------

/*
  bitwig: crash
  - sometimes crashes after a few seconds
  - rapidly moving mouse over editor often makes it crash

  carla:
  - works

  qtractor:
  - works

  ardour4:
    The program 'ardour-4.6.22' received an X Window System error.
    This probably reflects a bug in the program.
    The error was 'BadLength (poly request too large or internal Xlib length erro'.
      (Details: serial 921 error_code 16 request_code 77 minor_code 0)
      (Note to programmers: normally, X errors are reported asynchronously;
       that is, you will receive the error a while after causing it.
       To debug your program, run it with the --sync command line
       option to change this behavior. You can then get a meaningful
       backtrace from your debugger if you break on the gdk_x_error() function.)
    pure virtual method called
    terminate called without an active exception

    XRequest.77 = "X_ImageText16"

  ----------

  i think this might be fixed now..
  if i call XInitThread also for vst plugins, bitwig doesn't crash
*/

//----------------------------------------------------------------------

//#define MIN_DB   -96
//#define MAX_DB    0

// power of 2
#define NUM_BUFFERS    16
//#define FFT_SIZE       1024
//#define INV_FFT_SIZE   ( 1.0f / (float)FFT_SIZE )

#define MAX_FFT_SIZE   65536

/*
  double t0 = uclock_sec();
  ...
  double t1 = uclock_sec();
*/

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

#define SKEI_XLIB
//#define SKEI_XRENDER
//#define SKEI_XFT

//#define SKEI_DEBUG_MEM
#ifdef SKEI_VST
  //#define SKEI_DEBUG_VST
  #define SKEI_DEBUG_SOCKET
#endif

#define SKEI_PLUGIN_PER_SAMPLE
#define SKEI_PLUGIN_HAS_EDITOR

//----------------------------------------------------------------------

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include <math.h>
#include "skei.h"
#include "skei_plugin.h"
#include "skei_math_fast.h"
#include "skei_queue.h"

//#include "extern/pffft/pffft.h"
//#include "extern/pffft/pffft.c"

//----------

#include "skei_editor.h"
#include "skei_widget_slider.h"

#include "skei_lock.h"
#include "skei_fft.h"


//----------------------------------------------------------------------
// widget
//----------------------------------------------------------------------

class myWidget : public SWidget {

  private:
    float*  MFFTBuffer; // MFftSize * 2 (re,im)
    float   MFFTSize;
    float   MInvFftSize;
    float   MMinDb;
    float   MMaxDb;
    float   MMinHz;
    float   MMaxHz;

  public:

    myWidget(SRect ARect, float* ABuffer, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MFFTBuffer  = ABuffer;
      MFFTSize    = 1024.0f;
      MInvFftSize = 1.0f / MFFTSize;
      MMinDb      = -96.0f;
      MMaxDb      = 0.0f;
      MMinHz      = 20;
      MMaxHz      = 20000;
    }

    //----------

    virtual ~myWidget() {
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void setFftSize(float v) { MFFTSize = v; MInvFftSize = 1.0f / MFFTSize; }
    void setMinDb(float v) { MMinDb = v; }
    void setMaxDb(float v) { MMaxDb = v; }
    void setMinHz(float v) { MMinHz = v; }
    void setMaxHz(float v) { MMaxHz = v; }


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

      /*
        test:
        cheap re-entrancy lock..
      */

      //if (MIsPainting) {
      //  return;
      //}
      //else {
      //  MIsPaiting = true;

      APainter->setFillColor( SDarkGrey );
      APainter->fillRect( MRect );
      APainter->setDrawColor( SLightGrey );
      //APainter->fillRect( MRect );
      if ((MRect.w > 0) && (MRect.h>0)) {
        //float h2 = (float)MRect.h / 2.0f;
        int32 prevx = MRect.x;
        int32 prevy = MRect.y2() - 1; //MRect.y + h2;
        //float range_db = (float)(MAX_DB - MIN_DB);
        float range_db = (float)(MMaxDb - MMinDb);
        //SAssert( range_db > 0 );
        if (range_db > 0) {

          float db_scale = (float)MRect.h / range_db;
          float index = 0.0f; // 1?
          float index_add = (float)(MFFTSize * 0.5f) / (float)(MRect.w);

          // https://github.com/automatl/dsp-utility/blob/master/Scaling.h
          float octaveCount = log2(MMaxHz) - log2(MMinHz);                           // 10.1
          SAssert( octaveCount>0 );
          float pixelsPerOctave = (float)MFFTSize / octaveCount;                // 405.3
          SAssert( pixelsPerOctave>0 );
          float offset = round(log2(MMinHz / pixelsPerOctave) * pixelsPerOctave);  // -1759

          for (int32 x=1; x<MRect.w; x++) {

            // linear
            //int32 idx = (floor)(index);

            // logarithmic
            float value = pixelsPerOctave * pow(2.0f, ((index + offset) / pixelsPerOctave));
            int32 idx = (floor)(value);

            index += index_add;

            // draw
            SAssert( idx>=0 );
            SAssert( idx < MFFTSize );
            //if (idx<0) idx = 0;
            //if (idx>=MFFTSize) idx = MFFTSize-1;
            float re = MFFTBuffer[idx*2];
            float im = MFFTBuffer[idx*2+1];
            float re_im = re*re + im*im;
            //SAssert( im == 0 );

            // sqrt : If the argument is negative, a domain error occurs.
            SAssert( re_im >= 0 );
            //float magnitude = 2.0f * sqrt(re_im) * MInvFftSize;  //  normalized bin magnitude
            float magnitude = 2.0f * SSqrtF(re_im) * MInvFftSize;  //  normalized bin magnitude
            // log10 : If the argument is negative, a domain error occurs.
            SAssert( magnitude >= 0 );
            float amplitude = 20.0f * log10( magnitude );         // convert to dB
            //float amplitude = 20.0f * SLog10F( magnitude );         // convert to dB
            float y = amplitude * db_scale;                       // -32 * 6.49 = -207

            //int mh2 = (floor)(MRect.h / 2);
            //if (y < -mh2) y = -mh2;
            //if (y > mh2)  y = mh2;

            int32 xx = MRect.x + x;
            //int32 yy = MRect.y + MAX_DB - y;
            int32 yy = MRect.y + MMaxDb - y;

            APainter->drawLine( prevx,prevy, xx, yy );
            //APainter->drawPoint(xx,yy);

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

const uint32  val_fftsize[10] = { 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384 };
const char*   txt_fftsize[10] = { "32", "64", "128", "256", "512", "1024", "2048", "4096", "8192", "16384" };
const char*   txt_window[7]   = { "rect", "hann", "hamming", "blackman", "gaussian", "welch", "bartlett" };
const char*   txt_db_scale[2] = { "linear", "logarithmic" };
const char*   txt_hz_scale[2] = { "linear", "logarithmic" };

//----------------------------------------------------------------------

class myPlugin
: public SPlugin
{

  private:

    PFFFT_Setup*        MFFT;
    float*              MFFTBuffer;
    float*              MTempBuffer;
    float*              MBuffers;
    uint32              MRecPos;
    uint32              MRecOfs;
    uint32              MFilledBuf;
    SQueue<uint32,256>  MQueue;
    myWidget*           MWidget_FFT;

    //bool                MInsideIdle;

  private:

    float   _new_fft_size;

    float   _fft_size;
    uint32  _window;
    float   _attack;
    float   _decay;
    float   _db_min;
    float   _db_max;
    uint32  _db_scale;
    float   _hz_min;
    float   _hz_max;
    uint32  _hz_scale;

    SLock MLock;

  public:

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    //virtual
    void on_create(void) {
      MInfo.name        = "ana_fft";
      MInfo.author      = "skei";
      MInfo.product     = MInfo.name;
      MInfo.uniqueId    = SKEI_MAGIC + 0x0000;
      MInfo.numInputs   = 2;
      MInfo.numOutputs  = 2;
      MEditorRect       = SRect(780,300);

      //

      appendParameter( new SParamText(  "fft size",     7,      10, txt_fftsize ));
      appendParameter( new SParamText(  "window",       2,      7,  txt_window ));
      appendParameter( new SParameter(  "attack",       0.1 ));
      appendParameter( new SParameter(  "decay",        0.3 ));

      appendParameter( new SParamInt(   "db min db",   -64,    -128, 0 ));
      appendParameter( new SParamInt(   "db max db",    0,     -128, 0 ));
      appendParameter( new SParamText(  "db scale",     0,      2, txt_db_scale ));

      appendParameter( new SParamInt(   "hz min",       20,     1, 22050 ));
      appendParameter( new SParamInt(   "hz max",       20000,  1, 22050 ));
      appendParameter( new SParamText(  "hz scale",     0,      2, txt_hz_scale ));

      //


      //MInsideIdle = false;

          /*
          float low  = 20;
          float high = 22050;
          float octaveCount = log2(high) - log2(low);
          STrace("octaveCount %f\n",octaveCount);
          float pixelsPerOctave = (float)MFFTSize / octaveCount;
          STrace("pixelsPerOctave %f\n",pixelsPerOctave);
          float offset = round(log2(low / pixelsPerOctave) * pixelsPerOctave);
          STrace("offset %f\n",offset);
          float value = pixelsPerOctave * pow(2.0f, (( ((float)FFT_SIZE * 0.5f) + offset) / pixelsPerOctave));
          STrace("value %f\n",value);
          */

          /*
            [ana_fft.h:on_create:258] octaveCount 10,106563
            [ana_fft.h:on_create:260] pixelsPerOctave 405,281219
            [ana_fft.h:on_create:262] offset -1759,000000
            [ana_fft.h:on_create:264] value 664,379883
          */

      _fft_size = 1024;
      _window   = 0;
      _attack   = 0;
      _decay    = 0;
      _db_min   = -96;
      _db_max   = 0;
      _db_scale = 1;
      _hz_min   = 20;
      _hz_max   = 20000;
      _hz_scale = 1;

      _new_fft_size = _fft_size;

      //uint32 size = _fft_size * 2 * sizeof(float);
      uint32 size = MAX_FFT_SIZE * 2 * sizeof(float);

      MFFT        = pffft_new_setup(_fft_size,PFFFT_COMPLEX);
      MFFTBuffer  = (float*)pffft_aligned_malloc(size);
      MTempBuffer = (float*)pffft_aligned_malloc(size);
      MBuffers    = (float*)pffft_aligned_malloc(size*NUM_BUFFERS);
      MRecPos     = 0;
      MRecOfs     = 0;
      MFilledBuf  = 0;
      SMemset(MFFTBuffer,0,size);
      SMemset(MTempBuffer,0,size);
      SMemset(MBuffers,0,size*NUM_BUFFERS);

    }

    //----------

    //virtual
    void on_destroy(void) {
      pffft_aligned_free(MFFTBuffer);
      pffft_aligned_free(MTempBuffer);
      pffft_aligned_free(MBuffers);
      pffft_destroy_setup(MFFT);
    }

    //------------------------------------------------------------
    // events
    //------------------------------------------------------------

    //virtual
    void on_stateChange(uint32 AState) {
      switch(AState) {
        case sps_resume:
          MRecPos = 0;
          MRecOfs = 0;
          MFilledBuf = 0;
          break;
      }
    }

    //----------

    virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      switch(AIndex) {
        case 0: // fftsize
          _new_fft_size = val_fftsize[(uint32)AValue];
          break;
        case 1:
          _window = AValue;
          break;
        case 2:
          _attack = AValue;
          break;
        case 3:
          _decay = AValue;
          break;
        case 4:
          _db_min = AValue;
          if ((MEditor) && (MEditorIsOpen)) MWidget_FFT->setMinDb(_db_min);
          break; // min db
        case 5:
          _db_max = AValue;
          if ((MEditor) && (MEditorIsOpen)) MWidget_FFT->setMaxDb(_db_max);
          break; // max db
        case 6:
          _hz_min = AValue;
          if ((MEditor) && (MEditorIsOpen)) MWidget_FFT->setMinHz(_hz_min);
          break;
        case 7:
          _hz_max = AValue;
          if ((MEditor) && (MEditorIsOpen)) MWidget_FFT->setMaxHz(_hz_max);
          break;
      }
    }

    //------------------------------------------------------------
    // audio
    //------------------------------------------------------------

    //virtual
    //void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
    //}

    //----------

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {
      float spl0 = *AInputs[0];
      float spl1 = *AInputs[1];
      uint32 p2 = (MRecPos+MRecOfs) * 2;
      //SAssert( (p2 < (FFT_SIZE*2*NUM_BUFFERS)) );
      MBuffers[p2]    = (spl0 + spl1) * 0.5f;  // re
      MBuffers[p2+1]  = 0;   // im
      MRecPos++;
      if (MRecPos>=_fft_size) {
        MRecPos = 0;
        MQueue.write(MFilledBuf);
        MFilledBuf++;
        MFilledBuf &= (NUM_BUFFERS-1);
        MRecOfs = _fft_size * MFilledBuf;
      }
      *AOutputs[0] = spl0;
      *AOutputs[1] = spl1;
    }

    //------------------------------------------------------------
    // editor
    //------------------------------------------------------------

    #define SLIDER_WIDTH 150

    //virtual
    void* on_openEditor(void* AParent) {
      SEditor* editor = new SEditor(this,MEditorRect.w,MEditorRect.h,AParent);
      editor->fillColor( SLightGrey );
      editor->fillBackground(true);
      editor->margins(10);
      editor->padding(10);

      SWidget* left = editor->appendWidgetW( new SWidget(SRect(SLIDER_WIDTH), swa_fillLeft ));
      //top->margins(10,10);
      left->padding(0,2);

      left->appendWidget( new SWidget_Slider( SRect( 0,   0, SLIDER_WIDTH, 18), 0) );   // fft size
      left->appendWidget( new SWidget_Slider( SRect( 0,  20, SLIDER_WIDTH, 18), 0) );   // window
      left->appendWidget( new SWidget_Slider( SRect( 0,  40, SLIDER_WIDTH, 18), 0) );   // attack
      left->appendWidget( new SWidget_Slider( SRect( 0,  60, SLIDER_WIDTH, 18), 0) );   // decay

      left->appendWidget( new SWidget_Slider( SRect( 0, 100, SLIDER_WIDTH, 18), 0) );  // db min
      left->appendWidget( new SWidget_Slider( SRect( 0, 120, SLIDER_WIDTH, 18), 0) );  // db max
      left->appendWidget( new SWidget_Slider( SRect( 0, 140, SLIDER_WIDTH, 18), 0) );  // db scale

      left->appendWidget( new SWidget_Slider( SRect( 0, 180, SLIDER_WIDTH, 18), 0) );  // hz min
      left->appendWidget( new SWidget_Slider( SRect( 0, 200, SLIDER_WIDTH, 18), 0) );  // hz max
      left->appendWidget( new SWidget_Slider( SRect( 0, 220, SLIDER_WIDTH, 18), 0) );  // hz scale

      for (int32 i=0; i<10; i++) {
        editor->connect( left->subWidget(i), getParameter(i) );
      }

      MWidget_FFT = new myWidget( SRect(0), MFFTBuffer, swa_fillClient);
      editor->appendWidget(MWidget_FFT);

      // transfer some vars..

      MWidget_FFT->setMinDb(_db_min);
      MWidget_FFT->setMaxDb(_db_max);

      return editor;
    }

    #undef SLIDER_WIDTH


    //----------

    //virtual
    void on_closeEditor(void* AEditor) {
      SEditor* editor = (SEditor*)AEditor;
      delete editor;
    }

    //----------

    /*
      - what if we get another idleEditor call (reentrant) while
        we're still painting or fft'ing?
    */

    //virtual
    void on_idleEditor(void* AEditor) {

      //MLock.lock();

      //SAssert( MInsideIdle==false );
      //MInsideIdle = true;

      SEditor* editor = (SEditor*)AEditor;
      if ((editor) && (MEditorIsOpen)) {

        MWidget_FFT->setMinDb(_db_min);
        MWidget_FFT->setMaxDb(_db_max);
        MWidget_FFT->setMinHz(_hz_min);
        MWidget_FFT->setMaxHz(_hz_max);

        if (_new_fft_size != _fft_size) {
          _fft_size = _new_fft_size;
          pffft_destroy_setup(MFFT);
          MFFT = pffft_new_setup(_fft_size,PFFFT_COMPLEX);
          MWidget_FFT->setFftSize(_fft_size);

          MRecPos = 0; //while (MRecPos >= _fft_size) MRecPos -= _fft_size;

        }

        //MWidget_FFT->setSampleRate(MState.sampleRate);

        // read (and use) latest buffer # from queue
        uint32 index = -1;
        while ( MQueue.read(&index) ); //{}
        if ((index>=0) && (index<NUM_BUFFERS)) {

          // copy buffer
          int32 offset = (_fft_size*2) * index;
          float* b = MBuffers + offset;
          //STrace("new buffer: %i offset %i\n",index,offset);
          SMemcpy(MTempBuffer,b,(_fft_size*2*sizeof(float)));

          // window
          for (int32 i=0; i<_fft_size; i++) {
            //float pos = (float)i / (float)(_fft_size-1);
            float pos = (float)i / (float)_fft_size;
            //MTempBuffer[i*2] *= SWindowingHamming(pos);
            switch (_window) {
              case 0: MTempBuffer[i*2] *= SWindowingRect(pos);      break;
              case 1: MTempBuffer[i*2] *= SWindowingHann(pos);      break;
              case 2: MTempBuffer[i*2] *= SWindowingHamming(pos);   break;
              case 3: MTempBuffer[i*2] *= SWindowingBlackman(pos);  break;
              case 5: MTempBuffer[i*2] *= SWindowingGaussian(pos);  break;
              case 6: MTempBuffer[i*2] *= SWindowingWelch(pos);     break;
              case 4: MTempBuffer[i*2] *= SWindowingBartlett(pos);  break;
            }
            MTempBuffer[i*2+1] = 0;//*= SWindowingHann(pos); // skip imaginary.. always 0
          } //fft_size

          // fft
          pffft_transform_ordered(MFFT,MTempBuffer,MFFTBuffer,SKEI_NULL,PFFFT_FORWARD);

          // redraw
          editor->do_redraw( MWidget_FFT, MWidget_FFT->rect() );

        } // index > 0
        //editor->do_redraw( MWidget_FFT, MWidget_FFT->rect() );
      } // editor is open

      //MInsideIdle = false;

      //MLock.unlock();

    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif




