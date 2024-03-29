#ifndef skei_host_sync_included
#define skei_host_sync_included
//----------------------------------------------------------------------

/*
  - handles sample accurate host synchronization
    (synced to host beat position)
  - you can have multiple intervals
  - measured in beats
  ***
  when loop is on, slice #0 is called twice..
  todo: check if prev_slice = slice ??

  bitwig handles looping a bit weird..
  i get an extra transport changed / repeat or something
  when it loops back.. (todo: investigare)

*/

//----------

#include "skei_array.h"

#define SKEI_MAX_INTERVALS 16

//----------

struct SInterval {
  int32 MSlice;
  int32 MSlicePrev;
  float MSliceSize;
};

//----------

class SHostSync_Listener {
  public:
    virtual void on_hostsync_tick(uint32 AIndex) {}
};

//----------

class SHostSync {

  private:

    SHostSync_Listener* MListener;
    SInterval           MIntervals[SKEI_MAX_INTERVALS];
    uint32              MNumIntervals;
    float               MTempo;
    float               MSampleRate;

  private:

    float               MBeatPos;
    float               MBeatSize;
    float               MBeatAdd;
    bool                MLoopActive;
    float               MLoopStart;
    float               MLoopEnd;

  public:

    SHostSync(SHostSync_Listener* AListener) {
      MListener = AListener;
      SMemset(MIntervals,0,sizeof(MIntervals));
      MNumIntervals = 0;
      MTempo        = 0;
      MSampleRate   = 0;
      MBeatPos      = 0;
      MBeatSize     = 0;
      MBeatAdd      = 0;
      MLoopActive   = false;
      MLoopStart    = 0;
      MLoopEnd      = 0;
    }

    //----------

    virtual ~SHostSync() {
    }

    //----------

    uint32 appendInterval(float ASize) {
      uint32 index = MNumIntervals;
      MIntervals[index].MSlice = 0;
      MIntervals[index].MSlicePrev = -1;
      MIntervals[index].MSliceSize = ASize;
      MNumIntervals += 1;
      return index;
    }

    //----------

    /*
      when MSlicePrev = -1, we get two callbacks when we start playback..
      when = 0, we don't get the initial, first callback..
    */


    void restart(void) {
      for (uint32 i=0; i<MNumIntervals; i++) {
        MIntervals[i].MSlice = 0;
        MIntervals[i].MSlicePrev = -1;
      }

    }

    //----------

    int32 getSlice(uint32 AIndex) {
      return MIntervals[AIndex].MSlice;
    }

    //----------

    float getBeatPos(void) {
      return MBeatPos;
    }

    //----------

    float getBeatSize(void) {
      return MBeatSize;
    }

    //----------

    void setSliceSize(uint32 AIndex, float ASize) {
      MIntervals[AIndex].MSliceSize = ASize;
    }

    //----------

    float getSliceSize(uint32 AIndex) {
      return MIntervals[AIndex].MSliceSize;
    }


    //----------

    void setLoop(bool AActive, float AStart, float AEnd) {
      MLoopActive = AActive;
      MLoopStart = AStart;
      MLoopEnd = AEnd;
      //if (MLoopActive) {
      //  MSliceFirst = STrunc(MLoopStart/MSliceSize);
      //}
      //else {
      //  MSliceFirst = 0;
      //}
    }

    //----------

    void setTempo(float ATempo) {
      MTempo = ATempo;
    }

    //----------

    void setSampleRate(float ASampleRate) {
      MSampleRate = ASampleRate;
    }

    //----------

    void setBeatPos(float ABeatPos) {
      SAssert(MTempo>0);
      MBeatSize = MSampleRate * 60.0f / MTempo;
      SAssert(MBeatSize>0);
      MBeatAdd = 1.0f / MBeatSize;
      MBeatPos = ABeatPos;
    }

    //------------------------------

    //void preProcess(void) {
    //}

    //virtual void on_tick(uint32 AIndex) {
    //  //STrace("tick(%i) = %i\n",AIndex,MIntervals[AIndex].MSlice);
    //}

    //----------


    //bool process(void) {
    uint32 process(void) {
      uint32 bits = 0;
      for (uint32 i=0; i<MNumIntervals; i++) {
        SAssert( MIntervals[i].MSliceSize > 0 );
        float b = MBeatPos / MIntervals[i].MSliceSize;
        MIntervals[i].MSlice = STrunc(b);
        if (MIntervals[i].MSlice != MIntervals[i].MSlicePrev) {
          MListener->on_hostsync_tick(i);
          bits += (1<<i);
        }
        MIntervals[i].MSlicePrev = MIntervals[i].MSlice;
      }
      MBeatPos += MBeatAdd;
      if (MLoopActive) {
        if (MBeatPos >= MLoopEnd) {
          MBeatPos = MLoopStart + (MBeatPos-MLoopEnd);
          restart();
        }
      }
      return bits;
    }

};

//----------------------------------------------------------------------
#endif
