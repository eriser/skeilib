
//#define SKEI_DEBUG
#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_SOCKET

//#define SKEI_EXE
#define SKEI_MAIN

#define SKEI_XLIB
#define SKEI_XRENDER
#define SKEI_XFT

//----------------------------------------------------------------------

#include "skei.h"
#include "skei_color.h"
#include "skei_widget.h"
#include "skei_window.h"

#include "skei_widget_color.h"
#include "skei_widget_panel.h"

//----------------------------------------------------------------------
// main class
//---------------------------------------------------------------------

class mainClass {

  private:
    SWindow*  window;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    int main(void) {
      window = new SWindow(640,480);
      window->setTitle("Hello World!");

      STrace("hello world!");

        // widgets

        SWidget* colorpanel = (SWidget*) window->appendWidgetW( new SWidget_Color( SRect(0), SDarkGrey, swa_fillClient ) );
        colorpanel->margins( SEdges(10) );
        colorpanel->padding( SPoint(5) );

          SWidget* w;

          w = colorpanel->appendWidgetW( new SWidget_Panel(SRect(100), swa_fillLeft   ) );
          w->cursor( smc_arrowLeft );
          //w->sizeLimit( SRect(0,0,SKEI_INT32_MAX,300) );

          w = colorpanel->appendWidgetW( new SWidget_Panel(SRect(100), swa_fillTop    ) );
          w->cursor( smc_arrowUp );

          w = colorpanel->appendWidgetW( new SWidget_Panel(SRect(100), swa_fillRight  ) );
          w->cursor( smc_arrowRight );

          w = colorpanel->appendWidgetW( new SWidget_Panel(SRect(100), swa_fillBottom ) );
          w->cursor( smc_arrowDown );

          w = colorpanel->appendWidgetW( new SWidget_Panel(SRect(0),   swa_fillClient ) );
          w->cursor( smc_cross );

        //

      window->alignWidgets();
      window->paintBuffer();
      window->open();
      window->eventLoop();
      window->close();
      delete window;
      return 0;
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

int main(void) {
  mainClass app;
  return app.main();
}
