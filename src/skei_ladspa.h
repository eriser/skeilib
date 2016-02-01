#ifndef skei_ladspa_included
#define skei_ladspa_included
//----------------------------------------------------------------------

#define MAX_LADSPA_PORTS    256
#define MAX_LADSPA_INPUTS   16
#define MAX_LADSPA_OUTPUTS  16
#define MAX_LADSPA_PARAMS   256

// ladspa hosting

// -ldl
#include <dlfcn.h>
#include "extern/ladspa.h"
//#include "skei_array.h"

//----------------------------------------------------------------------

struct SLadspa_Port {
  //uint32      portindex;
  const char* name;
  uint32      flags;
  float       val;
  float       minval;
  float       maxval;
};

//----------

class SLadspa {

  private:

    bool                        MLibLoaded;
    void*                       MLibHandle;
    LADSPA_Descriptor_Function  MGetDescriptor;
    const LADSPA_Descriptor*    MDescriptor;
    LADSPA_Handle               MLadspa;
    SLadspa_Port                MPorts[MAX_LADSPA_PORTS];
    int32                       MInputs[MAX_LADSPA_INPUTS];
    int32                       MOutputs[MAX_LADSPA_OUTPUTS];
    int32                       MParams[MAX_LADSPA_PARAMS];
    uint32                      MFlags;
    uint32                      MNumInputs;
    uint32                      MNumOutputs;
    uint32                      MNumParams;

  public:

    const char*       name(void)        { return MDescriptor->Name; }
    const char*       label(void)       { return MDescriptor->Label; }
    const char*       maker(void)       { return MDescriptor->Maker; }
    const char*       copyright(void)   { return MDescriptor->Copyright; }
    uint32            uniqueId(void)    { return MDescriptor->UniqueID; }
    LADSPA_Properties properties(void)  { return MDescriptor->Properties; }
    uint32            flags(void)       { return MFlags; }
    uint32            numPorts(void)    { return MDescriptor->PortCount; }
    uint32            numInputs(void)   { return MNumInputs; }
    uint32            numOutputs(void)  { return MNumOutputs; }
    uint32            numParams(void)   { return MNumParams; }
    SLadspa_Port*     port(int i)       { return &MPorts[i]; }
    SLadspa_Port*     inputPort(int i)  { return &MPorts[MInputs[i]]; }
    SLadspa_Port*     outputPort(int i) { return &MPorts[MOutputs[i]]; }
    SLadspa_Port*     paramPort(int i)  { return &MPorts[MParams[i]]; }
    uint32            input(int i)      { return MInputs[i]; }
    uint32            output(int i)     { return MOutputs[i]; }
    uint32            param(int i)      { return MParams[i]; }

  public:

    SLadspa() {
      MLibLoaded  = false;
      MLibHandle  = SKEI_NULL;
      MDescriptor = SKEI_NULL;
      MLadspa     = SKEI_NULL;
      SMemset(MPorts,  0,sizeof(MPorts));
      SMemset(MInputs, 0,sizeof(MInputs));
      SMemset(MOutputs,0,sizeof(MOutputs));
      SMemset(MParams, 0,sizeof(MParams));
      MFlags      = slf_none;
      MNumInputs  = 0;
      MNumOutputs = 0;
      MNumParams  = 0;
    }

    //----------

    ~SLadspa() {
    }

    //----------

    int32 load(const char* AFilename) {
      DTrace("loading..\n");
      MLibHandle = dlopen(AFilename,RTLD_NOW/*RTLD_LAZY*/);
      if (!MLibHandle) {
        DTrace("ladspa plugin '%s' not found\n",AFilename);
        return sle_libNotFound;
      }
      MLibLoaded = true;
      MGetDescriptor = (LADSPA_Descriptor_Function)dlsym(MLibHandle,(char*)"ladspa_descriptor");
      if (!MGetDescriptor) {
        DTrace("'ladspa_descriptor' function not found\n");
        return sle_mainNotFound;
      }
      int MNumPlugins = 0;
      while ( MGetDescriptor(MNumPlugins) ) MNumPlugins++;
      return sle_none;
    }

    //----------

    void unload(void) {
      if (MLibLoaded && MLibHandle) {
        DTrace("unloading..\n");
        dlclose(MLibHandle);
        MLibLoaded = false;
        MLibHandle = SKEI_NULL;
      }
    }

    //----------

