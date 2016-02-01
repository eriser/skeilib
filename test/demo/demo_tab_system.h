#ifndef demo_tab_system_included
#define demo_tab_system_included
//----------------------------------------------------------------------

#include "skei_widget_menuitem.h"
#include "skei_widget_scrollbox.h"
#include "skei_widget_text.h"

#include "demo/demo_tab_base.h"
#include "demo/demo_tab_system_info.h"

//----------

class demo_tab_system
: public demo_tab_base {

  private:

    SWidget* MMenu;

  public:

    demo_tab_system()
    : demo_tab_base() {
      //margins(20);
      //padding(10);
      MMenu = new SWidget( SRect(0), swa_fillClient );
      MMenu->appendWidget( new SWidget_MenuItem( SRect(16),"info",swa_fillTop) );
      MScrollBox->appendWidget(MMenu);
    }

    //----------

    virtual ~demo_tab_system() {
    }

    //----------

    //virtual
    void do_update(SWidget* AWidget) {
      if (AWidget->parent() == MMenu ) {
        switch(AWidget->index()) {
          case 0: setClient( new demo_tab_system_info() );     break;
        }
      }
      demo_tab_base::do_update(AWidget);
    }

    //----------



    //----------



};

//----------------------------------------------------------------------
#endif
