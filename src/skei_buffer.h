#ifndef skei_buffer_included
#define skei_buffer_included
//----------------------------------------------------------------------

// TODO

#include "skei_memory.h"

//----------------------------------------------------------------------

template<class _T> class SBuffer;

typedef SBuffer<uint8>  SByteBuffer;
typedef SBuffer<char>   SCharBuffer;
typedef SBuffer<char*>  SStringBuffer; // conflicts with SString class...
typedef SBuffer<int32>  SIntBuffer;
typedef SBuffer<uint32> SUIntBuffer;
typedef SBuffer<void*>  SPtrBuffer;
typedef SBuffer<float>  SFloatBuffer;

//----------------------------------------------------------------------

template<class _T>
class SBuffer {

  private:

    _T*     MData;
    uint32  MSize;
    uint32  MReadPos;
    uint32  MWritePos;

  public:

    SBuffer() {
      MSize = 0;
      MData = SKEI_NULL;
    }

    //----------

    SBuffer(uint32 ASize) {
      setSize(ASize);
    }

    //----------

    //virtual
    ~SBuffer() {
      if (MData) SFree(MData);
    }

    //----------

    void setSize(uint32 ASize) {
      if (MData) MFree(MData);
      MSize = ASize;
      MData = (_T*)SMalloc( MSize * sizeof(_T) );
    }

    //----------

    void resize(uint32 ASize) {
      MSize = ASize;
      MData = (_T*)SRealloc( MData, MSize * sizeof(_T) );
    }

    //----------

    _T& operator [] (const uint32 AIndex) {
      return MData[AIndex];
    }

    //----------

    _T& item(const uint32 AIndex) {
      return MData[AIndex];
    }

    //----------

    uint32 size(void) {
      return MSize;
    }

    //----------

    uint32 itemSize(void) {
      return sizeof(_T);
    }

    //----------

    void* ptr(void) {
      return MData;
    }

    //----------

    SBuffer& operator = (const SBuffer &ABuffer) {
      if (this==&ABuffer) return *this;
      //if (ABuffer.MSize==0) clear();
      setSize(ABuffer.MSize);
      SMemcpy(MData, ABuffer.MData, sizeof(_T)*ABuffer.MSize);
      return *this;
    }

    //----------

    void clear(void) {
      SMemset(MData,0,MSize*sizeof(_T));
    }

    //----------

    void fill(uint8 AValue) {
      SMemset(MData,AValue,MSize*sizeof(_T));
    }

    //----------

    void copy(_T* APointer, uint32 ASize) {
      if (ASize != MSize) resize(ASize);
      SMemcpy(MData,MSize*sizeof(_T));
    }

    //----------------------------------------
    // circular buffer
    //----------------------------------------

    void    reset(void)           { MReadPos=0; MWritePos=0; }
    void    readPos(uint32 APos)  { MReadPos=APos; }
    void    writePos(uint32 APos) { MWritePos=APos; }
    uint32  readPos(void)         { return MReadPos; }
    uint32  writePos(void)        { return MWritePos; }

    //----------

    _T read(void) {
      _T result = MData[MReadPos];
      MReadPos++;
      if (MReadPos>=MSize) MReadPos = 0;
      return result;
    }

    //----------

    _T write(_T AItem) {
      _T result = MData[MWritePos];
      MData[MWritePos] = AItem;
      MWritePos++;
      if (MWritePos>=MSize) MWritePos = 0;
      return result;
    }

};

//----------------------------------------------------------------------
#endif


