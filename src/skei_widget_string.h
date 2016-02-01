#ifndef skei_widget_string_included
#define skei_widget_string_included
//----------------------------------------------------------------------

#include "skei_widget.h"

//----------

class SWidget_String
: public SWidget {

  protected:
    //const char* MText;
    SString*    MString;
    SColor      MTextColor;
    uint32      MTextAlignment;
    SColor      MBackColor;
    bool        MDrawBackground;

  public:

    //void text(const char* AText) { MText = AText; }

  public:

    SWidget_String(SRect ARect, SString* AString, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_String";
      MHint = "text";
      //MText           = AText;
      MString         = AString;
      MBackColor      = SLightGrey;
      MTextColor      = SBlack;
      MDrawBackground = false;
      MTextAlignment  = sta_left;
    }

    //----------

    virtual ~SWidget_String() {
    }

  //----------------------------------------

  public:

    void        string(SString* AString) { MString = AString; }
    const char* text(void) { return MString->ptr(); }
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
      APainter->drawText(MRect,MString->ptr(),MTextAlignment);
      SWidget::on_paint(APainter,ARect,AMode);
    }

    //----------

};

//----------------------------------------------------------------------
#endif

