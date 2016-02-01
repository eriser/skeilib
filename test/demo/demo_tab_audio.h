#ifndef demo_tab_audio_included
#define demo_tab_audio_included
//----------------------------------------------------------------------

#include "demo/demo_tab_base.h"

#include "skei_widget_menuitem.h"
#include "skei_widget_scrollbox.h"
#include "skei_widget_text.h"

//----------

class demo_tab_audio
: public demo_tab_base {

  public:

    demo_tab_audio()
    : demo_tab_base() {
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"envelope",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"filter",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"host sync",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"lfo",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"midi timing",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"modular",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"note manager",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"osc",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"timeline",swa_fillTop) );
      MScrollBox->appendWidget( new SWidget_MenuItem( SRect(16),"voice manager",swa_fillTop) );
    }

    virtual ~demo_tab_audio() {
    }


};

//----------------------------------------------------------------------
#endif

