#ifndef skei_global_included
#define skei_global_included
//----------------------------------------------------------------------

#include "skei_array.h"

//----------------------------------------------------------------------

class SGlobalObject {
  private:
    int32   MGlobalIndex;
  public:
    int32   globalIndex(void) { return MGlobalIndex; }
    void    globalIndex(int32 AIndex) { MGlobalIndex = AIndex; }
  public:
    SGlobalObject() {
      MGlobalIndex = -1;
    }
    virtual ~SGlobalObject() {
    }
    virtual void on_globalAppend(void) {}
    virtual void on_globalDelete(void) {}
    virtual void on_globalInit(void) {}
};

//----------

typedef SArray<SGlobalObject*> SGlobalObjectArray;

//----------------------------------------------------------------------

class SGlobal {

  private:
    SGlobalObjectArray  MObjects;

  public:

    SGlobal() {
    }

    ~SGlobal() {
      #ifndef SKEI_NO_AUTODELETE
      deleteObjects();
      #endif
    }

  public:

    //virtual
    int32 appendObject(SGlobalObject* AObject) {
      MObjects.append(AObject);
      int32 index = MObjects.size();
      AObject->globalIndex(index);
      MObjects.append(AObject);
      AObject->on_globalAppend();
      return index;
    }

    //virtual
    void deleteObjects(void) {
      for (int32 i=0; i<MObjects.size(); i++) {
        SGlobalObject* obj = MObjects[i];
        // hmm, crashes when i try to use the obj pointer..
        obj->on_globalDelete();
        delete obj;
      }
    }

    //virtual
    void initialize(void) {
      for (int32 i=0; i<MObjects.size(); i++) {
        SGlobalObject* obj = MObjects[i];
        obj->on_globalInit();
      }
    }

};

//----------------------------------------------------------------------

static
SGlobal SKEI_GLOBAL;

//----------------------------------------------------------------------
#endif
