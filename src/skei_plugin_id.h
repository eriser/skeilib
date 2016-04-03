#ifndef skei_plugin_id_included
#define skei_plugin_id_included
//----------------------------------------------------------------------

/*

  12:34
    1 = flags (4)
      1 gui
      2
      4
      8 gpl
    2 = type (0..7)
      0 none
      1 effect
      2 midi
      3 synth
      4 analysis
      +8 = port
    34 = id (0..255
*/

//----------------------------------------------------------------------

#define SKEI_ID_NONE  0x0000

#define SKEI_ID_FX    0x0100
#define SKEI_ID_MIDI  0x0200
#define SKEI_ID_SYN   0x0300
#define SKEI_ID_ANA   0x0400

#define SKEI_ID_PORT  0x0800

#define SKEI_ID_GUI   0x1000
//#define SKEI_ID_      0x2000
//#define SKEI_ID_      0x4000
#define SKEI_ID_GPL   0x8000

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define FX_CUTTER_ID          ( SKEI_MAGIC  + SKEI_ID_FX    + 0x01 )
#define FX_CUTTER2_ID         ( SKEI_MAGIC  + SKEI_ID_FX    + 0x02 )
#define FX_FRACDELAY_ID       ( SKEI_MAGIC  + SKEI_ID_FX    + 0x03 )
#define FX_FREEZE_ID          ( SKEI_MAGIC  + SKEI_ID_FX    + 0x04 )
#define FX_GRAINS_ID          ( SKEI_MAGIC  + SKEI_ID_FX    + 0x05 )
#define FX_JUNGLE_ID          ( SKEI_MAGIC  + SKEI_ID_FX    + 0x06 )
#define FX_LOOPER_ID          ( SKEI_MAGIC  + SKEI_ID_FX    + 0x07 )
#define FX_LOOPER2_ID         ( SKEI_MAGIC  + SKEI_ID_FX    + 0x08 )
#define FX_MUTE_ID            ( SKEI_MAGIC  + SKEI_ID_FX    + 0x09 )
#define FX_PITCH_ID           ( SKEI_MAGIC  + SKEI_ID_FX    + 0x0a )
#define FX_REPEATER_ID        ( SKEI_MAGIC  + SKEI_ID_FX    + 0x0b )
#define FX_STUTTER_ID         ( SKEI_MAGIC  + SKEI_ID_FX    + 0x0c )
#define FX_STUTTER2_ID        ( SKEI_MAGIC  + SKEI_ID_FX    + 0x0d )

#define MIDI_FIBONACCI_ID     ( SKEI_MAGIC  + SKEI_ID_MIDI  + 0x01 )

#define SYN_TEST1_ID          ( SKEI_MAGIC  + SKEI_ID_SYN   + 0x01 )

//----------------------------------------------------------------------
// ports
//----------------------------------------------------------------------

#define FX_EVENTHORIZON_ID    ( SKEI_MAGIC + SKEI_ID_FX + SKEI_ID_PORT + 0x01 }
#define FX_HALLREVERB_ID      ( SKEI_MAGIC + SKEI_ID_FX + SKEI_ID_PORT + 0x02 }
#define FX_SATURATION_ID      ( SKEI_MAGIC + SKEI_ID_FX + SKEI_ID_PORT + 0x03 }
#define FX_SIMPVERB_ID        ( SKEI_MAGIC + SKEI_ID_FX + SKEI_ID_PORT + 0x04 }
#define FX_SONICMAXIMIZER_ID  ( SKEI_MAGIC + SKEI_ID_FX + SKEI_ID_PORT + 0x05 }

//----------------------------------------------------------------------
// ports gpl
//----------------------------------------------------------------------

#define FX_CABINET_ID         ( SKEI_MAGIC + SKEI_ID_FX + SKEI_ID_PORT + SKEI_ID_GPL + 0x01 }
#define FX_MOOG24DB_ID        ( SKEI_MAGIC + SKEI_ID_FX + SKEI_ID_PORT + SKEI_ID_GPL + 0x02 }
#define FX_SONICENHANCER_ID   ( SKEI_MAGIC + SKEI_ID_FX + SKEI_ID_PORT + SKEI_ID_GPL + 0x03 }
#define FX_TRANSIENT_ID       ( SKEI_MAGIC + SKEI_ID_FX + SKEI_ID_PORT + SKEI_ID_GPL + 0x04 }
#define FX_VOCODER_ID         ( SKEI_MAGIC + SKEI_ID_FX + SKEI_ID_PORT + SKEI_ID_GPL + 0x05 }


//----------------------------------------------------------------------
#endif
