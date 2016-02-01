#ifndef skei_widget_text_included
#define skei_widget_text_included
//----------------------------------------------------------------------

#include "skei_widget.h"

//----------

class SWidget_Text
: public SWidget {

  protected:
    const char* MText;
    SColor      MTextColor;
    uint32      MTextAlignment;
    SColor      MBackColor;
    bool        MDrawBackground;

  public:

    void text(const char* AText) { MText = AText; }

  public:

    SWidget_Text(SRect ARect, const char* AText, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_Text";
      MHint = "text";
      MText           = AText;
      MBackColor      = SLightGrey;
      MTextColor      = SBlack;
      MDrawBackground = false;
      MTextAlignment  = sta_left;
    }

    //----------

    virtual ~SWidget_Text() {
    }

  //----------------------------------------

  public:
    const char* text(void) { return MText; }
    //SColor      MTextColor;
    //uint32      MTextAlignment;
    //SColor      MBackColor;
    //bool        MDrawBackground;



  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      if (MDrawBackground) {
        APainter->setFillColor(MBackColor);
        APainter->fillRect(MRect);
      }
      APainter->setTextColor(MTextColor);
      APainter->drawText(MRect,MText,MTextAlignment);
      SWidget::on_paint(APainter,ARect,AMode);
    }

    //----------

};

//----------------------------------------------------------------------
#endif

