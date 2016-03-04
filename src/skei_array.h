#ifndef skei_array_included
#define skei_array_included
//----------------------------------------------------------------------

/// template dynamic array class (top)

/**
  template dynamic array class (top2)

  ...

  Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod
  tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,
  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo
  consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse
  cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non
  proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
*/

#include "skei_const.h"
#include "skei_memory.h"

//----------

/**
  template dynamic array class (mid)
*/

template<class _T>
class SArray {

  /**
    template dynamic array class (class)
  */

	private:

    _T*          MBuffer;
    /*unsigned int*/ int32 MTsize;
    /*unsigned int*/ int32 MSize;
    /*unsigned int*/ int32 MRealSize;

  public:

    //----------

    /**
      constructor
    */

    SArray() {
      MTsize = sizeof(_T);
      MRealSize = SKEI_ARRAY_SIZE_INIT;
      MSize = 0;
      MBuffer = (_T*)SMalloc(MRealSize*MTsize);
    }

    //----------

    /**
      constructor
      \param AArray array to copy
    */

    SArray(const SArray& AArray) {
      MTsize = sizeof(_T);
      MBuffer = (_T*)SMalloc(MTsize*AArray.MRealSize);
      SMemcpy(MBuffer, AArray.MBuffer, MTsize*AArray.MRealSize);
      MRealSize = AArray.MRealSize;
      MSize = AArray.MSize;
    }

    //----------

    /**
      destructor
    */

    ~SArray() {
      if (MBuffer) {
        SFree(MBuffer);
        MBuffer = SKEI_NULL;
      }
    }

    //----------

    void deleteItems(void) {
      for (int32 i=0; i<MSize; i++) delete MBuffer[i];
    }

    //----------

    /**
      x = SArray[n];
    */

    _T& operator [] (const /*unsigned int*/ int32 AIndex) {
      return MBuffer[AIndex];
    }

    //----------

    /**
      x = item(n);
    */

    _T& item(const /*unsigned int*/ int32 AIndex) {
      return MBuffer[AIndex];
    }

    //----------

    /**
      returns size of array (number of items)
    */

    /*int*/ int32 size() {
      return MSize;
    }

    //----------

    /**
      returns ptr to buffer (ddata)
    */

    void* ptr() {
      return MBuffer;
    }

    //----------

    /**
      copies array
    */

    SArray& operator = (const SArray &AArray) {
      if (this==&AArray) return *this;
      if (AArray.MSize==0) clear();
      setSize(AArray.MSize);
      SMemcpy(MBuffer, AArray.MBuffer, MTsize*AArray.MSize);
      return *this;
    }

    //----------

    /**
      clear items and optionally erase (reallocate) buffer
    */

    void clear(bool AErase=true) {
      MSize = 0;
      if (AErase) {
        MBuffer = (_T*)SRealloc((char*)MBuffer, MTsize*SKEI_ARRAY_SIZE_INIT);
        MRealSize = SKEI_ARRAY_SIZE_INIT;
      }
    }

    //----------

    /**
      append item to end of buffer
    */

    void append(const _T& AItem) {
      MSize++;
      if (MSize > MRealSize) {
        MRealSize *= SKEI_ARRAY_SIZE_MULT;
        MBuffer = (_T*)SRealloc(MBuffer, MTsize*MRealSize);
      }
      MBuffer[MSize-1] = AItem;
    }

    //----------

    /**
      set size of buffer
    */

    void setSize(const /*unsigned int*/ int32 ASize) {
      if (ASize != 0) {
        if ( (ASize>MRealSize) || (ASize<MRealSize/SKEI_ARRAY_SIZE_MULT) ) {
          MRealSize = ASize;
          MBuffer = (_T*)SRealloc(MBuffer, MTsize*MSize);
          MSize = ASize;
        }
      }
      else clear();
    }

    //----------

    /**
      remove item from buffer
    */

    void remove(const /*unsigned int*/ int32 APos) {
      if (MSize==1) clear();
      else {
        for(/*unsigned int*/ int32 i=APos; i<MSize-1; i++ ) MBuffer[i] = MBuffer[i+1];
        MSize--;
      }
    }

    //----------

    //TODO: insert

    //----------------------------------------
    //
    //----------------------------------------

    /**
      push item to end of buffer
    */

    void push(_T AItem) {
      append(AItem);
    }

    //----------

    /**
      pop item from end of buffer
    */

    _T pop(void) {
      if (MSize>0) {
        MSize--;
        return MBuffer[MSize];
      }
      else return SKEI_NULL;
    }

};

//----------------------------------------------------------------------
// standard/predefined array types
//----------------------------------------------------------------------

typedef SArray<char>  SCharArray;
typedef SArray<char*> SStringArray;
typedef SArray<int32> SIntegerArray;
typedef SArray<float> SFloatArray;
typedef SArray<void>  SPointerArray;
typedef SArray<bool>  SBoolArray;

//----------------------------------------------------------------------
#endif

