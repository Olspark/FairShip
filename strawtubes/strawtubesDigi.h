#ifndef STRAWTUBESDIGI_H
#define STRAWTUBESDIGI_H

#include "TRandom.h"
#include "TRandom3.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TMath.h"
#include "TVector3.h"
#include <map>
#include "strawtubesPoint.h"
#include "strawtubes.h"

enum RANDTYPE{None, Gaus, Unif};
typedef enum RANDTYPE RandType;

class strawtubesDigi {
public:
   static strawtubesDigi &Instance()
   {
      static strawtubesDigi singleton;
      return singleton;
   }

   /**
    * This function sets user parameters for the time signal parametrization (via Landau)
    * f2 = p1 * exp(-p2 * dist2Wire) + p3 * exp(-p4 * dist2Wire) + p5;
    * LandauSigma = mpvTime * f2 / 100;
    */
   void SetLandauParams(Double_t p1, Double_t p2, Double_t p3, Double_t p4, Double_t p5);

   Double_t DriftTimeFromDist2Wire(Double_t dist2Wire, Double_t wireOffset, bool inSmallerArea);
   Double_t NewDist2WireFromDriftTime(Double_t driftTime);
   Double_t DriftTimeFromTDC(Double_t TDC, Double_t t0, Double_t signalPropagationTime);
   void d2w_dtRelation(const TH1D* TDC, TGraph* graph);
   Double_t getMinimalDriftTime() {return minimalDriftTime;};

    // For turnOn/Off the Drift Time Calculation part
    void UseDefaultDriftTime(bool inDefaultDriftTime) { defaultDriftTime = inDefaultDriftTime;}
    bool IsDefaultDriftTime() { return defaultDriftTime;}

    // to set the parameter of misalignment
    void PassRadius(Double_t r) {tubeRadius = r;};
    void SetDebug(bool debugFlag) {debug = debugFlag;};
    void SetSameSagging(Double_t inMaxTubeSagging, Double_t inMaxWireSagging);
    void SetGausSagging(Double_t inMaxTubeSagging, Double_t inTubeGausSigma, Double_t inMaxWireSagging, Double_t inWireGausSigma);
    void SetUnifSagging(Double_t inMaxTubeSagging, Double_t inTubeUnifDelta, Double_t inMaxWireSagging, Double_t inWireUnifDelta);
    bool CheckInTube(TVector3 pPos, TVector3 start, TVector3 stop, Int_t ID);
    Double_t FindMisalignDist2Wire(TVector3 pPos, TVector3 start, TVector3 stop, Int_t ID);
    Double_t FindMisalignDist2Wire(strawtubesPoint* p);
    bool InSmallerSection(TVector3 pPos, TVector3 start, TVector3 stop, Int_t ID);
    bool IsMisalign() { return misalign;}
    bool IsInitialized() { return beingInit;}

    // old version
    void InitializeMisalign(Double_t tubeSag, Double_t wireSag, Double_t r, bool inDebug);
    void InitializeMisalign(Double_t tubeMean, Double_t tubeSigma, Double_t wireSigma, Double_t wireMean, Double_t r, bool inDebug);

   Double_t GetWireOffset(Int_t ID);
   TH2D *initialVShape = new TH2D("initialVShape", "initialVShape", 1200, 0., 1.2, 1500, 0, 1500);
   TH1D *residualsInStraw = new TH1D("resid", "resid", 10000, -1, 1);
   Int_t counter = 0;

private:
   strawtubesDigi();
   virtual ~strawtubesDigi();
   strawtubesDigi(const strawtubesDigi &);
   strawtubesDigi &operator=(const strawtubesDigi &);

   Double_t mpvTime;     //! MPV for the Landau distribution
   Double_t LandauSigma; //! sigma for the Landau distribution
   TF1 *timeDependence;  //! time-coordinate dependence
   TF1 *leftChain;
   TF1 *rightChain;
   Double_t driftTime;
   Double_t p1 = 8.52; //! Parametrization parameters
   Double_t p2 = 4.66;
   Double_t p3 = 31.81;
   Double_t p4 = 23.92;
   Double_t p5 = 0.419;
   Double_t newDist2Wire; //! Reconstructed distance to the Wire after drift time smearing
   Double_t f2;
   TRandom3 *rand;

   Double_t minimalDriftTime = 0;

   bool defaultDriftTime = false;
   void driftTimeCalculation(Double_t dist2Wire,
                             bool inSmallerArea); //! Calculates the drift time from input distance to the wire

    void NewDist2WireCalculation(Double_t driftTime, Double_t wireOffset);         //! Calculates distance to the wire after drift time smearing for the user time-coordinate dependence function
    void default_NewDist2WireCalculation(Double_t driftTime); //! Calculates distance to the wire after drift time smearing for the default time-coordinate dependence function
    void parabolaChainsEstimation(Double_t wireOffset);

    // Misalignment part
    Double_t tubeRadius = 1.0;
    Double_t maxTubeSagging = 0.0;
    Double_t maxWireSagging = 0.0;
    Double_t tubeGausSigma = 0.0;
    Double_t wireGausSigma = 0.0;
    Double_t tubeUnifDelta = 0.0;
    Double_t wireUnifDelta = 0.0;
    std::map<Int_t, Double_t> tubeSaggingMap;
    std::map<Int_t, Double_t> wireSaggingMap;
    bool misalign = false;
    RandType randType = None;
    bool debug = false;
    bool beingInit = false;

    Double_t FindTubeShift(Double_t x, Double_t startx, Double_t stopx, Int_t ID);
    Double_t FindWireShift(Double_t x, Double_t startx, Double_t stopx, Int_t ID);
    Double_t GetMaxTubeSagging(Int_t ID);
    Double_t GetMaxWireSagging(Int_t ID);
    Double_t FindWireSlope(Double_t x, TVector3 start, TVector3 stop, Int_t ID);

};



#endif //STRAWTUBESDIGI_H
