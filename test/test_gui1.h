
/*
  qtractor  - crashes when i try to resize the window
  carla     - doesn't want me to resize the window
  ardour4   - doesn't find the newest version of the plugin ???
  jost      - resizing doesn't work
*/

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//----- config -----

//#define SKEI_EXE
//#define SKEI_VST

#define SKEI_XLIB
#define SKEI_XRENDER
#define SKEI_XFT

//#define SKEI_CAIRO

//----- debug -----

//#define SKEI_DEBUG
//#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_MEM_PRINT
//#define SKEI_DEBUG_VST
//#define SKEI_DEBUG_CALLSTACK
//#define SKEI_DEBUG_CRASHHANDLER
//#define SKEI_DEBUG_XLIB

//

#ifdef SKEI_VST
  #define SKEI_DEBUG_SOCKET
#endif

//----- plugin -----

//#define SKEI_PLUGIN_PER_SAMPLE
#define SKEI_PLUGIN_HAS_EDITOR
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_SEND_MIDI
//#define SKEI_PLUGIN_RECEIVE_MIDI
//#define SKEI_PLUGIN_AUTOSYNC

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"
#include "skei_editor.h"

#include "skei_widget.h"
#include "skei_widget_button.h"
#include "skei_widget_buttonrow.h"
#include "skei_widget_circular_waveform.h"
#include "skei_widget_color.h"
#include "skei_widget_grid.h"
#include "skei_widget_groupbox.h"
#include "skei_widget_image.h"
#include "skei_widget_imagestrip.h"
#include "skei_widget_keyboard.h"
#include "skei_widget_knob.h"
#include "skei_widget_menu.h"
#include "skei_widget_menuitem.h"
#include "skei_widget_nodes.h"
#include "skei_widget_number.h"
#include "skei_widget_panel.h"
#include "skei_widget_scrollbar.h"
#include "skei_widget_scrollbox.h"
#include "skei_widget_sizer.h"
#include "skei_widget_slider.h"
#include "skei_widget_sliderbank.h"
#include "skei_widget_switch.h"
#include "skei_widget_surface.h"
#include "skei_widget_tabs.h"
#include "skei_widget_text.h"
#include "skei_widget_textedit.h"
#include "skei_widget_value.h"
#include "skei_widget_waveform.h"

#include "skei_bitmap.h"
#include "skei_random.h"
#include "skei_tilemap.h"

//#include "skei_imagestrip.h"

#include "../data/knob2_32x32_31.h"
#include "../data/fader1_20x64_65.h"
#include "../data/skei_png.h"

//#include "skei_module.h"
//#include "skei_voice_manager.h"
//#include "skei_png.h"

// $ freetype-config --cflags
// -I/usr/include/freetype2
//#include <X11/Xft/Xft.h>

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class myWidget
: public SWidget {

  private:

    void* MFont;
    //void* MFontColor;

    //SPainter* MPainter;
    //SWindow* MWindow;

  public:

    myWidget(void* AFont, /*void* AColor,*/ SRect ARect, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "myWidget";
      //MWindow = AWindow;
      // ouch.. crashes if bufferPainter changes (window resize)..
//      MWindow->bufferPainter()->loadFont("bad robot",24);
//      MWindow->bufferPainter()->setFontColor(SGreen);
      MFont = AFont;
      //MFontColor = AColor;
    }

    virtual ~myWidget() {
    }

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {

      APainter->setDrawColor(SWhite);
      APainter->drawRoundedRect(MRect,16);

      //APainter->setFontColor( SColor( 1,1,0, 0.5) );
      APainter->setFontColor( SLightYellow );
      APainter->drawFont(MFont, MRect.x+8,MRect.y+8,"AaBbCc123@%#?");

      //APainter->setDrawColor( SColor(0,0,0, 0.1) );
      //APainter->blendLine(MRect.x,MRect.y,MRect.x2(),MRect.y2());

    }

};

//----------------------------------------------------------------------
// editor
//----------------------------------------------------------------------

/*
  this should optimally be in SSurface.. but since it uses SPainter, that
  creates a cicular dependency
  (painter uses surface, surface can't use painter)

*/

