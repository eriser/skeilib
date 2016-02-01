#ifndef skei_widget_surface_included
#define skei_widget_surface_included
//----------------------------------------------------------------------

#include "skei_surface.h"
#include "skei_widget.h"

//----------

class SWidget_Surface
: public SWidget {

  private:
    SSurface* MSurface;
    uint32    MDrawMode;

  public:

    SWidget_Surface(SRect ARect, SSurface* ASurface, uint32 ADrawMode=ssm_draw, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_Surface";
      MHint = "surface";
      MSurface = ASurface;
      MDrawMode = ADrawMode;
    }

    virtual ~SWidget_Surface() {
    }

    void surface(SSurface* ASurface) {
      MSurface = ASurface;
    }

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 ADrawMode=0) {
      //STrace("%i,%i\n",MRect.w,MRect.h);
      if (MSurface) {
        switch(MDrawMode) {
          case ssm_draw:
            APainter->drawSurface(MRect.x,MRect.y,MSurface,0,0,MSurface->width(),MSurface->height() );
            break;
          case ssm_blend:

            APainter->setFillColor( SColor(0,1,1,0.3) );
            APainter->fillTri(MRect.x, MRect.y, MRect.x+100, MRect.y+50, MRect.x+50, MRect.y+100 );

            APainter->blendSurface(MRect.x,MRect.y,MSurface,0,0,MSurface->width(),MSurface->height() );

            //APainter->setFillColor( SColor(1,0,0,0.3) );
            //APainter->blendRect(MRect);

            break;
          case ssm_stretch:
            APainter->stretchSurface(MRect.x,MRect.y,MRect.w,MRect.h,MSurface,0,0,MSurface->width(),MSurface->height() );
            break;
        } // switch
      }
    }

};

//----------------------------------------------------------------------
#endif

