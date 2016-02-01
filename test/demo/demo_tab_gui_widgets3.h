#ifndef demo_tab_gui_widgets3_included
#define demo_tab_gui_widgets3_included
//----------------------------------------------------------------------

#include "skei_widget.h"
#include "skei_widget_buttonrow.h"
#include "skei_widget_keyboard.h"
#include "skei_widget_textedit.h"


//#include "skei_basepath.h"

//----------

class demo_tab_gui_widgets3
: public SWidget {

  private:

  public:

    demo_tab_gui_widgets3()
    : SWidget( SRect(0), swa_fillClient) {
      margins(10);
      padding(5);

      appendWidget( new SWidget_ButtonRow( SRect(16), 8, swa_fillTop) );
      appendWidget( new SWidget_Keyboard(  SRect(40), swa_fillTop ));
      appendWidget( new SWidget_TextEdit(  SRect(20), "textedit", swa_fillTop ));

      //SWidget_Waveform* waveform = (SWidget_Waveform*)left->appendWidgetW( new SWidget_Waveform(   SRect(40), swa_fillTop ));
      //left->appendWidget( new SWidget_ImageStrip( SRect(32,32), 0, tilemap1, swa_stackedHoriz )); // topLeft ));

    }

    //----------

    virtual ~demo_tab_gui_widgets3() {
    }

    //----------

};

//----------------------------------------------------------------------
#endif
