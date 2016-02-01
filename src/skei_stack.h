#ifndef skei_stack_included
#define skei_stack_included
//----------------------------------------------------------------------

/*
  - primitive
  - no error checking (yet)
*/

//----------

template<class _T,int SIZE>
class SStack
{
  private:
    _T  MBuffer[SIZE];
    int MIndex;

  public:

    SStack() {
      reset();
    }

    ~SStack() {
    }

    _T* getTop(void) {
      return &MBuffer[MIndex-1];
    }

    // a b -  ->  - - -

    void reset(void) {
      MIndex = 0;
    }

    // c  ->  a b -  ->  c a b -

    void push(_T AItem) {
      MBuffer[MIndex] = AItem;
      MIndex++;
    }

    // a b -  ->  b - -  -> [a]

    _T pop(void) {
      MIndex--;
      return MBuffer[MIndex];
    }

    // a b -  ->  a a b -

    void dup(void) {
      MBuffer[MIndex] = MBuffer[MIndex-1];
      MIndex++;
    }

    // a b -  ->  b a b -

    void dup2(void) {
      MBuffer[MIndex] = MBuffer[MIndex-2];
      MIndex++;
    }

    // a b -  ->  b - -  ->

    void drop(void) {
      MIndex--;
    }

};

//----------------------------------------------------------------------
#endif

