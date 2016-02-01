#ifndef demo_tab_utils_included
#define demo_tab_utils_included
//----------------------------------------------------------------------

#include "demo/demo_tab_base.h"
#include "skei_widget_menuitem.h"
#include "skei_widget_scrollbox.h"
#include "skei_widget_text.h"

//----------

class demo_tab_utils
: public demo_tab_base {

  public:

    demo_tab_utils()
    : demo_tab_base() {
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"convert",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"integer",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"interpolate",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"math",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"math fast",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"math stdlib",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"random",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"string",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"string stdlib",swa_fillTop) );
    }

    virtual ~demo_tab_utils() {
    }


};

//----------------------------------------------------------------------
#endif
