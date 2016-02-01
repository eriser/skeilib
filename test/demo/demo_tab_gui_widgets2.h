#ifndef demo_tab_gui_widgets2_included
#define demo_tab_gui_widgets2_included
//----------------------------------------------------------------------

#include "skei_widget.h"
#include "skei_widget_button.h"
#include "skei_widget_switch.h"
#include "skei_widget_value.h"
#include "skei_widget_slider.h"
#include "skei_widget_scrollbar.h"
#include "skei_widget_knob.h"

//----------

class demo_tab_gui_widgets2
: public SWidget {

  private:

  public:

    demo_tab_gui_widgets2()
    : SWidget( SRect(0), swa_fillClient) {
      margins(10);
      padding(5);
      appendWidget( new SWidget_Button( SRect(16), "button on", "button off", swa_fillTop) );
      appendWidget( new SWidget_Switch( SRect(16), "switch on", "switch off", swa_fillTop) );
      appendWidget( new SWidget_Value( SRect(16), 0.1, swa_fillTop) );
      appendWidget( new SWidget_Slider( SRect(16), 0.2, swa_fillTop) );
      appendWidget( new SWidget_ScrollBar( SRect(16), 0.3, false, swa_fillTop) );
      appendWidget( new SWidget_Knob( SRect(32), 0.4, swa_leftTop) );
    }

    //----------

    virtual ~demo_tab_gui_widgets2() {
    }

    //----------

};

//----------------------------------------------------------------------
#endif
