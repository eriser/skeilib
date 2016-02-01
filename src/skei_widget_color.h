#ifndef skei_widget_color_included
#define skei_widget_color_included
//----------------------------------------------------------------------

#include "skei_color.h"
#include "skei_painter.h"
#include "skei_rect.h"
#include "skei_widget.h"

//----------

class SWidget_Color
: public SWidget {

  protected:

    SColor  MColor;

  public:

    SWidget_Color(SRect ARect, SColor AColor, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MColor = AColor;
      MName = "SWidget_Color";
      MHint = "color";
    }

    //----------

    virtual ~SWidget_Color() {
    }

  //----------------------------------------

  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      APainter->setFillColor(MColor);
      APainter->fillRect(MRect);
      SWidget::on_paint(APainter,ARect,AMode);
    }

};

//----------------------------------------------------------------------
#endif
