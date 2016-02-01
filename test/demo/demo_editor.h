#ifndef demo_editor_included
#define demo_editor_included
//----------------------------------------------------------------------

#include "skei_editor.h"
#include "skei_widget_tabs.h"

//#include "demo/demo_tab_base.h"
#include "demo/demo_tab_system.h"
#include "demo/demo_tab_lib.h"
#include "demo/demo_tab_utils.h"
#include "demo/demo_tab_plugin.h"
#include "demo/demo_tab_gui.h"
#include "demo/demo_tab_audio.h"

#include "../data/skei_png.h"

//----------------------------------------------------------------------
// utils
//----------------------------------------------------------------------

void uploadBitmap(SDrawable* ADrawable, SSurface* ASurface, SBitmap* ABitmap) {
  ABitmap->convertRgbaBgra();
  ABitmap->premultAlpha();
  SImage* image = new SImage(/*this*/ADrawable,ABitmap,32/*depth()*/);
  SPainter* painter = new SPainter(ASurface);
  painter->drawImage(0,0,image,0,0,ABitmap->width(),ABitmap->height());
  delete painter;
  delete image;
}

//----------------------------------------------------------------------
// editor
//----------------------------------------------------------------------

class myEditor
: public SEditor {

  private:
    SSurface* surface1;

  public:

    myEditor(SEditor_Listener* AListener, int32 AWidth, int32 AHeight, void* AParent=SKEI_NULL)
    : SEditor(AListener,AWidth,AHeight,AParent) {
      MName = "myEditor";

      setTitle("skeilib demo");

      fillBackground();
      fillColor(SGrey);
      margins(10);
      padding(5);

      SBitmap*  bitmap;

      bitmap = new SBitmap(skei_png,skei_png_size);
      surface1 = new SSurface(this,bitmap->width(),bitmap->height(),32);
      uploadBitmap(this,surface1,bitmap);
      delete bitmap;

      //----------

      SWidget_Tabs* tabs = new SWidget_Tabs( SRect(0), swa_fillClient );
      appendWidget( tabs );
      tabs->margins(10);
      tabs->padding(5);
      tabs->appendPage( new demo_tab_system(),  (char*)"system" );
      tabs->appendPage( new demo_tab_lib(),     (char*)"lib" );
      tabs->appendPage( new demo_tab_utils(),   (char*)"utils" );
      tabs->appendPage( new demo_tab_plugin(),  (char*)"plugin" );
      tabs->appendPage( new demo_tab_gui(),     (char*)"gui" );
      tabs->appendPage( new demo_tab_audio(),   (char*)"audio" );
      tabs->selectTab(0);
    }

    //--------------------------------------------------

    virtual ~myEditor() {
      delete surface1;
    }

    //--------------------------------------------------

    /*
    //virtual
    void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (menu->isOpen()) {
        if (AButton==smb_right) menu->close();
      }
      SEditor::on_mouseDown(AXpos,AYpos,AButton,AState);
    }
    */

    //----------


    /*
    //virtual
    void do_update(SWidget* AWidget) {
      if (AWidget==menuitem) menu->open( menuitem->rect().x+10, menuitem->rect().y + 10 );
      if (AWidget==menu) {
        STrace("selected: %i\n",menu->selected() );
        menu->close();
        //menuitem->text( menu->selectedWidget()->name() );
      }

      SEditor::do_update(AWidget);
    }
    */

    //----------

    /*
    //virtual
    void do_hint(SWidget* AWidget, const char* AHint) {
      if (AHint) {
        hinttext->text(AHint);
        do_redraw(hinttext,hinttext->rect());
      }
    }
    */

    //----------

};

//----------------------------------------------------------------------
#endif

