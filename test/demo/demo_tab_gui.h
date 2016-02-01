#ifndef demo_tab_gui_included
#define demo_tab_gui_included
//----------------------------------------------------------------------

#include "skei_widget_menuitem.h"
#include "skei_widget_scrollbox.h"
#include "skei_widget_text.h"

#include "demo/demo_tab_base.h"
#include "demo/demo_tab_gui_xlib.h"
#include "demo/demo_tab_gui_widgets1.h"
#include "demo/demo_tab_gui_widgets2.h"
#include "demo/demo_tab_gui_widgets3.h"
#include "demo/demo_tab_gui_widgets4.h"

//----------

class demo_tab_gui
: public demo_tab_base {

  private:

    SWidget* MMenu;

  public:

    demo_tab_gui()
    : demo_tab_base() {
      //margins(20);
      //padding(10);
      MMenu = new SWidget( SRect(0), swa_fillClient );
      MMenu->name("menu");
      MMenu->appendWidget( new SWidget_MenuItem( SRect(16),"xlib",swa_fillTop) );
      MMenu->appendWidget( new SWidget_MenuItem( SRect(16),"basic widgets",swa_fillTop) );
      MMenu->appendWidget( new SWidget_MenuItem( SRect(16),"value widgets",swa_fillTop) );
      MMenu->appendWidget( new SWidget_MenuItem( SRect(16),"other widgets",swa_fillTop) );
      MMenu->appendWidget( new SWidget_MenuItem( SRect(16),"container widgets",swa_fillTop) );
      MScrollBox->appendWidget(MMenu);
    }

    //----------

    virtual ~demo_tab_gui() {
    }

    //----------

    //virtual
    void do_update(SWidget* AWidget) {
      if (AWidget->parent() == MMenu ) {
        switch(AWidget->index()) {
          case 0: setClient( new demo_tab_gui_xlib() );     break;
          case 1: setClient( new demo_tab_gui_widgets1() ); break;
          case 2: setClient( new demo_tab_gui_widgets2() ); break;
          case 3: setClient( new demo_tab_gui_widgets3() ); break;
          case 4: setClient( new demo_tab_gui_widgets4() ); break;
        }
      }
      demo_tab_base::do_update(AWidget);
    }

    //----------



};

//----------------------------------------------------------------------
#endif

