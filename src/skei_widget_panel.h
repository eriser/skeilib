#ifndef skei_widget_panel_included
#define skei_widget_panel_included
//----------------------------------------------------------------------

#include "skei_color.h"
#include "skei_painter.h"
#include "skei_rect.h"
#include "skei_widget.h"

//----------

class SWidget_Panel
: public SWidget {

  private:

    SColor  MLightColor;
    SColor  MDarkColor;
    SColor  MFillColor;

  public:

    void lightcolor(SColor AColor) { MLightColor = AColor; }
    void darkcolor(SColor AColor) { MDarkColor = AColor; }
    void fillcolor(SColor AColor) { MFillColor = AColor; }

  public:

    SWidget_Panel(SRect ARect, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_Panel";
      MHint = "panel";
      MLightColor = SLightGrey;
      MDarkColor = SDarkGrey;
      MFillColor = SGrey;
    }

    //----------

    virtual ~SWidget_Panel() {
    }

  //----------------------------------------
  // widget listener
  //----------------------------------------

  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      APainter->setFillColor( MFillColor );
      APainter->fillRect( MRect);
      APainter->setDrawColor( MLightColor );
      APainter->drawLine( MRect.x, MRect.y, MRect.x2()-1, MRect.y     );
      APainter->drawLine( MRect.x, MRect.y, MRect.x,      MRect.y2()-1 );
      APainter->setDrawColor( MDarkColor );
      APainter->drawLine( MRect.x+1,  MRect.y2(), MRect.x2(), MRect.y2() );
      APainter->drawLine( MRect.x2(), MRect.y+1,  MRect.x2(), MRect.y2() );
      SWidget::on_paint(APainter,ARect,AMode);
    }

};
//----------------------------------------------------------------------
#endif
