#ifndef skei_flags_included
#define skei_flags_included
//----------------------------------------------------------------------

/*
  enum
    rename to skei_enums.h

    instead of:
      #define V_TRUCK  1
      #define V_CAR    2
      #define V_BIKE   3
    do:
      typedef enum {
        V_TRUCK = 1,
        V_CAR,
        V_BIKE
      } Vehicle;
*/

//----------------------------------------------------------------------

// envelope stages

#define ses_off       0
#define ses_attack    1
#define ses_decay     2
#define ses_sustain   3
#define ses_release   4
#define ses_finished  5

// flex mode (painter)

#define sfm_none    0
#define sfm_stretch 1
#define sfm_tile    2

// font slant (painter)

#define sfs_normal  0
#define sfs_italic  1
#define sfs_oblique 2

// font weight (painter)

#define sfw_normal  0
#define sfw_bold    1

// host cando

#define shc_none                            0
#define shc_sendVstEvents                   1
#define shc_sendVstMidiEvent                2
#define shc_sendVstTimeInfo                 4
#define shc_receiveVstEvents                8
#define shc_receiveVstMidiEvent             16
#define shc_reportConnectionChanges         32
#define shc_acceptIOChanges                 64
#define shc_sizeWindow                      128
#define shc_offline                         256
#define shc_openFileSelector                512
#define shc_closeFileSelector               1024
#define shc_startStopProcess                2048
#define shc_shellCategory                   4096
#define shc_sendVstMidiEventFlagIsRealtime  8192

// host id

#define shi_unknown   0
#define shi_reaper    1
#define shi_energyxt  2
#define shi_live      3
#define shi_fruity    4
#define shi_qtractor  5
#define shi_carla     6
#define shi_bitwig    7
#define shi_jost      8

// key codes (window)

#define skc_up        128
#define skc_down      129
#define skc_left      130
#define skc_right     131
#define skc_insert    132
#define skc_delete    133
#define skc_home      134
#define skc_end       135
#define skc_pgup      136
#define skc_pgdn      137
#define skc_backspace 138
#define skc_tab       139
#define skc_enter     140
#define skc_shift     141
#define skc_ctrl      142
#define skc_alt       142
#define skc_capslock  143
#define skc_esc       144
#define skc_space     145
#define skc_winleft   146
#define skc_winright  147
#define skc_winmenu   148
#define skc_f1        149
#define skc_f2        150
#define skc_f3        151
#define skc_f4        152
#define skc_f5        153
#define skc_f6        154
#define skc_f7        155
#define skc_f8        156
#define skc_f9        157
#define skc_f10       158
#define skc_f11       159
#define skc_f12       160
#define skc_prtscr    161
#define skc_scrlock   162
#define skc_pause     163
#define skc_numlock   164
#define skc_numdiv    165
#define skc_nummul    166
#define skc_numminus  167
#define skc_numplus   168
#define skc_numenter  169
#define skc_numdot    170
#define skc_num0      171
#define skc_num1      172
#define skc_num2      173
#define skc_num3      174
#define skc_num4      175
#define skc_num5      176
#define skc_num6      177
#define skc_num7      178
#define skc_num8      179
#define skc_num9      180
#define skc_altgr     181
#define skc_umlaut    182


// ladspa errors

#define sle_none           0
#define sle_libNotFound   -2
#define sle_mainNotFound  -3
#define sle_descrNotFound -4

// ladspa flags

#define slf_none            0
#define slf_realtime        1
#define slf_inplaceBroken   2
#define slf_hardRTCapable   4

// ladspa port hints

#define slp_none          0
#define slp_audio         1
#define slp_control       2
#define slp_input         4
#define slp_output        8
#define slp_toggled       16
#define slp_samplerate    32
#define slp_logarithmic   64
#define slp_integer       128

// mouse buttons (window)

#define smb_none      0
#define smb_left      1
#define smb_middle    2
#define smb_right     3
#define smb_wheelUp   4
#define smb_wheelDown 5
#define smb_backward  8
#define smb_forward   9

// midi control change (AMsg2)

#define smc_modWheel            1
#define smc_breathControl       2
#define smc_footController      4
#define smc_portamentoTime      5
#define smc_dataEntry           6
#define smc_mainVolume          7
#define smc_msb                 0
#define smc_lsb                 32
#define smc_modWheel_lsb        (smc_modWheel + smc_lsb)
#define smc_breathControl_lsb   (smc_breathControl + smc_lsb)
#define smc_footController_lsb  (smc_footController + smc_lsb)
#define smc_portamentoTime_lsb  (smc_portamentoTime + smc_lsb)
#define smc_dataEntry_lsb       (smc_dataEntry + smc_lsb)
#define smc_mainVolume_lsb      (smc_mainVolume + smc_lsb)
#define smc_damperPedal         64 /* 0=off, 127=on */
#define smc_portamento          65 /* 0=off, 127=on */
#define smc_sustenuto           66 /* 0=off, 127=on */
#define smc_dataEntryPlus       96 /* 127 */
#define smc_dataEntryMinus      97 /* 127 */
#define smc_localControl        122 /* 0=off, 127=on */
#define smc_allNotesOff         123 /* 0 */
#define smc_omniModeOff         124 /* 0 */
#define smc_omniModeOn          125 /* 0 */
#define smc_polyModeOnOff       126 /* !! */
#define smc_polyModeOn          127 /* 0 */

// mouse cursors (window)

#define smc_default         0
#define smc_arrow           1
#define smc_arrowUp         2
#define smc_arrowDown       3
#define smc_arrowLeft       4
#define smc_arrowRight      5
#define smc_arrowUpDown     6
#define smc_arrowLeftRight  7
#define smc_arrowDiagLeft   8
#define smc_arrowDiagRight  9
#define smc_move            10
#define smc_wait            11
#define smc_arrowWait       12
#define smc_hand            13
#define smc_finger          14
#define smc_cross           15
#define smc_pencil          16
#define smc_plus            17
#define smc_question        18
#define smc_ibeam           19

