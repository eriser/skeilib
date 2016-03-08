
#define SKEI_MAIN
//#define SKEI_EXE

//#define SKEI_DEBUG
//#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_MEM_PRINT
//#define SKEI_DEBUG_CALLSTACK

//#define SKEI_DEBUG_CONSOLE
//#define SKEI_DEBUG_SOCKET

#define SKEI_XLIB
#define SKEI_XRENDER
#define SKEI_XFT

//----------------------------------------------------------------------

#include "skei.h"
#include "skei_color.h"
#include "skei_widget.h"
#include "skei_window.h"

//----------------------------------------------------------------------

class myWidget
: public SWidget {

  public:

    myWidget(SRect ARect, uint32 AAlignment=swa_fillClient)
    : SWidget(ARect,AAlignment){
      DUMP_CALLSTACK;
    }

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      APainter->setFillColor(SDarkGrey);
      APainter->fillRect(MRect);
      SWidget::on_paint(APainter,ARect,AMode);
    }

    //virtual
    void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      //STrace("clicky %i\n",AButton);
      do_cursorPos(this,0,0);
    }

};

//----------------------------------------------------------------------

int main(void) {

  SWindow* window = new SWindow(640,480);

    //if (EasyTab_Load( window->display(), window->drawable() ) != EASYTAB_OK) { printf("Tablet init failed\n"); }

  window->setTitle("Hello World!");

  SWidget* widget = new myWidget(SRect(0),swa_fillClient);
  widget->margins(10,10);
  widget->padding(5);
  window->appendWidget(widget);

  window->alignWidgets();
  window->paintBuffer();
  window->open();
  window->eventLoop();

    //EasyTab_Unload();

  window->close();
  delete window;
  return 0;
}