    int32 init(uint32 AIndex=0) {
      MDescriptor = MGetDescriptor(AIndex);
      if (!MDescriptor) {
        DTrace("error creating descriptor (%i)\n",AIndex);
        return sle_descrNotFound;
      }
      MNumInputs  = 0;
      MNumOutputs = 0;
      MNumParams  = 0;
      DTrace("flags: ");
      MFlags = slf_none;
      if (MDescriptor->Properties & LADSPA_PROPERTY_REALTIME)         { MFlags |= slf_realtime;       DTrace("realtime "); }
      if (MDescriptor->Properties & LADSPA_PROPERTY_INPLACE_BROKEN)   { MFlags |= slf_inplaceBroken;  DTrace("inplacebroken "); }
      if (MDescriptor->Properties & LADSPA_PROPERTY_HARD_RT_CAPABLE)  { MFlags |= slf_hardRTCapable;  DTrace("hardrtcapable "); }
      DTrace("\n");
      for (uint32 i=0; i<MDescriptor->PortCount; i++) {
        MPorts[i].name = MDescriptor->PortNames[i];
        DTrace("port(%i) '%s'\n");
        DTrace("  flags: ");
        LADSPA_PortDescriptor pd = MDescriptor->PortDescriptors[i];
        if (pd&LADSPA_PORT_AUDIO)   { MPorts[i].flags |= slp_audio;     DTrace("audio "); }
        if (pd&LADSPA_PORT_CONTROL) { MPorts[i].flags |= slp_control;   DTrace("control "); }
        if (pd&LADSPA_PORT_INPUT)   { MPorts[i].flags |= slp_input;     DTrace("input "); }
        if (pd&LADSPA_PORT_OUTPUT)  { MPorts[i].flags |= slp_output;    DTrace("output "); }
        uint32 hints = MDescriptor->PortRangeHints[i].HintDescriptor;
        if (hints&LADSPA_HINT_BOUNDED_BELOW)  { MPorts[i].minval = MDescriptor->PortRangeHints[i].LowerBound; }
        if (hints&LADSPA_HINT_BOUNDED_ABOVE)  { MPorts[i].maxval = MDescriptor->PortRangeHints[i].UpperBound; }
        if (hints&LADSPA_HINT_TOGGLED)        { MPorts[i].flags |= slp_toggled;     DTrace("toggled "); }
        if (hints&LADSPA_HINT_SAMPLE_RATE)    { MPorts[i].flags |= slp_samplerate;  DTrace("samplerate "); }
        if (hints&LADSPA_HINT_LOGARITHMIC)    { MPorts[i].flags |= slp_logarithmic; DTrace("logarithmic "); }
        if (hints&LADSPA_HINT_INTEGER)        { MPorts[i].flags |= slp_integer;     DTrace("integer "); }
        DTrace("\n");
        uint32 defmask = hints & LADSPA_HINT_DEFAULT_MASK;
        if (defmask != LADSPA_HINT_DEFAULT_NONE) {
          if (defmask == LADSPA_HINT_DEFAULT_MINIMUM) MPorts[i].val = MPorts[i].minval;
          if (defmask == LADSPA_HINT_DEFAULT_LOW)     MPorts[i].val = (MPorts[i].minval * 0.75 + MPorts[i].maxval * 0.25);
          if (defmask == LADSPA_HINT_DEFAULT_MIDDLE)  MPorts[i].val = (MPorts[i].minval * 0.50 + MPorts[i].maxval * 0.50);
          if (defmask == LADSPA_HINT_DEFAULT_HIGH)    MPorts[i].val = (MPorts[i].minval * 0.25 + MPorts[i].maxval * 0.75);
          if (defmask == LADSPA_HINT_DEFAULT_MAXIMUM) MPorts[i].val = MPorts[i].maxval;
          if (defmask == LADSPA_HINT_DEFAULT_0)       MPorts[i].val = 0;
          if (defmask == LADSPA_HINT_DEFAULT_1)       MPorts[i].val = 1;
          if (defmask == LADSPA_HINT_DEFAULT_100)     MPorts[i].val = 100;
          if (defmask == LADSPA_HINT_DEFAULT_440)     MPorts[i].val = 440;
        } // DEFAULT_NONE
        DTrace("  val: %f\n",MPorts[i].val);
        DTrace("  minval: %f\n",MPorts[i].minval);
        DTrace("  maxval: %f\n",MPorts[i].maxval);
        if ( (MPorts[i].flags&slp_audio)   && (MPorts[i].flags&slp_input)  ) MInputs[MNumInputs++] = i;
        if ( (MPorts[i].flags&slp_audio)   && (MPorts[i].flags&slp_output) ) MOutputs[MNumOutputs++] = i;
        if ( (MPorts[i].flags&slp_control) && (MPorts[i].flags&slp_input)  ) MParams[MNumParams++] = i;
      } // for
      DTrace("Inputs (%i): ",MNumInputs);
      for (uint32 i=0; i<MNumInputs; i++) { DTrace( "%i ",MInputs[i] ); }
      DTrace("\nOutputs (%i): ",MNumOutputs);
      for (uint32 i=0; i<MNumOutputs; i++) { DTrace( "%i ",MOutputs[i] ); }
      DTrace("\nParams (%i): ",MNumParams);
      for (uint32 i=0; i<MNumParams; i++) { DTrace( "%i ",MParams[i] ); }
      DTrace("\n");
      DTrace("\n");
      DTrace("instantiate: %i\n",MDescriptor->instantiate);
      DTrace("connect_port: %i\n",MDescriptor->connect_port);
      DTrace("activate: %i\n",MDescriptor->activate);
      DTrace("run: %i\n",MDescriptor->run);
      DTrace("run_adding: %i\n",MDescriptor->run_adding);
      DTrace("set_run_adding_gain: %i\n",MDescriptor->set_run_adding_gain);
      DTrace("deactivate: %i\n",MDescriptor->deactivate);
      DTrace("cleanup: %i\n",MDescriptor->cleanup);
      return sle_none;
    }

