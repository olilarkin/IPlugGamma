#ifndef __IPLUGGAMMA__
#define __IPLUGGAMMA__

#include "IPlug_include_in_plug_hdr.h"

#include "Gamma/Gamma.h"		// core functions
//#include "Gamma/Access.h"
//#include "Gamma/Delay.h"
//#include "Gamma/DFT.h"
//#include "Gamma/Effects.h"
//#include "Gamma/Envelope.h"
//#include "Gamma/FormantData.h"
#include "Gamma/Noise.h"
#include "Gamma/Oscillator.h"
//#include "Gamma/SamplePlayer.h"
//#include "Gamma/Print.h"
//#include "Gamma/Recorder.h"
//#include "Gamma/SoundFile.h"
//#include "Gamma/Timer.h"
//#include "Gamma/Types.h"
//#include "Gamma/UnitMaps.h"

using namespace gam;

class IPlugGamma : public IPlug
{
public:
  IPlugGamma(IPlugInstanceInfo instanceInfo);
  ~IPlugGamma();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
  NoiseWhite<> white;		// 1/f^0 noise
  double mGain;
};

#endif
