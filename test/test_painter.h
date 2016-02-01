
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

  private:

    SSurface*   MSurface1;
    SPainter*   MPainter1;

  public:

    myWidget(SWindow* AWindow, SRect ARect, uint32 AAlignment=swa_fillClient)
    : SWidget(ARect,AAlignment){

      MSurface1 = new SSurface(AWindow,256,256);
      MPainter1 = new SPainter(MSurface1);

    }

    //virtual
    ~myWidget() {
      delete MSurface1;
      delete MPainter1;
    }

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {

      // fill background

      APainter->setFillColor(SLightGrey);
      APainter->fillRect(MRect);

      // draw shapes

      APainter->setDrawColor(SRed);

      APainter->drawPoint(        10,   10);
      APainter->drawPoint(        12,   10);
      APainter->drawPoint(        14,   10);
      APainter->drawPoint(        10,   12);
      APainter->drawPoint(        10,   14);
      APainter->drawPoint(        12,   12);

      APainter->drawLine(         20,   10,  40,  10 );
      APainter->drawLine(         20,   10,  40,  20 );
      APainter->drawLine(         20,   10,  40,  30 );
      APainter->drawLine(         20,   10,  30,  30 );
      APainter->drawLine(         20,   10,  20,  30 );

      APainter->drawRect(         50,   10,  70,  30 );
      APainter->drawEllipse(      80,   10, 100,  30 );
      APainter->drawArc(          110,  10, 130,  30, 0.2,0.7);
      APainter->drawTri(          140,  10, 160,  20, 150, 30);
      APainter->drawRoundedRect(  170,  10, 190,  30, 6 );

      // fill shapes

      APainter->setFillColor(SBlue);

      APainter->fillRect(         50,   40,  70,  60 );
      APainter->fillEllipse(      80,   40, 100,  60 );
      APainter->fillArc(          110,  40, 130,  60, 0.2,0.7);
      APainter->fillTri(          140,  40, 160,  50, 150, 60);
      APainter->fillRoundedRect(  170,  40, 190,  60, 6 );

      // text

      APainter->setTextColor(SGreen);
      APainter->drawText(         10,  70, "Text");

      //

      APainter->fillGradient(     10,140, 70,160, SWhite,SBlack);
      APainter->fillGradient(     10,170, 70,190, SBrightRed,SBrightGreen,false);

      /*
      drawFont
      blendRect
      drawImage
      drawSurface
      blendSurface
      stretchSurface
      tileSurface
      flexSurface
      */

      SWidget::on_paint(APainter,ARect,AMode);
    }

};

//----------------------------------------------------------------------

int main(void) {

  SWindow* window = new SWindow(640,480);
  window->setTitle("Hello World!");

  SWidget* widget = new myWidget(window,SRect(0),swa_fillClient);
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

