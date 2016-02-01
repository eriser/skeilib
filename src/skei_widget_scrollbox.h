#ifndef skei_widget_scrollbox_included
#define skei_widget_scrollbox_included
//----------------------------------------------------------------------

/*
  this is a bit clumsy/hack-ish..
  that un_scroll and re_scroll things..

  one minor issue:
  when resizing the window (not using scrollbar), in do_update,
  scrollbox is one pixel off (top)
*/


#include "skei_widget.h"
#include "skei_widget_scrollbar.h"

//----------

class SWidget_ScrollBox
: public SWidget {

  protected:

    SWidget_ScrollBar*  MScrollBar;
    SWidget*            MContainer;   // has background
    int32               MScrolledX;   // how much we've already scrolled
    int32               MScrolledY;   // -_"--
    float               MVisible;     // how much of the content is visible (calculated in doRealign)

  public:

    SWidget* container(void) { return MContainer; }
    SWidget* scrollbar(void) { return MScrollBar; }

  public:

    SWidget_ScrollBox(SRect ARect, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_ScrollBox";
      MHint = "scrollbox";
      //KClearFlag(FFlags,kwf_opaque);
      MScrollBar = new SWidget_ScrollBar(SRect(10),0,true,swa_fillRight);
      MContainer = new SWidget(SRect(0), swa_fillClient);
      MScrollBar->setFlag(swf_vertical);
      MScrollBar->thumbsize(1);
      MContainer->setFlag(swf_clip);
      SWidget::appendWidget( MScrollBar );
      SWidget::appendWidget( MContainer );
      MScrolledX = 0;
      MScrolledY = 0;
      MVisible = 1;
    }

    virtual ~SWidget_ScrollBox() {
    }

  public:

    /*
      append sub-widgets to FContainer, not self
    */


    virtual
    int32 appendWidget(SWidget* AWidget) {
      return MContainer->appendWidget(AWidget);
    }

    //SWidget* subWidget(uint32 AIndex) {
    //  return MContainer->MSubWidgets[AIndex];
    //}

    /*
      called from on_align

      calculates the size of the thumb-bar (scrollbar), based on FContent
      (calculated in on_align), and how much of the content that is visible
    */

    void calc_thumbsize(void) {
      float height  = (float)MContainer->rect().h; // size of container
      float content = (float)MContainer->contentRect().h;    // size of content
      if (content > height) {
        MVisible = height / content;
        MScrollBar->thumbsize(MVisible);
      }
      else {
        MVisible = 1;
        MScrollBar->thumbsize(1);
      }
    }

    //----------

    // called from on_align

    void un_scroll(void) {
      //do_scroll(-mScrolledX,-mScrolledY);
      MContainer->on_scroll(MScrolledX,MScrolledY);
      MScrolledX = 0;
      MScrolledY = 0;
      MScrollBar->value(0);
    }

    //----------

    // called from on_align
    // x/y = 0..1

    void re_scroll(float x, float y) {
      float scrollable = (float)MContainer->contentRect().h * (1-MVisible);
      int32 yscroll = floorf( scrollable*y );
      int32 dy = yscroll - MScrolledY;
      MContainer->on_scroll(0,-dy);
      MScrolledX += 0;
      MScrolledY += dy;
    }

  public:

    /*
      on_align resets/modifies x/y pos and sizes, so we remember the current
      y position, realigns, and then resets the position..

      perhaps we could have an offset field in each widget instead?
      FRect being the 'unscrolled' position, that is used in on_align, etc,
      and then a FOffsetRect that is set
    */

    //virtual
    void on_align(void) {
      float y = MScrollBar->value();
      un_scroll();
      //inherited;// on_align;
      SWidget::on_align();
      calc_thumbsize();
      re_scroll(0,y);
      MScrollBar->value(y);
    }

    //virtual
    void do_update(SWidget* AWidget) {
      if (AWidget == MScrollBar) {
        float val = MScrollBar->value();
        float scrollable = (float)MContainer->contentRect().h * (1-MVisible);
        int32 i = floorf(scrollable*val);
        int32 dy = i - MScrolledY;
        MContainer->on_scroll(0,-dy);
        MScrolledX += 0;
        MScrolledY += dy;
        do_redraw(MContainer,MContainer->rect()); // self
      }
      else
      SWidget::do_update(AWidget);
    }


};

//----------------------------------------------------------------------
#endif
