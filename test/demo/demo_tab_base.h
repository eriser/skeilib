#ifndef demo_tab_base_included
#define demo_tab_base_included
//----------------------------------------------------------------------

#include "skei_widget_color.h"
#include "skei_widget_scrollbox.h"
#include "skei_widget_sizer.h"
#include "skei_widget_text.h"

//----------

class demo_tab_base : public SWidget {

  protected:
    SWidget_ScrollBox*  MScrollBox;
    SWidget_Sizer*      MSizer;
    SWidget*            MClient;

  public:

    demo_tab_base()
    : SWidget(SRect(0),swa_client) {

      MScrollBox = new SWidget_ScrollBox( SRect(150), swa_fillLeft );
      appendWidget( MScrollBox );
      MScrollBox->sizeLimit( SRect(100,0,-1,-1) );
      MScrollBox->container()->margins( 10 );
      MScrollBox->container()->padding( 5 );

      MSizer = new SWidget_Sizer( SRect(5), ssm_left, MScrollBox, swa_fillLeft );
      appendWidget( MSizer );

      MClient = new SWidget( SRect(0), swa_fillClient);
      MClient->margins(10);
      MClient->padding(10);
      appendWidget( MClient );

    }

    //----------

    virtual ~demo_tab_base() {
    }

    //----------

    virtual
    void setClient(SWidget* AWidget) {
      MClient->deleteWidgets();
      MClient->appendWidget(AWidget);
      if (hasFlag(swf_autoalign)) MClient->on_align();
      do_redraw(MClient,MClient->rect());
      //paintBuffer();
    }

    //----------

    /*
    //virtual
    void do_update(SWidget* AWidget) {
      int32 index = AWidget->index();
      STrace("%i\n",index);
      SWidget::do_update(AWidget);
    }
    */

    //----------

};

//----------------------------------------------------------------------
#endif



      /*
      SWidget_GroupBox* groupbox = new SWidget_GroupBox( SRect(200), swa_fillTop );
      left->appendWidget( groupbox );
      SWidget_Switch* grouptitle = (SWidget_Switch*)groupbox->titlebar();
      grouptitle->backOnColor(SDarkRed);
      grouptitle->textOnColor(SLightYellow);
      SWidget_ScrollBox* sb = new SWidget_ScrollBox( SRect(0), swa_fillClient );
      groupbox->appendWidget( sb );
      for (int i=0; i<25; i++) {
        sb->appendWidget( new SWidget_Knob( SRect(40,40), 0, swa_stackedHoriz )  );
      }
      */

//      MScrollBox->appendWidget( new SWidget_Text( SRect(20),"text",swa_fillTop) );
