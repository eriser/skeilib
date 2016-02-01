#ifndef skei_linux_included
#define skei_linux_included
//----------------------------------------------------------------------

#include <time.h>

#ifdef SKEI_XLIB
  #include "skei_xlib.h"
#endif

#ifdef SKEI_XRENDER
  #include "skei_xrender.h"
#endif

#ifdef SKEI_XFT
  #include "skei_xft.h"
#endif

#include <pthread.h>
//#include <unistd.h> // sleep


//----------------------------------------------------------------------

// usleep is deprecated

int SSleep(long ms) {
  struct timespec req, rem;
  if (ms > 999) {
    req.tv_sec = (int)(ms / 1000);                            /* Must be Non-Negative */
    req.tv_nsec = (ms - ((long)req.tv_sec * 1000)) * 1000000; /* Must be in range of 0 to 999999999 */
  }
  else {
    req.tv_sec = 0;                         /* Must be Non-Negative */
    req.tv_nsec = ms * 1000000;    /* Must be in range of 0 to 999999999 */
  }
   return nanosleep(&req , &rem);
}

//----------------------------------------------------------------------

#ifdef SKEI_XLIB
#ifdef SKEI_GUI

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

uint32 SRemapKeyboard(uint32 ks) {
  //char c[2];
  //int l;
  //KeySym ks; // XID = unsigned long
  //l = XLookupString(&AEvent->xkey, c, 1, &ks, SKEI_NULL);
  //if (l==0) c[0] = '\0';
  //uint32 ks = SRemapKeyboard(k,s,c[0],keysym);

  uint32 ks_ = ((ks&0xff00) >> 8);

  if (ks_ == 0xfe) {
    switch (ks&0x00ff) {
      case 0x03 : return skc_altgr;
      case 0x57 : return skc_umlaut;    // ûüũ
    }
  }

  if (ks_ == 0xff) {
    switch (ks&0x00ff) {
      case 0x08 : return skc_backspace;
      case 0x09 : return skc_tab;
      case 0x0d : return skc_enter;
      case 0x13 : return skc_pause;
      case 0x14 : return skc_scrlock;
      case 0x1b : return skc_esc;
      case 0x50 : return skc_home;
      case 0x51 : return skc_left;
      case 0x52 : return skc_up;
      case 0x53 : return skc_right;
      case 0x54 : return skc_down;
      case 0x55 : return skc_pgup;
      case 0x56 : return skc_pgdn;
      case 0x57 : return skc_end;
      case 0x67 : return skc_winmenu;   //right
      case 0x63 : return skc_insert;
      case 0x7f : return skc_numlock;
      case 0x8d : return skc_enter;     // skc_numenter;
      case 0x95 : return skc_num7;
      case 0x96 : return skc_num4;
      case 0x97 : return skc_num8;
      case 0x98 : return skc_num6;
      case 0x99 : return skc_num2;
      case 0x9a : return skc_num9;
      case 0x9b : return skc_num3;
      case 0x9c : return skc_num1;
      case 0x9d : return skc_num5;
      case 0x9e : return skc_num0;
      case 0x9f : return skc_numdot;
      case 0xaa : return skc_nummul;
      case 0xab : return skc_numplus;
      case 0xad : return skc_numminus;
      case 0xaf : return skc_numdiv;
      case 0xbe : return skc_f1;
      case 0xbf : return skc_f2;
      case 0xc0 : return skc_f3;
      case 0xc1 : return skc_f4;
      case 0xc2 : return skc_f5;
      case 0xc3 : return skc_f6;
      case 0xc4 : return skc_f7;
      case 0xc5 : return skc_f8;
      case 0xc6 : return skc_f9;
      case 0xc7 : return skc_f10;
      case 0xc8 : return skc_f11;
      case 0xc9 : return skc_f12;
      case 0xe1 : return skc_shift;     // left
      case 0xe2 : return skc_shift;     // right
      case 0xe3 : return skc_ctrl;      // left
      case 0xe4 : return skc_ctrl;      // right
      case 0xe5 : return skc_capslock;
      case 0xe9 : return skc_alt;       // left
      case 0xec : return skc_winright;
      case 0xff : return skc_delete;
    }
  }

  return ks & 0xff;
}

#endif // SKEI_GUI
#endif // SKEI_XLIB

//----------------------------------------------------------------------


//----------------------------------------------------------------------
#endif

