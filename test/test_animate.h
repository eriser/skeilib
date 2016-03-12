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
#include "skei_editor.h"
#include "skei_plugin.h"
#include "skei_random.h"
#include "skei_widget.h"

//----------------------------------------------------------------------
// widget
//----------------------------------------------------------------------

class myWidget : public SWidget {

  public:

    myWidget(SRect ARect, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
    }

    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {

      SColor col;

      float r = SRandom();
      float g = SRandom();
      float b = SRandom();
      col.set(r,g,b);
      APainter->setFillColor(col);
      APainter->fillRect(MRect);

      int32 prevx = 0;
      int32 prevy = 0;
      for (int32 i=0; i<4096; i++) {
        float r = SRandom();
        float g = SRandom();
        float b = SRandom();
        col.set(r,g,b);
        APainter->setDrawColor(col);
        int32 x = SRandomRangeInt(0,MRect.w-1);
        int32 y = SRandomRangeInt(0,MRect.h-1);
        APainter->drawLine(prevx,prevy,x,y);
        prevx = x;
        prevy = y;
      }

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
      MInfo.name       = "test_animate";
      MInfo.author     = "skei";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      //
      MEditorRect = SRect(640,480);
    }

    //----------------------------------------
    //
    //----------------------------------------

    //virtual
    void* on_openEditor(void* AParent) {
      SEditor* editor = new SEditor(this,MEditorRect.w,MEditorRect.h,AParent);
      MWidget = new myWidget( SRect(0), swa_fillClient );
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



