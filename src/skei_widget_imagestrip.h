#ifndef skei_widget_imagestrip_included
#define skei_widget_imagestrip_included
//----------------------------------------------------------------------

#include "skei_tilemap.h"
#include "skei_widget_value.h"

//----------

class SWidget_ImageStrip
: public SWidget_Value {

  protected:
    //SColor    MBarColor;
    //SColor    MClearColor;
    //SSurface* MSurface;
    //uint32    MCount;
    //uint32    MOrientation;
    STileMap*   MTileMap;

  public:

    SWidget_ImageStrip(SRect ARect, float AValue, STileMap* ATileMap, /*SSurface* ASurface, uint32 ACount, uint32 AOrientation=sto_vertical,*/ uint32 AAlignment=swa_none)
    : SWidget_Value(ARect,AValue,AAlignment) {
      MName = "SWidget_ImageStrip";
      MHint = "ImageStrip";
      MCursor = smc_arrowUpDown;
      MTileMap = ATileMap;
    }

    virtual ~SWidget_ImageStrip() {
    }

  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      //APainter->drawTile(MRect.x,MRect.y,MValue,MSurface,MCount,MOrientation);
      uint32 num = MTileMap->numTiles();
      float idx = (float)MValue * (float)num;
      uint32 index = SMinI( num-1, floorf(idx) );
      SPoint pos = MRect.pos();
      SSurface* surface = MTileMap->surface();
      SRect rect = MTileMap->getTileRect(index);
      APainter->blendSurface(pos,surface,rect);
    }

};

//----------------------------------------------------------------------
#endif


