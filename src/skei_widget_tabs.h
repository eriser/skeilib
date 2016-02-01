#ifndef skei_widget_tabs_included
#define skei_widget_tabs_included
//----------------------------------------------------------------------

#include "skei_widget.h"
#include "skei_widget_buttonrow.h"
#include "skei_widget_pages.h"

//----------

class SWidget_Tabs
: public SWidget {

  protected:

    SWidget_ButtonRow*  MHeader;
    SWidget_Pages*      MPages;
    int32               MNumPages;

  public:

    SWidget*  header(void)    { return MHeader; }
    SWidget*  pages(void)     { return MPages; }
    int32     numpages(void)  { return MNumPages; }

  public:

    SWidget_Tabs(SRect ARect, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_Tabs";
      MHint = "tabs";
      MHeader = new SWidget_ButtonRow(SRect(20),0,swa_fillTop);
      MPages = new SWidget_Pages( SRect(0), swa_fillClient);
      MNumPages = 0;
      MHeader->width(0);
      appendWidget(MHeader);
      appendWidget(MPages);
    }

    virtual ~SWidget_Tabs() {
    }

  public:

    int32 appendPage(SWidget* AWidget, char* ATitle) {
      int32 result = MPages->appendPage(AWidget);
      MHeader->setName(MNumPages,ATitle);
      MNumPages += 1;
      MHeader->width(MNumPages);
      return result;
    }

    void selectTab(int32 AIndex/*, bool ARedraw=false*/) {
      if (AIndex < MNumPages) {
        MHeader->select(AIndex/*,ARedraw*/);
        MPages->setPage(AIndex/*,ARedraw*/);
      }
    }

  public:

    //virtual
    void do_update(SWidget* AWidget) {
      if (AWidget == MHeader) {
        int32 sel = MHeader->selected();
        MPages->setPage(sel/*,true*/);
        do_redraw(MPages,MPages->rect());
      }
      SWidget::do_update(AWidget);
    }

};

//----------------------------------------------------------------------
#endif
