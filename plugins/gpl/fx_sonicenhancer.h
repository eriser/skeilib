#ifndef fx_sonicenhancer_included
#define fx_sonicenhancer_included
//----------------------------------------------------------------------

/*
  based on:
    sonic enhancer thing
    Copyright (C) 2011 lubomir i. ivanov
    http://forum.cockos.com/showthread.php?t=91439
    (gpl3)
    see bottom of this file for original and license info
*/

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//#define SKEI_DEBUG_MEM

#ifdef SKEI_VST
  //#define SKEI_DEBUG_VST
  #define SKEI_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include <math.h>
#include "skei.h"
#include "skei_plugin.h"
#include "skei_random.h"

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

const char* txt_noise[2] = { "off", "on" };

//----------

class myPlugin
: public SPlugin
{

  private:

    // init
    float r_6, nf_k, rc0_k0, rc0_k1, fc;
    float alpha, omega;
    float q, sn, cs;
    float b0_ap, b1_ap, b2_ap;
    float a0_ap, a1_ap, a2_ap;
    float b0_lp, b1_lp, b2_lp;
    float a1_lp, a0_lp, a2_lp;
    float b0_hp, b1_hp, b2_hp;
    float a1_hp, a0_hp, a2_hp;
    // slider
    float g_in, g_lp, g_hp, cv_k, g_out, noise_floor;
    // sample
    float x1_ap0, x2_ap0, y1_ap0, y2_ap0;
    float x1_ap1, x2_ap1, y1_ap1, y2_ap1;
    float x1_lp0, x2_lp0, y1_lp0, y2_lp0;
    float x1_lp1, x2_lp1, y1_lp1, y2_lp1;
    float x1_hp0, x2_hp0, y1_hp0, y2_hp0;
    float x1_hp1, x2_hp1, y1_hp1, y2_hp1;
    float cv_rc0, cv_rc1;
    float out_rc0, out_rc1;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    void on_create(void) {
      MInfo.name       = "fx_sonicenhancer";
      MInfo.author     = "skeilib";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;

      appendParameter( new SParamFloat("Input",       0, -12, 12 ) );
      appendParameter( new SParamFloat("Low Contour", 0,  0,  10 ) );
      appendParameter( new SParamFloat("Process",     0,  0,  10 ) );
      appendParameter( new SParamFloat("CV",          2,  0,  10 ) );
      appendParameter( new SParamFloat("Output",      0, -12, 12 ) );
      appendParameter( new SParamText( "Noise Floor", 0, 2, txt_noise ) );
      //
    }

    //----------

    //virtual
    void on_destroy(void) {
    }

  //----------------------------------------
  // private
  //----------------------------------------

  private:

    void init(float srate) {
      r_6     = 1.0f / 6.0f;
      nf_k    = pow(2.0f, -96.0f * r_6);
      rc0_k0  = 0.0003f;
      rc0_k1  = 1.0f - rc0_k0;
      fc      = 700.0f;
      q       = 0.23f;
      omega   = SKEI_PI2 * fc / srate;
      sn      = sin(omega);
      cs      = cos(omega);
      alpha   = sn / (2.0f * q);
      b0_ap   = 1.0f / (1.0f + alpha);
      b1_ap   = (-2.0f * cs) * b0_ap;
      b2_ap   = (1.0f - alpha) * b0_ap;
      a0_ap   = b2_ap;
      a1_ap   = b1_ap;
      a2_ap   = 1.0f;
      b0_lp   = 1.0f / (1.0f  + alpha);
      b1_lp   = (-2.0f * cs) * b0_lp;
      b2_lp   = (1.0f - alpha) * b0_lp;
      a1_lp   = (1.0f - cs) * b0_lp;
      a0_lp   = a1_lp * 0.5f;
      a2_lp   = a0_lp;
      b0_hp   = 1.0f / (1.0f + alpha);
      b1_hp   = (-2.0f * cs) * b0_hp;
      b2_hp   = (1.0f - alpha) * b0_hp;
      a1_hp   = -(1.0f + cs) * b0_hp;
      a0_hp   = -a1_hp * 0.5f;
      a2_hp   = a0_hp;
    }

  //----------------------------------------
  // events
  //----------------------------------------

  public:

    //virtual
    void on_stateChange(uint32 AState) {
      switch(AState) {
        //case sps_sampleRate:
        //  init(MState.sampleRate);
        //  break;
        case sps_resume:
          init(MState.sampleRate);
          break;
      }
    }

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      switch(AIndex) {
        case 0: g_in = pow(2.0f,AValue*r_6);  break;  // input
        case 1: g_lp = AValue * 0.5;          break;  // low contour
        case 2: g_hp = AValue * 0.5;          break;  // process
        case 3: cv_k = AValue;                break;  // cv
        case 4: g_out = pow(2,AValue*r_6);    break;  // output
        case 5: noise_floor = AValue;         break;  // noise floor
      }
    }

  //----------------------------------------
  // audio
  //----------------------------------------

  public:

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, int32 ANumSamples) {
      float* input0 = AInputs[0];
      float* input1 = AInputs[1];
      float* output0 = AOutputs[0];
      float* output1 = AOutputs[1];
      for (int i=0; i<ANumSamples; i++) {
        float spl0  = *input0++;
        float spl1  = *input1++;
        float in0   = spl0*g_in;
        float in1   = spl1*g_in;
        float nf0   = 0.0;
        float nf1   = 0.0;
        if (noise_floor >= 0.9f) {
          nf0 = SRandomSigned() * nf_k;
          nf1 = SRandomSigned() * nf_k;
        }
        float out_ap0 = a0_ap*in0 + a1_ap*x1_ap0 + a2_ap*x2_ap0 - b1_ap*y1_ap0 - b2_ap*y2_ap0;
        x2_ap0  = x1_ap0;
        x1_ap0  = in0;
        y2_ap0  = y1_ap0;
        y1_ap0  = out_ap0;
        float out_ap1 = a0_ap*in1 + a1_ap*x1_ap1 + a2_ap*x2_ap1 - b1_ap*y1_ap1 - b2_ap*y2_ap1;
        x2_ap1  = x1_ap1;
        x1_ap1  = in1;
        y2_ap1  = y1_ap1;
        y1_ap1  = out_ap1;
        float out_lp0 = a0_lp*in0 + a1_lp*x1_lp0 + a2_lp*x2_lp0 - b1_lp*y1_lp0 - b2_lp*y2_lp0;
        x2_lp0  = x1_lp0;
        x1_lp0  = in0;
        y2_lp0  = y1_lp0;
        y1_lp0  = out_lp0;
        float out_lp1 = a0_lp*in1 + a1_lp*x1_lp1 + a2_lp*x2_lp1 - b1_lp*y1_lp1 - b2_lp*y2_lp1;
        x2_lp1  = x1_lp1;
        x1_lp1  = in1;
        y2_lp1  = y1_lp1;
        y1_lp1  = out_lp1;
        float out_hp0 = a0_hp*in0 + a1_hp*x1_hp0 + a2_hp*x2_hp0 - b1_hp*y1_hp0 - b2_hp*y2_hp0;
        x2_hp0  = x1_hp0;
        x1_hp0  = in0;
        y2_hp0  = y1_hp0;
        y1_hp0  = out_hp0;
        float out_hp1 = a0_hp*in1 + a1_hp*x1_hp1 + a2_hp*x2_hp1 - b1_hp*y1_hp1 - b2_hp*y2_hp1;
        x2_hp1  = x1_hp1;
        x1_hp1  = in1;
        y2_hp1  = y1_hp1;
        y1_hp1  = out_hp1;
        float cv0     = 1 - abs(in0)*cv_k;
        float cv1     = 1 - abs(in1)*cv_k;
        cv_rc0  = rc0_k0*cv0 + rc0_k1*cv_rc0;
        cv_rc1  = rc0_k0*cv1 + rc0_k1*cv_rc1;
        float out_sv0 = out_ap0 + out_hp0*cv_rc0*g_hp + out_lp0*g_lp;
        float out_sv1 = out_ap1 + out_hp1*cv_rc1*g_hp + out_lp1*g_lp;
        out_rc0 = rc0_k0*out_sv0 + rc0_k1*out_rc0;
        out_rc1 = rc0_k0*out_sv1 + rc0_k1*out_rc1;
        float out_dc0 = out_sv0 - out_rc0;
        float out_dc1 = out_sv1 - out_rc1;
        spl0    = out_dc0*g_out + nf0;
        spl1    = out_dc1*g_out + nf1;
        *output0++ = spl0;
        *output1++ = spl1;
      }
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif





// original:





#if 0

// http://forum.cockos.com/showthread.php?t=91439

/*

  sonic enhancer thing

  Copyright (C) 2011 lubomir i. ivanov

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

desc: Sonic Enhancer

slider1:0<-12, 12, 0.1>Input
slider2:0<0, 10, 0.1>Low Contour
slider3:0<0, 10, 0.1>Process
slider4:2<0, 10, 0.1>CV
slider5:0<-12, 12, 0.1>Output
slider6:1<0, 1, {Off,On})>Noise Floor

@init
r_6 = 1/6;

nf_k = pow(2, -96 * r_6);

rc0_k0 = 0.0003;
rc0_k1 = 1 - rc0_k0;

fc = 700;
fs = srate;
pi2 = 2*$pi;
q = 0.23;

omega = pi2 * fc / fs;
sn = sin(omega);
cs = cos(omega);
alpha = sn / (2.0 * Q);

b0_ap = 1.0 / (1.0 + alpha);
b1_ap = (-2.0 * cs) * b0_ap;
b2_ap = (1.0 - alpha) * b0_ap;
a0_ap = b2_ap;
a1_ap = b1_ap;
a2_ap = 1;

b0_lp = 1.0 / (1.0  + alpha);
b1_lp = (-2.0 * cs) * b0_lp;
b2_lp = (1.0 - alpha) * b0_lp;
a1_lp = (1.0 - cs) * b0_lp;
a0_lp = a1_lp * 0.5;
a2_lp = a0_lp;

b0_hp = 1.0 / (1.0 + alpha);
b1_hp = (-2.0 * cs) * b0_hp;
b2_hp = (1.0 - alpha) * b0_hp;
a1_hp = -(1.0 + cs) * b0_hp;
a0_hp = -a1_hp * 0.5;
a2_hp = a0_hp;

@slider
g_in = pow(2, slider1 * r_6);
g_lp = slider2*0.5;
g_hp = slider3*0.5;
cv_k = slider4;
g_out = pow(2, slider5 * r_6);
noise_floor = slider6;

@sample
in0 = spl0*g_in;
in1 = spl1*g_in;

nf0 = 0.0;
nf1 = 0.0;
noise_floor ? (
  nf0 = (rand(2.0) - 1.0)*nf_k;
  nf1 = (rand(2.0) - 1.0)*nf_k;
);

out_ap0 = a0_ap*in0 + a1_ap*x1_ap0 + a2_ap*x2_ap0 - b1_ap*y1_ap0 - b2_ap*y2_ap0;
x2_ap0 = x1_ap0;
x1_ap0 = in0;
y2_ap0 = y1_ap0;
y1_ap0 = out_ap0;

out_ap1 = a0_ap*in1 + a1_ap*x1_ap1 + a2_ap*x2_ap1 - b1_ap*y1_ap1 - b2_ap*y2_ap1;
x2_ap1 = x1_ap1;
x1_ap1 = in1;
y2_ap1 = y1_ap1;
y1_ap1 = out_ap1;

out_lp0 = a0_lp*in0 + a1_lp*x1_lp0 + a2_lp*x2_lp0 - b1_lp*y1_lp0 - b2_lp*y2_lp0;
x2_lp0 = x1_lp0;
x1_lp0 = in0;
y2_lp0 = y1_lp0;
y1_lp0 = out_lp0;

out_lp1 = a0_lp*in1 + a1_lp*x1_lp1 + a2_lp*x2_lp1 - b1_lp*y1_lp1 - b2_lp*y2_lp1;
x2_lp1 = x1_lp1;
x1_lp1 = in1;
y2_lp1 = y1_lp1;
y1_lp1 = out_lp1;

out_hp0 = a0_hp*in0 + a1_hp*x1_hp0 + a2_hp*x2_hp0 - b1_hp*y1_hp0 - b2_hp*y2_hp0;
x2_hp0 = x1_hp0;
x1_hp0 = in0;
y2_hp0 = y1_hp0;
y1_hp0 = out_hp0;

out_hp1 = a0_hp*in1 + a1_hp*x1_hp1 + a2_hp*x2_hp1 - b1_hp*y1_hp1 - b2_hp*y2_hp1;
x2_hp1 = x1_hp1;
x1_hp1 = in1;
y2_hp1 = y1_hp1;
y1_hp1 = out_hp1;

cv0 = 1 - abs(in0)*cv_k;
cv1 = 1 - abs(in1)*cv_k;

cv_rc0 = rc0_k0*cv0 + rc0_k1*cv_rc0;
cv_rc1 = rc0_k0*cv1 + rc0_k1*cv_rc1;

out_sv0 = out_ap0 + out_hp0*cv_rc0*g_hp + out_lp0*g_lp;
out_sv1 = out_ap1 + out_hp1*cv_rc1*g_hp + out_lp1*g_lp;

out_rc0 = rc0_k0*out_sv0 + rc0_k1*out_rc0;
out_rc1 = rc0_k0*out_sv1 + rc0_k1*out_rc1;
out_dc0 = out_sv0 - out_rc0;
out_dc1 = out_sv1 - out_rc1;

spl0 = out_dc0*g_out + nf0;
spl1 = out_dc1*g_out + nf1;

@gfx 0 10
gfx_r = 0.8;
gfx_g = 0.5;
gfx_b = 0;

gfx_a = min(1, 0.55 + cv_rc0*0.25 + g_hp*0.1);
gfx_x = 5;
gfx_y = 5;
gfx_rectto(gfx_w*0.5 - 3, gfx_h - 5);

gfx_a = min(1, 0.55 + cv_rc1*0.25 + g_hp*0.1);
gfx_x = gfx_w*0.5 + 3;
gfx_y = 5;
gfx_rectto(gfx_w - 5, gfx_h - 5);

*)


#endif // 0
