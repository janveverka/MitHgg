#include <complex>      // std::abs
#include <iostream>     // std::cout
#include "TMath.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "MitHgg/PhotonTree/interface/CiCCategoryReader.h"
#include "MitCommon/MathTools/interface/MathUtils.h"

using ::mithep::MathUtils;
using ::mithep::hgg::CiCCategoryReader;

/// Make this a ROOT class
ClassImp(CiCCategoryReader)

const unsigned int CiCCategoryReader::kNumInclCats  = 8;
const unsigned int CiCCategoryReader::kNumDijetCats = 2;

//------------------------------------------------------------------------------
CiCCategoryReader::CiCCategoryReader(TTree       *iTree       ,
                                     EBeamEnergy  iBeamEnergy ,
                                     const char  *eventsToSkip) :
  TreeReader (iTree                      ),
  massOver120(-99                        ),
  VHMetTag   (-1                         ),
  dijetCat   (-1                         ),
  inclCat    (-1                         ),
  cicCat     (-1                         ),
  fBeamEnergy(iBeamEnergy                ),
  kIncl0     (0                          ),
  kDijet0    (kIncl0      + kNumInclCats ),
  kVHLepTight(kDijet0     + kNumDijetCats),
  kVHLepLoose(kVHLepTight + 1            ),
  kVHMet     (kVHLepLoose + 1            ),
  kTTHLep    (kVHMet      + 1            ),
  kTTHHad    (kTTHLep     + 1            ),
  kVHHad     (kTTHHad     + 1            ),
  fEventsToSkip(eventsToSkip),
  fEventFilter(0)
{
  Init();
} /// Ctor


//------------------------------------------------------------------------------
CiCCategoryReader::~CiCCategoryReader()
{
  delete fEventFilter;
} /// Dtor


//------------------------------------------------------------------------------
void
CiCCategoryReader::Init()
{
  Update();
  if (fEventsToSkip != "") {
    fEventFilter = new EventFilterFromListStandAlone(fEventsToSkip);
  }
} /// Init


//------------------------------------------------------------------------------
void
CiCCategoryReader::Update(void)
{
  massOver120 = mass / 120.;
  ph1_ecalRegion = GetEcalFiducialRegion(ph1.sceta);
  ph2_ecalRegion = GetEcalFiducialRegion(ph2.sceta);

  UpdateCategoryDefinitions();
  
  UpdateInclusiveCat       ();
  UpdateDijetCat           ();
  UpdateVHLepTag           ();
  UpdateVHMetTag           ();
  UpdateTTHTag             ();
  UpdateVHHadTag           ();
  
  UpdateCiCCat             ();
} /// Update


//------------------------------------------------------------------------------
CiCCategoryReader::EEcalFiducialRegion
CiCCategoryReader::GetEcalFiducialRegion(float scEta)
{
  float absEta = TMath::Abs(scEta);
  if      (absEta < 1.4442) return EEcalFiducialRegion::eBarrel;
  else if (absEta > 1.566 ) return EEcalFiducialRegion::eEndcaps;
  else                      return EEcalFiducialRegion::eGap;
} /// GetEcalFiducialRegion


//------------------------------------------------------------------------------
void
CiCCategoryReader::UpdateCategoryDefinitions(void)
{
  switch (fBeamEnergy) {
    case EBeamEnergy::k7TeV:
      UpdateCategoryDefinitions7TeV();
      break;
    case EBeamEnergy::k8TeV:
      UpdateCategoryDefinitions8TeV();
      break;
    default:
      /// This should never happen!
      cms::Exception exception("BadEnum");
      exception << __FILE__ << ":" << __LINE__ << " in " << __FUNCTION__
                << ": Illegal EBeamEnergy enum value: " << fBeamEnergy;
      throw exception;
  } /// fBeamEnergy  
}


//------------------------------------------------------------------------------
void
CiCCategoryReader::UpdateCategoryDefinitions7TeV(void)
{
  UpdateCategoryDefinitions8TeV();
  kTTHHad      = kTTHLep    ; /// 13 (same as ttH lep)
  kVHHad       = kTTHHad + 1; /// 14
} /// UpdateCategoryDefinitions7TeV


