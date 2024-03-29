#ifndef skei_goertzel_included
#define skei_goertzel_included
//----------------------------------------------------------------------

// http://www.musicdsp.org/showArchiveComment.php?ArchiveID=107

float SGoertzel(float* ABuffer, uint32 ALength, float AFrequency, float ASamplerate) {
  float skn = 0;
  float skn1 = 0;
  float skn2 = 0;
  float temp1 = SKEI_PI2 * AFrequency / ASamplerate;
  float temp2 = cosf(temp1);
  for (uint32 i=0; i<ALength; i++) {
    skn2 = skn1;
    skn1 = skn;
    skn = 2*temp2*skn1 - skn2 + ABuffer[i];
  }
  return (skn - expf(-temp1)*skn1);
}

//----------

// Here's a variant on the theme that compensates for harmonics:
// http://www.musicdsp.org/showArchiveComment.php?ArchiveID=172

float SGoertzelHarmonics(float* ABuffer, uint32 ALength, float AFrequency, float ASamplerate) {
  float qkn = 0;
  float qkn1 = 0;
  float wkn = SKEI_PI2 * AFrequency / ASamplerate;
  float mk = 2 * cos(wkn);
  for (uint32 i=0; i<ALength; i++) {
    float qkn2 = qkn1;
    qkn1 = qkn;
    qkn  = ABuffer[i] + mk*qkn1 - qkn2;
  }
  return sqrt(qkn*qkn + qkn1*qkn1 - mk*qkn*qkn1);
}

//----------------------------------------------------------------------
#endif

/*
  see also:
  http://stackoverflow.com/questions/11579367/implementation-of-goertzel-algorithm-in-c
*/
