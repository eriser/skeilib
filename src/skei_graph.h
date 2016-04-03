#ifndef skei_graph_included
#define skei_graph_included
//----------------------------------------------------------------------

/*
  graph
    is-a module
    list of sub-modules

  todo
  - incorporate this into the base module?
*/

//----------------------------------------------------------------------

#include "skei_module.h"

//----------

class SGraph
: public SModule {

  private:

    SModuleArray  MSubModules;

  public:

    SGraph(SOperatorGlobals* AGlobals)
    : SModule(AGlobals) {
    }

    virtual ~SGraph() {
      #ifndef SKEI_NO_AUTODELETE
      deleteModules();
      #endif
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void appendModule(SModule* AModule) {
      SAssert ( AModule != SKEI_NULL );
      AModule->parent(this);
      MSubModules.append(AModule);
    }

    //----------

    void deleteModules(void) {
      for (uint32 i=0; i<MSubModules.size(); i++) {
        //SAssert ( MSubModules[i] != SKEI_NULL );
        delete MSubModules[i];
        MSubModules[i] = SKEI_NULL;
      }
    }

  //----------------------------------------
  // process
  //----------------------------------------

  public:

    //virtual /*SModule*/
    //void processEvents(void) {
    //}


    //virtual /*SModule*/
    //void processTick(void) {
    //  for (uint32 i=0; i<MSubModules.size(); i++) {
    //    SAssert ( MSubModules[i] != SKEI_NULL );
    //    MSubModules[i]->processTick();
    //  }
    //  // self (operators)
    //  //SModule::processTick();
    //}

    //virtual /*SModule*/
    void processBlock(SSample** AInputs, SSample** AOutputs, uint32 ALength) {
      for (uint32 i=0; i<MSubModules.size(); i++) {
        SAssert ( MSubModules[i] != SKEI_NULL );
        MSubModules[i]->processBlock(AInputs,AOutputs,ALength);
      }
      // self
      //SModule::processBlock(AInputs,AOutputs,ALength);
    }

    //virtual /*SModule*/
    //void processTicks(SSample** AInputs, SSample** AOutputs, uint32 ALength) {
    //  for (uint32 i=0; i<MSubModules.size(); i++) {
    //    SAssert ( MSubModules[i] != SKEI_NULL );
    //    MSubModules[i]->processTicks(AInputs,AOutputs,ALength);
    //  }
    //  // self
    //  //SModule::processBlock(AInputs,AOutputs,ALength);
    //}

};

//----------------------------------------------------------------------
#endif

