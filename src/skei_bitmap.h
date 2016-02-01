#ifndef skei_bitmap_included
#define skei_bitmap_included
//----------------------------------------------------------------------

/*
  ram/client bitmap, rgba (32bit)

  MBuffer[0] = b
  MBuffer[1] = g
  MBuffer[2] = r
  MBuffer[3] = a

*/

//----------------------------------------------------------------------

#ifndef SKEI_NO_PNG
  #define UPNG_NO_LOAD_FROM_FILE
  //#include "extern/upng/upng/upng.h"
  #include "extern/upng/upng.c"
#endif // SKEI_NO_PNG

#include "skei_rect.h"


//----------------------------------------------------------------------

class SBitmap {

  private:

    int32  MWidth;
    int32  MHeight;
    //uint32  MDepth; // always 32
    int32  MPixelSize;
    int32  MBufferSize;
    uint8*  MBuffer;
    bool    MBufferAllocated;

    upng_t* upng;

  public:

    /*inline*/ int32 width(void)  { return MWidth; }
    /*inline*/ int32 height(void) { return MHeight; }
    // /*inline*/ uint32 depth(void)  { return MDepth; }
    /*inline*/ uint32* buffer(void) { return (uint32*)MBuffer; }

  public:

    SBitmap(int32 AWidth, int32 AHeight/*, uint32 ADepth*/) {
      MWidth      = AWidth;
      MHeight     = AHeight;
      //MDepth      = ADepth;
      MPixelSize  = sizeof(uint32); // rgba/32bit
      MBufferSize = AWidth * AHeight * MPixelSize;
      MBuffer     = (uint8*)SMalloc(MBufferSize);
      MBufferAllocated = true;
    }

    //----------

    SBitmap(int32 AWidth, int32 AHeight/*, uint32 ADepth*/, uint32* ABuffer) {
      MWidth      = AWidth;
      MHeight     = AHeight;
      //MDepth      = ADepth;
      MPixelSize  = sizeof(uint32);                               // rgba/32bit
      MBufferSize = AWidth * AHeight * MPixelSize;
      MBuffer     = (uint8*)ABuffer;
      MBufferAllocated = false;
    }

    //----------

    #ifndef SKEI_NO_PNG
    SBitmap(const uint8* buffer, uint32 length) {
      upng = upng_new_from_bytes(buffer,length);
      if (upng) {
        upng_header(upng);
        MWidth = upng_get_width(upng);
        MHeight = upng_get_height(upng);
        MPixelSize  = sizeof(uint32);                             // rgba/32bit
        MBufferSize = MWidth * MHeight * MPixelSize;
        MBuffer     = (uint8*)SMalloc(MBufferSize);
        MBufferAllocated = true;
        upng_decode(upng);
        SMemcpy(MBuffer,upng_get_buffer(upng),upng_get_size(upng));
        upng_free(upng);
        upng = SKEI_NULL;
      }
    }
    #endif // SKEI_NO_PNG

    //----------

    virtual ~SBitmap() {
      if (MBufferAllocated && MBuffer) SFree(MBuffer);
    }

    //----------------------------------------
    // todo: move these out of SBitmap
    // to skei_color.h ???
    //----------------------------------------

    /*

    //inline
    uint32 makeColor(uint8 r, uint8 g, uint8 b, uint8 a) {
      return   ((uint32)a<<24)
             + ((uint32)r<<16)
             + ((uint32)g<<8)
             + b;
    }

    //----------

    //inline
    uint8 alpha(uint32 c, uint32 a) {
      uint32 ret = (c*a) >> 8;
      return ret & 0xff;
    }

    //----------

    //inline
    uint8 scale(uint8 c, float n ) {
      float nc = n * (float)c;
      return (uint8)SMin(255,nc);
    }

    */

    //----------------------------------------
    //
    //----------------------------------------

    void clearBuffer(uint8 AValue) {
      int32 size = MWidth*MHeight*4;
      SMemset(MBuffer,AValue,size);
    }

    //----------

    void fillBuffer(uint32 AValue) {
      int32 size = MWidth*MHeight;
      uint32* ptr = (uint32*)MBuffer;
      for (int32 i=0; i<size; i++) *ptr++ = AValue;
    }

    //----------

    void fillAlpha(uint8 AValue) {
      //uint32* ptr = (uint32*)MBuffer[3];
      uint8* ptr = (uint8*)&MBuffer[3]; // ptr to first alpha value (r,g,b,a)
      int32 size = MWidth*MHeight;
      for (int32 i=0; i<size; i++) {
        *ptr = AValue;
        ptr += 4;
      }
    }

    //----------------------------------------
    //
    //----------------------------------------

    //inline
    uint32 getPixel(uint32 x, uint32 y) {
      uint32* ptr = (uint32*)MBuffer;
      return ptr[y*MWidth+x];
    }

    //----------

    //inline
    void setPixel(uint32 x, uint32 y, uint32 c) {
      uint32* ptr = (uint32*)MBuffer;
      ptr[y*MWidth+x] = c;
    }

    //----------------------------------------
    //
    //----------------------------------------

