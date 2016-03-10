
#define SKEI_MAIN

//#define SKEI_DEBUG
//#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_MEM_PRINT
//#define SKEI_DEBUG_CALLSTACK
//#define SKEI_DEBUG_CRASHHANDLER
//#define SKEI_DEBUG_CONSOLE
//#define SKEI_DEBUG_SOCKET

#define SKEI_XLIB
//#define SKEI_XRENDER
//#define SKEI_XFT

//----------------------------------------------------------------------

#include "skei.h"
#include "skei_widget.h"
#include "skei_window.h"

//----------------------------------------------------------------------

//----------------------------------------------------------------------

class myWidget
: public SWidget {

    friend int main(void);

  private:

    SWindow* win;

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


    #ifdef SKEI_TABLET
    virtual
    void on_tabletEvent(int32 AXpos, int32 AYpos, float APressure) {
      //STrace("tablet x:%i y:%i p:%f\n",AXpos,AYpos,APressure);
    }
    #endif

    //virtual
    void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      //STrace("mouse click x:%i y:%i b:%i\n",AXpos,AYpos,AButton);
    }

    //virtual
    void on_mouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      //STrace("mouse release x:%i y:%i b:%i\n",AXpos,AYpos,AButton);
    }

    //virtual
    void on_mouseMove(int32 AXpos, int32 AYpos, int32 AState) {
      //STrace("mouse move x:%i y:%i\n",AXpos,AYpos);
    }


    //virtual
    //void on_tabletEvent(int32 AXpos, int32 AYpos, float APressure) {
    //  STrace("x:%i y:%i p:%f\n",AXpos,AYpos,APressure);
    //}

};

//----------------------------------------------------------------------

int main(void) {

  //STrace("%i\n",SNumCpuCores() );

  SWindow* window = new SWindow(640,480);

  window->setTitle("Hello World!");
  myWidget* widget = (myWidget*) new myWidget(SRect(0),swa_fillClient);
  widget->win = window;
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

