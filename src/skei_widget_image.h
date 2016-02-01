#ifndef skei_widget_image_included
#define skei_widget_image_included
//----------------------------------------------------------------------

#include "skei_image.h"
#include "skei_widget.h"

//----------

class SWidget_Image
: public SWidget {

  private:
    SImage* MImage;

  public:

    SWidget_Image(SRect ARect, SImage* AImage, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_Image";
      MHint = "image";
      MImage = AImage;//SKEI_NULL;
    }

    virtual ~SWidget_Image() {
    }

    void image(SImage* AImage) {
      MImage = AImage;
    }

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      if (MImage) {
        APainter->drawImage(MRect.x,MRect.y,MImage,0,0,MImage->width(),MRect.h);
      }
    }

};

//----------------------------------------------------------------------
#endif
