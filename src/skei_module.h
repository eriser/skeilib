#ifndef skei_module_included
#define skei_module_included
//----------------------------------------------------------------------

#define SKEI_MAX_MODULE_CONNECTIONS 16

//----------------------------------------------------------------------

#include "skei_array.h"
class SModule;
typedef SArray<SModule*> SModuleArray;

//----------

class SModule {

  protected:

    const char*   MName;
    SModule*      MOwner;
    SModuleArray  MSubModules;
    float         MSampleRate;
    uint32        MBlockSize;
    uint32        MOverSample;
    float*        MInputs[SKEI_MAX_MODULE_CONNECTIONS];
    float*        MOutputs[SKEI_MAX_MODULE_CONNECTIONS];

  //----------------------------------------
  //
  //----------------------------------------

  public:

    SModule() {
      MName = "SModule";
      MOwner = SKEI_NULL;
      MSubModules.clear();
      MSampleRate = 0.0f;
      MBlockSize = 0;
      MOverSample = 1;
      SMemset(MInputs,0,sizeof(MInputs));
      SMemset(MOutputs,0,sizeof(MOutputs));
    }

    //----------

    virtual ~SModule() {
      #ifndef SKEI_NO_AUTODELETE
      deleteModules();
      #endif
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    const char* getName(void) {
      return MName;
    }

    //SModuleProcess getProcessBlock(void) {
    //  return &SModule::processBlock;
    //}

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    void appendModule(SModule* AModule) {
      AModule->MOwner = this;
      MSubModules.append(AModule);
    }

    //----------

    //virtual
    void deleteModules(void) {
      for (int32 i=0; i<MSubModules.size(); i++) delete MSubModules[i];
    }

    //----------

    //virtual
    SModule* findRoot(void) {
      SModule* module = MOwner;
      while (module->MOwner) module = module->MOwner;
      return module;
    }

    //----------

    //virtual
    void connectInput(int32 AIndex, float* ABuffer) {
      MInputs[AIndex] = ABuffer;
    }

    //----------

    //virtual
    void connectOutput(int32 AIndex, float* ABuffer) {
      MOutputs[AIndex] = ABuffer;
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //--------------------
    // set
    //--------------------

    virtual
    void setSampleRate(float ARate) {
      MSampleRate = ARate;
      for (int32 i=0; i<MSubModules.size(); i++) MSubModules[i]->setSampleRate(ARate);
    }

    //----------

    virtual
    void setBlockSize(uint32 ASize) {
      MBlockSize = ASize;
      for (int32 i=0; i<MSubModules.size(); i++) MSubModules[i]->setBlockSize(ASize);
    }

    //----------

    virtual
    void setOverSample(uint32 AFactor) {
      MOverSample = AFactor;
      for (int32 i=0; i<MSubModules.size(); i++) MSubModules[i]->setOverSample(AFactor);
    }

    //--------------------
    // handle    //--------------------

    virtual
    void handleControl(uint32 AIndex, float AValue) {
      for (int32 i=0; i<MSubModules.size(); i++) MSubModules[i]->handleControl(AIndex,AValue);
    }

    //----------

    virtual
    void handleMidi(uint8 AMessage, uint8 AData1, uint8 AData2) {
      for (int32 i=0; i<MSubModules.size(); i++) MSubModules[i]->handleMidi(AMessage,AData1,AData2);
    }

    //--------------------
    // process
    //--------------------

    virtual
    void processBlock(void) {
      for (int32 i=0; i<MSubModules.size(); i++) MSubModules[i]->processBlock();
    }

    //----------

    virtual
    void processSample(void) {
      for (int32 i=0; i<MSubModules.size(); i++) MSubModules[i]->processSample();
    }

    //----------

    virtual
    void postProcess(void) {
      for (int32 i=0; i<MSubModules.size(); i++) MSubModules[i]->postProcess();
    }

    //----------

};

//----------------------------------------------------------------------
//----------------------------------------------------------------------


//----------------------------------------------------------------------
#endif

