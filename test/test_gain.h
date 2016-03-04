#ifndef test_gain_included
#define test_gain_included
//----------------------------------------------------------------------

#define SKEI_PLUGIN_PER_SAMPLE

//----------

#include "skei.h"
#include "skei_plugin.h"

//----------

class myPlugin : public SPlugin {

  private:

    float MGain;

  public:

    //virtual
    void on_create(void) {
      MInfo.name        = "test_gain";
      MInfo.author      = "skei";
      MInfo.product     = MInfo.name;
      MInfo.uniqueId    = SKEI_MAGIC + 0x0000;
      MInfo.numInputs   = 2;
      MInfo.numOutputs  = 2;
      appendParameter( new SParameter( "Gain", 0.5f ) );
      MGain             = 0.0f;
    }

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      if (AIndex==0) MGain = AValue;
    }

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {
      *AOutputs[0] = *AInputs[0] * MGain;
      *AOutputs[1] = *AInputs[1] * MGain;
    }

};

//----------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif
