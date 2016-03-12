#ifndef test_animate_included
#define test_animate_included
//----------------------------------------------------------------------

/*
  debug version prints a lot of stuff through a socket..
  enter 'nc -U -l -k /tmp/skei.sock' in another terminal window
*/

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_MEM_PRINT
#define SKEI_DEBUG_CRASHHANDLER

#ifdef SKEI_VST
  //#define SKEI_DEBUG_VST
  #define SKEI_DEBUG_SOCKET
#endif

#define SKEI_XLIB
#define SKEI_XRENDER
#define SKEI_XFT

#define SKEI_PLUGIN_HAS_EDITOR

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"
#include "skei_editor.h"
#include "skei_widget.h"

#include "skei_bitmap.h"
#include "skei_image.h"
#include "skei_random.h"
#include "skei_bresenham.h"

//----------------------------------------------------------------------
// widget
//----------------------------------------------------------------------

class myWidget : public SWidget {

  private:

    SWindow*  MWindow;
    SBitmap*  MBitmap;
    SImage*   MImage;

  public:

    myWidget(SWindow* AWindow,SRect ARect, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MWindow = AWindow;
      MBitmap = new SBitmap(1024,512);
      MImage = new SImage(MWindow,MBitmap,AWindow->depth());
    }

    virtual ~myWidget() {
      delete MImage;
      delete MBitmap;
    }

    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      MBitmap->clearBuffer();

      #define NUM 64

      int x[NUM];
      int y[NUM];

      for (int32 i=0; i<NUM; i++) {
        x[i] = SRandomRangeInt(0,MBitmap->width()-1 );
        y[i] = SRandomRangeInt(0,MBitmap->height()-1);
      }

      MBitmap->drawQuadSpline( NUM-4, x,y );
      //MBitmap->drawCubicSpline( NUM-4, x,y );

      for (int32 j=0; j<NUM-4; j++) {
        MBitmap->drawCircle(x[j],y[j],5);
      }

      #undef NUM

      /*
      MBitmap->drawLine(                    10,     10,     110,  40    );
      MBitmap->drawLineAA(                  10,     50,     110,  80    );
      MBitmap->drawLineWidth(               10,     90,     110,  120, 5 );
      MBitmap->drawEllipse(                 120+50, 10+15,  50,   15 );
      MBitmap->drawOptimizedEllipse(        120+50, 50+15,  50,   15 );
      MBitmap->drawEllipseRect(             120,    90,     220,  120 );
      MBitmap->drawEllipseRectAA(           120,    130,    220,  160 );
      MBitmap->drawRotatedEllipse(          120+50, 170+15, 50,   15,  0.2*SKEI_PI2 );
      MBitmap->drawRotatedEllipseRect(      120,    210,    220,  240, 2560 );
      MBitmap->drawCircle(                  230+50, 10+15,  15 );
      MBitmap->drawCircleAA(                230+50, 50+15,  15 );
      MBitmap->drawQuadBezierSeg(           340, 10,  390, 10,  440, 40 );
      MBitmap->drawQuadBezierSegAA(         340, 50,  390, 50,  440, 80 );
      MBitmap->drawQuadBezier(              340, 90,  390, 90,  440, 120 );
      MBitmap->drawQuadRationalBezier(      450, 10,  500, 10,  550, 40,  1 );
      MBitmap->drawQuadRationalBezierSeg(   450, 50,  500, 50,  550, 80,  1 );
      MBitmap->drawQuadRationalBezierSegAA( 450, 90,  500, 90,  550, 120, -1 );
      MBitmap->drawCubicBezier(             560, 40,  560, 10,  660, 10,  660, 40 );
      //MBitmap->drawCubicBezierSeg(          560, 80,  560, 50,  660, 50,  660, 80 );
      //MBitmap->drawCubicBezierSegAA(        560, 120, 560, 90,  660, 90,  660, 120 );
      //MBitmap->drawQuadSpline(int n, int x[], int y[]);
      //MBitmap->drawCubicSpline(int n, int x[], int y[]);
      */

      APainter->drawImage(0,0,MImage,0,0,1024,512);
    }

};

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin
{

  private:

    myWidget* MWidget;

  public:

    //----------------------------------------
    //
    //----------------------------------------

    //virtual
    void on_create(void) {
      MInfo.name       = "test_image";
      MInfo.author     = "skei";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      //
      MEditorRect = SRect(1024,512);
    }

    //----------------------------------------
    //
    //----------------------------------------

    //virtual
    void* on_openEditor(void* AParent) {
      SEditor* editor = new SEditor(this,MEditorRect.w,MEditorRect.h,AParent);
      MWidget = new myWidget( editor, SRect(0), swa_fillClient );
      editor->appendWidget( MWidget );
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
        editor->do_redraw(MWidget,MWidget->rect());
      }

    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif




