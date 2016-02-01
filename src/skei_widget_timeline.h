#ifndef skei_widget_timeline_included
#define skei_widget_timeline_included
//----------------------------------------------------------------------



//----------------------------------------------------------------------
#endif












#if 0

{$include kode.inc}
unit kode_widget_timeline;

{
  todo:
  - on_leave.. unselect hovertrack/segment
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  kode_canvas,
  kode_color,
  //kode_const,
  kode_flags,
  kode_rect,
  kode_timeline,
  kode_widget;

type

  KWidget_Timeline = class(KWidget)
  private
    FZoom             : Single;
    FStartPos         : Single;
    FPrevMouseX       : LongInt;
    FPrevMouseY       : LongInt;
  private
    FHoverTrack       : KTimeline_Track;
    FHoverSegment     : KTimeline_Segment;
    FHoverTime        : Single;
    FHoverLeft        : Boolean;
    FHoverRight       : Boolean;
  private
    FDraggingSegment  : KTimeline_Segment;
    FDraggingLeft     : Boolean;
    FDraggingRight    : Boolean;
  protected
    FTimeline         : KTimeline;
    FBackColor        : KColor;
    FTrackBackColor   : KColor;
    FTrackTextColor   : KColor;
    FSegmentBackColor : KColor;
    FSegmentTextColor : KColor;
    FBorderColor      : KColor;
    FTextColor        : KColor;
  public
    property          zoom : Single read FZoom write FZoom;
    property          startpos : Single read FStartPos write FStartPos;
  public
    constructor create(ARect:KRect; ATimeline:KTimeline; AAlignment:LongWord=kwa_none);
    //destructor  destroy; override;
    function    calcLength(APixels:LongInt) : Single;
    function    calcPixels(ATime:LongInt) : LongInt;
    function    calcTime(AXpos:LongInt) : Single;
    function    calcTimeDiff(AXpos,APrev:LongInt) : Single;
    function    calcXpos(ATime:Single) : LongINt;
    function    findTrack(AYpos:LongInt) : KTimeline_Track;
    function    findSegment(ATrack:KTimeline_Track; ATime:Single) : KTimeline_Segment;
  public
    procedure   on_mouseDown(AXpos,AYpos,AButton,AState:LongInt); override;
    procedure   on_mouseUp(AXpos,AYpos,AButton,AState:LongInt); override;
    procedure   on_mouseMove(AXpos,AYpos,AState:LongInt); override;
    procedure   on_leave(AWidget:KWidget); override;
    procedure   on_paint(ACanvas:KCanvas; ARect:KRect; AMode:LongWord=0); override;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  {%H-}kode_const,
  {%H-}kode_debug,
  kode_list,
  kode_math;

const
  TRACK_HEIGHT      = 20;
  TRACK_WIDTH       = 100;
  PIXELS_PER_SECOND = 10;
  SECONDS_PER_PIXEL = (1/PIXELS_PER_SECOND);
  DRAG_HANDLE_SIZE  = 5;

//----------

constructor KWidget_Timeline.create(ARect:KRect; ATimeline:KTimeline; AAlignment:LongWord);
begin
  inherited create(ARect,AAlignment);
  FTimeline         := ATimeline;
  FName             := 'KWidget_Timeline';
  FStartPos         := 0;
  FBackColor        := KGrey;
  FTrackBackColor   := KDarkGrey;
  FTrackTextColor   := KLightGrey;
  FSegmentBackColor := KLightGrey;
  FSegmentTextColor := KDarkGrey;
  FBorderColor      := KBlack;
  FZoom             := 1;
  FStartPos         := 0;
  FPrevMouseX       := 0;
  FPrevMouseY       := 0;
  {}
  FHoverTrack       := nil;
  FHoverSegment     := nil;
  FHoverTime        := -1;
  FHoverLeft        := false;
  FHoverRight       := false;
  {}
  FDraggingSegment  := nil;
  FDraggingLeft     := false;
  FDraggingRight    := false;

end;

//----------

function KWidget_Timeline.calcLength(APixels:LongInt) : Single;
var
  time : single;
begin
  time := single( APixels * SECONDS_PER_PIXEL );
  result := time / FZoom;
end;

//----------

function KWidget_Timeline.calcPixels(ATime:LongInt) : LongInt;
var
  pixel : single;
begin
  pixel := ATime * PIXELS_PER_SECOND;
  result := trunc( pixel*FZoom );
end;

//----------

function KWidget_Timeline.calcTime(AXpos:LongInt) : Single;
var
  time : single;
begin
  if FZoom <= 0 then exit(-1);
  time := single( AXpos - TRACK_WIDTH );
  if time < 0 then exit(-1);
  time *= SECONDS_PER_PIXEL;
  result := (FStartPos + time) / FZoom;
end;

//----------

function KWidget_Timeline.calcTimeDiff(AXpos,APrev:LongInt) : Single;
var
  diff : single;
begin
  if FZoom <= 0 then exit(0);
  diff := single( AXpos - APrev );
  //if time < 0 then exit(0);
  diff *= SECONDS_PER_PIXEL;
  result := diff / FZoom;
end;

//----------

function KWidget_Timeline.calcXpos(ATime:Single) : LongInt;
var
  pixel : Single;
begin
  if ATime >= 0 then
  begin
    pixel := ((ATime*FZoom) * PIXELS_PER_SECOND);
    result := TRACK_WIDTH + trunc(pixel);
  end
  else result := -1;
end;

//----------

function KWidget_Timeline.findTrack(AYpos:LongInt) : KTimeline_Track;
var
  y,t : longint;
  node : KListNode;
begin
  y := AYpos - FRect.y;
  if y < 0 then exit(nil);
  t := y div TRACK_HEIGHT;
  result := nil;
  node := FTimeline.tracks.head;
  while Assigned(node) do
  begin
    if t = 0 then exit(node as KTimeline_Track);
    t -= 1;
    node := node.next;
  end;
end;

//----------

function KWidget_Timeline.findSegment(ATrack:KTimeline_Track; ATime:Single) : KTimeline_Segment;
var
  node : KListNode;
  segment : KTimeline_Segment;
begin
  result := nil;
  node := ATrack.segments.head;
  while Assigned(node) do
  begin
    segment := node as KTimeline_Segment;
    if (ATime >= segment.startpos) and (ATime <= segment.endpos) then exit(segment);
    node := node.next;
  end;
end;

//----------------------------------------------------------------------
// on_
//----------------------------------------------------------------------

procedure KWidget_Timeline.on_mouseDown(AXpos,AYpos,AButton,AState:LongInt);
var
  changed : boolean;
begin
  changed := false;
  if AButton = kmb_left then
  begin
    FPrevMouseX := AXpos;
    FPrevMouseY := AYpos;
    if not Assigned(FHoverSegment) then exit;
    FDraggingSegment := FHoverSegment;
    FDraggingLeft := FHoverLeft;
    FDraggingRight := FHoverRight;
  end;
  if changed then do_redraw(self,FRect);
  inherited;
end;

//----------

procedure KWidget_Timeline.on_mouseUp(AXpos,AYpos,AButton,AState:LongInt);
var
  changed : boolean;
begin
  changed := false;
  if AButton = kmb_left then
  begin
    //if not Assigned(FDraggingSegment) then exit;
    FDraggingSegment := nil;
    FDraggingLeft := false;
    FDraggingRight := false;
  end;
  if changed then do_redraw(self,FRect);
  inherited;
end;

//----------

procedure KWidget_Timeline.on_mouseMove(AXpos,AYpos,AState:LongInt);
var
  track   : KTimeline_Track;
  segment : KTimeline_Segment;
  prv,nxt : KTimeline_Segment;
  changed : boolean;
  leftx,rightx:longint;
  hoverleft,hoverright : boolean;
  diff : single;
  st,en,le,temp : single;
  mintime : single;
begin
  changed := false;
  track := findTrack(AYpos);
  segment := nil;
  FHoverTime := calcTime(AXpos);

  if Assigned(FDraggingSegment) then
  begin
    mintime := calcLength(10); // minimum 10 pixels
    diff := calcTimeDiff(AXpos,FPrevMouseX);
    st := FDraggingSegment.startpos;
    en := FDraggingSegment.endpos;

    { dragging left }
    if FDraggingLeft then
    begin
      st += diff;
      st := KMax(st,0);
      if st > (en-mintime) then st := (en-mintime);
    end else

    { dragging right }
    if FDraggingRight then
    begin
      en += diff;
      if en <  (st+mintime) then en := (st+mintime);
    end else

    { dragging segment }
    begin
      le := en - st;
      st += diff;
      st := KMax(st,0);
      en := st + le;
    end;

    prv := FDraggingSegment.prev as KTimeline_Segment;
    if Assigned(prv) then
    begin
      if st <= prv.endpos then
      begin
        le := en - st;
        st := prv.endpos {+ 0.0001};
        if not FDraggingLeft then en := st + le;
      end;
    end;

    nxt := FDraggingSegment.next as KTimeline_Segment;
    if Assigned(nxt) then
    begin
      if en >= nxt.startpos then
      begin
        le := en - st;
        en := nxt.startpos {- 0.0001};
        if not FDraggingRight then st := en - le;
      end;
    end;

    FDraggingSegment.startpos := st;
    FDraggingSegment.endpos := en;
    changed := true;

  end // dragging
  else
  begin // not dragging

    if Assigned(track) then
    begin
      segment := findSegment(track,FHoverTime);
      if Assigned(segment) then
      begin
        leftx  := calcXpos(segment.startpos);
        rightx := calcXpos(segment.endpos);
        if AXpos < (leftx+DRAG_HANDLE_SIZE) then hoverleft := true else hoverleft := false;
        if AXpos >= (rightx-DRAG_HANDLE_SIZE) then hoverright := true else hoverright := false;
      end;
    end;
    if track <> FHoverTrack   then
    begin
      FHoverTrack := track;
      changed := true;
    end;
    if segment <> FHoverSegment then
    begin
      FHoverSegment := segment;
      changed := true;
    end;
    if hoverleft <> FHoverLeft   then
    begin
      FHoverLeft := hoverleft;
      changed := true;
    end;
    if hoverright <> FHoverRight   then
    begin
      FHoverRight := hoverright;
      changed := true;
    end;

  end; // not dragging

  if changed then do_redraw(self,FRect);
  FPrevMouseX := AXpos;
  FPrevMouseY := AYpos;
  inherited;
end;

//----------

procedure KWidget_Timeline.on_leave(AWidget:KWidget);
var
  changed : boolean;
begin
  changed := false;
  if Assigned(FHoverTrack) then
  begin
    FHoverTrack := nil;
    changed := true;
  end;
  if Assigned(FHoverSegment) then
  begin
    FHoverSegment := nil;
    changed := true;
  end;
  if changed then do_redraw(self,FRect);
  inherited;
end;

//----------

procedure KWidget_Timeline.on_paint(ACanvas:KCanvas; ARect:KRect; AMode:LongWord=0);
var
  tnode   : KListNode;
  snode   : KListNode;
  track   : KTimeline_Track;
  segment : KTimeline_Segment;
  x1,y1   : longint;
  x2,y2   : longint;
  ss,se   : single;
  cur     : single;
begin
  ACanvas.setFillColor(FBackColor);
  ACanvas.fillRect(FRect.x,FRect.y,FRect.x2,FRect.y2);
  ACanvas.setDrawColor(FBorderColor);
  ACanvas.drawLine(FRect.x,FRect.y,FRect.x2,FRect.y);
  x1 := FRect.x;
  //x2 := FRect.x2;
  y1 := FRect.y;
  tnode := FTimeline.tracks.head;
  while Assigned(tnode) do
  begin
    track := tnode as KTimeline_Track;
    x2 := x1 + TRACK_WIDTH - 1;
    y2 := y1 + TRACK_HEIGHT - 1;
    { track background }
    if track = FHoverTrack then ACanvas.setFillColor( color(0.3,0.3,0.3) ) else
    ACanvas.setFillColor(FTrackBackColor);
    ACanvas.fillRect(x1,y1,x2,y2);
    { track name }
    ACanvas.setTextColor(FTrackTextColor);
    ACanvas.drawText(x1+2,y1,x2-2,y2,track.name, kta_center);
    { track border }
    ACanvas.setDrawColor(FBorderColor);   // below
    ACanvas.drawLine(x1,y2,FRect.x2,y2);  // below
    ACanvas.drawLine(x2,y1,x2,y2);        // right
    { track segments }
    snode := track.segments.head;
    while Assigned(snode) do
    begin
      segment := snode as KTimeline_Segment;
      ss := segment.startpos * FZoom * PIXELS_PER_SECOND;
      se := segment.endpos * FZoom * PIXELS_PER_SECOND;
      { back }
      if segment = FHoverSegment then ACanvas.setFillColor( color(0.7,0.7,0.7) ) else
      ACanvas.setFillColor(FSegmentBackColor);
      ACanvas.fillRect(x2+1+trunc(ss),y1,x2+1+trunc(se),y2-1);
      { name }
      ACanvas.setTextColor(FSegmentTextColor);
      ACanvas.drawText(x2+1+trunc(ss),y1,x2+1+trunc(se),y2,segment.name,kta_center);
      { border }
      ACanvas.setDrawColor(FBorderColor);
      //ACanvas.drawRect(x2+1+trunc(ss),y1,x2+1+trunc(se),y2);
      ACanvas.drawLine(x2+1+trunc(ss),y1,x2+1+trunc(ss),y2);
      ACanvas.drawLine(x2+1+trunc(se),y1,x2+1+trunc(se),y2);
      { resize indicators }
      if (segment = FHoverSegment) then
      begin
        ACanvas.setFillColor( KDarkGrey );
        if FHoverLeft  then ACanvas.fillRect(x2+1+trunc(ss),y1,x2+1+trunc(ss)+(DRAG_HANDLE_SIZE-1),y2);
        if FHoverRight then ACanvas.fillRect(x2+1+trunc(se)-(DRAG_HANDLE_SIZE-1),y1,x2+1+trunc(se),y2);
      end;
      snode := snode.next;
    end;

    y1 += TRACK_HEIGHT;
    tnode := tnode.next;
  end;
  cur := FTimeline.cursor * FZoom * PIXELS_PER_SECOND;
  ACanvas.setDrawColor( KLightRed );
  ACanvas.drawLine( FRect.x + trunc(cur), FRect.y, FRect.x + trunc(cur), FRect.y2 );
  inherited;
end;

//----------------------------------------------------------------------
end.


#endif // 0