void uploadBitmap(SDrawable* ADrawable, SSurface* ASurface, SBitmap* ABitmap) {
  ABitmap->premultAlpha();
  SImage* image = new SImage(/*this*/ADrawable,ABitmap,32/*depth()*/);
  SPainter* painter = new SPainter(ASurface);
  painter->drawImage(0,0,image,0,0,ABitmap->width(),ABitmap->height());
  delete painter;
  delete image;
}

//----------

class myEditor
: public SEditor {

  private:

    SBitmap*  bitmap;
    SImage*   image;
    SSurface* surface1;
    SSurface* surface2;
    SSurface* surface3;
    SSurface* surface4;

    STileMap* tilemap1;
    STileMap* tilemap2;

    SWidget_Menu* menu;
    //SWidget*      menuitem;
    SWidget_MenuItem* menuitem;

    SWidget_Text* hinttext;

    float* buffer1;
    float* buffer2;
    int32  buffersize;

    void* MFont;
    void* MFontColor;

  public:

//    /*
//      this should optimally be in SSurface.. but since it uses SPainter, that
//      creates a cicular dependency
//      (painter uses surface, surface can't use painter)
//
//    */
//
//    void uploadBitmap(SDrawable ADrawable, SSurface* ASurface, SBitmap* ABitmap) {
//      ABitmap->premultAlpha();
//      image = new SImage(/*this*/ADrawable,ABitmap,32/*depth()*/);
//      SPainter* painter = new SPainter(ASurface);
//      painter->drawImage(0,0,image,0,0,ABitmap->width(),ABitmap->height());
//      delete painter;
//      delete image;
//    }

    //--------------------------------------------------
    //--------------------------------------------------

    myEditor(SEditor_Listener* AListener, int32 AWidth, int32 AHeight, void* AParent=SKEI_NULL)
    : SEditor(AListener,AWidth,AHeight,AParent) {

      MFont = loadFont("arial black",32);

      // window
      setTitle("skeilib :: test_gui1");
      fillBackground();
      fillColor(SGrey);

      //

      // bitmap
      //bitmap = new SBitmap(256,256);
      bitmap = new SBitmap(skei_png,skei_png_size);
      bitmap->convertRgbaBgra();

      //for (int32 y=0; y<256; y++) {
      //  for (int32 x=0; x<256; x++) {
      //    bitmap->setPixel(x,y, (128<<24)+(x<<16)+(y<<8)+(x) );
      //  }
      //}
      //bitmap->premultAlpha();

//----------

      surface1 = new SSurface(this,256,256,32);
      uploadBitmap(this,surface1,bitmap);
      delete bitmap;

//----------

      bitmap = new SBitmap(256,256);
      for (int32 y=0; y<256; y++) {
        for (int32 x=0; x<256; x++) {
          bitmap->setPixel(x,y, (x<<24)+(0<<16)+(x<<8)+(y) );
        }
      }
      //bitmap->premultAlpha();
      surface2 = new SSurface(this,256,256,32);
      uploadBitmap(this,surface2,bitmap);
      delete bitmap;

      //

      bitmap = new SBitmap(knob2_32x32_31,knob2_32x32_31_size);
      //bitmap->premultAlpha();
      surface3 = new SSurface(this,bitmap->width(),bitmap->height(),32);
      uploadBitmap(this,surface3,bitmap);
      delete bitmap;

      bitmap = new SBitmap(fader1_20x64_65,fader1_20x64_65_size);
      //bitmap->premultAlpha();
      surface4 = new SSurface(this,bitmap->width(),bitmap->height(),32);
      uploadBitmap(this,surface4,bitmap);
      delete bitmap;

      tilemap1 = new STileMap(surface3,SRect(surface3->width(),surface3->height()),1,31);
      tilemap2 = new STileMap(surface4,SRect(surface4->width(),surface4->height()),1,65);

      //

      buffersize = 1024;
      buffer1 = (float*)SMalloc(buffersize*sizeof(float));
      buffer2 = (float*)SMalloc(buffersize*sizeof(float));
      for (int32 i=0; i<buffersize; i++) buffer1[i] = SRandomSigned();
      for (int32 i=0; i<buffersize; i++) buffer2[i] = SRandom();

      //----- left -----

      SWidget_ScrollBox* left = new SWidget_ScrollBox( SRect(200), swa_fillLeft );
      appendWidget( left );
      left->sizeLimit( SRect(100,0,-1,-1) );
      left->container()->margins( 10 );
      left->container()->padding( 5 );
      // groupbox
      SWidget_GroupBox* groupbox = new SWidget_GroupBox( SRect(200), swa_fillTop );
      left->appendWidget( groupbox );
      SWidget_Switch* grouptitle = (SWidget_Switch*)groupbox->titlebar();
      grouptitle->backOnColor(SDarkRed);
      grouptitle->textOnColor(SLightYellow);
      SWidget_ScrollBox* sb = new SWidget_ScrollBox( SRect(0), swa_fillClient );
      groupbox->appendWidget( sb );
      for (int i=0; i<25; i++) {
        sb->appendWidget( new SWidget_Knob( SRect(40,40), 0, swa_stackedHoriz )  );
      }
      // tabs
      SWidget_Tabs* tabs = new SWidget_Tabs( SRect(100), swa_fillTop );
      left->appendWidget( tabs );
        tabs->appendPage( new SWidget_Color( SRect(0), SDarkRed,   swa_client ), (char*)"tab1"  );
        tabs->appendPage( new SWidget_Color( SRect(0), SDarkGreen, swa_client ), (char*)"tab2"  );
        tabs->appendPage( new SWidget_Color( SRect(0), SDarkBlue,  swa_client ), (char*)"tab3"  );
        tabs->selectTab(1);
      // widgets
      left->appendWidget( new SWidget_Color(      SRect(20), SDarkCyan, swa_fillTop ));
      left->appendWidget( new SWidget_Panel(      SRect(40), swa_fillTop ));
      left->appendWidget( new SWidget_Text(       SRect(20), "text", swa_fillTop ));
      left->appendWidget( new SWidget_Number(     SRect(20), 0.666, swa_fillTop ));
      left->appendWidget( new SWidget_Grid(       SRect(50), 12,4, swa_fillTop ));
      SWidget_Waveform* waveform = (SWidget_Waveform*)left->appendWidgetW( new SWidget_Waveform(   SRect(40), swa_fillTop ));
        waveform->setBuffer(buffer1);
        waveform->setBufferSize(buffersize);
        waveform->setMono(true);
      left->appendWidget( new SWidget_Button(     SRect(20), "button on", "button off", swa_fillTop ));
      left->appendWidget( new SWidget_Switch(     SRect(20), "switch on", "switch off", swa_fillTop ));
      left->appendWidget( new SWidget_Value(      SRect(20), 0, swa_fillTop ));
      left->appendWidget( new SWidget_Slider(     SRect(20), 0.5, swa_fillTop ));
      left->appendWidget( new SWidget_ScrollBar(  SRect(20), 0.5, false, swa_fillTop ));
      left->appendWidget( new SWidget_ButtonRow(  SRect(20), 5, swa_fillTop ));
      left->appendWidget( new SWidget_Keyboard(   SRect(40), swa_fillTop ));

      //left->appendWidget( new SWidget_ImageStrip( SRect(32,32), 0, surface3, 31, sto_vertical, swa_stackedHoriz )); // topLeft ));
      //left->appendWidget( new SWidget_ImageStrip( SRect(32,32), 0, surface3, 31, sto_vertical, swa_stackedHoriz )); // topLeft ));
      //left->appendWidget( new SWidget_ImageStrip( SRect(32,32), 0, surface3, 31, sto_vertical, swa_stackedHoriz )); // topLeft ));
      //left->appendWidget( new SWidget_ImageStrip( SRect(32,32), 0, surface3, 31, sto_vertical, swa_stackedHoriz )); // topLeft ));

      left->appendWidget( new SWidget_ImageStrip( SRect(32,32), 0, tilemap1, swa_stackedHoriz )); // topLeft ));
      left->appendWidget( new SWidget_ImageStrip( SRect(32,32), 0, tilemap1, swa_stackedHoriz )); // topLeft ));
      left->appendWidget( new SWidget_ImageStrip( SRect(32,32), 0, tilemap1, swa_stackedHoriz )); // topLeft ));
      left->appendWidget( new SWidget_ImageStrip( SRect(32,32), 0, tilemap1, swa_stackedHoriz )); // topLeft ));


      left->appendWidget( new SWidget_TextEdit(   SRect(20), "textedit", swa_fillTop ));

      //left->appendWidget( new SWidget_ImageStrip( SRect(20,64), 0, surface4, 65, sto_vertical, swa_stackedHoriz )); // topLeft ));
      //left->appendWidget( new SWidget_ImageStrip( SRect(20,64), 0, surface4, 65, sto_vertical, swa_stackedHoriz )); // topLeft ));
      //left->appendWidget( new SWidget_ImageStrip( SRect(20,64), 0, surface4, 65, sto_vertical, swa_stackedHoriz )); // topLeft ));
      //left->appendWidget( new SWidget_ImageStrip( SRect(20,64), 0, surface4, 65, sto_vertical, swa_stackedHoriz )); // topLeft ));

      left->appendWidget( new SWidget_ImageStrip( SRect(20,64), 0, tilemap2, swa_stackedHoriz )); // topLeft ));
      left->appendWidget( new SWidget_ImageStrip( SRect(20,64), 0, tilemap2, swa_stackedHoriz )); // topLeft ));
      left->appendWidget( new SWidget_ImageStrip( SRect(20,64), 0, tilemap2, swa_stackedHoriz )); // topLeft ));
      left->appendWidget( new SWidget_ImageStrip( SRect(20,64), 0, tilemap2, swa_stackedHoriz )); // topLeft ));

      menuitem = (SWidget_MenuItem*)left->appendWidgetW( new SWidget_MenuItem(   SRect(20), "menu", swa_fillTop ));
      left->appendWidget( new SWidget_SliderBank( SRect(50), 16, swa_fillTop ));
      SWidget_Nodes* nodes = (SWidget_Nodes*)left->appendWidgetW( new SWidget_Nodes( SRect(50), swa_fillTop ));
        nodes->numNodes(16);
        for (int32 i=0; i<16; i++) nodes->setNode(i,SRandomSigned(),0);
      SWidget_CircularWaveform* cwf = (SWidget_CircularWaveform*)left->appendWidgetW( new SWidget_CircularWaveform( SRect(200), swa_fillTop ));
        //for (int32 i=0; i<1024; i++) cwf->buffer(i,SRandomSigned()*0.5f);
        cwf->buffer(buffer2);
        cwf->buffersize(buffersize);
        cwf->bipolar(false);

      left->appendWidget( new myWidget( MFont, SRect(50), swa_fillTop ));

      //----- left/right sizer -----

      SWidget_Sizer* sizer = new SWidget_Sizer( SRect(5), ssm_left, left, swa_fillLeft );
      appendWidget( sizer );

      //----- right -----

      SWidget* footer = new SWidget( SRect(15), swa_fillBottom );
      appendWidget( footer );
      footer->margins(2);
      footer->appendWidget( new SWidget_Sizer(   SRect(15), ssm_window, SKEI_NULL, swa_rightBottom ) );
      hinttext = (SWidget_Text*)footer->appendWidgetW( new SWidget_Text( SRect(16), "", swa_fillClient ));

      SWidget* right = new SWidget( SRect(0), swa_fillClient );
      appendWidget( right );
      right->setFlag(swf_clip);
      right->margins( 10 );
      right->padding( 5 );

      //right->appendWidget( new SWidget_Sizer(   SRect(15), ssm_window, SKEI_NULL, swa_bottomRight ) );
      right->appendWidget( new SWidget_Knob(    SRect(80), 0.333, swa_topLeft ));
      //hinttext = (SWidget_Text*)right->appendWidgetW( new SWidget_Text( SRect(16), "", swa_fillTop ));
      right->appendWidget( new SWidget_Surface( SRect(0), surface1, ssm_stretch, swa_client ));
      right->appendWidget( new SWidget_Surface( SRect(256), surface2, ssm_blend, swa_bottomRight ));
      //right->appendWidget( new SWidget_Image(   SRect(256), image, swa_topLeft ));

      //---

      //----- menu -----
      menu = new SWidget_Menu(SRect(100,160), swa_none );
        menu->margins( 5 );
        menu->padding( 0 );
        menu->appendWidget( new SWidget_MenuItem( SRect(15), "meunitem0", swa_fillTop  ) );
        menu->appendWidget( new SWidget_MenuItem( SRect(15), "meunitem1", swa_fillTop  ) );
        menu->appendWidget( new SWidget_MenuItem( SRect(15), "meunitem2", swa_fillTop  ) );
        menu->appendWidget( new SWidget_MenuItem( SRect(15), "meunitem3", swa_fillTop  ) );
        menu->appendWidget( new SWidget_MenuItem( SRect(15), "meunitem4", swa_fillTop  ) );
        menu->appendWidget( new SWidget_MenuItem( SRect(15), "meunitem5", swa_fillTop  ) );
        menu->appendWidget( new SWidget_MenuItem( SRect(15), "meunitem6", swa_fillTop  ) );
        menu->appendWidget( new SWidget_MenuItem( SRect(15), "meunitem7", swa_fillTop  ) );
        menu->appendWidget( new SWidget_MenuItem( SRect(15), "meunitem8", swa_fillTop  ) );
        menu->appendWidget( new SWidget_MenuItem( SRect(15), "meunitem9", swa_fillTop  ) );
      appendWidget( menu );


      //STileMap* tilemap = new STileMap(surface1,SRect(10,10,256,256),8,8);
      //SRect r = tilemap->getTileRect(13);
      //STrace("x %i y %i w %i h %i\n",r.x,r.y,r.w,r.h);
      //delete tilemap;

    }

    //----------

    virtual ~myEditor() {

      unloadFont(MFont);

      delete buffer1;
      delete buffer2;
      //delete bitmap;
      //delete image;
      delete surface1;
      delete surface2;
      delete surface3;
      delete surface4;
      delete tilemap1;
      delete tilemap2;
    }

    //----------

    //virtual
    //void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
    //  SEditor::on_paint(APainter,ARect,AMode);
    //  APainter->setFillColor( SColor(1,1,0,0.1) );
    //  APainter->blendRect(100,100,400,300);
    //}

    //----------


    //virtual
    void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (menu->isOpen()) {
        if (AButton==smb_right) menu->close();
      }
      SEditor::on_mouseDown(AXpos,AYpos,AButton,AState);
    }

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

    //virtual
    void do_hint(SWidget* AWidget, const char* AHint) {
      if (AHint) {
        //if (AHint[0]!=0) {
        //  DTrace("hint from %s: %s\n",AWidget->name(),AHint);
        //}
        hinttext->text(AHint);
        do_redraw(hinttext,hinttext->rect());
      }
    }

};

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

