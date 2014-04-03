#ifndef __IPLUGGAMMA__
#define __IPLUGGAMMA__

#include "IPlug_include_in_plug_hdr.h"

#include "Gamma/Gamma.h"		// core functions
#include "Gamma/DFT.h"

#define MAX_BINS 4097 // (8192/2) + 1
#define DEFAULT_FFTSIZE 2048
#define DEFAULT_WINDOW HANN
#define DEFAULT_OVERLAP 4

enum EParams
{
  kParamSize = 0,
  kParamOverlap,
  kParamWindowType,
  kNumParams
};

using namespace gam;

class IPlugGamma : public IPlug
{
public:
  IPlugGamma(IPlugInstanceInfo instanceInfo);
  ~IPlugGamma();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);
  void initSTFT();

private:
  STFT* mSTFT;
  int mFrameSize;
  int mOverlap;
  bool mSTFTParamChange;
  WindowType mWindowType;
};

#endif
