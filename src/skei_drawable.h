#ifndef skei_drawable_included
#define skei_drawable_included
//----------------------------------------------------------------------

//#ifdef SKEI_LINUX
//#include <X11/Xlib.h>
//#ifdef SKEI_XRENDER
//#include <X11/extensions/Xrender.h>
//#endif
//#endif

//----------

class SDrawable {

  public:

    #ifdef SKEI_LINUX
    virtual Display*  display(void)   { return SKEI_NULL; }
    virtual Drawable  drawable(void)  { return 0; }
    virtual int32     width(void)     { return 0; }
    virtual int32     height(void)    { return 0; }
    virtual int32     depth(void)     { return 0; }
    #ifdef SKEI_XRENDER
    virtual Picture   picture(void)   { return 0; }
    #endif
    #endif

    //virtual Visual    visual(void)    { return 0; }
    //virtual Colormap  colormap(void)  { return 0; }
    //virtual Screen    screen(void)    { return 0; }

};

//----------------------------------------------------------------------
#endif