//------------------------------------------------------------------------------
void
CiCCategoryReader::UpdateCategoryDefinitions8TeV(void)
{
  kIncl0       = 0                          ; /// 0-7
  kDijet0      = kIncl0      + kNumInclCats ; /// 8-9
  kVHLepTight  = kDijet0     + kNumDijetCats; /// 10
  kVHLepLoose  = kVHLepTight + 1            ; /// 11
  kVHMet       = kVHLepLoose + 1            ; /// 12
  kTTHLep      = kVHMet      + 1            ; /// 13
  kTTHHad      = kTTHLep     + 1            ; /// 14
  kVHHad       = kTTHHad     + 1            ; /// 15
} /// UpdateCategoryDefinitions8TeV


//------------------------------------------------------------------------------
void
CiCCategoryReader::UpdateInclusiveCat(void)
{
  if (ph1.pt < 40. * massOver120 || ph2.pt < 30. * massOver120) {
/*  if (ph1.pt < 40. * massOver120 || ph2.pt < 30. * massOver120 ||
      ph1_ecalRegion == EEcalFiducialRegion::eGap ||
      ph2_ecalRegion == EEcalFiducialRegion::eGap) {*/
    inclCat = -1;
    return;
  }
  inclCat = 0;
  bool isHighPtGG = (ptgg / mass > 40. / 125.      );
/*  bool isBothEB   = (ph1_ecalRegion == EEcalFiducialRegion::eBarrel &&
                     ph2_ecalRegion == EEcalFiducialRegion::eBarrel);*/
  bool isBothEB   = (ph1.isbarrel && ph2.isbarrel);
  bool isBothR9   = (ph1.r9 > 0.94 && ph2.r9 > 0.94);
  if (!isHighPtGG) inclCat += 4;
  if (!isBothEB  ) inclCat += 2;
  if (!isBothR9  ) inclCat += 1;
} /// UpdateInclusiveCat


//------------------------------------------------------------------------------
void
CiCCategoryReader::UpdateDijetCat(void)
{
  dijetCat = -1;
  if (jet1pt < 30. || jet2pt < 20.) return;
  
  /// We have two jets and all dijet variables are well defined.
  
  if (ph1.pt                      > 60. * massOver120 &&
      ph2.pt                      > 25.               &&
      std::abs(jet1eta - jet2eta) > 3.0               &&
      std::abs(zeppenfeld)        < 2.5               &&
      dijetmass                   > 250.              &&
      std::abs(dphidijetgg)       > 2.6                ) {
    /// Passes loose dijet tag for VBF selection
    dijetCat = 1;
  }
  
  if (dijetCat  == 1    &&
      jet2pt    >  30.  &&
      dijetmass >  500.  ) {
    /// Passes tight dijet tag for VBF selection
    dijetCat = 0;
  }
  
} /// UpdateDijetCat


//------------------------------------------------------------------------------
void
CiCCategoryReader::UpdateVHLepTag(void)
{
  if (VHLepTag < 0) return;
  
  /// Treat VH lep loose and tight independently.
  /// This is not necessary right now but it would make a difference
  /// if the photon pt or diphoMVA cuts were different for VH lep loose
  /// and VH lep tight.
  bool isVHLepTight = (VHLepTag      > 1); /// true for VHLepTag = 2 or 3
  bool isVHLepLoose = (VHLepTag % 2 == 1); /// true for VHLepTag = 1 or 3
  
  if (ph1.pt < 45. * massOver120 || 
      ph2.pt < 25.                ) {
    isVHLepLoose = false;
    isVHLepTight = false;
  }
  
  /// Apply the arbitration, tight has precedence.
  if      (isVHLepTight) VHLepTag = 2;
  else if (isVHLepLoose) VHLepTag = 1;
  else                   VHLepTag = 0;
} /// UpdateVHLepTag


//------------------------------------------------------------------------------
void
CiCCategoryReader::UpdateVHMetTag(void)
{
  if (PassesCommonVHMetTagCuts()) VHMetTag = 1;
  else                            VHMetTag = 0;
}


