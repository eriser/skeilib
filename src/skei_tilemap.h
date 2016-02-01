#ifndef skei_tilemap_included
#define skei_tilemap_included
//----------------------------------------------------------------------

#include "skei_surface.h"

//----------

class STileMap {

  private:

    SSurface* MSurface;
    SRect     MRect;
    uint32    MXcount;
    uint32    MYcount;
    uint32    MTileWidth;
    uint32    MTileHeight;
    uint32    MNumTiles;

  public:

    STileMap(SSurface* ASurface, SRect ARect, uint32 AXcount, uint32 AYcount) {

      MSurface    = ASurface;
      MRect       = ARect;
      MXcount     = AXcount;
      MYcount     = AYcount;
      MTileWidth  = ARect.w / AXcount;
      MTileHeight = ARect.h / AYcount;
      MNumTiles   = MXcount * MYcount;
      //STrace("tilewidth %i tileheight %i\n",MTileWidth,MTileHeight);
    }

    //----------

    virtual ~STileMap() {
    }

    //--------------------------------------------------

    SSurface* surface(void)   { return MSurface; }
    uint32    numTiles(void)  { return MNumTiles; }

    //--------------------------------------------------

    SRect getTileRect(uint32 AIndex) {
      uint32 x = MRect.x + ( (AIndex % MXcount) * MTileWidth );
      uint32 y = MRect.y + ( (AIndex / MXcount) * MTileHeight );
      uint32 w = (MTileWidth-1);
      uint32 h = (MTileHeight-1);
      //STrace("> getTileRect: %i, %i, %i, %i\n",x,y,w,h);
      return SRect(x,y,w,h);
    }


      /*
      float s;
      int32 x,y,w,h,tilecount,num;
      switch(MOrientatATileOrientation) {
        case sto_vertical:
          tilecount = ACount;//ASurface->height() / ASurface->width();
          s = (float)tilecount * AValue;
          num = SMinI( tilecount-1, floorf(s) );
          //STrace("num %i\n",num);
          w = ASurface->width();
          h = ASurface->height() / ACount; // ASurface->width();
          //STrace("h %i\n",h);
          x = 0;
          y = h*num;
          break;
        case sto_horizontal:
          tilecount = ACount;//ASurface->width() / ASurface->height();
          s = (float)tilecount * AValue;
          num = SMinI( tilecount-1, floorf(s) );
          //STrace("num %i\n",num);
          w = ASurface->width() / ACount;
          h = ASurface->height();
          x = w*num;
          y = 0;
          break;
      } // switch
      //blendSurface(ADstX,ADstY,ASurface,x,y,x+w-1,y+h-1);
      blendSurface(ADstX,ADstY,ASurface,x,y,w,h);
      */

};


//----------------------------------------------------------------------
#endif