    void convertRgbaBgra(void) {
      if (MBuffer) {
        for (int32 y=0; y<MHeight; y++) {
          for (int32 x=0; x<MWidth; x++) {
            int32 pos = (y*MWidth + x) * 4;
            uint8 r = MBuffer[pos+0];
            uint8 g = MBuffer[pos+1];
            uint8 b = MBuffer[pos+2];
            uint8 a = MBuffer[pos+3];
            MBuffer[pos+0] = b;
            MBuffer[pos+1] = g;
            MBuffer[pos+2] = r;
            MBuffer[pos+3] = a;
          } //for x
        } //for y
      } //mBuffer
    }

    //----------

    /*
      change background color
      with alpha for transparency..
    */

    void setBackground(uint8 AR, uint8 AG, uint8 AB) {
      if (MBuffer) {
        for (int32 y=0; y<MHeight; y++) {
          for (int32 x=0; x<MWidth; x++) {
            int32 pos = (y*MWidth + x) * 4;
            uint8 r = MBuffer[pos+0];
            uint8 g = MBuffer[pos+1];
            uint8 b = MBuffer[pos+2];
            uint8 a = MBuffer[pos+3];
            MBuffer[pos+0] = alphaColor(r,a) + alphaColor(AR,(255-a));
            MBuffer[pos+1] = alphaColor(g,a) + alphaColor(AG,(255-a));
            MBuffer[pos+2] = alphaColor(b,a) + alphaColor(AB,(255-a));
            MBuffer[pos+3] = a;
          } //for x
        } //for y
      } //mBuffer
    }

    //----------

    /*
      set background from another bitmap,
      with alpha for transparency..
    */

    //TODO:
    void setBackground(SBitmap* ABitmap, SRect ARect) {
      uint8* buf2 = (uint8*)ABitmap->buffer();
      uint32 h = ABitmap->height();
      //trace("aBitmap->getHeight() = " << h );
      if (MBuffer) {
        uint32 x2 = ARect.x;
        uint32 y2 = ARect.y;
        uint32 w2 = ARect.w;//x2();
        uint32 h2 = ARect.h;//y2();
        for (int32 y=0; y<MHeight; y++) {
          x2 = ARect.x;
          for(int32 x=0; x<MWidth; x++) {
            int32 pos = (y*MWidth + x) * 4;
            uint8 r = MBuffer[pos+0];
            uint8 g = MBuffer[pos+1];
            uint8 b = MBuffer[pos+2];
            uint8 a = MBuffer[pos+3];
            uint32 pos2 = (y2*h + x2) * 4;
            //trace("pos2 = " << pos2 );
            uint8 r2 = buf2[pos2+0];
            uint8 g2 = buf2[pos2+1];
            uint8 b2 = buf2[pos2+2];
            //uint8 a2 = buf2[pos+3];

            MBuffer[pos+0] = alphaColor(r,a) + alphaColor(r2,(255-a));
            MBuffer[pos+1] = alphaColor(g,a) + alphaColor(g2,(255-a));
            MBuffer[pos+2] = alphaColor(b,a) + alphaColor(b2,(255-a));
            MBuffer[pos+3] = a;

            x2++;
            if (x2>=w2) x2 = ARect.x;
          } //for x
          y2++;
          if (y2>=h2) y2 = ARect.y;
        } //for y

      } //mBuffer
    }

    //----------

    void premultAlpha(void) {
      if (MBuffer) {
        for(int32 y=0; y<MHeight; y++) {
          for(int32 x=0; x<MWidth; x++) {
            int32 pos = (y*MWidth + x) * 4;
            uint8 r = MBuffer[pos+0];
            uint8 g = MBuffer[pos+1];
            uint8 b = MBuffer[pos+2];
            uint8 a = MBuffer[pos+3];
            MBuffer[pos+0] = alphaColor(r,a);
            MBuffer[pos+1] = alphaColor(g,a);
            MBuffer[pos+2] = alphaColor(b,a);
            MBuffer[pos+3] = a;
          } //for x
        } //for y
      } //mBuffer
    }


    //----------

    //TODO: rename -> matrix?

    void colorMatrix( float rr, float rg, float rb, float ra,
                      float gr, float gg, float gb, float ga,
                      float br, float bg, float bb, float ba,
                      float ar, float ag, float ab, float aa) {
      if (MBuffer) {
        //dtrace("swizzle");
        for (int32 y=0; y<MHeight; y++) {
          for (int32 x=0; x<MWidth; x++) {
            int32 pos = (y*MWidth + x) * 4;
            uint8 r = MBuffer[pos+0];
            uint8 g = MBuffer[pos+1];
            uint8 b = MBuffer[pos+2];
            uint8 a = MBuffer[pos+3];
            MBuffer[pos+0] = scaleColor(r,rr) + scaleColor(g,rg) + scaleColor(b,rb) + scaleColor(a,ra);
            MBuffer[pos+1] = scaleColor(r,gr) + scaleColor(g,gg) + scaleColor(b,gb) + scaleColor(a,ga);
            MBuffer[pos+2] = scaleColor(r,br) + scaleColor(g,bg) + scaleColor(b,bb) + scaleColor(a,ba);
            MBuffer[pos+3] = scaleColor(r,ar) + scaleColor(g,ag) + scaleColor(b,ab) + scaleColor(a,aa);
            //mBuffer[pos+3] = a;
          } //for x
        } //for y
      } //mBuffer
    }

    //----------------------------------------
    //
    //----------------------------------------


};

//----------------------------------------------------------------------
#endif

