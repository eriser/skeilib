#ifndef skei_widget_pages_included
#define skei_widget_pages_included
//----------------------------------------------------------------------

#include "skei_widget.h"

//----------

class SWidget_Pages
: public SWidget {

  private:

    int32 MActivePage;

  public:

    SWidget_Pages(SRect ARect, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_Pages";
      MHint = "pages";
      MActivePage = 0;
    }

    virtual ~SWidget_Pages() {
    }

  public:

    int32 appendPage(SWidget* APage) {
      APage->clearFlag(swf_active);
      APage->clearFlag(swf_visible);
      return appendWidget(APage);
    }

    void setPage(int32 APage/*, bool ARedraw=false*/) {
      if ( APage >= MSubWidgets.size() ) APage = MSubWidgets.size() - 1;
      //if (APage!=MActivePage) {
        for (int32 i=0; i<MSubWidgets.size(); i++) {
          SWidget* wdg = MSubWidgets[i];
          if (i == APage) {
            wdg->setFlag(swf_active);
            wdg->setFlag(swf_visible);
          }
          else {
            wdg->clearFlag(swf_active);
            wdg->clearFlag(swf_visible);
          }
        } //for
        //mPages->initMouseState();
        MActivePage = APage;
        //if (ARedraw) do_redraw(this,MRect);
      //} // !active page
    }

};

//----------------------------------------------------------------------
#endif

