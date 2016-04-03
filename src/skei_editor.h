#ifndef skei_editor_included
#define skei_editor_included
//----------------------------------------------------------------------

/*
  saving/restoring editor state is untested
*/

//----------

//#include "skei_controls.h"
#include "skei_color.h"
#include "skei_editor_listener.h"
#include "skei_parameter.h"
#include "skei_plugin_listener.h"
#include "skei_window.h"

#ifdef SKEI_PLUGIN_HAS_EDITOR
#include "skei_window.h"
#endif

//----------

class SEditor
: public SWindow
, public SPlugin_Listener {

  protected:
    SEditor_Listener* MListener;
    SColor            MFillColor;
    bool              MFillBackground;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    SEditor(SEditor_Listener* AListener, int32 AWidth, int32 AHeight, void* AParent=SKEI_NULL)
    : SWindow(AWidth,AHeight,AParent) {
      MListener = AListener;
      MFillColor = SGrey;
      MFillBackground = false;

    }

    //----------

    virtual ~SEditor() {
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void fillColor(SColor AColor)         { MFillColor = AColor; }
    void fillBackground(bool AFill=true)  { MFillBackground = AFill; }

  //----------------------------------------
  //
  //----------------------------------------

  public:

  public:

    void connect(SWidget* AWidget, SParameter* AParameter) {
      AParameter->widget(AWidget);
      AWidget->parameter(AParameter);
    }

    //----------

    void connect(SWidget* AWidget, SParameter** AParameters, int32 ANumParams) {
      for (int32 i=0;i <ANumParams; i++) {
        connect(AWidget,AParameters[i]);
      }
    }

  //----------------------------------------
  // SPlugin_Listener
  //----------------------------------------

  public:

    /*
      called from:
      - KPlugin_Vst.updateEditorInIdle
    */

    //virtual
    //void updateParameterFromPlugin(int32 AParameter, float AValue) {
    void updateParameterFromPlugin(SParameter* AParameter, float AValue) {
      //STrace("%s = %f\n",AParameter->name(),AValue);
      SWidget* wdg = (SWidget*)( AParameter->widget() );
      if (wdg) {
        wdg->value(AValue);
        SRect rec = wdg->rect();
        do_redraw(wdg,rec); // this was commented out... why?
        //if mapped & exposed ?
        //invalidate(rec.x,rec.y,rec.w,rec.h);
        //do_redraw(wdg);
        //paint(wdg);
      }
    }

  //----------------------------------------
  // on_
  //----------------------------------------

  public:

    // editor state

    virtual uint32  on_getStateSize(void) { return 0; }
    virtual void    on_saveState(void* ABuffer, uint32 ASize) {}
    virtual void    on_restoreState(void* ABuffer, uint32 ASize) {}

  //----------------------------------------
  // on_
  //----------------------------------------


  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      if (MFillBackground) {
        APainter->setFillColor(MFillColor);
        APainter->fillRect(MRect);
      }
      SWindow::on_paint(APainter,ARect,AMode);
    }

    //----------

    /*
      this is called from timer thread
      we simulate a vst-host on_idleEditor

      todo: this is a hack.. find a better way..
    */

    //#ifdef SKEI_LINUX_EVENT_THREAD_SLEEP
    ////virtual
    //void on_idle(void) {
    //  // ifdef linux?
    //  MListener->idleFromEditor();
    //  //STrace("on_idle\n");
    //  //MListener->on_idleEditor();
    //  //MListener->updateEditorInIdle();
    //}
    //#else

    /*
      also called from eventLoop()
      standalone exe
    */

    //#ifdef SKEI_LINUX_IDLE_THREAD

    //virtual
    void on_idle(void) {
      //STrace("on_idle\n");
      if (MListener) MListener->idleFromEditor();
    }

    //#endif // SKEI_LINUX_IDLE_THREAD

  //----------------------------------------
  // do_
  //----------------------------------------

    /*
      when we tweak a widget in the editor, we need to let the host know
      about it..
    */

    //virtual
    void do_update(SWidget* AWidget) {
      SParameter* param = (SParameter*)AWidget->parameter();
      if (param) {
        float value = AWidget->value();
        MListener->updateParameterFromEditor(param->index(),value);
      }
      SWindow::do_update(AWidget);
    }

    //----------

    /*
      overloads the window version..
      we are a plugin, so we call host -> resize window
      (window doesn't know about plugin or vst)
    */


    void do_sizer(SWidget* AWidget, int32 ADeltaX, int32 ADeltaY, int32 AMode) {
      if (AMode==ssm_window) {
        //setSize(MRect.w+ADeltaX,MRect.h+ADeltaY);
        MListener->resizeWindowFromEditor(MRect.w+ADeltaX,MRect.h+ADeltaY);
      }
      //if (hasFlag(swf_autoalign)) on_align();
      //paintBuffer(this);
      SWindow::do_sizer(AWidget,ADeltaX,ADeltaY,AMode);
    }

    //----------

};

//----------------------------------------------------------------------
#endif
