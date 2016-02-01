#ifndef skei_widget_waveform_included
#define skei_widget_waveform_included
//----------------------------------------------------------------------

#include "skei_color.h"
#include "skei_widget.h"

struct SWaveformMarker {
  int32   pos;
  SColor  color;
};

struct SWaveformArea {
  int32   pos;
  int32   size;
  SColor  color;
};

//typedef SArray<SWaveformMarker> SWaveformMarkers;
//typedef SArray<SWaveformArea> SWaveformAreas;

//----------

class SWidget_Waveform
: public SWidget {

  private:
    float*            MBuffer;
    int32             MBufferSize;
  protected:
    SColor            MBackColor;
    SColor            MWaveColor;
    SColor            MGridColor;
    bool              MDrawBackground;
    int32             MNumMarkers;
    int32             MNumAreas;
    SWaveformMarker*  MMarkers;
    SWaveformArea*    MAreas;
    int32             MNumGrid;
    bool              MMono;

  public:

    SWidget_Waveform(SRect ARect, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName           = "SWidget_Waveform";
      MHint           = "waveform";
      MBuffer         = SKEI_NULL;
      MBufferSize     = 0;
      MBackColor      = SBlack;      // SGrey;
      MWaveColor      = SLightGrey;
      MGridColor      = SGrey;
      MDrawBackground = true;
      MNumMarkers     = 0;
      MNumAreas       = 0;
      MMarkers        = SKEI_NULL;
      MAreas          = SKEI_NULL;
      MNumGrid        = 0;
      MMono           = false;
    }

    virtual ~SWidget_Waveform() {
      if (MMarkers) SFree(MMarkers);
      if (MAreas) SFree(MAreas);
    }

    //----------

    void setBuffer(float* ABuffer) {
      MBuffer = ABuffer;
    }

    void setBufferSize(int32 ASize) {
      MBufferSize = ASize;
    }

    void setBackColor(SColor AColor) {
      MBackColor = AColor;
    }

    void setWaveColor(SColor AColor) {
      MWaveColor = AColor;
    }

    void drawBackground(bool ADraw=true) {
      MDrawBackground = ADraw;
    }

    void setNumMarkers(int32 ANum) {
      MNumMarkers = ANum;
      //SetLength(MMarkers,ANum);
      if (MMarkers) SFree(MMarkers);
      MMarkers = (SWaveformMarker*)SMalloc(ANum * sizeof(SWaveformMarker));
    }

    void setNumAreas(int32 ANum) {
      MNumAreas = ANum;
      //SetLength(MAreas,ANum);
      if (MAreas) SFree(MAreas);
      MAreas = (SWaveformArea*)SMalloc(ANum * sizeof(SWaveformArea));
    }

    void setMarker(int32 AIndex, int32 APos, SColor AColor) {
      MMarkers[AIndex].pos = APos;
      MMarkers[AIndex].color = AColor;
    }

    void setMarkerPos(int32 AIndex, int32 APos) {
      MMarkers[AIndex].pos = APos;
    }

    void setMarkerColor(int32 AIndex, SColor AColor) {
      MMarkers[AIndex].color = AColor;
    }

    void setArea(int32 AIndex, int32 APos, int32 ASize, SColor AColor) {
      MAreas[AIndex].pos = APos;
      MAreas[AIndex].size = ASize;
      MAreas[AIndex].color = AColor;
    }

    void setAreaPos(int32 AIndex, int32 APos) {
      MAreas[AIndex].pos = APos;
    }

    void setAreaPosSize(int32 AIndex, int32 APos, int32 ASize) {
      MAreas[AIndex].pos = APos;
      MAreas[AIndex].size = ASize;
    }

    void setAreaSize(int32 AIndex, int32 ASize) {
      MAreas[AIndex].size = ASize;
    }

    void setAreaColor(int32 AIndex, SColor AColor) {
      MAreas[AIndex].color = AColor;
    }

    void setNumGrid(int32 ANum) {
      MNumGrid = ANum;
    }

    void setMono(bool AMono=true) {
      MMono = AMono;
    }

  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {

      //SAssert(MBufferSize>0);

      if (MRect.w > 0 ) {

        float h2 = (float)MRect.h * 0.5;
        int32 midy = MRect.y + (MRect.h >> 1);

        /* background */

        if (MDrawBackground) {
          APainter->setFillColor(MBackColor);
          APainter->fillRect(MRect);
        }

        if (MBufferSize > 0) {

          /* areas */

          for (int32 i=0; i<MNumAreas; i++) {
            if (MAreas[i].size > 0) {
              float x = (float)MAreas[i].pos / (float)MBufferSize;
              float w = (float)MAreas[i].size / (float)MBufferSize;
              int32 ix = MRect.x + ( STrunc( x * MRect.w ) );
              int32 iw = ix + STrunc( w * MRect.w );
              APainter->setFillColor(MAreas[i].color);
              APainter->fillRect(ix,MRect.y,iw,MRect.y2());
            }
          }

          /* waveform */

          float xadd = (float)MBufferSize / (float)MRect.w;
          if (MBuffer) {
            APainter->setDrawColor(MWaveColor);
            float x = 0;
            //APainter->drawLine( MRect.x, midy, MRect.x2(), midy );
            for (int32 i=0; i<MRect.w; i++) {
              int32 index = (int)x;
              float s;// = 0;
              if (MMono) s = MBuffer[index];
              else s = ( MBuffer[ index*2 ] + MBuffer[ index*2 + 1 ] ) * 0.5;
              s *=  h2;
              int32 ix = MRect.x + i;
              int32 iy = (int)s;
              APainter->drawLine( ix, midy, ix, midy-iy );
              x += xadd;
            }
          }

          /* markers */

          for (int32 i=0; i<MNumMarkers; i++) {
            float x = MMarkers[i].pos / (float)MBufferSize;
            //STrace("x %f\n",x);
            if ((x>=0.0f) && (x<=1.0f)) {
              int32 ix = MRect.x + ( (int)( x * MRect.w ) );
              APainter->setDrawColor(MMarkers[i].color);
              APainter->drawLine( ix, MRect.y, ix, MRect.y2() );
            }
          }

        } // MBufferSize > 0

        /* grid */

        if (MNumGrid > 1) {
          float xadd = (float)MRect.w / (float)MNumGrid;
          float x = MRect.x + xadd;
          APainter->setDrawColor(MGridColor);
          for (int32 i=1; i<MNumGrid; i++) {
            int32 ix = STrunc(x);
            APainter->drawLine( ix, MRect.y, ix, MRect.y2() );
            x += xadd;
          }
        }

      } // w>0

      SWidget::on_paint(APainter,ARect,AMode);

    }

};

//----------------------------------------------------------------------
#endif
