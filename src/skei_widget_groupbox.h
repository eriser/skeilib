#ifndef skei_widget_groupbox_included
#define skei_widget_groupbox_included
//----------------------------------------------------------------------

#include "skei_widget.h"
#include "skei_widget_switch.h"

//----------

class SWidget_GroupBox
: public SWidget {

  private:

    SWidget_Switch* MTitleBar;
    SWidget*        MContainer;
    bool            MClosed;
    bool            MClosable;
    int32           MPrevSize;

  public:

    //property    _container : KWidget read FContainer;
    //property    _titlebar : KWidget_Switch{Button} read FTitleBar;

  public:

    SWidget_GroupBox(SRect ARect, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_GroupBox";
      MHint = "groupbox";
      MTitleBar = new SWidget_Switch(SRect(20),"groupbox","groupbox",swa_fillTop );
      MContainer = new SWidget(SRect(0),swa_fillClient);
      MTitleBar->name("groupbox titlebar");
      MContainer->name("groupbox container");

      //FContainer.setFlag(kwf_clip);
      MContainer->setFlag(swf_clip);

      SWidget::appendWidget( MTitleBar );
      SWidget::appendWidget( MContainer );
      MClosed = false;
      MClosable = true;
      MPrevSize = 0;
      //FTitleBar.clearFlag(kwf_active);

      close();
    }

    virtual ~SWidget_GroupBox() {
    }

  public:

    SWidget* titlebar(void) { return MTitleBar; }
    SWidget* container(void) { return MContainer; }


  public:

    virtual
    int32 appendWidget(SWidget* AWidget) {
      return MContainer->appendWidget(AWidget);
    }

    void open(void) {
      if (MClosed) {
        MClosed = false;
        MContainer->setFlag(swf_active);
        MContainer->setFlag(swf_visible);
        MRect.h = MPrevSize;
      }
    }

    void close(void) {
      if (!MClosed) {
        MClosed = true;
        MContainer->clearFlag(swf_active);
        MContainer->clearFlag(swf_visible);
        MPrevSize = MRect.h;
        MRect.h = MTitleBar->rect().h;
      }
    }

    void toggle(void) {
      if (MClosed) {
        MClosed = false;
        MContainer->setFlag(swf_active);
        MContainer->setFlag(swf_visible);
        MRect.h = MPrevSize;
      }
      else {
        MClosed = true;
        MContainer->clearFlag(swf_active);
        MContainer->clearFlag(swf_visible);
        MPrevSize = MRect.h;
        MRect.h = MTitleBar->rect().h;
      }
    }

  public:

    virtual void do_update(SWidget* AWidget) {
      if (AWidget == MTitleBar) {
        if (MClosable) {
          if (MTitleBar->value() > 0.5) open();
          else close();
          do_resize(MContainer,0,MContainer->rect().h);
          do_redraw(MContainer,MContainer->rect());
        }
      }
      //else
      SWidget::do_update(AWidget);
    }

};

//----------------------------------------------------------------------
#endif

