#ifndef skei_widget_keyboard_included
#define skei_widget_keyboard_included
//----------------------------------------------------------------------

/*
  mind-storm:
    black or white, black = 1 rect, white = 2 rect
    only 2 numbers per rectangle is unique
    use rectangles for hit testing (mouse click)
    precalc them in setup(scale,dist,y1,y2) ?
    array with 12 elememnts, one for each note in octave
    1 find octave
    octave width = mScale*3*7
    so, a key could be
    - int black/white
    -  num1,num2
    -  num1,num2
*/

//
//  |  |4| | |  |
//  |  |_| |_|  |
//  |1  |2  |3  |
//  |___|___|___|
//

#include "skei_widget.h"

//----------

// ofs,type

int32 internal_note_data[12*2] = {
  0,0,
  2,3,
  1,1,
  2,3,
  1,2,
  3,0,
  2,3,
  1,1,
  2,3,
  1,1,
  2,3,
  1,2
};


class SWidget_Keyboard
: public SWidget {

  private:

    bool    MActiveMap[12];
    int32   MNoteRects1[12*4];
    int32   MNoteRects2[12*4];

  protected:

    int32   MDist;
    float   MRatio;
    SColor  MBlackColor;
    SColor  MWhiteColor;
    SColor  MBlackActiveColor;
    SColor  MWhiteActiveColor;

  public:

    SWidget_Keyboard(SRect ARect, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_Keyboard";
      MHint = "keyboard";
      MDist = 2;
      MRatio = 0.7;
      MWhiteColor       = SLightGrey;
      MWhiteActiveColor = SWhite;
      MBlackColor       = SDarkGrey;
      MBlackActiveColor = SBlack;
      for (int32 i=0; i<12; i++) MActiveMap[i] = false;
      recalc();
      MCursor = smc_finger;
    }

    virtual ~SWidget_Keyboard() {
    }

  public:

    void setColors(SColor AWhite, SColor ABlack, SColor AWhiteActive, SColor ABlackActive) {
      MWhiteColor       = AWhite;
      MWhiteActiveColor = AWhiteActive;
      MBlackColor       = ABlack;
      MBlackActiveColor = ABlackActive;
    }

    void setDist(int32 ADist) {
      MDist = ADist + 1;
    }

    void setRatio(float ARatio) {
      MRatio = ARatio;
    }

    void activate(int32 num) {
      MActiveMap[num] = true;
    }

    void deactivate(int32 num) {
      MActiveMap[num] = false;
    }

    //----------

    void recalc(void) {
    /*var
      width : longint;
      step1,step2,step3 : single;
      x1 : single;
      y1,y2 : longint;
      note,n2,n4,iofs : longint;
      ofs,x1s1,x1s2,x1s3:single;
      typ:longint;*/

      int32 width = MRect.w;
      float step1 = (float)width / 21.0;
      float step2 = step1 * 2;
      float step3 = step1 * 3;
      float x1    = 0;
      int32 y1    = floorf((float)MRect.h * MRatio);
      int32 y2    = MRect.h - 1;
      for (int32 note=0; note<12; note++) {
        int32 n2    = note+note;
        int32 n4    = n2+n2;
        int32 iofs  = internal_note_data[note*2];
        float ofs   = (float)iofs * step1;
        x1         += ofs;
        float x1s1  = x1 + step1;
        float x1s2  = x1 + step2;
        float x1s3  = x1 + step3;
        int32 typ   = internal_note_data[(note*2)+1];
        switch(typ) {
          case 0: // C
            MNoteRects1[n4+0] = floorf(x1);            MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1s2) - MDist;  MNoteRects1[n4+3] = y1-1;
            MNoteRects2[n4+0] = floorf(x1);            MNoteRects2[n4+1] = y1;
            MNoteRects2[n4+2] = floorf(x1s3) - MDist;  MNoteRects2[n4+3] = y2;
            break;
          case 1: // D
            MNoteRects1[n4+0] = floorf(x1s1);          MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1s2) - MDist;  MNoteRects1[n4+3] = y1-1;
            MNoteRects2[n4+0] = floorf(x1);            MNoteRects2[n4+1] = y1;
            MNoteRects2[n4+2] = floorf(x1s3) - MDist;  MNoteRects2[n4+3] = y2;
            break;
          case 2: // E
            MNoteRects1[n4+0] = floorf(x1s1);          MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1s3) - MDist;  MNoteRects1[n4+3] = y1-1;
            MNoteRects2[n4+0] = floorf(x1);            MNoteRects2[n4+1] = y1;
            MNoteRects2[n4+2] = floorf(x1s3) - MDist;  MNoteRects2[n4+3] = y2;
            break;
          case 3: // black
            MNoteRects1[n4+0] = floorf(x1);            MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1s2) - MDist;  MNoteRects1[n4+3] = y1 - MDist;
            break;
        } // case
      } // for note
    }

    //----------

  public:

    //virtual
    void on_setSize(int32 AWidth, int32 AHeight) {
      SWidget::on_setSize(AWidth,AHeight);
      recalc();
    }

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      for (int32 note=0; note<12; note++) {
        int32 n2 = note*2;
        int32 n4 = note*4;
        bool active = MActiveMap[note];
        int32 typ = internal_note_data[n2+1];
        if (typ == 3) {
          if (active) APainter->setFillColor(MBlackActiveColor);
          else APainter->setFillColor(MBlackColor);
          APainter->fillRect( MRect.x + MNoteRects1[n4],  MRect.y + MNoteRects1[n4+1],
                            MRect.x + MNoteRects1[n4+2],MRect.y + MNoteRects1[n4+3] );
        }
        else {
          if (active) APainter->setFillColor(MWhiteActiveColor);
          else APainter->setFillColor(MWhiteColor);
          APainter->fillRect( MRect.x + MNoteRects1[n4  ], MRect.y + MNoteRects1[n4+1],
                              MRect.x + MNoteRects1[n4+2], MRect.y + MNoteRects1[n4+3] );
          APainter->fillRect( MRect.x + MNoteRects2[n4  ], MRect.y + MNoteRects2[n4+1],
                              MRect.x + MNoteRects2[n4+2], MRect.y + MNoteRects2[n4+3] );
        } //type=0,1,2
      } //for note;
    }

    //virtual
    void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    //var
    //  hit,note,n2,n4,typ : longint;
    //  rec : KRect;
      if (AButton == smb_left) {
        SRect rec;
        int32 hit = -1;
        for (int32 note=0; note<12; note++) {
          int32 n4 = note*4;
          rec.x =  MRect.x + MNoteRects1[n4  ];
          rec.y =  MRect.y + MNoteRects1[n4+1];
          rec.w = (MRect.x + MNoteRects1[n4+2]) - rec.x;
          rec.h = (MRect.y + MNoteRects1[n4+3]) - rec.y;
          if (rec.contains(AXpos,AYpos)) {
            hit = note;
            if (MActiveMap[note]) deactivate(note);
            else activate(note);
            do_update(this);
            do_redraw(this,MRect);
          } // contain
          if (hit < 0) { // check lower part?
            int32 n2 = note*2;
            int32 typ = internal_note_data[n2+1];
            if (typ < 3) { //0,1,2 = white keys
              rec.x =  MRect.x + MNoteRects2[n4  ];
              rec.y =  MRect.y + MNoteRects2[n4+1];
              rec.w = (MRect.x + MNoteRects2[n4+2]) - rec.x;
              rec.h = (MRect.y + MNoteRects2[n4+3]) - rec.y;
              if (rec.contains(AXpos,AYpos)) {
                if (MActiveMap[note]) deactivate(note);
                else activate(note);
                do_update(this);
                do_redraw(this,MRect);
              } //contains
            } //type 0,1,2
          } //hit
        } // for note
      }
      //axWidget::doMouseDown(aXpos,aYpos,aButton);
    }

  //

};

//----------------------------------------------------------------------
#endif

