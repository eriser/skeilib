#ifndef demo_tab_gui_widgets1_included
#define demo_tab_gui_widgets1_included
//----------------------------------------------------------------------

#include "skei_widget.h"
#include "skei_widget_color.h"
#include "skei_widget_panel.h"
#include "skei_widget_text.h"
#include "skei_widget_string.h"
#include "skei_widget_number.h"
#include "skei_widget_grid.h"

//#include "skei_basepath.h"

//----------

class demo_tab_gui_widgets1
: public SWidget {

  private:

    //char    temp[256];
    SString MString;
    //SString MVisualInfo;

  public:

    demo_tab_gui_widgets1()
    : SWidget( SRect(0), swa_fillClient) {
      margins(10);
      padding(5);

      MString = "string";

      appendWidget( new SWidget_Color( SRect(16), SWhite, swa_fillTop) );
      appendWidget( new SWidget_Panel( SRect(32), swa_fillTop) );
      appendWidget( new SWidget_Text( SRect(16), "text", swa_fillTop) );
      appendWidget( new SWidget_String( SRect(16), &MString, swa_fillTop) );
      appendWidget( new SWidget_Number( SRect(16), 3.14, swa_fillTop) );
      appendWidget( new SWidget_Grid( SRect(32), 8,5, swa_fillTop) );

    }

    //----------

    virtual ~demo_tab_gui_widgets1() {
    }

    //----------

};

//----------------------------------------------------------------------
#endif

