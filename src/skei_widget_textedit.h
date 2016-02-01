#ifndef skei_widget_textedit_included
#define skei_widget_textedit_included
//----------------------------------------------------------------------

#include "skei_widget.h"

//----------

class SWidget_TextEdit
: public SWidget {

  private:

    char    MText[256];
    SColor  MTextColor;
    SColor  MBackColor;
    SColor  MCaretColor;
    bool    MEditing;
    int32   MCaretPos;

  public:

    SWidget_TextEdit(SRect ARect, const char* AText, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_TextEdit";
      MHint = "textedit";
      SStrncpy(MText,AText,255);
      MTextColor  = SBlack;
      MBackColor  = SLightGrey;
      MCaretColor = SBrightRed;
      MEditing    = false;
      MCaretPos   = 0;
    }

    virtual ~SWidget_TextEdit() {
    }

  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      SWidget::on_paint(APainter,ARect,AMode);
      APainter->setFillColor(MBackColor);
      APainter->fillRect(MRect);
      if (MEditing) {
        APainter->setTextColor(MTextColor);
        APainter->drawText(MRect.x+2,MRect.y,MRect.x2()-2,MRect.y2(),MText,sta_left);

        char c = MText[MCaretPos];
        MText[MCaretPos] = 0;
        int32 txtwidth = APainter->getTextWidth(MText);
        MText[MCaretPos] = c;
        int32 x = MRect.x + 2 + txtwidth;

        APainter->setDrawColor(MCaretColor);
        APainter->drawLine(x,MRect.y,x,MRect.y2());

      } // editing
      else {
        APainter->setTextColor(MTextColor);
        APainter->drawText(MRect.x+2,MRect.y,MRect.x2()-4,MRect.y2(),MText,sta_left);
      } // not editing
    }

    ////----------------------------------------
    //
    //{
    //  assume APos is valid
    //}
    //
    //// pos = 2
    //// len = 6
    //// 01234567
    //// ..::::0
    //
    //procedure insertChar(ABuffer:PChar; APos:LongInt; AChar:Char);
    //var
    //  i,len : longint;
    //begin
    //  if not Assigned(ABuffer) then exit;
    //  len := KStrlen(ABuffer);
    //  for i := (len+1) downto (APos+1) do ABuffer[i] := ABuffer[i-1];
    //  ABuffer[Apos] := AChar;
    //end;
    //
    ////----------
    //
    //procedure deleteChar(ABuffer:PChar; APos:LongInt);
    //var
    //  i,len : longint;
    //begin
    //  if not Assigned(ABuffer) then exit;
    //  len := KStrlen(ABuffer);
    //  for i := Apos to len do ABuffer[i] := ABuffer[i+1];
    //end;

    //----------------------------------------

    //virtual
    void on_keyDown(int32 AKey, int32 AState) {
    //var
    //  c : char;
    //  len : longint;
    ////  changed : boolean;

      int32 len;
      char  c;

      SWidget::on_keyDown(AKey,AState);

      //STrace("key: %i, skift: %i\n",AKey,AState);

      //changed := false;
      switch(AKey) {
        case skc_enter:
          MEditing = false;
          do_wantkeys(SKEI_NULL);
          do_update(this);
          do_redraw(this,MRect);
          //changed := true;
          //KTrace(['"',MText,'"',KODE_CR])
          break;
        case skc_esc:
          MEditing = false;
          do_wantkeys(SKEI_NULL);
          do_update(this);
          do_redraw(this,MRect);
          //changed = true;
          break;
        case skc_home:
          MCaretPos = 0;
          //changed = true;
          do_update(this);
          do_redraw(this,MRect);
          break;
        case skc_end:
          len = SStrlen(MText);
          MCaretPos = len;
          //changed = true;
          do_update(this);
          do_redraw(this,MRect);
          break;
        case skc_left:
          MCaretPos -= 1;
          if (MCaretPos < 0) MCaretPos = 0;
          //changed = true;
          do_update(this);
          do_redraw(this,MRect);
          break;
        case skc_right:
          len = SStrlen(MText);
          MCaretPos += 1;
          if (MCaretPos > len) MCaretPos = len;
          //changed = true;
          do_update(this);
          do_redraw(this,MRect);
          break;
        case skc_delete:
          len = SStrlen(MText);
          if ((uint32)MCaretPos < SStrlen(MText)) {
            SDeleteChar(MText,MCaretPos);
            do_update(this);
            do_redraw(this,MRect);
          }
          break;
        case skc_backspace:
          if (MCaretPos > 0) {
            MCaretPos -= 1;
            SDeleteChar(MText,MCaretPos);
            do_update(this);
            do_redraw(this,MRect);
          }
          break;
        default:
          if ((AKey >= 32) && (AKey <= 127)) {
            c = AKey & 0xff;
            SInsertChar(MText,MCaretPos,c);
            MCaretPos += 1;
            //if FCaretPos > len then FCaretPos = len;
            do_update(this);
            do_redraw(this,MRect);
            //changed = true;
          }
          break;
      } // switch
      //if changed then
      //begin
      //  do_update(this);
      //  do_redraw(this,MRect);
      //end;
    }

    //virtual
    //void on_keyUp(int32 AKey, int32 AState) {
    //}

    //virtual
    void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      SWidget::on_mouseDown(AXpos,AYpos,AButton,AState);
      MEditing = true;
      MCaretPos = SStrlen(MText);
      //do_update(this);
      do_redraw(this,MRect);
      //do_modal(this);
      do_wantkeys(this);
    }

};

//----------------------------------------------------------------------
#endif

