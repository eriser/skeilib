#ifndef skei_widget_nodes_included
#define skei_widget_nodes_included
//----------------------------------------------------------------------

#include "skei_widget.h"

#define SKEI_NUM_WIDGETNODES 1024

//----------

struct SWdgNode {
  float   value;
  uint32  flags;
};

//----------

class SWidget_Nodes
: public SWidget {

  private:
    SColor    MBackColor;
    SColor    MLineColor;
    SColor    MNodeColor;
    int32     MNumNodes;
    SWdgNode  MNodes[SKEI_NUM_WIDGETNODES];
    int32     MSelected;

  public:

    SWidget_Nodes(SRect ARect, uint32 AAlignment=swa_none)
    : SWidget(ARect,AAlignment) {
      MName = "SWidget_Nodes";
      MHint = "nodes";
      MBackColor = SDarkGrey;
      MLineColor = SLightGrey;
      MNodeColor = SBrightYellow;
      SMemset(MNodes,0,sizeof(MNodes));
      MSelected = -1;
      setFlag(swf_clip);
    }

    virtual ~SWidget_Nodes() {
    }

  public:

    void numNodes(int32 ANum) {
      MNumNodes = ANum;
    }

    void setNode(int32 AIndex, float AValue, uint32 AFlags) {
      MNodes[AIndex].value = AValue;
      MNodes[AIndex].flags = AFlags;
    }

    int32 findNode(int32 AXpos, int32 AYpos) {
      float w = (float)(AXpos - MRect.x) / (float)MRect.w; // 0..1
      w *= (MNumNodes-1);
      int32 index = roundf(w);// SMin( MNumNodes-1, roundf(w) );
      //STrace("w %f index %i\n",w,index);
      return index;//-1;
    }

  public:

    //virtual
    void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
      APainter->setFillColor(MBackColor);
      APainter->fillRect(MRect);
      //APainter->setFillColor(MNodeColor);
      if (MNumNodes>0) {
        //int32 halfw = (MRect.w/2);
        int32 halfh = (MRect.h/2);
        if (MNumNodes==1) {
          //STrace("NumNodes: %i\n",MNumNodes);
          int32 h = halfh + ( (MNodes[0].value * halfh ) );
          APainter->setDrawColor(MLineColor);
          APainter->drawLine(MRect.x,MRect.y+h,MRect.x2(),MRect.y+h);
        }
        else if (MNumNodes==2) {
          int32 h1 = halfh + ( (MNodes[0].value * halfh ) );
          int32 h2 = halfh + ( (MNodes[1].value * halfh ) );
          APainter->setDrawColor(MLineColor);
          APainter->drawLine(MRect.x,MRect.y+h1,MRect.x2(),MRect.y+h2);
        }
        else {
          float w = (float)MRect.w / (float)(MNumNodes-1);
          float x = MRect.x;
          for (int32 i=0; i<MNumNodes; i++) {
            int32 h1,h2;
            h1 = halfh + ( (MNodes[i].value * halfh ) );
            if (i<MNumNodes-1) {
              h2 = halfh + ( (MNodes[i+1].value * halfh ) );
              APainter->setDrawColor(MLineColor);
              APainter->drawLine(x,MRect.y+h1,x+w,MRect.y+h2);
            }

            if (i==MSelected) {
              //APainter->drawRect( x+w-2, MRect.y+h2-2, x+w+2, MRect.y+h2+2 );
              APainter->setFillColor(MNodeColor);
              APainter->fillRect( x-2, MRect.y+h1-2, x+2, MRect.y+h1+2 );
            }

            x += w;
          }
        }
      }
    }

    //----------

    //virtual
    void on_mouseMove(int32 AXpos, int32 AYpos, int32 AState) {
      //SWidget::on_mouseMove(AXpos,AYpos,AState);

      //float w = (float)(AXpos - MRect.x) / (float)MRect.w; // 0..1
      //int32 index = (w * (float)MNumNodes);

      int32 index = findNode(AXpos,AYpos);
      MSelected = index;

      //STrace("MSelected: %i\n",MSelected);

      do_redraw(this,MRect);
    }

    //----------

    //virtual
    void on_leave(SWidget* AWidget) {
      //SWidget::on_leave(AWidget);
      if (MSelected>=0) {
        MSelected = -1;
        do_redraw(this,MRect);
      }
    }


    //virtual
    //void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    //}

    //virtual
    //void on_mouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    //}

    //virtual
    //void on_mouseMove(int32 AXpos, int32 AYpos, int32 AState) {
    //}

    //virtual
    //void on_mouseDoubleClick(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    //}

    //virtual
    //void on_keyDown(int32 AKey, int32 AState) {
    //}

    //virtual
    //void on_keyUp(int32 AKey, int32 AState) {
    //}

    //virtual
    //void on_enter(SWidget* AWidget) {
    //}

    //virtual
    //void on_leave(SWidget* AWidget) {
    //}

};

#undef SKEI_NUM_WIDGETNODES

//----------------------------------------------------------------------
#endif

