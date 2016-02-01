#ifndef demo_tab_gui_widgets4_included
#define demo_tab_gui_widgets4_included
//----------------------------------------------------------------------

#include "skei_widget.h"
#include "skei_widget_groupbox.h"
#include "skei_widget_switch.h"
#include "skei_widget_scrollbox.h"
#include "skei_widget_tabs.h"
#include "skei_widget_color.h"

//#include "skei_basepath.h"

//----------

class demo_tab_gui_widgets4
: public SWidget {

  private:

  public:

    demo_tab_gui_widgets4()
    : SWidget( SRect(0), swa_fillClient) {
      margins(10);
      padding(5);
      //appendWidget( new SWidget_Color( SRect(16), SWhite, swa_fillTop) );

      // groupbox
      SWidget_GroupBox* groupbox = new SWidget_GroupBox( SRect(150), swa_fillTop );
      appendWidget( groupbox );
        SWidget_Switch* grouptitle = (SWidget_Switch*)groupbox->titlebar();
        grouptitle->backOnColor(SDarkRed);
        grouptitle->textOnColor(SLightYellow);
        SWidget_ScrollBox* sb = new SWidget_ScrollBox( SRect(0), swa_fillClient );
        groupbox->appendWidget( sb );
        for (int i=0; i<20; i++) {
          sb->appendWidget( new SWidget_Knob( SRect(35,35), 0, swa_stackedHoriz )  );
        }

      // tabs
      SWidget_Tabs* tabs = new SWidget_Tabs( SRect(100), swa_fillTop );
      appendWidget( tabs );
        tabs->appendPage( new SWidget_Color( SRect(0), SDarkRed,   swa_client ), (char*)"tab1"  );
        tabs->appendPage( new SWidget_Color( SRect(0), SDarkGreen, swa_client ), (char*)"tab2"  );
        tabs->appendPage( new SWidget_Color( SRect(0), SDarkBlue,  swa_client ), (char*)"tab3"  );
        tabs->selectTab(1);



    }

    //----------

    virtual ~demo_tab_gui_widgets4() {
    }

    //----------

};

//----------------------------------------------------------------------
#endif
