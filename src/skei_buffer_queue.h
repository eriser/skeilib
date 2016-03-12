#ifndef skei_buffer_queue_included
#define skei_buffer_queue_included
//----------------------------------------------------------------------

/*
  class to keep track of last buffer sent from another thread..
  for example, the last audio block during processing..

  todo: all blocks instead of just latest?
*/

#include "skei_queue.h"

//----------------------------------------------------------------------
// thread buffer
//----------------------------------------------------------------------

/*
  passes blocks of audio to another thread..
  (originally made for sending audio from the audio processing thread
  to the gui thread for visualization)
  sender (producer) calls write() for every sample
  receiver (consumer) calls getBuffer() to get the latest buffer..
  it is copied to an internal/temporary bufer, so it's safe to work on it
  until next time you call getBuffer()
*/

//----------------------------------------------------------------------

template<typename _T, int _SIZE>
class SBufferQueue {

  #define _Tsize sizeof(_T)
  typedef SQueue<int32,_SIZE> SIntQueue;

  private:
    _T*       MBuffer;
    _T*       MBufferCopy;
    SIntQueue MQueue;
    uint32    MBufferIndex;
    uint32    MBufferSize;  // pow2
    uint32    MBufferMask;  // MBufferSize-1
    uint32    MBufferCount; // sizeof(;Buffer) = MBufferSize*MBufferCount;
    uint32    MWritePos;
    uint32    MWriteMask;   // MBufferSize-1
    int32     MSkipped;

  public:

    SBufferQueue() {
      MBuffer       = SKEI_NULL;
      MBufferCopy   = SKEI_NULL;
      //MQueue.clear();
      MBufferIndex  = 0;
      MBufferSize   = _SIZE;//0;  // pow2
      MBufferMask   = _SIZE-1;//0;  // MBufferSize-1
      MBufferCount  = 0; // sizeof(;Buffer) = MBufferSize*MBufferCount;
      MWritePos     = 0;
      MWriteMask    = 0;   // MBufferSize-1
      MSkipped      = 0;
    }

    //----------

    virtual ~SBufferQueue() {
      if (MBuffer) SFree(MBuffer);
      if (MBufferCopy) SFree(MBufferCopy);
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    uint32 skipped(void)      { return MSkipped; }
    uint32 bufferIndex(void)  { return MBufferIndex; }

  public:

    void reset(void) {
      MWritePos = 0;
      MBufferIndex = 0;
      SMemset(MBufferCopy,0,_Tsize);
    }

    //----------

    // size must be power of two

    void setSize(uint32 ASize, uint32 ANum) {
      if ( (MBufferSize!=ASize) && (MBufferCount!=ANum) ) {
        MBufferSize   = ASize; // pow2
        MWriteMask    = MBufferSize - 1;
        MBufferCount  = ANum;
        MBufferMask   = MBufferCount - 1;
        if (MBuffer) SFree(MBuffer);
        if (MBufferCopy) SFree(MBufferCopy);
        MBuffer       = (float*)SMalloc(MBufferSize*MBufferCount*_Tsize);   // todo: aligned memory (for sse/fft/etc)
        MBufferCopy   = (float*)SMalloc(MBufferSize*_Tsize);                // --"--
        MWritePos     = 0;
        MBufferIndex  = 0;
        //MQueue.clear();
      }
    }

  //----------------------------------------
  // editor (reader)
  //----------------------------------------

    // editor call these
    bool getBuffer(float** ABuffer) {

      SAssert(MBufferSize>0);

      *ABuffer = MBufferCopy;
      int32 newbuf = -1;
      MSkipped = -1;
      while ( MQueue.read(&newbuf) ) { MSkipped++; }
      if (newbuf>=0) {
        SAssert(newbuf<MBufferCount);
        void* dst   = MBufferCopy;
        void* src   = &MBuffer[newbuf*MBufferSize];
        uint32 siz  = MBufferSize;
        SMemcpy(dst,src,siz*_Tsize);
        return true;
      }
      return false;
    }

  //----------------------------------------
  // audio process (writer)
  //----------------------------------------

  public:

    void write(_T n) {

      SAssert(MBufferSize>0);

      MBuffer[MWritePos] = n;
      MWritePos += 1;
      //MWritePos &= MWriteMask;
      if ((MWritePos & MWriteMask)==0) {
        MQueue.write(MBufferIndex);
        MBufferIndex += 1;
        MBufferIndex &= MBufferMask;
        if (MBufferIndex==0) {
          MWritePos = 0;
        }
      }
    }

};


//----------------------------------------------------------------------
#endif
