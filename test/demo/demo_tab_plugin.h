#ifndef demo_tab_plugin_included
#define demo_tab_plugin_included
//----------------------------------------------------------------------

#include "demo/demo_tab_base.h"

#include "skei_widget_menuitem.h"
#include "skei_widget_scrollbox.h"
#include "skei_widget_text.h"

//----------

class demo_tab_plugin
: public demo_tab_base {

  public:

    demo_tab_plugin()
    : demo_tab_base() {
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"editor",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"host",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"parameter",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"plugin",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"program",swa_fillTop) );
    }

    virtual ~demo_tab_plugin() {
    }


};

//----------------------------------------------------------------------
#endif

