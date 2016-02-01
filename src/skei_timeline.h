#ifndef skei_module_included
#define skei_module_included
//----------------------------------------------------------------------

#include "skei_list.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class STimeline_Segment
: public SListNode {

  private:

    const char* MName;
    float MStartPos;
    float MEndPos;
    bool  MSelected;

  public:

    void name(const char* AName) { MName = AName; }
    void start(float APos) { MStartPos = APos; }
    void end(float APos) { MEndPos = APos; }
    void select(float ASelected=true) { MSelected = ASelected; }

    const char* name(void) { return MName; }
    float start(void) { return MStartPos; }
    float end(void) { return MEndPos; }
    bool  selected(void) { return MSelected; }

  public:

    STimeline_Segment(const char* AName, float AStart, float AEnd)
    : SListNode() {
      MName     = AName;
      MStartPos = AStart;
      MEndPos   = AEnd;
      MSelected = false;
    }

    //----------

    virtual ~STimeline_Segment() {
    }

    //----------

    bool isInside(float ATime) {
      if ((ATime >= MStartPos) && (ATime <= MEndPos)) return true;
      else return false;
    }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class STimeline_Track
: public SListNode {

  private:

    char*   MName;
    SList   MSegments;
    bool    MSelected;

  public:

    char*   name(void) { return MName; }
    void    name(char* AName) { MName = AName; }
    SList*  segments(void) { return &MSegments; }
    void    select(float ASelected=true) { MSelected = ASelected; }
    bool    selected(void) { return MSelected; }

  public:

    STimeline_Track(char* AName)
    : SListNode() {
      MName = AName;
      MSelected = false;
    }

    //----------

    virtual ~STimeline_Track() {
      // todo: if no autodelete - deletesegments
    }

    //----------

    void addSegment(STimeline_Segment* ASegment) {
      MSegments.append(ASegment);
    }

    //----------

    void deleteSegments(void) {
      SListNode* node = MSegments.head();
      while (node) {
        STimeline_Segment* segment = (STimeline_Segment*)node;
        node = node->next();
        delete segment;
      }
    }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class STimeline
: public SList {

  private:

    SList MTracks;
    int32 MCursor;

  public:

    SList* tracks(void) { return &MTracks; }
    int32  cursor(void) { return MCursor; }
    void   cursor(int32 ACursor) { MCursor=ACursor; }

  public:

    STimeline() {
      MCursor = 0;
    }

    //----------

    virtual ~STimeline() {
    }

    //----------

    void addTrack(STimeline_Track* ATrack) {
      MTracks.append(ATrack);
    }

    //----------

    void deleteTracks(void) {
      SListNode* node = MTracks.head();
      while (node) {
        STimeline_Track* track = (STimeline_Track*)node;
        node = node->next();
        track->deleteSegments();
        delete track;
      }
    }

};

//----------------------------------------------------------------------
#endif
