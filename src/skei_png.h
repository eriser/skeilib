#ifndef skei_png_included
#define skei_png_included
//----------------------------------------------------------------------

#define UPNG_NO_LOAD_FROM_FILE

//#include "extern/upng/upng/upng.h"
#include "extern/upng/upng.c"

class SPng {

  private:
    upng_t* upng;

  public:

    #ifndef UPNG_NO_LOAD_FROM_FILE
    SPng(const char* filename) {
      upng = upng_new_from_file(filename);
      if (upng) upng_header(upng);
    }
    #endif

    //----------

    SPng(const unsigned char* buffer, unsigned long length) {
      upng = upng_new_from_bytes(buffer,length);
      if (upng) upng_header(upng);
    }

    //----------

    virtual ~SPng() {
      if (upng) upng_free(upng);
    }

    //----------

    void header(void) {
      if (upng) upng_header(upng);
    }

    //----------

    void decode(void) {
      if (upng) upng_decode(upng);
    }

  public:

    uint32 error(void)      { if (upng) return upng_get_error(upng);          else return 0; }          // Returns the error state of the upng object (UPNG_EOK means no error)
    uint32 errorline(void)  { if (upng) return upng_get_error_line(upng);     else return 0; }          // Returns the line in the upng.c file where the error state was set
    uint32 width(void)      { if (upng) return upng_get_width(upng);          else return 0; }          // Returns width of image in pixels;
    uint32 height(void)     { if (upng) return upng_get_height(upng);         else return 0; }          // Returns height of image in pixels
    uint32 size(void)       { if (upng) return upng_get_size(upng);           else return 0; }          // Returns the total size of the image buffer in bytes
    uint32 bpp(void)        { if (upng) return upng_get_bpp(upng);            else return 0; }          // Returns the number of bits per pixel (e.g., 32 for 8-bit RGBA)
    uint32 bitdepth(void)   { if (upng) return upng_get_bitdepth(upng);       else return 0; }          // Returns the number of bits per component (e.g., 8 for 8-bit RGBA)
    uint32 pixelsize(void)  { if (upng) return upng_get_pixelsize(upng);      else return 0; }          // Returns the number of bytes per pixel (e.g., 4 for 8-bit RGBA)
    uint32 components(void) { if (upng) return upng_get_components(upng);     else return 0; }          // Returns the number of components per pixel (e.g., 4 for 8-bit RGBA)
    uint32 format(void)     { if (upng) return upng_get_format(upng);         else return 0; }          // Returns the format of the image buffer (see below)
    char*  buffer(void)     { if (upng) return (char*)upng_get_buffer(upng);  else return SKEI_NULL; }  // Returns a pointer to the image buffer

};

//----------------------------------------------------------------------
#endif
