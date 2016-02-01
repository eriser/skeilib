#ifndef skei_widget_buttonrow_included
#define skei_widget_buttonrow_included
//----------------------------------------------------------------------

#include "skei_widget_grid.h"

//----------

#define SKEI_MAX_STATES 32

#define kbm_single  0
#define kbm_multi   1

//----------

class SWidget_ButtonRow : public SWidget_Grid {

  protected:

    int32   MMode;
    bool    MStates[SKEI_MAX_STATES];
    char*   MLabels[SKEI_MAX_STATES];
    int32   MSelected;
    //SColor MValueColor;
    //SColor MBackColor;
    SColor MTextColor;
    SColor MActiveColor;

  public:

    //property    _mode : LongInt read FMode write FMode;
    int32 selected(void) { return MSelected; }

  public:

    SWidget_ButtonRow(SRect ARect, int32 ANum, uint32 AAlignment=swa_none)
    : SWidget_Grid(ARect,ANum,1,AAlignment) {
      MName = "KWidget_ButtonRow";
      MHint = "buttonrow";
      MMode = kbm_single;
      //MWidth = ANum;//5;
      //MHeight = 1;
      for (int32 i=0; i<SKEI_MAX_STATES; i++) {
        MStates[i] = false;
        MLabels[i] = SKEI_NULL;
      }
      MSelected = 0;
      MStates[0] = true;
      MCursor = smc_finger;
      MTextColor = SWhite;
      MActiveColor = SDarkGrey;
      MBackColor = SGrey;
    }

    //void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
    //}

    //void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    //}

  public:

    //virtual
    void on_clickCell(int32 AX, int32 AY, int32 AB) {
      select(AX);
      do_redraw(this,MRect);
      //do_update(this); (( widget_grid already sends do_update..
    }

    //virtual
    void on_paintCell(SPainter* APainter, SRect ARect, int32 AX, int32 AY) {
      char buf[256];
      if (MStates[AX]) APainter->setFillColor(MActiveColor);
      else APainter->setFillColor(MBackColor);
      APainter->fillRect( ARect );
      APainter->setTextColor(MTextColor);
      char* txt = MLabels[AX];
      if (txt) {
        APainter->drawText(ARect,txt,sta_center);
      }
      else {
        SIntToString(buf,AX);
        APainter->drawText(ARect,buf,sta_center);
      }
    }

    //----------

    bool getState(int32 i) {
      return MStates[i];
    }

    //----------

    void setState(int32 i, bool s) {
      MStates[i] = s;
    }

    //----------

    char* getName(int32 i) {
      return MLabels[i];
    }

    //----------

    void setName(int32 i, char* AName) {
      MLabels[i] = AName;
    }

    //----------

    void select(int32 index/*, bool ARedraw=false*/) {
      MSelected = index;
      //KTrace(['click cell: ',AX,',',AY,',',AB,KODE_CR]);
      if (MMode == kbm_single) {
        for (int32 i=0; i<MWidth; i++) { // FHeight if vertical
          if (i==MSelected) MStates[i] = true;
          else MStates[i] = false;
        }
        /*
        5 steps => 0,1,2,3,4 => 0, 0.25, 0.50, 0.75, 1.0
        selected/(steps-1)
        */
        MValue = MSelected / (MWidth-1);
        //KTrace(['KWidget_ButtonRow.select. FValue = ',FValue,KODE_CR]);
      }
      else {
        MStates[MSelected] = MStates[MSelected] ? false : true;
      }
      //if (ARedraw) do_redraw(this,MRect);
    }

    //----------

    //function KWidget_ButtonRow.getValue: Single;
    //begin
    //  result := FValue;
    //end;

    void setValue(float AValue) {
      float num = AValue * MWidth; // 0 = 0, 1 = 4, 0.999 = 3
      num = SMin(num,MWidth-1);
      select( (int)num );
      //do_redraw(self,FRect);
      //do_update(self);
    }

};

#undef SKEI_MAX_STATES

//----------------------------------------------------------------------
#endif
