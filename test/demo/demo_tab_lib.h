#ifndef demo_tab_lib_included
#define demo_tab_lib_included
//----------------------------------------------------------------------

#include "demo/demo_tab_base.h"

#include "skei_widget_menuitem.h"
#include "skei_widget_scrollbox.h"
#include "skei_widget_text.h"

//----------

class demo_tab_lib
: public demo_tab_base {

  public:

    demo_tab_lib()
    : demo_tab_base() {
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"array",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"buffer",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"edges",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"list",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"point",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"queue",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"rect",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"stack",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"tree",swa_fillTop) );
    }

    virtual ~demo_tab_lib() {
    }


};

//----------------------------------------------------------------------
#endif
