#ifndef skei_operator_included
#define skei_operator_included
//----------------------------------------------------------------------

/*
  operator
    dsp-function

  operators always process SKEI_OPERATOR_TICKSIZE samples
  use the 'data' argument to find pointer to needed buffers, etc..
  struct for each operator (type)..
*/

//----------------------------------------------------------------------

#include "skei_array.h"

//----------

#define SKEI_OPERATOR_MAX_PARAMS  16
#define SKEI_OPERATOR_TICKSHIFT   4
#define SKEI_OPERATOR_TICKSIZE    (1<<SKEI_OPERATOR_TICKSHIFT)
#define SKEI_OPERATOR_TICKMASK    (SKEI_OPERATOR_TICKSIZE-1)

//----------------------------------------------------------------------

struct SOperatorGlobals {
  float     sampleRate;
  uint32    blockSize; // ???
  float     beatPos;
  float     pitchBend;
  float     pitchBendRange;
};

//----------

typedef void (*SOperator)(void* global, void* data, float* params);
typedef SArray<SOperator> SOperatorArray;

//----------------------------------------------------------------------
#endif
