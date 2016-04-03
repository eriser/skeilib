#ifndef skei_module_included
#define skei_module_included
//----------------------------------------------------------------------

/*
  module
    list of operators
*/

//----------------------------------------------------------------------

#include "skei_array.h"
#include "skei_operator.h"

//----------------------------------------------------------------------

class SModule;
typedef SArray<SModule*> SModuleArray;

//----------

class SModule {

  //friend class SGraph;

  private:

    SModule*          MParent;
    SOperatorGlobals* MOperatorGlobals;
    SOperatorArray    MOperators;
    SPtrArray         MOperatorStates;
    SFloatPtrArray    MOperatorParams;

    //__SKEI_ALIGNED(32)
    SSample   MTickBuffer[SKEI_OPERATOR_TICKSIZE*2]; // stereo
    uint32    MTickRemaining;
    uint32    MBlockOffset;
    SSample*  MLeftOutput;
    SSample*  MRightOutput;
    float*    MParamPtr[SKEI_OPERATOR_MAX_PARAMS];

  public:

    SModule(SOperatorGlobals* AGlobals) {
      MParent = SKEI_NULL;
      MOperatorGlobals = AGlobals;

      SMemset(MTickBuffer,0,sizeof(MTickBuffer));
      MTickRemaining  = 0;
      MBlockOffset    = 0;
      MLeftOutput     = SKEI_NULL;
      MRightOutput    = SKEI_NULL;
      SMemset(MParamPtr,0,sizeof(MParamPtr));

    }

    //----------

    virtual ~SModule() {
    }

  //----------------------------------------
  // accessors
  //----------------------------------------

  public:

    void      parent(SModule* AParent)  { MParent = AParent; }
    SModule*  parent(void)              { return MParent; }

    SSample* tickBuffer(uint32 i) { return MTickBuffer + SKEI_OPERATOR_TICKSIZE*i; }

  //----------------------------------------
  // config
  //----------------------------------------

  public:

    void appendOperator(SOperator AOperator, void* AState, float* AParams=SKEI_NULL) {
      MOperators.append(AOperator);
      MOperatorStates.append(AState);
      MOperatorParams.append(AParams);
    }

    //----------

    void connectOutput(SSample* ALeft, SSample* ARight) {
      MLeftOutput = ALeft;
      MRightOutput = ARight;
    }

    //----------

    void connectParameter(uint32 AIndex, float* APtr) {
      MParamPtr[AIndex] = APtr;
    }


  //----------------------------------------
  // internal (tick)
  //----------------------------------------

  private:

    //void processMidi(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
    //}

    void processEvents(void) {
      eventNoteOn(0,36,127);
      eventNoteOff(0,36,127);
      eventCtrl(0,1,65);
      eventBend(0,65);
    }

    //----------

    void processTick(void) {
      for (uint32 i=0; i<MOperators.size(); i++) {
        SAssert ( MOperators[i] != SKEI_NULL );
        MOperators[i]( MOperatorGlobals, MOperatorStates[i], MOperatorParams[i] );
      }
    }

    //----------

    /*
      copy from internal per-tick buffer to output buffer
      MTickBuffer -> MLEft/RightOutput
    */

    void copyBufferedTick(uint32 ALength=SKEI_OPERATOR_TICKSIZE, uint32 AOffset=0) {
      SSample* dst;
      SSample* src;
      SAssert(MLeftOutput!=SKEI_NULL);
      SAssert(MRightOutput!=SKEI_NULL);
      if (MLeftOutput) {
        dst = MLeftOutput + MBlockOffset;
        src = MTickBuffer + AOffset;
        SMemcpy(dst,src,ALength*sizeof(SSample));
      }
      if (MRightOutput) {
        dst = MRightOutput + MBlockOffset;
        src = MTickBuffer + SKEI_OPERATOR_TICKSIZE + AOffset;
        SMemcpy(dst,src,ALength*sizeof(SSample));
      }
      MBlockOffset += ALength;
    }

    //----------

    void processTicks(SSample** AInputs, SSample** AOutputs, uint32 ALength) {
      //connectOutput(AOutputs[0],AOutputs[1]);
      MBlockOffset = 0;
      if (MTickRemaining>0) {
        uint32 offset = SKEI_OPERATOR_TICKSIZE - MTickRemaining;
        if (ALength <= MTickRemaining) {
          copyBufferedTick(ALength,offset);
          MTickRemaining -= ALength;
          return;
        }
        else {
          copyBufferedTick(MTickRemaining,offset);
          ALength -= MTickRemaining;
          MTickRemaining = 0;
        }
      }
      uint32 post = ALength & SKEI_OPERATOR_TICKMASK;
      ALength -= post;
      uint32 ticks  = ALength >> SKEI_OPERATOR_TICKSHIFT;
      while (ticks>0) {
        processEvents();
        processTick();
        copyBufferedTick();
        ticks--;
      }
      if (post>0) {
        processEvents();
        processTick();
        copyBufferedTick(post);
        MTickRemaining = SKEI_OPERATOR_TICKSIZE - post;
      }

    }
  //----------------------------------------
  // tick
  //----------------------------------------

  protected:

    virtual void eventNoteOn(uint32 chn, uint32 note, uint32 vel) {}
    virtual void eventNoteOff(uint32 chn, uint32 note, uint32 vel) {}
    virtual void eventCtrl(uint32 chn, uint32 index, uint32 val) {}
    virtual void eventBend(uint32 chn, int32 bend) {}

  //----------------------------------------
  // block
  //----------------------------------------

  public:

    virtual
    void parameterChange(int32 AIndex, float AValue) {
      SAssert(MParamPtr!=SKEI_NULL);
      SAssert(MParamPtr[AIndex]!=SKEI_NULL);
      *MParamPtr[AIndex] = AValue;
    }

    //----------

    virtual
    void midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
    }

    //----------

    virtual
    void processBlock(SSample** AInputs, SSample** AOutputs, uint32 ALength) {
      //connectOutput(AOutputs[0],AOutputs[1]);
      processTicks(AInputs,AOutputs,ALength);
    }

    //----------


    //----------

};

//----------------------------------------------------------------------
#endif
