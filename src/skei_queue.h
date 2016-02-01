#ifndef skei_queue_included
#define skei_queue_included
//----------------------------------------------------------------------

// thread-safe, lock-free queue
// single producer, single consumer, probably..

#include "skei_memory.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  WARNING:
  - loses (new) events if buffer is full (when writing)
*/

//----------

template<class _T, int SIZE>
class SQueue
{

  private:

    _T*     MBuffer;
    uint32  MBufferSize;
    uint32  MBufferMask;
    uint32  MReadPos;
    uint32  MWritePos;

  public:

    SQueue() {
      MBuffer     = (_T*)SMalloc(SIZE*sizeof(_T));
      MBufferSize = SIZE;
      MBufferMask = SIZE - 1;
      MReadPos    = 0;
      MWritePos   = 0;
    }

    //----------

    virtual ~SQueue() {
      SFree(MBuffer);
    }

    //----------

    void clear(void) {
      MReadPos  = 0;
      MWritePos = 0;
    }

    //----------

    /*
      TODO: remove when we know it works
      (wp-rp) and FBufferMask = 'items in buffer'
      in write(), we avoid filling the entire buffer, by letting there always be
      at least one free item, because read() could misinterpret a full buffer
      as empty..
      if the write fail (returns false), what should we do?
      - ignore, losing events..
      - sleep a little and try again? (spin?)
      - the reading happens at the start of process, so in worst case,
        if buffer is full already, we must wait until the next processBlock,
        (reaper's pause doesn't call process, and doesn't let us know it's paused, i think..)
      - buffer/cache updates in case the original buffer is full? double-buffer..
      buffer is emptied at the start of process()
    */

    bool read(_T* AItem) {
      uint32 wp = MWritePos;
      uint32 rp = MReadPos;
      //result := false;
      if (( (wp-rp) & MBufferMask ) > 0) {
        //AItem.FIndex := FBuffer[rp].FIndex;
        //AItem.FValue := FBuffer[rp].FValue;
        *AItem = MBuffer[rp];
        uint32 next = (rp+1) & MBufferMask;
        MReadPos = next;
        return true;
      }
      return false;
    }

    //----------

    bool write(_T AItem) {
      uint32 rp = MReadPos;
      uint32 wp = MWritePos;
      //result := false;
      // -1 to avoid filling the entire buffer..
      // .. which could be mis-interpreted by read() as empty..
      if (((wp-rp) & MBufferMask) < (MBufferSize-1)) {
        //FBuffer[wp].FIndex := AItem.FIndex;
        //FBuffer[wp].FValue := AItem.FValue;
        MBuffer[wp] = AItem;
        uint32 next = (wp+1) & MBufferMask;
        MWritePos = next;
        return true;
      }
      return false;
    }

    //----------

    /*
      check if there is an event with same index already in the queue
      todo: a) update value (replace event)
            b) remove old event, insert new at end of queue
    */

    // this is not thread-safe !!

    //bool hasEventWithIndex(int32 AIndex) {
    //  //result := False;
    //  uint32 = MReadPos;
    //  uint32 = MWritePos;
    //  while (rpos != wpos) {
    //    if FBuffer[rpos].FIndex =;
    //  }
    //}

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  safe for single-reader, single-writer ??
  (thread-safe?) circular/ring-buffer
*/

template<class _T, int SIZE>
class SQueue_Buffer
{

  private:

    _T            MData[SIZE];
    volatile int  MWritePos;    // volatile?
    volatile int  MReadPos;

  public:

    SQueue_Buffer() {
      SMemset(MData,0,SIZE*(sizeof(_T)));
      MWritePos = 0;
      MReadPos = 0;
    }

    //----------

    // write 1

    void write(_T a_Data) {
      MData[MWritePos] = a_Data;
      MWritePos = (MWritePos + 1) % SIZE; // & SIZE if ^2
    }

    //----------

    // return num items copied into buffer

    int read(_T* a_Buffer) {
      int count = 0;
      int writepos = MWritePos;
      if (MReadPos > writepos) {
        SMemcpy((char*)&a_Buffer[count],
                (char*)&MData[MReadPos],
                (SIZE - MReadPos) * sizeof(_T));
        count = SIZE - MReadPos;
        MReadPos = 0;
      }
      SMemcpy((char*)&a_Buffer[count],
              (char*)&MData[MReadPos],
              (writepos - MReadPos) * sizeof(_T));
      count += writepos - MReadPos;
      MReadPos = writepos;
      return count;
    }

    //----------

    //TODO: return one:
    //_T* read(void)
    //  {
    //  }

};

//----------------------------------------------------------------------
#endif

