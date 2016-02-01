#ifndef skei_list_included
#define skei_list_included
//----------------------------------------------------------------------

/*
  a templates (ala array)data-container)
  b multiple inheritance (ala widgetListener)
  
  todo: (?)
  - sort
  - find
  - swap two items
  - move item to head/tail  
*/

class SListNode
{
  friend class SList;
  //protected:
  private:
    SListNode* MPrev;
    SListNode* MNext;
  public:
    inline SListNode* next(void) { return MNext; }
    inline SListNode* prev(void) { return MPrev; }
    inline void next(SListNode* ANode) { MNext = ANode; }
    inline void prev(SListNode* ANode) { MPrev = ANode; }
};

//----------

class SList
{

  //protected:
  private:

    int        MSize;
    SListNode* MHead;
    SListNode* MTail;

  public:

    SList() {
      reset();
    }

    inline SListNode* head(void) { return MHead; }
    inline SListNode* tail(void) { return MTail; }
    inline void head(SListNode* ANode) { MHead = ANode; }
    inline void tail(SListNode* ANode) { MTail = ANode; }

    inline SListNode* next(SListNode* ANode) { return ANode->MNext; }
    inline SListNode* prev(SListNode* ANode) { return ANode->MPrev; }

    //----------

    inline void reset(void) {
      MHead = SKEI_NULL;
      MTail = SKEI_NULL;
    }

    //----------

    // TODO: insertBefore/After

    //----------

    void insertHead(SListNode* ANode) {
      ANode->MPrev = SKEI_NULL;
      if (MTail==SKEI_NULL) {
        MTail = ANode;
        ANode->MNext = SKEI_NULL;
      }
      else {
        MHead->MPrev = ANode;
        ANode->MNext = MHead;
      }
      MHead = ANode;
      MSize++;
    }

    //----------

    void insertTail(SListNode* ANode) {
      ANode->MNext = SKEI_NULL;
      if (MHead==SKEI_NULL) {
        MHead = ANode;
        ANode->MPrev = SKEI_NULL;
      }
      else {
        MTail->MNext = ANode;
        ANode->MPrev = MTail;
      }
      MTail = ANode;
      MSize++;
    }

    void removeHead(void) {
      if (MHead==MTail) reset();
      else {
        MHead = MHead->MNext;
        MHead->MPrev = SKEI_NULL;
      }
    }

    void removeTail(void) {
      if (MHead==MTail) reset();
      else {
        MTail = MTail->MPrev;
        MTail->MNext = SKEI_NULL;
      }
    }

    void append(SListNode* ANode) {
      insertTail(ANode);
    }

    void remove(SListNode* ANode) {
      if (ANode==MHead) removeHead();
      else if (ANode==MTail) removeTail();
      else {
        ANode->MNext->MPrev = ANode->MPrev;
        ANode->MPrev->MNext = ANode->MNext;
      }
    }

};

//----------------------------------------------------------------------

/*

class KDoubleListNode {
  private:
    KDoubleListNode* MPrevListNode;
    KDoubleListNode* MNextListNode;
  public:
    inline void prev(KDoubleListNode* ANode) { MPrevListNode=ANode; }
    inline void next(KDoubleListNode* ANode) { MNextListNode=ANode; }
    inline KDoubleListNode* prev(void) { return MPrevListNode; }
    inline KDoubleListNode* next(void) { return MNextListNode; }
};

//----------

class KDoubleList {
  private:
    KDoubleListNode* MListHead;
    KDoubleListNode* MListTail;

  public:

    KDoubleList() {
      MListHead = SKEI_NULL;
      MListTail = SKEI_NULL;
    }

    virtual ~KDoubleList() {
    }

    inline void head(KDoubleListNode* ANode) { MListHead=ANode; }
    inline void tail(KDoubleListNode* ANode) { MListTail=ANode; }
    inline KDoubleListNode* head(void) { return MListHead; }
    inline KDoubleListNode* tail(void) { return MListTail; }

    // initialize the list
    virtual void clear(void) {
      MListHead = SKEI_NULL;
      MListTail = SKEI_NULL;
    }

    // delete all nodes...
    void deleteNodes(void) {
      KDoubleListNode* node = MListHead;
      KDoubleListNode* next;
      while (node) {
        next = node->next();
        delete node;
        node = next;
      }
      MListHead = SKEI_NULL;
      MListTail = SKEI_NULL;
    }

    // append to tail...
    void appendTail(KDoubleListNode* ANode) {
      if (MListTail) {
        // xx XX --
        ANode->prev(MListTail);
        ANode->next(SKEI_NULL);
        MListTail->next(ANode);
        MListTail = ANode;
      }
      else {
        // -- XX --
        MListHead = ANode;
        ANode->prev(SKEI_NULL);
        ANode->next(SKEI_NULL);
        MListTail = ANode;
      }
    }

    void removeHead(void) {
      //TODO
    }

    void removeTail(void) {
      //TODO
    }

    // insert ANewNode after ANode
    void insertNode(KDoubleListNode* ANode, KDoubleListNode* ANewNode) {
      KDoubleListNode* next = ANode->next();
      KDoubleListNode* prev = ANode->prev();
      if (prev) {
        if (next) {
          //  xx XX xx
          prev->next(ANewNode);
          ANewNode->prev(prev);
          ANewNode->next(next);
          next->prev(ANewNode);
        }
        else {
          // xx XX --
          prev->next(ANewNode);
          ANewNode->prev(prev);
          ANewNode->next(SKEI_NULL);
          MListTail = ANewNode;
        }
      }
      else {
        // !head
        if (next) {
          // -- XX xx
          MListHead = ANewNode;
          ANewNode->prev(SKEI_NULL);
          ANewNode->next(next);
          next->prev(ANewNode);
        }
        else {
          // -- XX --
          MListHead = ANewNode;
          ANewNode->prev(SKEI_NULL);
          ANewNode->next(SKEI_NULL);
          MListTail = ANewNode;
        }
      } // !next
    }

    // remove node
    void removeNode(KDoubleListNode* ANode) {
      KDoubleListNode* next = ANode->next();
      KDoubleListNode* prev = ANode->prev();
      if (prev) {
        if (next) {
          //  xx XX xx
          // prev, next
          prev->next(next);
          next->prev(prev);
        }
        else {
          // xx XX --
          // prev, NULL
          // tail
          prev->next(SKEI_NULL);
          MListTail = prev;
        }
      }
      else {
        // !head
        if (next) {
          // -- XX xx
          // NULL,next
          // head
          MListHead = next;
          next->prev(SKEI_NULL);
        }
        else {
          // -- XX --
          // NULL,NULL
          // single
          MListHead = SKEI_NULL;
          MListTail = SKEI_NULL;
        }
      } // !next
    }

};

//----------

typedef SDoubleListNode  SListNode;
typedef SDoubleList      SList;

*/

//----------------------------------------------------------------------
#endif

