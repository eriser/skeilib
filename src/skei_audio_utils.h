#ifndef skei_audio_utils_included
#define skei_audio_utils_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// mono
//----------------------------------------------------------------------

static __SKEI_UNUSED
void SCopyMonoBuffer(SSample** ADst, SSample** ASrc, uint32 ALength) {
  SMemcpy(ADst[0],ASrc[0],ALength*sizeof(SSample));
  //SMemcpy(ADst[1],ASrc[1],ALength*sizeof(SSample));
}

//----------

static __SKEI_UNUSED
void SClearMonoBuffer(SSample** ADst, uint32 ALength) {
  SMemset(ADst[0],0,ALength*sizeof(SSample));
  //SMemset(ADst[1],0,ALength*sizeof(SSample));
}

//----------

static __SKEI_UNUSED
void SAddMonoBuffer(SSample** ADst, SSample** AAdd, uint32 ALength) {
  SSample* dst0 = ADst[0];
  SSample* add0 = AAdd[0];
  //SSample* dst1 = ADst[1];
  //SSample* add1 = AAdd[1];
  for(uint32 i=0; i<ALength; i++) {
    *dst0++ += *add0++;
    //*dst1++ += *add1++;
  }
}

//----------

static __SKEI_UNUSED
void SScaleMonoBuffer(SSample** ADst, float AScale, uint32 ALength) {
  SSample* dst0 = ADst[0];
  SSample* dst1 = ADst[1];
  for(uint32 i=0; i<ALength; i++) {
    *dst0++ *= AScale;
    *dst1++ *= AScale;
  }
}

//----------------------------------------------------------------------
// stereo
//----------------------------------------------------------------------

static __SKEI_UNUSED
void SCopyStereoBuffer(SSample** ADst, SSample** ASrc, uint32 ALength) {
  SMemcpy(ADst[0],ASrc[0],ALength*sizeof(SSample));
  SMemcpy(ADst[1],ASrc[1],ALength*sizeof(SSample));
}

//----------

static __SKEI_UNUSED
void SClearStereoBuffer(SSample** ADst, uint32 ALength) {
  SMemset(ADst[0],0,ALength*sizeof(SSample));
  SMemset(ADst[1],0,ALength*sizeof(SSample));
}

//----------

static __SKEI_UNUSED
void SAddStereoBuffer(SSample** ADst, SSample** AAdd, uint32 ALength) {
  SSample* dst0 = ADst[0];
  SSample* add0 = AAdd[0];
  SSample* dst1 = ADst[1];
  SSample* add1 = AAdd[1];
  for(uint32 i=0; i<ALength; i++) {
    *dst0++ += *add0++;
    *dst1++ += *add1++;
  }
}

//----------

static __SKEI_UNUSED
void SScaleStereoBuffer(SSample** ADst, float AScale, uint32 ALength) {
  SSample* dst0 = ADst[0];
  SSample* dst1 = ADst[1];
  for(uint32 i=0; i<ALength; i++) {
    *dst0++ *= AScale;
    *dst1++ *= AScale;
  }
}

//----------

static __SKEI_UNUSED
void SScaleStereoBuffer(SSample** ADst, float ALeft, float ARight, uint32 ALength) {
  SSample* dst0 = ADst[0];
  SSample* dst1 = ADst[1];
  for(uint32 i=0; i<ALength; i++) {
    *dst0++ *= ALeft;
    *dst1++ *= ARight;
  }
}

//----------------------------------------------------------------------
// mono to stereo
//----------------------------------------------------------------------

static __SKEI_UNUSED
void SCopyMonoToStereoBuffer(SSample** ADst, SSample** ASrc, uint32 ALength) {
  SSample* dst0 = ADst[0];
  SSample* dst1 = ADst[1];
  SSample* src0 = ASrc[0];
  for (uint32 i=0; i<ALength; i++) {
    SSample spl0 = *src0++;
    *dst0++ = spl0;
    *dst1++ = spl0;
  }
}

//----------

static __SKEI_UNUSED
void SAddMonoToStereoBuffer(SSample** ADst, SSample** ASrc, uint32 ALength) {
  SSample* dst0 = ADst[0];
  SSample* dst1 = ADst[1];
  SSample* src0 = ASrc[0];
  for(uint32 i=0; i<ALength; i++) {
    SSample spl0 = *src0++;
    *dst0++ += spl0;
    *dst1++ += spl0;
  }
}

//----------------------------------------------------------------------
#endif
