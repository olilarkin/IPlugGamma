#ifndef __IPLUGGAMMA__
#define __IPLUGGAMMA__

#include "IPlug_include_in_plug_hdr.h"

class IPlugGamma : public IPlug
{
public:
  IPlugGamma(IPlugInstanceInfo instanceInfo);
  ~IPlugGamma();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
  double mGain;
};

#endif
