#ifndef skei_widget_sliderbank_included
#define skei_widget_sliderbank_included
//----------------------------------------------------------------------

#include "skei_random.h"
#include "skei_widget.h"

//----------

#define SKEI_MAX_NUM_SLIDERS 256

class SWidget_SliderBank
: public SWidget {

  private:

    int32   MHoverSlider;
    int32   MNumSliders;
    SColor  MBarColor;
    SColor  MHoverColor;
    float   MSliderValues[SKEI_MAX_NUM_SLIDERS];


  public:

    SWidget_SliderBank(SRect ARect, int32 ANum, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_SliderBank";
      MHint = "sliderbank";
      MNumSliders = ANum;
      MHoverSlider = -1;
      MBarColor = SDarkGrey;
      MHoverColor = SLightGrey;
      //SMemset(MSliderValues,0,sizeof(MSliderValues));
      for (int32 i=0; i<SKEI_MAX_NUM_SLIDERS; i++) MSliderValues[i] = SRandom();
    }

    virtual ~SWidget_SliderBank() {
    }

  public:

    void numSliders(int32 ANum) { MNumSliders = ANum; }
    void barColor(SColor AColor) { MBarColor = AColor; }

  //public:
  //
  //  void drawBar(SPainter* APainter, int32 AIndex) {
  //    float w = ( (float)MRect.w / (float)MNumSliders );
  //    float x = (float)MRect.x + (w * AIndex);
  //    float y = MRect.y2() - floorf(MSliderValues[AIndex] * MRect.h);
  //    APainter->fillRect( floorf(x), y/*MRect.y*/, floorf(x+w)-2, MRect.y2() );
  //  }

  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      //STrace("painting %s\n",MName);
      if (MNumSliders > 1) {
        float w = ( (float)MRect.w / (float)MNumSliders );
        float x = (float)MRect.x /*+ sliderwidth - 1*/;
        for (int32 i=0; i<MNumSliders; i++) {
          if (i==MHoverSlider) APainter->setDrawColor( MHoverColor );
          else APainter->setDrawColor( MBarColor );
          float y = MRect.y2() - floorf(MSliderValues[i] * MRect.h);
          APainter->fillRect( floorf(x), y, floorf(x+w)-2, MRect.y2() );
          x += w;
        }
      }
    }

    //----------

    //virtual
    void on_mouseMove(int32 AXpos, int32 AYpos, int32 AState) {
      //SWidget::on_mouseMove(AXpos,AYpos,AState);
      float w = (float)(AXpos - MRect.x) / (float)MRect.w; // 0..1
      int32 index = (w * (float)MNumSliders);
      MHoverSlider = index;
      do_redraw(this,MRect);
    }

    //----------

    //virtual
    void on_leave(SWidget* AWidget) {
      //SWidget::on_leave(AWidget);
      if (MHoverSlider>=0) {
        MHoverSlider = -1;
        do_redraw(this,MRect);
      }
    }

  /*

  public:

    virtual void on_reset(int32 AMode=0) {}
    virtual void on_setPos(int32 AXpos, int32 AYpos) {}
    virtual void on_setSize(int32 AWidth, int32 AHeight) {}
    virtual void on_scroll(int32 ADeltaX, int32 ADeltaY) {}
    virtual void on_align(void) {}
    virtual void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {}
    virtual void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_mouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_mouseMove(int32 AXpos, int32 AYpos, int32 AState) {}
    virtual void on_mouseDoubleClick(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_keyDown(int32 AKey, int32 AState) {}
    virtual void on_keyUp(int32 AKey, int32 AState) {}
    virtual void on_enter(SWidget* AWidget) {}
    virtual void on_leave(SWidget* AWidget) {}

  public:

    virtual void do_move(SWidget* AWidget, int32 AXpos, int32 AYpos) {}
    virtual void do_sizer(SWidget* AWidget, int32 ADeltaX, int32 ADeltaY, int32 AMode) {}
    virtual void do_resize(SWidget* AWidget, int32 AWidth, int32 AHeight) {}
    virtual void do_redraw(SWidget* AWidget, SRect ARect) {}
    virtual void do_update(SWidget* AWidget) {}
    virtual void do_cursor(SWidget* AWidget, int32 ACursor) {}
    virtual void do_hint(SWidget* AWidget, const char* AHint) {}
    virtual void do_modal(SWidget* AWidget, int32 AMode=0) {}
    virtual void do_wantkeys(SWidget* AWidget) {}

  */

};

//----------

#undef SKEI_MAX_NUM_SLIDERS


//----------------------------------------------------------------------
#endif

