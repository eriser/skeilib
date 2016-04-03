#ifndef skei_channel_included
#define skei_channel_included
//----------------------------------------------------------------------

/*

  aka SModule_AudioOut;

  channel
    is-a graph
    processBlock()
*/

//----------------------------------------------------------------------

#include "skei_graph.h"

//----------

class SChannel
: public SGraph {

  private:

    /*
    //__SKEI_ALIGNED(32)
    SSample   MTickBuffer[SKEI_OPERATOR_TICKSIZE*2]; // stereo
    uint32    MTickRemaining;
    uint32    MBlockOffset;
    SSample*  MLeftOutput;
    SSample*  MRightOutput;
    float*    MParamPtr[SKEI_OPERATOR_MAX_PARAMS];
    */

  public:

    SChannel(SOperatorGlobals* AGlobals)
    : SGraph(AGlobals) {
      /*
      SMemset(MTickBuffer,0,sizeof(MTickBuffer));
      MTickRemaining  = 0;
      MBlockOffset    = 0;
      MLeftOutput     = SKEI_NULL;
      MRightOutput    = SKEI_NULL;
      SMemset(MParamPtr,0,sizeof(MParamPtr));
      */
    }

    //----------

    virtual ~SChannel() {
    }

  //----------------------------------------
  // accessors
  //----------------------------------------

  public:

    //SSample* tickBuffer(uint32 i) { return MTickBuffer + SKEI_OPERATOR_TICKSIZE*i; }

  //----------------------------------------
  // config
  //----------------------------------------

  public:

    /*
      where processBuffer should copy the final buffer
    */

    /*
    void connectOutput(SSample* ALeft, SSample* ARight) {
      MLeftOutput = ALeft;
      MRightOutput = ARight;
    }
    */

    //----------

    /*
    void connectParameter(uint32 AIndex, float* APtr) {
      MParamPtr[AIndex] = APtr;
    }
    */

  //----------------------------------------
  // internal
  //----------------------------------------

  private:

    /*
      copy from internal per-tick buffer to output buffer
    */

    /*
    void copyBufferedTick(uint32 ALength=SKEI_OPERATOR_TICKSIZE, uint32 AOffset=0) {
      SSample* dst;
      SSample* src;
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
    */

    /*
      called by channel.processBlock just before each processTick
      todo:
      - midi buffer
      - keep track of block offset
      - check if next event is equal block event..
    */

    /*
    void processEvents(void) {
      eventNoteOn(0,36,127);
      eventNoteOff(0,36,127);
      eventCtrl(0,1,65);
      eventBend(0,65);
    }
    */

    //----------

  //----------------------------------------
  //
  //----------------------------------------

  public:

    /*
      hmm.. shouldn't the module be notified in some way,
      in case it needs to do some processing?
      interpolate parameters

      MParamPtr is set up in connectParameter
    */

    /*
    void parameterChange(int32 AIndex, float AValue) {
      //processParameter(AIndex,AValue);
      *MParamPtr[AIndex] = AValue;
    }
    */

    //----------

    /*
    void midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      //processMidi(AOffset,AMsg1,AMsg2,AMsg3);
    }
    */

    //----------

    //void processBlock(SSample** AInputs, SSample** AOutputs, uint32 ALength) {
    //}

  //----------------------------------------
  //
  //----------------------------------------

  public:

    /*
    void processBlock(SSample** AInputs, SSample** AOutputs, uint32 ALength) {
      connectOutput(AOutputs[0],AOutputs[1]);
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

      //SGraph::processBlock(AInputs,AOutputs,ALength);

    }
    */

    //----------

};

//----------------------------------------------------------------------
#endif

