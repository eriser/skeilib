#ifndef skei_audio_utils_included
#define skei_audio_utils_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// mono
//----------------------------------------------------------------------

void copyMonoBuffer(SSample** ADst, SSample** ASrc, uint32 ALength) {
  SMemcpy(ADst[0],ASrc[0],ALength*sizeof(SSample));
  //SMemcpy(ADst[1],ASrc[1],ALength*sizeof(SSample));
}

//----------

void clearMonoBuffer(SSample** ADst, uint32 ALength) {
  SMemset(ADst[0],0,ALength*sizeof(SSample));
  //SMemset(ADst[1],0,ALength*sizeof(SSample));
}

//----------

void addMonoBuffer(SSample** ADst, SSample** AAdd, uint32 ALength) {
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

void scaleMonoBuffer(SSample** ADst, float AScale, uint32 ALength) {
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

void copyStereoBuffer(SSample** ADst, SSample** ASrc, uint32 ALength) {
  SMemcpy(ADst[0],ASrc[0],ALength*sizeof(SSample));
  SMemcpy(ADst[1],ASrc[1],ALength*sizeof(SSample));
}

//----------

void clearStereoBuffer(SSample** ADst, uint32 ALength) {
  SMemset(ADst[0],0,ALength*sizeof(SSample));
  SMemset(ADst[1],0,ALength*sizeof(SSample));
}

//----------

void addStereoBuffer(SSample** ADst, SSample** AAdd, uint32 ALength) {
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

void scaleStereoBuffer(SSample** ADst, float AScale, uint32 ALength) {
  SSample* dst0 = ADst[0];
  SSample* dst1 = ADst[1];
  for(uint32 i=0; i<ALength; i++) {
    *dst0++ *= AScale;
    *dst1++ *= AScale;
  }
}

//----------------------------------------------------------------------
// mono to stereo
//----------------------------------------------------------------------

void copyMonoToStereoBuffer(SSample** ADst, SSample** ASrc, uint32 ALength) {
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

void addMonoToStereoBuffer(SSample** ADst, SSample** ASrc, uint32 ALength) {
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
