#ifndef test_animate_included
#define test_animate_included
//----------------------------------------------------------------------

/*
  debug version prints a lot of stuff through a socket..
  enter 'nc -U -l -k /tmp/skei.sock' in another terminal window
*/

//----------

/*
  crashes in:
  - ardour4 (xcb-related)
  - bitwig
  - carla-vst hosted in bitwig
  works in:
  - carla standalone
  - qtractor
  - jost
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
//#define SKEI_PLUGIN_PER_SAMPLE
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_SEND_MIDI
//#define SKEI_PLUGIN_RECEIVE_MIDI
//#define SKEI_PLUGIN_AUTOSYNC

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_lock.h"
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

    /*
      possible problem..
      Xlib is generally not thread-safe.
      a Display* pointer can only be used from one thread
      we start our own event handler thread, and the incoming vst/effEditIdle
      events are called from the gui hread, probably? differet threads..

      i read somewhere that Window IDs could be shared among display
      connections, so i tried to open a temporary connection, and use that to
      send an idle event to the window, so that we were in the right thread
      when acting on it..
      but sometimes the sent events aren't picked up.. they get sent, but
      the event thread doesn't receive any clientmessages
    */

    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {

      SColor col;

      //for (int32 i=0; i<1000; i++) {
        float r = SRandom();
        float g = SRandom();
        float b = SRandom();
        col.set(r,g,b);
        APainter->setFillColor(col);
        APainter->fillRect(MRect);
      //}

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

      //SSleep(1000);
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
    SLock     MLock;

  public:

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

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

    //----------

    //virtual
    void on_destroy(void) {
    }

    //------------------------------------------------------------
    // events
    //------------------------------------------------------------

    //virtual
    void on_stateChange(uint32 AState) {
    }

    //virtual
    void on_transportChange(uint32 ATransport) {
    }

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
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
    }

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {
      float spl0 = *AInputs[0];
      float spl1 = *AInputs[1];
      *AOutputs[0] = spl0;
      *AOutputs[1] = spl1;
    }

    //virtual
    void on_postProcess(void) {
    }

    //------------------------------------------------------------
    // editor
    //------------------------------------------------------------

    //virtual
    void* on_openEditor(void* AParent) {
      SEditor* editor = new SEditor(this,MEditorRect.w,MEditorRect.h,AParent);
      //editor->fillColor( SLightGrey );
      //editor->fillBackground(true);
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
      //STrace("on_idleEditor\n");
        SEditor* editor = (SEditor*)AEditor;
        if ((editor) && (MEditorIsOpen)) {
          editor->do_redraw(MWidget,MWidget->rect());
        }

      /*

      myEditor* editor = (myEditor*)AEditor;
      if ((editor) && (MEditorIsOpen)) {

        // trying a lock/mutex

        //if (MLock.tryLock()) {
        //  editor->do_redraw(MWidget,MWidget->rect());//  paintBuffer();
        //  MLock.unlock();
        //}

        // trying some reentrancy testing

        //static bool reentrant = false;
        //if (!reentrant) {
        //  reentrant = true;
        //  SRect r = MWidget->rect();
        //  //DTrace("x %i y %i w %i h %i\n",r.x,r.y,r.w,r.h);
        //  editor->do_redraw(MWidget,r);
        //  reentrant = false;
        //}
        //else {
        //  STrace("reentrant!!\n");
        //}

        //editor->do_redraw(MWidget,MWidget->rect());

      }

      */
    }

  //----------

    //void idleFromEditor(void) {
    //  //DTrace("myPlugin.idleFromEditor\n");
    //  MEditor->do_redraw(MWidget,MWidget->rect());
    //}


};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif



