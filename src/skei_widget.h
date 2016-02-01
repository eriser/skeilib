#ifndef skei_widget_included
#define skei_widget_included
//----------------------------------------------------------------------

/*
  todo (maybe?):
  this is too large! split it up into smaller sub-classes:
    SWidget_Base
    SWidget_Visual
    SWidget_Container
    SWidget_Layout

*/

#include "skei_array.h"
#include "skei_edges.h"
#include "skei_painter.h"
#include "skei_rect.h"

//----------

class SWidget;
typedef SArray<SWidget*> SWidgetArray;

//----------------------------------------------------------------------

/*
  sizeLimit:
    x = minwidth
    y = minheight
    w = maxwidth
    h = maxheight
*/

struct skei_widget_layout {
  uint32  alignment;
  SRect   sizeLimit;
};

/*
  margins = around sub-widgets
  padding = between sub-widgets
*/

struct skei_container_layout {
  SEdges  margin;
  SPoint  padding;
};

//----------------------------------------------------------------------

class SWidget {

  //private:
  protected:

    // base

    const char*           MName;
    SWidget*              MParent;
    //SWidget*              MOwner;
    SRect                 MRect;
    skei_widget_layout    MWidgetLayout;
    float                 MValue;
    uint32                MFlags;
    int32                 MCursor;
    const char*           MHint;
    int32                 MIndex;
    SRect                 MInitialRect;
    void*                 MParameter;

  protected:

    // container

    SWidgetArray          MSubWidgets;
    SWidget*              MCapturedWidget;
    SWidget*              MHoverWidget;
    SWidget*              MModalWidget;
    SWidget*              MWantKeysWidget;
    skei_container_layout MContainerLayout;
    SRect                 MContentRect;

  private:

    // layout

    int32                 MStackedX;
    int32                 MStackedY;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    SWidget(SRect ARect, uint32 AAlignment=swa_none) {
      //STrace("SWidget\n");

      // base

      MName                   = "SWidget";
      MParent                 = SKEI_NULL;
      //MOwner                  = SKEI_NULL;
      MRect                   = ARect;
      MWidgetLayout.alignment = AAlignment;
      MWidgetLayout.sizeLimit = SRect(0,0,999999,999999);
      MValue                  = 0.0f;
      MFlags                  = swf_active
                              + swf_autoalign
                              + swf_capture
                              //+ swf_clip
                              + swf_opaque
                              + swf_visible;
      MCursor                 = smc_default;
      MHint                   = "widget";//SKEI_NULL;
      MIndex                  = -1;
      MInitialRect            = MRect;
      MParameter              = SKEI_NULL;

      // container

      MCapturedWidget           = SKEI_NULL;
      MHoverWidget              = SKEI_NULL;
      MModalWidget              = SKEI_NULL;
      MWantKeysWidget           = SKEI_NULL;
      MContainerLayout.margin   = SEdges(0);
      MContainerLayout.padding  = SPoint(0);//SEdges(0);
      MContentRect              = SRect(0);

      // layout

      MStackedX = 0;
      MStackedY = 0;

    }

    //----------

    virtual ~SWidget() {

      // container

      #ifndef SKEI_NO_AUTODELETE
      deleteWidgets();
      #endif

    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    // base

    void        alignment(uint32 AAlignment)  { MWidgetLayout.alignment = AAlignment; }
    void        cursor(int32 ACursor)         { MCursor = ACursor; }
    void        flags(uint32 AFlags)          { MFlags = AFlags; }
    void        hint(const char* AHint)       { MHint = AHint; }
    void        index(int32 AIndex)           { MIndex = AIndex; }
    void        name(const char* AName)       { MName = AName; }
    void        parameter(void* AParameter)   { MParameter = AParameter; }
    void        sizeLimit(SRect ALimit)       { MWidgetLayout.sizeLimit = ALimit; }
    void        value(float AValue)           { MValue = AValue; }

    SRect       contentRect(void)             { return MContentRect; }
    uint32      flags(void)                   { return MFlags; }
    int32       index(void)                   { return MIndex; }
    const char* name(void)                    { return MName; }
    void*       parameter(void)               { return MParameter; }
    SRect       rect(void)                    { return MRect; }
    float       value(void)                   { return MValue; }

    bool        hasFlag(uint32 AFlag)         { return (MFlags & AFlag); }
    void        clearFlag(uint32 AFlag)       { MFlags &= ~AFlag; }
    void        setFlag(uint32 AFlag)         { MFlags |= AFlag; }

  public:

    // container

    void      margins(SEdges AMargins)    { MContainerLayout.margin = AMargins; }
    void      padding(SPoint APadding)    { MContainerLayout.padding = APadding; }

    uint32    numSubWidgets(void)         { return MSubWidgets.size(); }
    SWidget*  parent(void)                { return MParent; }
    SWidget*  subWidget(uint32 AIndex)    { return MSubWidgets[AIndex]; }

    void margins(int32 ALeft, int32 ATop, int32 ARight, int32 ABottom) {
      MContainerLayout.margin.left = ALeft;
      MContainerLayout.margin.top = ATop;
      MContainerLayout.margin.right = ARight;
      MContainerLayout.margin.bottom = ABottom;
    }

    void margins(int32 AHoriz, int32 AVert) {
      MContainerLayout.margin.left = AHoriz;
      MContainerLayout.margin.top = AVert;
      MContainerLayout.margin.right = AHoriz;
      MContainerLayout.margin.bottom = AVert;
    }

    void margins(int32 AValue) {
      MContainerLayout.margin.left = AValue;
      MContainerLayout.margin.top = AValue;
      MContainerLayout.margin.right = AValue;
      MContainerLayout.margin.bottom = AValue;
    }

    void padding(int32 AHoriz, int32 AVert) {
      MContainerLayout.padding.x = AHoriz;
      MContainerLayout.padding.y = AVert;
    }

    void padding(int32 AValue) {
      MContainerLayout.padding.x = AValue;
      MContainerLayout.padding.y = AValue;
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    virtual
    int32 appendWidget(SWidget* AWidget) {
      //STrace("appendWidget %x (%s)\n",AWidget,AWidget->name());
      AWidget->MParent = this;
      //AWidget->MOwner = MOwner;
      int32 index = MSubWidgets.size();
      AWidget->MIndex = index;
      //AWidget.applySkin(FSkin,true,false); // AWidget (and children) 'inherit' skin, unless already applied..
      MSubWidgets.append(AWidget);
      return index;
    }

    //----------

    virtual
    SWidget* appendWidgetW(SWidget* AWidget) {
      appendWidget(AWidget);
      return AWidget;
    }

    //----------

    virtual
    void deleteWidgets(void) {
      for (int32 i=0; i<MSubWidgets.size(); i++) {
        SWidget* w = MSubWidgets[i];
        delete w;
      }
      MSubWidgets.clear();
    }

    //----------

    /*
      find widget, or sub-widget, or sub-sub-..widget at X,Y
      (depth first traversal)
      scans backward in sub-widgets list
      (opposite of the canvas algo we use for drawing)
      so we find the top-most widget, hopefully the one we clicked on
      if none found, returns self
    */

    virtual
    SWidget* findSubWidget(int32 AXpos, int32 AYpos) {
      SWidget* wdg = this;
      //if (wdg->isChildOf(MModalWidget)) {
      //  DTrace("child of modal\n");
      //}
      int32 num = MSubWidgets.size();
      for (int32 i=num-1; i>=0; i--) {
        SWidget* w = MSubWidgets[i];
        //if (w->isChildOf(MModalWidget)) {
        if (w->hasFlag(swf_active+swf_visible)) { // was swf_active only
          if (w->MRect.contains(AXpos,AYpos)) {
            wdg = w->findSubWidget(AXpos,AYpos);
            if (wdg != w) return wdg;
            else return w;
          } //contains
        } //active
        //} // ischildofhovewr
      } //for num
      //end; //num>0
      return wdg;
    }

    //----------

    virtual
    bool isChildOf(SWidget* AWidget) {
      if (!AWidget) return false;
      if (AWidget==this) return false;
      SWidget* wdg = MParent;
      while (wdg) {
        if (wdg==AWidget) return true;
        wdg = wdg->MParent;
      }
      return false;
    }

    //----------

    virtual
    SWidget* getOwner(void) {
      SWidget* parent = this;
      while (parent->MParent) parent = parent->MParent;
      return parent;
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    /*
      align subwidgets (not itself)..
      keep track of rect encapsulating each widgets content (sub-widgets),
      so we can resize widgets depending on their content,
      or resize scrollbar size, etc..
    */

    /*
      M = margin
      P = padding (between widgets when stacked)

       ________________________________________ _ _          _ ____
      |          ^                                                  |
      |          M                                                  |
      |      ____v___       ________       ____ _            _      |
      |     |        |     |        |     |                   |     |
      |<-M->|        |<-P->|        |<-P->|                   |<-M->|
      |     |________|     |________|     |______ _       _ __|     |
      |          ^                                                  :
      |          P
      |      ____v____      ___ _
      |     |         |    |
      |     :         .
      |

      TODO:
      - skip widget if not enough space for it?
        (null or negative space left)
      - break up this (too large) function into smaller pieces
        to make it easier to follow and see any overview...
    */

    //virtual
    void alignWidgets(void) {

      //KTrace("KWidget.align\n");
      if (hasFlag(swf_autoalign)) {
        SRect parent = MRect;
        parent.add(
            MContainerLayout.margin.left,
            MContainerLayout.margin.top,
          -(MContainerLayout.margin.left+MContainerLayout.margin.right),
          -(MContainerLayout.margin.top+MContainerLayout.margin.bottom)
        );
        SRect client    = parent;
        MContentRect    = SRect(MRect.x,MRect.y,0,0); //set(MRect.x,MRect.y,0,0);
        int32 stackx    = client.x;
        int32 stacky    = client.y;
        int32 largestw  = 0;
        int32 largesth  = 0;
        int32 remain    = 0;
        int32 diff      = 0;
        int32 prevx     = 0;
        int32 currx     = 0;
        int32 prevy     = 0;
        int32 curry     = 0;

        //if (MSubWidgets.size() > 0) {

          for (int32 i=0; i<MSubWidgets.size(); i++) {

            SWidget* widget = MSubWidgets[i];

            int32 widgetwidth  = widget->MRect.w;  // current widget width
            int32 widgetheight = widget->MRect.h;  // height

            switch (widget->MWidgetLayout.alignment) {

              //  _____
              // |  _  |
              // | |X| |
              // |_____|
              //
              case swa_none:
                  widget->on_setPos(widget->MInitialRect.x+parent.x, widget->MInitialRect.y+parent.y);
                  break;

              //   _____
              //  |XXXXX|
              //  |XXXXX|
              //  |XXXXX|
              //
              case swa_client: // overlay
                widget->on_setPos(client.x,client.y);
                widget->on_setSize(client.w,client.h);
                break;

              //   _____
              //  |XXXXX|
              //  |XXXXX|
              //  |XXXXX|
              //
              case swa_fillClient:
                widget->on_setPos(client.x,client.y);
                widget->on_setSize(client.w,client.h);
                client.setPos(parent.x,parent.y);
                client.setSize(0,0);
                break;

              //  ______
              // |X|
              // |X|
              // |X|____
              //
              case swa_fillLeft:
                widget->on_setPos(client.x,client.y);
                widget->on_setSize(widgetwidth,client.h);
                client.x = client.x + (widgetwidth+MContainerLayout./*padding.left*/padding.x);
                client.w = client.w - (widgetwidth+MContainerLayout./*padding.left*/padding.x);
                stackx = client.x;
                stacky = client.y;
                largesth = 0;
                break;

              //  _____
              //     |X|
              //     |X|
              //  ___|X|
              //
              case swa_fillRight:
                widget->on_setPos(client.x2()-widgetwidth+1,client.y);
                widget->on_setSize(widgetwidth,client.h);
                client.w = client.w - (widgetwidth+MContainerLayout./*padding.left*/padding.x);
                break;

              //  _____
              // |XXXXX|
              // |     |
              // |     |
              //
              case swa_fillTop:
                widget->on_setPos(client.x,client.y);
                widget->on_setSize(client.w,widgetheight);
                client.y = client.y + (widgetheight+MContainerLayout./*padding.top*/padding.y);
                client.h = client.h - (widgetheight+MContainerLayout./*padding.top*/padding.y);
                stackx = client.x;
                stacky = client.y;
                largestw = 0;
                break;

              //
              // |     |
              // |_____|
              // |XXXXX|
              //
              case swa_fillBottom:
                widget->on_setPos(client.x,client.y2()-widgetheight+1);
                widget->on_setSize(client.w,widgetheight);
                client.h = client.h - (widgetheight+MContainerLayout./*padding.top*/padding.y);
                break;

              //  _____
              // |_.
              // |X|
              // |_.___
              //
              case swa_left:
                widget->on_setPos(client.x,client.y);
                widget->on_setPos(client.x,widget->MInitialRect.y+parent.y); // setsize ???
                client.x = client.x + (widgetwidth+MContainerLayout./*padding.left*/padding.x);
                client.w = client.w - (widgetwidth+MContainerLayout./*padding.left*/padding.x);
                stackx = client.x;
                stacky = client.y;
                largestw = 0;
                break;

              //  _____
              // |X|
              // | .
              // |_.___
              //
              case swa_leftTop:
                widget->on_setPos(client.x,client.y);
                client.x = client.x + (widgetwidth+MContainerLayout./*padding.left*/padding.x);
                client.w = client.w - (widgetwidth+MContainerLayout./*padding.left*/padding.x);
                stackx = client.x;
                stacky = client.y;
                largestw = 0;
                break;

              //  _____
              // | .
              // |_.
              // |X|___
              //
              case swa_leftBottom:
                widget->on_setPos( client.x, client.y2()-widgetheight+1 );
                client.x = client.x + (widgetwidth + MContainerLayout./*padding.left*/padding.x);
                client.w = client.w - (widgetwidth + MContainerLayout./*padding.left*/padding.x);
                stackx = client.x;
                stacky = client.y;
                break;

              //  _____
              //     ._|
              //     |X|
              //  ___._|
              //
              case swa_right:
                widget->on_setPos( client.x2()-widgetwidth+1,MInitialRect.y+parent.y );
                client.w = client.w - (widgetwidth + MContainerLayout./*padding.left*/padding.x);
                break;

              //  _____
              //     |X|
              //     . |
              //  ___._|
              //
              case swa_rightTop:
                widget->on_setPos( client.x2()-widgetwidth+1, client.y );
                client.w = client.w - (widgetwidth + MContainerLayout./*padding.left*/padding.x);
                break;

              //  _____
              //     . |
              //     ._|
              //  ___|X|
              //
              case swa_rightBottom:
                widget->on_setPos( client.x2()-widgetwidth+1, client.y2()-widgetheight+1 );
                client.w = client.w - (widgetwidth + MContainerLayout./*padding.left*/padding.x);
                break;

              //  _____
              // |.|X|.|
              // |     |
              // |     |
              //
              case swa_top:
                widget->on_setPos(MInitialRect.x+parent.x,client.y);
                client.y = client.y + (widgetheight+MContainerLayout./*padding.top*/padding.y);
                client.h = client.h - (widgetheight+MContainerLayout./*padding.top*/padding.y);
                stackx = client.x;
                stacky = client.y;
                break;

              //  _____
              // |X|.. |
              // |     |
              // |     |
              //
              case swa_topLeft:
                widget->on_setPos(client.x,client.y);
                client.y = client.y + (widgetheight+MContainerLayout./*padding.top*/padding.y);
                client.h = client.h - (widgetheight+MContainerLayout./*padding.top*/padding.y);
                stackx = client.x;
                stacky = client.y;
                break;

              //  _____
              // |.. |X|
              // |     |
              // |     |
              //
              case swa_topRight:
                widget->on_setPos( client.x2()-widgetwidth+1, client.y );
                client.y = client.y + (widgetheight + MContainerLayout./*padding.top*/padding.y);
                client.h = client.h - (widgetheight + MContainerLayout./*padding.top*/padding.y);
                stackx = client.x;
                stacky = client.y;
                break;

              //
              // |     |
              // |. _..|
              // |_|X|_|
              //
              case swa_bottom:
                widget->on_setPos( MInitialRect.x+parent.x, client.y2()-widgetheight+1 );
                //client.y += (wh + mPaddingY);
                client.h = client.h - (widgetheight + MContainerLayout./*padding.top*/padding.y);
                break;

              //
              // |     |
              // |_ ...|
              // |X|___|
              //
              case swa_bottomLeft:
                widget->on_setPos( client.x, client.y2()-widgetheight+1 );
                //client.y += (wh + mPaddingY);
                client.h = client.h - (widgetheight + MContainerLayout./*padding.top*/padding.y);
                break;

              //
              // |     |
              // |... _|
              // |___|X|
              //
              case swa_bottomRight:
                widget->on_setPos( client.x2()-widgetwidth+1, client.y2()-widgetheight+1 );
                //wdg->doSetSize( ww, client.h );
                client.h = client.h - (widgetheight + MContainerLayout./*padding.top*/padding.y);
                break;

              //  __________________
              // |XXX_|XXX_|XX_|...... .
              // |
              // |
              //
              case swa_stackedHoriz:
                remain = client.x2() - stackx + 1;
                if (remain >= widgetwidth) {
                  // enough space
                  widget->on_setPos(stackx,stacky);
                  stackx = stackx + (widgetwidth+MContainerLayout./*padding.left*/padding.x);
                  if (widgetheight > largesth) largesth = widgetheight;
                  prevy = client.y;
                  curry = stacky + (largesth+MContainerLayout./*padding.top*/padding.y);
                  diff = curry-prevy;
                  client.y = curry;
                  if (diff > 0) client.h = client.h - diff;
                }
                else {
                  // not enougb space
                  stackx = client.x;
                  stacky = stacky + (largesth+MContainerLayout./*padding.top*/padding.y);
                  largesth = widgetheight;
                  widget->on_setPos(stackx,stacky);
                  client.y = client.y + (largesth+MContainerLayout./*padding.top*/padding.y);
                  client.h = client.h - (largesth+MContainerLayout./*padding.top*/padding.y);
                  stackx = stackx + (widgetwidth+MContainerLayout./*padding.left*/padding.x);
                }
                break;


              //  ________
              // |XX_|
              // |XX_|
              // |XX_|
              // |   |
              // |   :
              //     .
              case swa_stackedVert:
                remain = client.y2() - stacky + 1;
                if (remain >= widgetheight) {
                  // enough space
                  widget->on_setPos(stackx,stacky);
                  stacky = stacky + (widgetheight+MContainerLayout./*padding.top*/padding.y);
                  if (widgetwidth > largestw) largestw = widgetwidth;
                  prevx = client.x;
                  currx = stackx + (largestw+MContainerLayout./*padding.left*/padding.x);
                  diff = currx-prevx;
                  client.x = currx;
                  if (diff > 0) client.w = client.w - diff;
                }
                else {
                  // not enougb space
                  stackx = stackx + (largestw+MContainerLayout./*padding.left*/padding.x);
                  stacky = client.y;
                  largestw = widgetwidth;
                  widget->on_setPos(stackx,stacky);
                  client.x = client.x + (largestw+MContainerLayout./*padding.left*/padding.x);
                  client.w = client.w - (largestw+MContainerLayout./*padding.left*/padding.x);
                  stacky = stacky + (widgetheight+MContainerLayout./*padding.top*/padding.y);
                }
                break;
            } // case
            MContentRect.combine( widget->MRect ); // keep track of outer boundary
            widget->on_align();
          } // for
          MContentRect.add(
            0,
            0,
            (MContainerLayout.margin.left /*+ MContainerLayout.margin.right*/),
            (MContainerLayout.margin.top  /*+ MContainerLayout.margin.bottom*/)
          );

        //} //sub.size >0

      } // swf_autoalign
    }

  //----------------------------------------
  // on_
  //----------------------------------------

  public:

    virtual
    void on_reset(int32 AMode=0) {
      for (int i=0; i<MSubWidgets.size(); i++) {
        MSubWidgets[i]->on_reset(AMode);
      }
    }

    //----------

    virtual
    void on_setPos(int32 AXpos, int32 AYpos) {
      int32 dx = AXpos - MRect.x;
      int32 dy = AYpos - MRect.y;
      for (int32 i=0; i<MSubWidgets.size(); i++) {
        SWidget* widget = MSubWidgets[i];
        int32 wx = widget->MRect.x;
        int32 wy = widget->MRect.y;
        widget->on_setPos( wx+dx, wy+dy );
      }
      //SWidget::on_setPos(AXpos,AYpos);
      MRect.x = AXpos;
      MRect.y = AYpos;

    }

    //----------

    virtual
    void on_setSize(int32 AWidth, int32 AHeight) {
      if ((MWidgetLayout.sizeLimit.x>=0) && (AWidth  < MWidgetLayout.sizeLimit.x)) AWidth  = MWidgetLayout.sizeLimit.x;
      if ((MWidgetLayout.sizeLimit.w>=0) && (AWidth  > MWidgetLayout.sizeLimit.w)) AWidth  = MWidgetLayout.sizeLimit.w;
      if ((MWidgetLayout.sizeLimit.y>=0) && (AHeight < MWidgetLayout.sizeLimit.y)) AHeight = MWidgetLayout.sizeLimit.y;
      if ((MWidgetLayout.sizeLimit.h>=0) && (AHeight > MWidgetLayout.sizeLimit.h)) AHeight = MWidgetLayout.sizeLimit.h;
      MRect.w = AWidth;
      MRect.h = AHeight;
    }

    //----------

    virtual
    void on_scroll(int32 ADeltaX, int32 ADeltaY) {
      for (int32 i=0; i<MSubWidgets.size(); i++) {
        SWidget* wdg = MSubWidgets[i];
        int32 wx = wdg->MRect.x;
        int32 wy = wdg->MRect.y;
        wdg->on_setPos( wx+ADeltaX, wy+ADeltaY );
      }
    }

    //----------

    //virtual
    //void on_move(int32 AXdelta, int32 AYdelta) {
    //}

    //----------

    //virtual
    //void on_resize(int32 AWidth, int32 AHeight) {
    //}

    //----------

    virtual
    void on_align(void) {
      alignWidgets();
    }

    //----------

    virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {

      if (ARect.isEmpty()) return;
      if (MRect.isEmpty()) return;

      // these has already been checked by parent widget (in on_paint)
      //if hasFlag(swf_visible) then
      //if FRect.intersect(ARect) then
      /* clip */
      if (MFlags&swf_clip) APainter->pushClip(MRect);
      /* paint children */
      for (int32 i=0; i<MSubWidgets.size(); i++) {
        SWidget* wdg = MSubWidgets[i];
        if (wdg->hasFlag(swf_visible)) {
          //STrace("visible\n");
          //vis = ACanvas.visibleIntersection(wdg.FRect);
          //if not vis.empty then;
          if (wdg->MRect.isNotEmpty()) {
            if (wdg->MRect.intersects(ARect)) {
              wdg->on_paint(APainter,ARect,AMode);
            } // subwdg.intersect
          } // not empty
        } // subwdg.visible
      } // for subwdg
      /* un-clip */
      if (MFlags&swf_clip) APainter->popClip();
    }

    //----------

    virtual
    void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (MModalWidget) MModalWidget->on_mouseDown(AXpos,AYpos,AButton,AState);
      else {
        if (MCapturedWidget) MCapturedWidget->on_mouseDown(AXpos,AYpos,AButton,AState);
        else {
          //if zwf_active in FFlags then
          if (hasFlag(swf_active)) {
            SWidget* hover = findSubWidget(AXpos,AYpos);
            if (hover) {
              if (hover != this) {
                //if zwf_capture in FFlags then FCapturedWidget = hover;
                if (MFlags&swf_capture) MCapturedWidget = hover;
                hover->on_mouseDown(AXpos,AYpos,AButton,AState);
              } // !hover
            }
          } // active
        } // !capture
      } // modal
    }

    //----------

    virtual
    void on_mouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (MModalWidget) MModalWidget->on_mouseUp(AXpos,AYpos,AButton,AState);
      else {
        if (MCapturedWidget) {
          MCapturedWidget->on_mouseUp(AXpos,AYpos,AButton,AState);
          MCapturedWidget = SKEI_NULL;
        } //capture
        else {
          if (hasFlag(swf_active)) {
            SWidget* hover = findSubWidget(AXpos,AYpos);
            if (hover) {
              if (hover != this) hover->on_mouseUp(AXpos,AYpos,AButton,AState);
            }
          } //active
        }
      } // modal
    }

    //----------

    virtual
    void on_mouseMove(int32 AXpos, int32 AYpos, int32 AState) {
      if (MModalWidget) MModalWidget->on_mouseMove(AXpos,AYpos,AState);
      else {
        if (MCapturedWidget) {
          MCapturedWidget->on_mouseMove(AXpos,AYpos,AState);
        }
        else {
          //if (hasFlag(swf_active) {
            SWidget* hover = findSubWidget(AXpos,AYpos);
            //if (hover) {
              if (hover != MHoverWidget) {
                if (MHoverWidget) MHoverWidget->on_leave(hover);
                hover->on_enter(MHoverWidget);
                MHoverWidget = hover;
              }
              // make sure we don't call ourselves recursively!
              if (hover != this) hover->on_mouseMove(AXpos,AYpos,AState);
            //} // hover
          //} // active
        }
      } // modal
    }

    //----------

    virtual
    void on_mouseDoubleClick(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    }

    //----------

    virtual
    void on_keyDown(int32 AKey, int32 AState) {
      if (MWantKeysWidget) MWantKeysWidget->on_keyDown(AKey,AState);
      if (MModalWidget) MModalWidget->on_keyDown(AKey,AState);
      else if (MCapturedWidget) MCapturedWidget->on_keyDown(AKey,AState);
    }

    //----------

    virtual
    void on_keyUp(int32 AKey, int32 AState) {
      if (MWantKeysWidget) MWantKeysWidget->on_keyUp(AKey,AState);
      if (MModalWidget) MModalWidget->on_keyUp(AKey,AState);
      else if (MCapturedWidget) MCapturedWidget->on_keyUp(AKey,AState);
    }

    //----------

    virtual
    void on_enter(SWidget* AWidget) {
      if (MParent) {
        if (MCursor >= 0) MParent->do_cursor(this,MCursor);
        if (MHint) MParent->do_hint(this,MHint);
      }
    }

    virtual
    void on_leave(SWidget* AWidget) {
      if (MParent) {
        MParent->do_cursor(this,smc_default);
        MParent->do_hint(this,"");
      }
    }

    //virtual
    //void on_timer(void) {
    //  for (int32 i=0; i<MSubWidgets.size(); i++) {
    //    SWidget* w = MSubWidgets[i];
    //    if (w->hasFlag(swf_timer)) w->on_timer();
    //  }
    //}

    //----------

    //virtual
    //void on_idle(void) {
    //}

  //----------------------------------------
  // do_
  //----------------------------------------

  public:

    virtual
    void do_move(SWidget* AWidget, int32 AXpos, int32 AYpos) {
      if (MParent) MParent->do_move(AWidget,AXpos,AYpos);
    }

    //----------

    virtual
    void do_sizer(SWidget* AWidget, int32 ADeltaX, int32 ADeltaY, int32 AMode) {
      if (AWidget==this) {
        on_setSize(MRect.w+ADeltaX, MRect.h+ADeltaY);
      }
      if (MParent) MParent->do_sizer(AWidget,ADeltaX,ADeltaY,AMode);
    }

    //----------

    virtual
    void do_resize(SWidget* AWidget, int32 AWidth, int32 AHeight) {
      if (MParent) MParent->do_resize(AWidget,AWidth,AHeight);
    }

    //----------

    /*
      if widget is not opaque (fully filling its rect), we should draw parent
      first..
      TODO: redraw parent before child, until arect is fully filled ???
    */

    virtual
    void do_redraw(SWidget* AWidget, SRect ARect) {
      if (MParent) {

        //if (!hasFlag(swf_opaque)) MParent->do_redraw(MParent,ARect);
        //else MParent->do_redraw(AWidget,ARect);

        if (AWidget->hasFlag(swf_opaque)) MParent->do_redraw(AWidget,ARect);
        else MParent->do_redraw(this,ARect);

        //MParent->do_redraw(AWidget,ARect);

      }
    }

    //----------

    virtual
    void do_update(SWidget* AWidget) {
      if (MParent) MParent->do_update(AWidget);
    }

    //----------

    virtual
    void do_cursor(SWidget* AWidget, int32 ACursor) {
      if (MParent) MParent->do_cursor(AWidget,ACursor);
    }

    //----------

    virtual
    void do_hint(SWidget* AWidget, const char* AHint) {
      if (MParent) MParent->do_hint(AWidget,AHint);
    }

    //----------

    virtual
    void do_modal(SWidget* AWidget, int32 AMode=0) {
      if (MParent) MParent->do_modal(AWidget,AMode);
    }

    //----------

    virtual
    void do_wantkeys(SWidget* AWidget) {
      if (MParent) MParent->do_wantkeys(AWidget);
    }

};

//----------------------------------------------------------------------
#endif