// midi event event

#define sme_noteOff         8   /* note, vel */
#define sme_noteOn          9   /* note, vel */
#define sme_polyAfterTouch  10  /* note, aftertouch */
#define sme_controlChange   11  /* ctrl, value */
#define sme_programChange   12  /* program, - */
#define sme_chanAfterTouch  13  /* aftertouch, - */
#define sme_pitchBend       14  /* lsb, msb */
#define sme_system          15  /* lsb, msb */

// module event

#define sme_none      0
#define sme_init      1
#define sme_exit      2
#define sme_start     3
#define sme_stop      4
#define sme_exec      5
#define sme_compile   6
#define sme_process   7
#define sme_signal    8

// module flags

#define smf_none      0
#define smf_active    1     // driver, library
//#define smf_single    256   // driver, library

// modifier keys (window)

#define smk_none      0
#define smk_shift     1
#define smk_caps      2
#define smk_ctrl      4
#define smk_alt       8
#define smk_altgr     16

// midi system messages (chan, AMsg1 & 15)

#define sms_sysex           0   /* !! */
//#define sms_common        1
#define sms_songPos         2   /* lsb, msb */
#define sms_songSelect      3   /* song, - */
//#define sms_common        4
//#define sms_common        5
#define sms_tuneRequest     6
#define sms_endSysex        7
#define sms_clock           8
//#define sms_undefined     9
#define sms_start           10
#define sms_continue        11
#define sms_stop            12
//#define sms_undefined     13
#define sms_activeSensing   14
#define sms_reset           15

// module states

#define sms_off       0   /* silent */
#define sms_active    1   /* playing */
#define sms_finished  2   /* released */

// pin direction (module)

#define spd_output    0
#define spd_input     1

// parameter flags

#define spf_none        0  // name 'conflict' with plugin flags
#define spf_automate    1
#define spf_interpolate 2

// plugin flags

//#define spf_none        0
//#define spf_perSample   1   /* call on_processSample per sample */
//#define spf_sendMidi    2
//#define spf_receiveMidi 4
//#define spf_hasEditor   8
//#define spf_isSynth     16
//#define spf_autoSync    32  /* automatically handles getTime and on_transportChange */
//#define spf_reaper      64  /* has/uses reaper support */

// pin rate (modul)

#define spr_const     0   // const
#define spr_static    1   // signal [block]
#define spr_dynamic   2   // frame  [sample]

// parameter types

#define spt_none        0
#define spt_parameter   1
#define spt_paramFloat  2
#define spt_paramInt    3
#define spt_paramText   4

// plugin state

#define sps_open        1
#define sps_close       2
#define sps_sampleRate  3
#define sps_blockSize   4
#define sps_suspend     5
#define sps_resume      6
#define sps_start       7
#define sps_stop        8
#define sps_bypass      9
#define sps_bypassOff   10

// pin type (module)

#define spt_none      0
#define spt_audio     1
#define spt_signal    2
#define spt_int       3
#define spt_float     4
#define spt_ptr       5

// plugin transport

#define spt_changed     1     /* play, cycle or record state has changed */
#define spt_play        2     /* Host sequencer is currently playing */
#define spt_cycle       4     /* Host sequencer is in cycle mode */
#define spt_record      8     /* Host sequencer is in record mode */
#define spt_awrite      64    /* automation write mode active (record parameter changes) */
#define spt_aread       128   /* automation read mode active (play parameter changes) */
//todo:
#define spt_pause       256

// sizer modes (widget)

#define ssm_window   -1
#define ssm_none      0
#define ssm_left      1
#define ssm_right     2
#define ssm_top       3
#define ssm_bottom    4

// surface draw modes (widget_surface)

#define ssm_draw      1
#define ssm_blend     2
#define ssm_stretch   3

// signal types (module)

#define sst_none   0
#define sst_signal 1
//#define sst_gate   1

// text align (painter)

#define sta_center        0
#define sta_left          1
#define sta_top           2
#define sta_right         4
#define sta_bottom        8
#define sta_topLeft       16
#define sta_topRight      32
#define sta_bottomLeft    64
#define sta_bottomRight   128
#define sta_rightStretch  256
#define sta_bottomStretch 512
#define sta_horz          1024
#define sta_vert          2048

// tile orientation (painter)

#define sto_horizontal  1
#define sto_vertical    2

// thread signals (window)

#define sts_timer 666
#define sts_kill  667
#define sts_idle  668

// voice states

//#define svs_off       0
//#define svs_playing   1
//#define svs_released  2

// widget alignment

#define swa_none          0
#define swa_client        1
#define swa_fillClient    2
#define swa_fillLeft      3
#define swa_fillRight     4
#define swa_fillTop       5
#define swa_fillBottom    6
#define swa_left          7
#define swa_leftTop       8
#define swa_leftBottom    9
#define swa_right         10
#define swa_rightTop      11
#define swa_rightBottom   12
#define swa_top           13
#define swa_topLeft       14
#define swa_topRight      15
#define swa_bottom        16
#define swa_bottomLeft    17
#define swa_bottomRight   18
#define swa_stackedHoriz  19
#define swa_stackedVert   20

// widget flags

#define swf_none      0
#define swf_active    1
#define swf_visible   2
#define swf_timer     4
#define swf_clip      8
#define swf_autoalign 16
#define swf_capture   32
#define swf_vertical  64
#define swf_opaque    128
//#define swf_alpha   128

//----------------------------------------------------------------------
#endif
