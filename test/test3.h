
#define SKEI_MAIN
//#define SKEI_EXE

//#define SKEI_DEBUG
#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_SOCKET

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

#include "skei_voice_manager.h"

//----------------------------------------------------------------------

class myWidget
: public SWidget {

  public:

    myWidget(SRect ARect, uint32 AAlignment=swa_fillClient)
    : SWidget(ARect,AAlignment){
    }

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      APainter->setFillColor(SDarkGrey);
      APainter->fillRect(MRect);
      SWidget::on_paint(APainter,ARect,AMode);
    }

};

//----------------------------------------------------------------------

int main(void) {

  SWindow* window = new SWindow(640,480);
  window->setTitle("Hello World!");

  SWidget* widget = new myWidget(SRect(0),swa_fillClient);
  widget->margins(10,10);
  widget->padding(5);
  window->appendWidget(widget);

  window->alignWidgets();
  window->paintBuffer();
  window->open();
  window->eventLoop();

  window->close();
  delete window;
  return 0;
}