const char* txt1[3] = { "one", "two", "three" };

class myPlugin
: public SPlugin {

  public:

    //virtual
    void on_create(void) {
      MInfo.name          = "test_gui1";
      MInfo.author        = "skei";
      MInfo.product       = MInfo.name;
      MInfo.uniqueId      = SKEI_MAGIC + 0x0000;
      MInfo.numInputs     = 2;
      MInfo.numOutputs    = 2;
      MEditorRect         = SRect(512,384);
      appendParameter( new SParameter(  "param",  0 ));
      appendParameter( new SParamFloat( "float",  0, -5, 5, 0.25 ));
      appendParameter( new SParamInt(   "int",    0,  0, 100 ));
      appendParameter( new SParamText(  "txt",    0,  3, txt1 ));
    }

    //----------

    //virtual
    void on_destroy(void) {
    }

    //----------------------------------------
    // editor
    //----------------------------------------

    //virtual
    void* on_openEditor(void* AParent) {
      myEditor* editor = new myEditor(this,MEditorRect.w,MEditorRect.h,AParent);
      // connect parameters
      //for (int i=0; i<9; i++) connect( (SWidget*)subWidget(i), parameter(i) );
      //connect( (SWidget*)panel1->subWidget(6), parameter(1) );
      return editor;
    }

    //----------

    //virtual
    void on_closeEditor(void* AEditor) {
      SEditor* editor = (SEditor*)AEditor;
      delete editor;
    }

    //----------

    //virtual
    void on_idleEditor(void* AEditor) {
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

