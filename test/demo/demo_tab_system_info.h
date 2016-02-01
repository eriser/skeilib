#ifndef demo_tab_system_info_included
#define demo_tab_system_info_included
//----------------------------------------------------------------------

#include "skei_widget.h"
#include "skei_widget_string.h"
#include "skei_widget_text.h"

#include "skei_basepath.h"

//----------

class demo_tab_system_info
: public SWidget {

  private:

    char    temp[256];

    SString MSystem;
    SString MHomePath;
    SString MBaseFile;
    SString MBasePath;

  public:

    demo_tab_system_info()
    : SWidget( SRect(0), swa_fillClient) {

      MSystem = "#defines: ";

      #ifdef SKEI_LINUX
        MSystem += "linux ";
      #endif

      #ifdef SKEI_WIN32
        MSystem += "win32 ";
      #endif

      #ifdef SKEI_EXE
        MSystem += "exe ";
      #endif

      #ifdef SKEI_VST
        MSystem += "vst ";
      #endif

      #ifdef SKEI_32BIT
        MSystem += "32bit ";
      #endif

      #ifdef SKEI_64BIT
        MSystem += "64bit ";
      #endif

      #ifdef SKEI_BIG_ENDIAN
        MSystem += "big_endian ";
      #endif

      #ifdef SKEI_LITTLE_ENDIAN
        MSystem += "little_endian ";
      #endif

      #ifdef SKEI_XLIB
        MSystem += "xlib ";
      #endif

      #ifdef SKEI_XRENDER
        MSystem += "xrender ";
      #endif

      #ifdef SKEI_XFT
        MSystem += "xft ";
      #endif

      #ifdef SKEI_GDI
        MSystem += "SKEI_GDI ";
      #endif

      #ifdef SKEI_CAIRO
        MSystem += "cairo ";
      #endif

      #ifdef SKEI_OPENGL
        MSystem += "opengl ";
      #endif

      MHomePath = "SHomePath: ";
      MHomePath += SGetHomePath(temp);
      MBaseFile = "MBaseFile: ";
      MBaseFile += SGetBaseFile(temp);
      MBasePath = "MBasePath: ";
      MBasePath += SGetBasePath(temp);

      appendWidget( new SWidget_String( SRect(16), &MSystem, swa_fillTop) );
      appendWidget( new SWidget_String( SRect(16), &MHomePath, swa_fillTop) );
      appendWidget( new SWidget_String( SRect(16), &MBaseFile, swa_fillTop) );
      appendWidget( new SWidget_String( SRect(16), &MBasePath, swa_fillTop) );

      //appendWidget( new SWidget_Text( SRect(16),"---", swa_fillTop) );

//      MGetFileExt[0] = 0;
//      SStrcat(MGetFileExt,"SGetFileExt(MBaseFile) = ");
//      SStrcat(MGetFileExt, SGetFileExt(MBaseFile_temp) );
//      appendWidget( new SWidget_Text( SRect(16),MGetFileExt,    swa_fillTop) );

//      MGetPath[0] = 0;
//      SStrcat(MGetPath,"SGetPath(MBaseFile) = ");
//      SStrcat(MGetPath, SGetPath(MBaseFile_temp) );
//      appendWidget( new SWidget_Text( SRect(16),MGetPath,       swa_fillTop) );

//      MStripFileExt[0] = 0;
//      SStrcat(MStripFileExt,"SStripFileExt(MBaseFile) = ");
//      SStrcat(MStripFileExt, SStripFileExt(MBaseFile_temp) );
//      appendWidget( new SWidget_Text( SRect(16),MStripFileExt,  swa_fillTop) );

//      MStripPath[0] = 0;
//      SStrcat(MStripPath,"SStripPath(MBaseFile) = ");
//      SStrcat(MStripPath, SStripPath(MBaseFile_temp) );
//      appendWidget( new SWidget_Text( SRect(16),MStripPath,     swa_fillTop) );

    }

    //----------

    virtual ~demo_tab_system_info() {
    }

    //----------

};

//----------------------------------------------------------------------
#endif
