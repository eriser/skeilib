#ifndef skei_config_included
#define skei_config_included
//----------------------------------------------------------------------

#define SKEI_ARRAY_SIZE_INIT              16
#define SKEI_ARRAY_SIZE_MULT              2
#define SKEI_CANVAS_DEFAULT_PEN_WIDTH     0
#define SKEI_CANVAS_CLIPSTACK_SIZE        256
#define SKEI_GLOBAL_MAX_OBJECTS           256
#define SKEI_LADSPA_MAX_PORTS             256
#define SKEI_LADSPA_MAX_INPUTS            16
#define SKEI_LADSPA_MAX_OUTPUTS           16
#define SKEI_LADSPA_MAX_PARAMS            256
#define SKEI_LINUX_SLEEP_MS               30
#define SKEI_LINUX_IDLE_MS                40
//#define SKEI_LINUX_THREADPROC_TICKLENGTH  1
//#define SKEI_LINUX_THREADPROC_TICKIDLE    30
//#define SKEI_MODULE_MAX_INPUTS            16
//#define SKEI_MODULE_MAX_OUTPUTS           16
//#define SKEI_MODULE_MAX_PARAMS            16
//#define SKEI_MODULE_MAX_OVERSAMPLE        32
#define SKEI_MODULE_TICKSIZE              16  /* 32 */
#define SKEI_MARKOV_THRESHOLD             2   /* 8 */
#define SKEI_MARKOV_BIGTHRESH             2   /* 8 */
#define SKEI_PLUGIN_MAX_QUEUE_SIZE        512
#define SKEI_PLUGIN_MAX_PARAMETERS        256
#define SKEI_PLUGIN_MAX_MIDISEND          256
#define SKEI_PLUGIN_PARAMETER_ARRAY_SIZE  ( SKEI_PLUGIN_MAX_PARAMETERS * sizeof(float) )
//#define SKEI_VOICE_MANAGER_MAX_EVENTS     1024
//#define SKEI_VOICE_MANAGER_MAX_VOICES     256

//----------------------------------------------------------------------

//#define SKEI_DEBUG
//#define SKEI_DEBUG_CALLSTACK
//#define SKEI_DEBUG_CONSOLE
//#define SKEI_DEBUG_LOG
//#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_MEM_PRINT
//#define SKEI_DEBUG_SOCKET
//#define SKEI_DEBUG_VST

//----------

//#define SKEI_DSSI
//#define SKEI_EXE
//#define SKEI_LADSPA
//#define SKEI_LV2
//#define SKEI_VST

//----------

//#define SKEI_XLIB
//#define SKEI_XRENDER
//#define SKEI_XFT
//#define SKEI_GDI
//#define SKEI_CAIRO
//#define SKEI_OPENGL

//#define SKEI_TABLET

//----------

//#define SKEI_MAIN
//#define SKEI_REPLACE_STDLIB
//#define SKEI_MODULES_IN_VOICE_MANAGER

//#define SKEI_NO_AUTODELETE
//#define SKEI_NO_SHRINK_BACKBUFFER
//#define SKEI_NO_DMC_COMPRESS
//#define SKEI_NO_BITMAP_PNG
//#define SKEI_NO_MODULE_TICKS

//----------

//#define SKEI_LINUX_WAIT_FOR_MAPNOTIFY
//#define SKEI_LINUX_XINITTHREADS
//#define SKEI_LINUX_SLEEP_THREAD
//#define SKEI_LINUX_IDLE_THREAD

//----------

//#define SKEI_PLUGIN_PER_SAMPLE
//#define SKEI_PLUGIN_HAS_EDITOR
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_SEND_MIDI
//#define SKEI_PLUGIN_RECEIVE_MIDI
//#define SKEI_PLUGIN_AUTOSYNC

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*

#define SKEI_LINUX
#define SKEI_WIN32

#define SKEI_32BIT
#define SKEI_64BIT

#define SKEI_LITTLE_ENDIAN
#define SKEI_BIG_ENDIAN

#define SKEI_GUI
#define SKEI_LIB

*/

//----------------------------------------------------------------------
#endif
