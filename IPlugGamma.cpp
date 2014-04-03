#include "IPlugGamma.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int kNumPrograms = 1;

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,

  kGainX = 100,
  kGainY = 100,
  kKnobFrames = 60
};

IPlugGamma::IPlugGamma(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo)
  , mFrameSize(DEFAULT_FFTSIZE)
  , mWindowType(DEFAULT_WINDOW)
  , mOverlap(DEFAULT_OVERLAP)
  , mSTFTParamChange(true)
  , mSTFT(0)
{
  TRACE;

  GetParam(kParamSize)->InitEnum("Win Size", 6, 7);
  GetParam(kParamSize)->SetDisplayText(0, "128");
  GetParam(kParamSize)->SetDisplayText(1, "256");
  GetParam(kParamSize)->SetDisplayText(2, "512");
  GetParam(kParamSize)->SetDisplayText(3, "1024");
  GetParam(kParamSize)->SetDisplayText(4, "2048");
  GetParam(kParamSize)->SetDisplayText(5, "4096");
  GetParam(kParamSize)->SetDisplayText(6, "8192");
  
  GetParam(kParamWindowType)->InitEnum("Win Type", 3, 7);
  GetParam(kParamWindowType)->SetDisplayText(0, "Bartlett (Triangle)");
  GetParam(kParamWindowType)->SetDisplayText(1, "Blackman");
  GetParam(kParamWindowType)->SetDisplayText(2, "Blackman-Harris");
  GetParam(kParamWindowType)->SetDisplayText(3, "Hann");
  GetParam(kParamWindowType)->SetDisplayText(4, "Welch");
  GetParam(kParamWindowType)->SetDisplayText(5, "Nyquist");
  GetParam(kParamWindowType)->SetDisplayText(6, "Rectangle (no window)");
  
  GetParam(kParamOverlap)->InitEnum("Overlap", 1, 2);
  GetParam(kParamOverlap)->SetDisplayText(0, "2");
  GetParam(kParamOverlap)->SetDisplayText(1, "4");
  
  //IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  //pGraphics->AttachPanelBackground(&COLOR_RED);

  //IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);

  //pGraphics->AttachControl(new IKnobMultiControl(this, kGainX, kGainY, kGain, &knob));

  //AttachGraphics(pGraphics);

  //MakePreset("preset 1", ... );
  MakeDefaultPreset((char *) "-", kNumPrograms);
}

IPlugGamma::~IPlugGamma() 
{
  if (mSTFT)
    DELETE_NULL(mSTFT);
}

void IPlugGamma::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  // Mutex is already locked for us.
  
  if (mSTFTParamChange) 
  {
    if (mSTFT)
      DELETE_NULL(mSTFT);
    
    mSTFT = new STFT(mFrameSize,          // Window size
                            mFrameSize/mOverlap, // Hop size; interval between transforms
                            0,                   // Pad size; zero padding amount on window
                            mWindowType,         // Window type: BARTLETT, BLACKMAN, BLACKMAN_HARRIS, HAMMING, HANN, WELCH, NYQUIST, or RECTANGLE
                            MAG_PHASE);          // Format of frequency samples
    
    mSTFT->inverseWindowing(true);
    
    mSTFTParamChange = false;
  }
  
  double* in1 = inputs[0];
  double* in2 = inputs[1];
  double* out1 = outputs[0];
  double* out2 = outputs[1];

  for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
  {
    if (mSTFT->operator()(*in1)) // returns true when ready to process a frame
    {      
      for(int k=0; k<mSTFT->numBins(); ++k)
      {
        mSTFT->bin(k)[0] *= 1.;
        mSTFT->bin(k)[1] *= 1.;
      }
    }
    
    *out1 = mSTFT->operator()() * 0.2;
    *out2 = *out1;
  }
}

void IPlugGamma::Reset()
{
  TRACE;
  IMutexLock lock(this);
  
  Sync::master().spu(GetSampleRate());
}

void IPlugGamma::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);
  
  switch (paramIdx)
  {
    case kParamSize:
    {
      int newSize = 2 << (GetParam(kParamSize)->Int() + 6);
      
      if (newSize != mFrameSize) 
      {
        mFrameSize = newSize;
        mSTFTParamChange = true;
      }
      
      break;
    }
    case kParamOverlap:
    {
      int overlap = 2;
      if (GetParam(kParamOverlap)->Int() == 0) overlap = 2;
      else if (GetParam(kParamOverlap)->Int() == 1) overlap = 4;
      
      if (overlap != mOverlap)
      {
        mOverlap = overlap;
        mSTFTParamChange = true;
      }
      
      break;
    }
    case kParamWindowType:
    {
      if (GetParam(kParamWindowType)->Int() != mWindowType)
      {
        mWindowType = (WindowType) GetParam(kParamWindowType)->Int();
        mSTFTParamChange = true;
      }
      break;
    }
    default:
      break;
  }
}

