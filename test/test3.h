
#define SKEI_MAIN

//#define SKEI_DEBUG
//#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_MEM_PRINT
//#define SKEI_DEBUG_CALLSTACK
#define SKEI_DEBUG_CRASHHANDLER
//#define SKEI_DEBUG_CONSOLE
//#define SKEI_DEBUG_SOCKET

#define SKEI_XLIB
//#define SKEI_XRENDER
//#define SKEI_XFT

//----------------------------------------------------------------------

#include "skei.h"
#include "skei_widget.h"
#include "skei_window.h"

#include "skei_dialogs.h"

//----------------------------------------------------------------------

class myWidget
: public SWidget {

    friend int main(void);

  private:

    SWindow* win;

  public:

    myWidget(SRect ARect, uint32 AAlignment=swa_fillClient)
    : SWidget(ARect,AAlignment){
      //SDumpCallStack;
    }

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      APainter->setFillColor(SDarkGrey);
      APainter->fillRect(MRect);
      SWidget::on_paint(APainter,ARect,AMode);
      //SDumpCallStack;
    }

    //virtual
    void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {

      int* ptr = SKEI_NULL;
      *ptr = 32;

      //do_cursorPos(this,0,0);

      //SDumpCallStack;
      //SKEI_ERROR("test-error","%i",32);
      //SKEI_BREAKPOINT;

      //int x,y;
      //win->getCursorPos(&x,&y);
      //win->setCursorPos(x+10,y+10);
      //STrace("x:%i y:%i\n",x,y);

      //char* f = SDialog_ColorSelect("select color",true);
      //char* f = SDialog_Error("error","blablabla and a much longer text...");
      //char* f = SDialog_FileOpen("load",true);
      //char* f = SDialog_FileSave("save","filename.txt");
      //char* f = SDialog_Info("info","blablabla and a much longer text...");
      //char* f = SDialog_Question("question","are you sure?");
      //char* f = SDialog_Warning("warning","are you sure?");
      //char* f = SDialog_TextEnter("entry","text","entry");
      //char* f = SDialog_Value("entry","text",15,0,100,1);
      //char* f = SDialog_TextInfo("text info","./skeilib.ini",false,false,SKEI_NULL);
      //STrace("%s\n",f);

    }

};

//----------------------------------------------------------------------

int main(void) {

  SWindow* window = new SWindow(640,480);

    //if (EasyTab_Load( window->display(), window->drawable() ) != EASYTAB_OK) { printf("Tablet init failed\n"); }

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

    //EasyTab_Unload();

  window->close();
  delete window;
  return 0;
}

