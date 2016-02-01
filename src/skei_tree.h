#ifndef skei_tree_included
#define skei_tree_included
//----------------------------------------------------------------------

/*
  SArray or SList?
  depends on if you plan to change the tree a lot..
  list is faster to add/remove nodes
  array is faster to access
  list is 'invasive' (prev/next nodes per item)

  widgets are trees (arrays)..

*/

class STreeNode;
typedef SArray<STreeNode*> SNodeArray;

//----------

class STreeNode {
  protected:
    STreeNode*  MSubNodes;
    uint32      MType;
    uint32      MState
  public:

};


//----------------------------------------------------------------------
#endif
