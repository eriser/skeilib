#ifndef syn_1_editor_included
#define syn_1_editor_included
//----------------------------------------------------------------------

#include "skei_editor.h"
//#include "skei_widget_color.h"
//#include "skei_widget_knob.h"
//#include "skei_widget_slider.h"

/*
#include "syn_1_editor_config.h"
#include "syn_1_editor_about.h"
#include "syn_1_editor_osc.h"
#include "syn_1_editor_mix.h"
#include "syn_1_editor_flt.h"
#include "syn_1_editor_lfo.h"
#include "syn_1_editor_env.h"
#include "syn_1_editor_seq.h"
#include "syn_1_editor_mod_matrix.h"
#include "syn_1_editor_master.h"
*/

//----------------------------------------------------------------------

class syn_1_editor : public SEditor {

  //----------------------------------------
  //
  //----------------------------------------

  public:

    syn_1_editor(SEditor_Listener* AListener, int32 AWidth, int32 AHeight, void* AParent=SKEI_NULL)
    : SEditor(AListener,AWidth,AHeight,AParent)
    {
      fillColor( SColor( 0.5f, 0.5f, 0.5f) );
      fillBackground(true);

      /*

      SWidget*          panel;
      SWidget*          widget;
      SWidget_Knob*     knob;
      SWidget_Slider*   slider;
    //SWidget_Selector* selector;

      //--------------------
      // osc 1
      //--------------------

      panel = new SWidget_Color( SRect(10,10,105,44), SGrey );

        // squ

        knob = new SWidget_Knob( SRect(0,0,32,32), 0 );
          knob->backColor( SDarkGrey );
          knob->valueColor( SLightGrey );
          knob->centerColor( SGrey );
        panel->appendWidget(knob);

        // tri

        knob = new SWidget_Knob( SRect(35,0,32,32), 0 );
          knob->backColor( SDarkGrey );
          knob->valueColor( SLightGrey );
          knob->centerColor( SGrey );
        panel->appendWidget(knob);

        // sin

        knob = new SWidget_Knob( SRect(70,0,32,32), 0 );
          knob->backColor( SDarkGrey );
          knob->valueColor( SLightGrey );
          knob->centerColor( SGrey );
        panel->appendWidget(knob);

        // width

        slider = new SWidget_Slider( SRect(2,35,101, 6), 0 );
          slider->drawValueName(false);
          slider->drawValueText(false);
          slider->backColor( SDarkGrey );
          slider->barColor( SLightGrey );
          slider->horizontal(true);
          slider->cursor(smc_arrowLeftRight);
        panel->appendWidget(slider);

      appendWidget(panel);

      //--------------------
      // osc 2
      //--------------------

      panel = new SWidget_Color( SRect(10,60,105,44), SGrey );

        // squ

        knob = new SWidget_Knob( SRect(0,0,32,32), 0 );
          knob->backColor( SDarkGrey );
          knob->valueColor( SLightGrey );
          knob->centerColor( SGrey );
        panel->appendWidget(knob);

        // tri

        knob = new SWidget_Knob( SRect(35,0,32,32), 0 );
          knob->backColor( SDarkGrey );
          knob->valueColor( SLightGrey );
          knob->centerColor( SGrey );
        panel->appendWidget(knob);

        // sin

        knob = new SWidget_Knob( SRect(70,0,32,32), 0 );
          knob->backColor( SDarkGrey );
          knob->valueColor( SLightGrey );
          knob->centerColor( SGrey );
        panel->appendWidget(knob);

        // width

        slider = new SWidget_Slider( SRect(2,35,101, 6), 0 );
          slider->drawValueName(false);
          slider->drawValueText(false);
          slider->backColor( SDarkGrey );
          slider->barColor( SLightGrey );
          slider->horizontal(true);
          slider->cursor(smc_arrowLeftRight);
        panel->appendWidget(slider);

      appendWidget(panel);

      //--------------------
      // mix
      //--------------------

      panel = new SWidget_Color( SRect(120,10,35,93), SGrey );

        slider = new SWidget_Slider( SRect(2,2,31, 14), 0 );
          slider->drawValueName(false);
          slider->drawValueText(false);
          slider->backColor( SDarkGrey );
          slider->barColor( SLightGrey );
          slider->horizontal(true);
          slider->cursor(smc_arrowLeftRight);
        panel->appendWidget(slider);

        // mix

        knob = new SWidget_Knob( SRect(0,16,32,32), 0 );
          knob->backColor( SDarkGrey );
          knob->valueColor( SLightGrey );
          knob->centerColor( SGrey );
        panel->appendWidget(knob);

      appendWidget(panel);

      //--------------------
      // filter
      //--------------------

      panel = new SWidget_Color( SRect(160,10,35,93), SGrey );

        slider = new SWidget_Slider( SRect(2,2,31, 14), 0 );
          slider->drawValueName(false);
          slider->drawValueText(false);
          slider->backColor( SDarkGrey );
          slider->barColor( SLightGrey );
          slider->horizontal(true);
          slider->cursor(smc_arrowLeftRight);
        panel->appendWidget(slider);

        // freq

        knob = new SWidget_Knob( SRect(0,16,32,32), 0 );
          knob->backColor( SDarkGrey );
          knob->valueColor( SLightGrey );
          knob->centerColor( SGrey );
        panel->appendWidget(knob);

        // res

        knob = new SWidget_Knob( SRect(0,48,32,32), 0 );
          knob->backColor( SDarkGrey );
          knob->valueColor( SLightGrey );
          knob->centerColor( SGrey );
        panel->appendWidget(knob);

      appendWidget(panel);

      //--------------------
      // lfo 1
      //--------------------

      panel = new SWidget_Color( SRect(10,110,35,80), SRed );

        slider = new SWidget_Slider( SRect(2,2,31, 14), 0 );
          slider->drawValueName(false);
          slider->drawValueText(false);
          slider->backColor( SDarkRed );
          slider->barColor( SLightRed );
          slider->horizontal(true);
          slider->cursor(smc_arrowLeftRight);
        panel->appendWidget(slider);

        // freq

        knob = new SWidget_Knob( SRect(0,16,32,32), 0 );
          knob->backColor( SDarkRed );
          knob->valueColor( SLightRed );
          knob->centerColor( SRed );
        panel->appendWidget(knob);

        // res

        knob = new SWidget_Knob( SRect(0,48,32,32), 0 );
          knob->backColor( SDarkRed );
          knob->valueColor( SLightRed );
          knob->centerColor( SRed );
        panel->appendWidget(knob);

      appendWidget(panel);

      //--------------------
      // lfo 2
      //--------------------

      panel = new SWidget_Color( SRect(50,110,35,80), SRed );

        slider = new SWidget_Slider( SRect(2,2,31, 14), 0 );
          slider->drawValueName(false);
          slider->drawValueText(false);
          slider->backColor( SDarkRed );
          slider->barColor( SLightRed );
          slider->horizontal(true);
          slider->cursor(smc_arrowLeftRight);
        panel->appendWidget(slider);

        // freq

        knob = new SWidget_Knob( SRect(0,16,32,32), 0 );
          knob->backColor( SDarkRed );
          knob->valueColor( SLightRed );
          knob->centerColor( SRed );
        panel->appendWidget(knob);

        // res

        knob = new SWidget_Knob( SRect(0,48,32,32), 0 );
          knob->backColor( SDarkRed );
          knob->valueColor( SLightRed );
          knob->centerColor( SRed );
        panel->appendWidget(knob);

      appendWidget(panel);

      //--------------------
      // lfo 3
      //--------------------

      panel = new SWidget_Color( SRect(90,110,35,80), SRed );

        slider = new SWidget_Slider( SRect(2,2,31, 14), 0 );
          slider->drawValueName(false);
          slider->drawValueText(false);
          slider->backColor( SDarkRed );
          slider->barColor( SLightRed );
          slider->horizontal(true);
          slider->cursor(smc_arrowLeftRight);
        panel->appendWidget(slider);

        // freq

        knob = new SWidget_Knob( SRect(0,16,32,32), 0 );
          knob->backColor( SDarkRed );
          knob->valueColor( SLightRed );
          knob->centerColor( SRed );
        panel->appendWidget(knob);

        // res

        knob = new SWidget_Knob( SRect(0,48,32,32), 0 );
          knob->backColor( SDarkRed );
          knob->valueColor( SLightRed );
          knob->centerColor( SRed );
        panel->appendWidget(knob);

      appendWidget(panel);

      //--------------------
      // lfo 4
      //--------------------

      panel = new SWidget_Color( SRect(130,110,35,80), SRed );

        slider = new SWidget_Slider( SRect(2,2,31, 14), 0 );
          slider->drawValueName(false);
          slider->drawValueText(false);
          slider->backColor( SDarkRed );
          slider->barColor( SLightRed );
          slider->horizontal(true);
          slider->cursor(smc_arrowLeftRight);
        panel->appendWidget(slider);

        // freq

        knob = new SWidget_Knob( SRect(0,16,32,32), 0 );
          knob->backColor( SDarkRed );
          knob->valueColor( SLightRed );
          knob->centerColor( SRed );
        panel->appendWidget(knob);

        // res

        knob = new SWidget_Knob( SRect(0,48,32,32), 0 );
          knob->backColor( SDarkRed );
          knob->valueColor( SLightRed );
          knob->centerColor( SRed );
        panel->appendWidget(knob);

      appendWidget(panel);

      */


    }

    virtual ~syn_1_editor() {
    }

  //----------------------------------------
  //
  //----------------------------------------

};

//----------------------------------------------------------------------
#endif