    //------------------------------
    //
    //------------------------------

    void instantiate(uint32 ASampleRate) {
      if (MDescriptor) {
        if (MDescriptor->instantiate) {
          MLadspa = MDescriptor->instantiate(MDescriptor,ASampleRate);
        }
      }
    }

    void connect_port(uint32 APort, float* APtr) {
      if (MDescriptor) {
        if (MDescriptor->connect_port) {
          MDescriptor->connect_port(MLadspa,APort,APtr);
        }
      }
    }

    void activate(void) {
      if (MDescriptor) {
        if (MDescriptor->activate) {
          MDescriptor->activate(MLadspa);
        }
      }
    }

    void run(uint32 ANum) {
      if (MDescriptor) {
        if (MDescriptor->run) {
          MDescriptor->run(MLadspa,ANum);
        }
      }
    }

    void run_adding(uint32 ANum) {
      if (MDescriptor) {
        if (MDescriptor->run_adding) {
          MDescriptor->run_adding(MLadspa,ANum);
        }
      }
    }

    void set_run_adding_gain(float AGain) {
      if (MDescriptor) {
        if (MDescriptor->set_run_adding_gain) {
          MDescriptor->set_run_adding_gain(MLadspa,AGain);
        }
      }
    }

    void deactivate(void) {
      if (MDescriptor) {
        if (MDescriptor->deactivate) {
          MDescriptor->deactivate(MLadspa);
        }
      }
    }

    void cleanup(void) {
      if (MDescriptor) {
        if (MDescriptor->cleanup) {
          MDescriptor->cleanup(MLadspa);
        }
      }
    }

    //----------

};

#undef MAX_LADSPA_PORTS
#undef MAX_LADSPA_INPUTS
#undef MAX_LADSPA_OUTPUTS
#undef MAX_LADSPA_PARAMS

//----------------------------------------------------------------------
#endif


/*
  https://ccrma.stanford.edu/mirrors/lalists/lad/2001/Sep/0014.html

  Hi,
  I would like to know if there is any agreed upon logarithmic function to
  use for LADSPA control ports when the LADSPA_HINT_LOGARITHMIC is set. In
  practice, the function should map display coordinates (the slider) to
  control values.
  The best I have come up with myself is this:
    Control port value, v
    Control port low limit, v_low
    Control port high limit, v_high
    Display coordinates, d
    Display coordinates low limit, d_low
    Display coordinates high limit, d_high
    Transposition to positive range, log_offset
    Setup:
      log_offset = -v_low+1;
      d_low = log(v_low + log_offset);
      d_high = log(v_high + log_offset);
      d = log(v + log_offset);
    Slider moved:
      v = exp(d) - log_offset;
  Marcus
*/

/*
  http://nwoeanhinnogaehr.github.io/ladspa.rs/ladspa/enum.DefaultValue.html
  Enum ladspa::DefaultValue
  Minimum
    Equal to the lower_bound of the Port.
  Low
    For ports with LADSPA_HINT_LOGARITHMIC, this should be
      exp(log(lower_bound) * 0.75 + log(upper_bound) * 0.25)
    Otherwise, this should be
      (lower_bound * 0.75 + upper_bound * 0.25)
  Middle
    For ports with CONTROL_HINT_LOGARITHMIC, this should be
      exp(log(lower_bound) * 0.5 + log(upper_bound) * 0.5)
    Otherwise, this should be
      (lower_bound * 0.5 + upper_bound * 0.5)
  High
    For ports with LADSPA_HINT_LOGARITHMIC, this should be
      exp(log(lower_bound) * 0.25 + log(upper_bound) * 0.75)
      Otherwise, this should be
      (lower_bound * 0.25 + upper_bound * 0.75)
*/

/*
  http://lists.linuxaudio.org/pipermail/linux-audio-dev/2006-April/015714.html
  if a is the users parameter UI control value in [0,1], u is the lower
  bound, v is the upper bound, and b is the log base, then:
  port val  = ( (1-a) log(u) + a log(v) )^b
            = ( -a log(u) + log(u) + a log(v) )^b
            = log( ( u+v^a ) / u^a )^b
            = (u + v^a) / u^a
*/