//------------------------------------------------------------------------------
bool
CiCCategoryReader::PassesCommonVHMetTagCuts(void)
{
  double jet_phi = jetleadNoIDphi;
  double jet_eta = jetleadNoIDeta;
  double dr1 = MathUtils::DeltaR(ph1.scphi, ph1.sceta, jet_phi, jet_eta);
  double dr2 = MathUtils::DeltaR(ph2.scphi, ph2.sceta, jet_phi, jet_eta);
  double dPhiMetGG  = MathUtils::DeltaPhi((Double_t) phigg  ,
                                          (Double_t) corrpfmetphi);
  double dPhiMetJet = MathUtils::DeltaPhi((Double_t) jet_phi,
                                          (Double_t) corrpfmetphi);

  return (
           corrpfmet           > 70.               &&
           ph1.pt              > 45. * massOver120 &&
           ph2.pt              > 25.               &&
           std::abs(dPhiMetGG) > 2.1               &&
           (
             jetleadNoIDpt        < 50. || 
             dr1                  < 0.5 || 
             dr2                  < 0.5 || 
             std::abs(dPhiMetJet) < 2.7
           )
         );
} /// PassesCommonVHMetTagCuts


//------------------------------------------------------------------------------
void
CiCCategoryReader::UpdateTTHTag(void)
{
  if (tthTag < 0) return;
  
  /// Similar logic here as for UpdateVHlepTag(..).
  bool isTTHLep = (tthTag > 1);
  bool isTTHHad = (tthTag % 2 == 1);
  
  if (ph1.pt < 60. * massOver120 || 
      ph2.pt < 25.                ) {
    isTTHLep = false;
    isTTHHad = false;
  }
    
  if      (isTTHLep) tthTag = 2;
  else if (isTTHHad) tthTag = 1;
  else               tthTag = 0;
} /// UpdateTTHTag


//------------------------------------------------------------------------------
void
CiCCategoryReader::UpdateVHHadTag(void)
{
  if (VHHadTag < 0) return;
  
  if (ph1.pt   < 60. * massOver120 || 
      ph2.pt   < 25.                ) VHHadTag = 0;
} /// UpdateVHHadTag


//------------------------------------------------------------------------------
void
CiCCategoryReader::UpdateCiCCat(void)
{
  switch (fBeamEnergy) {
    case EBeamEnergy::k7TeV:
      UpdateCiCCat7TeV();
      break;
    case EBeamEnergy::k8TeV:
      UpdateCiCCat8TeV();
      break;
    default:
      /// This should never happen!
      cms::Exception exception("BadEnum");
      exception << __FILE__ << ":" << __LINE__ << " in " << __FUNCTION__
                << ": Illegal EBeamEnergy enum value: " << fBeamEnergy;
      throw exception;
  } /// fBeamEnergy
} /// UpdateCiCCat


//------------------------------------------------------------------------------
// The meat!
void
CiCCategoryReader::UpdateCiCCat7TeV(void)
{
  cicCat = -999;
  if (!PassesPreselection()) return;
  if      (tthTag   == 2) cicCat = kTTHLep;
  else if (tthTag   == 1) cicCat = kTTHHad;
  else if (VHLepTag == 2) cicCat = kVHLepTight;
  else if (VHLepTag == 1) cicCat = kVHLepLoose;
  else if (dijetCat >= 0) cicCat = kDijet0 + dijetCat;
  else if (VHMetTag == 1) cicCat = kVHMet;
  else if (VHHadTag == 1) cicCat = kVHHad;
  else if (inclCat  >= 0) cicCat = kIncl0 + inclCat;
} /// UpdateCiCCat7TeV


//------------------------------------------------------------------------------
// The meat!
void
CiCCategoryReader::UpdateCiCCat8TeV(void)
{
  cicCat = -999;
  if (!PassesPreselection()) return;
  if      (tthTag   == 2) cicCat = kTTHLep;
  else if (VHLepTag == 2) cicCat = kVHLepTight;
  else if (VHLepTag == 1) cicCat = kVHLepLoose;
  else if (dijetCat >= 0) cicCat = kDijet0 + dijetCat;
  else if (VHMetTag == 1) cicCat = kVHMet;
  else if (tthTag   == 1) cicCat = kTTHHad;
  else if (VHHadTag == 1) cicCat = kVHHad;
  else if (inclCat  >= 0) cicCat = kIncl0 + inclCat;
} /// UpdateCiCCat8TeV


//------------------------------------------------------------------------------
bool
CiCCategoryReader::PassesPreselection(void)
{
  bool passesEventFilter = true;
  if (fEventFilter) {
    passesEventFilter = fEventFilter->filter(run, lumi, evt);
  }
  return (passesEventFilter && 100. < mass && mass < 180.);
} /// PassesPreselection
