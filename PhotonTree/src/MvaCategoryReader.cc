#include <algorithm>    // std::max
#include <complex>      // std::abs
#include <iostream>     // std::cout
#include "TMath.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "MitHgg/PhotonTree/interface/MvaCategoryReader.h"
#include "MitCommon/MathTools/interface/MathUtils.h"

using ::mithep::MathUtils;
using ::mithep::hgg::MvaCategoryReader;

namespace default_8tev = ::mithep::hgg::mva_category_defaults_8tev;

/// Make this a ROOT class
ClassImp(MvaCategoryReader)

//------------------------------------------------------------------------------
MvaCategoryReader::MvaCategoryReader(TTree       *iTree                    ,
                                     EBeamEnergy  iBeamEnergy              ,
                                     const char  *iDiphoWeights            ,
                                     const char  *iDijetWeights            ,
                                     const char  *iCombiWeights            ,
                                     bool         iDiphoUseSmearedMassError,
                                     Float_t      iDijetMaxDPhi            ,
                                     const char  *iDiphoTmvaOption         ,
                                     const char  *iDijetTmvaOption         ,
                                     const char  *iCombiTmvaOption         ,
                                     const char  *eventsToSkip             ) :
  CombinedMvaReader(iTree                    ,
                    iBeamEnergy              ,
                    iDiphoWeights            ,
                    iDijetWeights            ,
                    iCombiWeights            ,
                    iDiphoUseSmearedMassError,
                    iDijetMaxDPhi            ,
                    iDiphoTmvaOption         ,
                    iDijetTmvaOption         ,
                    iCombiTmvaOption         ),
  massOver120 (-99                       ),
  VHMetTag    (-1                        ),
  dijetCat    (-1                        ),
  inclCat     (-1                        ),
  mvaCat      (-1                        ),
  numInclCats (default_8tev::numInclCats ),
  numDijetCats(default_8tev::numDijetCats),
  kIncl0      (0                         ),
  kDijet0     (kIncl0      + numInclCats ),
  kVHLepTight (kDijet0     + numDijetCats),
  kVHLepLoose (kVHLepTight + 1           ),
  kVHMet      (kVHLepLoose + 1           ),
  kTTHLep     (kVHMet      + 1           ),
  kTTHHad     (kTTHLep     + 1           ),
  kVHHad      (kTTHHad     + 1           ),
  fEventsToSkip(eventsToSkip),
  fEventFilter(0)
{
  Init();
} /// Ctor


//------------------------------------------------------------------------------
MvaCategoryReader::~MvaCategoryReader()
{
  delete fEventFilter;
} /// Dtor


//------------------------------------------------------------------------------
void
MvaCategoryReader::Init()
{
  SetDiphoMvaCuts(default_8tev::diphoMvaCuts);
  SetDijetMvaCuts(default_8tev::dijetMvaCuts);
  SetCombiMvaCuts(default_8tev::combiMvaCuts);
  Update();
  /// FIXME: Modify the if-statement below so that we don't try to open
  /// the event-list file if the file name is an empty string
  /// This should fix the error message: "Unable to open event list file "
  if (fEventsToSkip.c_str()) {
    fEventFilter = new EventFilterFromListStandAlone(fEventsToSkip);
  }
} /// Init


//------------------------------------------------------------------------------
void
MvaCategoryReader::Update(void)
{
  CombinedMvaReader::Update();
  massOver120 = mass / 120.;
  switch (fBeamEnergy) {
    case EBeamEnergy::k7TeV:
      Update7TeV();
      break;
    case EBeamEnergy::k8TeV:
      Update8TeV();
      break;
    default:
      /// This should never happen!
      cms::Exception exception("BadEnum");
      exception << __FILE__ << ":" << __LINE__ << " in " << __FUNCTION__
                << ": Illegal EBeamEnergy enum value: " << fBeamEnergy;
      throw exception;
  } /// fBeamEnergy  
} /// Update


//------------------------------------------------------------------------------
void
MvaCategoryReader::SetDiphoMvaCuts(const vdouble & cuts)
{
  diphoMvaCuts = cuts;
  UpdateCategoryDefinitions();
} /// SetDiphoMvaCuts


//------------------------------------------------------------------------------
void
MvaCategoryReader::SetDijetMvaCuts(const vdouble & cuts)
{
  dijetMvaCuts = cuts;
  UpdateCategoryDefinitions();
} /// SetDijetMvaCuts


//------------------------------------------------------------------------------
void
MvaCategoryReader::SetCombiMvaCuts(const vdouble & cuts)
{
  combiMvaCuts = cuts;
  UpdateCategoryDefinitions();
} /// SetCombiMvaCuts


//------------------------------------------------------------------------------
void
MvaCategoryReader::Update7TeV()
{
  UpdateCategoryDefinitions7TeV();
  
  UpdateInclusiveCat           ();
  UpdateDijetCat               ();
  UpdateVHLepTag               ( 0.1, 0.1);
  UpdateVHMetTag               ( 0.8     );
  UpdateTTHTag                 ( 0.6, 0.6);
  UpdateVHHadTag               ( 0.6     );
  
  UpdateMvaCat7TeV             ();
} /// Update7TeV


//------------------------------------------------------------------------------
void
MvaCategoryReader::Update8TeV()
{
  UpdateCategoryDefinitions8TeV();
  
  UpdateInclusiveCat           ();
  UpdateDijetCat               ();
  UpdateVHLepTag               (-0.6, -0.6);
  UpdateVHMetTag               ( 0.0      );
  UpdateTTHTag                 (-0.6, -0.2);
  UpdateVHHadTag               ( 0.2      );
  
  UpdateMvaCat8TeV             ();
} /// Update8TeV


//------------------------------------------------------------------------------
void
MvaCategoryReader::UpdateCategoryDefinitions()
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
MvaCategoryReader::UpdateCategoryDefinitions7TeV()
{
  numInclCats  = diphoMvaCuts.size();
  numDijetCats = std::max(dijetMvaCuts.size(), combiMvaCuts.size());
  kDijet0      = kIncl0      + numInclCats ; /// 4-5
  kVHLepTight  = kDijet0     + numDijetCats; /// 6
  kVHLepLoose  = kVHLepTight + 1           ; /// 7
  kVHMet       = kVHLepLoose + 1           ; /// 8
  kTTHLep      = kVHMet      + 1           ; /// 9
  kTTHHad      = kTTHLep     + 0           ; /// 9 (same as ttH lep)
  kVHHad       = kTTHHad     + 1           ; /// 10
} /// UpdateCategoryDefinitions7TeV


//------------------------------------------------------------------------------
void
MvaCategoryReader::UpdateCategoryDefinitions8TeV()
{
  numInclCats  = diphoMvaCuts.size();
  numDijetCats = std::max(dijetMvaCuts.size(), combiMvaCuts.size());
  kDijet0      = kIncl0      + numInclCats ; /// 5-7
  kVHLepTight  = kDijet0     + numDijetCats; /// 8
  kVHLepLoose  = kVHLepTight + 1           ; /// 9
  kVHMet       = kVHLepLoose + 1           ; /// 10
  kTTHLep      = kVHMet      + 1           ; /// 11
  kTTHHad      = kTTHLep     + 1           ; /// 12
  kVHHad       = kTTHHad     + 1           ; /// 13
} /// UpdateCategoryDefinitions


//------------------------------------------------------------------------------
void
MvaCategoryReader::UpdateInclusiveCat(void)
{
  for (inclCat=0; inclCat < numInclCats; inclCat++) {
    if (diphoMVA > diphoMvaCuts[inclCat]) break;
  }
  if (inclCat >= numInclCats) inclCat = -1;
} /// UpdateInclusiveCat


//------------------------------------------------------------------------------
void
MvaCategoryReader::UpdateDijetCat(void)
{
  assert(dijetMvaCuts.size() == 0 || 
         dijetMvaCuts.size() == combiMvaCuts.size());

  dijetCat = -1;
  if (jet1pt < 30 || jet2pt < 20 || dijetmass < 250) return;
  
  for (dijetCat=0; dijetCat < numDijetCats; dijetCat++) {
    if (dijetMvaCuts.size() == 0) {
      if (combiMVA > combiMvaCuts[dijetCat]) break;
    } else {
      /// Use L-shaped cats in dijet and combined MVA's
      if (dijetMVA > dijetMvaCuts[dijetCat] &&
          combiMVA > combiMvaCuts[dijetCat]) break;
    } /// Use Combined MVA only
  } /// Loop over dijet categories

  if (dijetCat >= numDijetCats) {
    dijetCat = -1;
  }
} /// UpdateDijetCat


//------------------------------------------------------------------------------
void
MvaCategoryReader::UpdateVHLepTag(double minDiphoMVALoose,
                                  double minDiphoMVATight)
{
  if (VHLepTag < 0) return;
  
  /// Treat VH lep loose and tight independently.
  /// This is not necessary right now but it would make a difference
  /// if the photon pt or diphoMVA cuts were different for VH lep loose
  /// and VH lep tight.
  bool isVHLepTight = (VHLepTag      > 1); /// true for VHLepTag = 2 or 3
  bool isVHLepLoose = (VHLepTag % 2 == 1); /// true for VHLepTag = 1 or 3
  
  if (ph1.pt < 45 * massOver120 || 
      ph2.pt < 30 * massOver120) {
    isVHLepLoose = false;
    isVHLepTight = false;
  }
  
  if (diphoMVA < minDiphoMVALoose) isVHLepLoose = false;
  if (diphoMVA < minDiphoMVATight) isVHLepTight = false;
  
  /// Apply the arbitration, tight has precedence.
  if      (isVHLepTight) VHLepTag = 2;
  else if (isVHLepLoose) VHLepTag = 1;
  else                   VHLepTag = 0;
} /// UpdateVHLepTag


//------------------------------------------------------------------------------
void
MvaCategoryReader::UpdateVHMetTag(double minDiphoMVA)
{
  if (PassesCommonVHMetTagCuts() && diphoMVA > minDiphoMVA) VHMetTag = 1;
  else                                                      VHMetTag = 0;
}


//------------------------------------------------------------------------------
bool
MvaCategoryReader::PassesCommonVHMetTagCuts(void)
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
           corrpfmet           > 70.      &&
           pho1_ptOverM        > 45./120. &&
           pho2_ptOverM        > 30./120. &&
           std::abs(dPhiMetGG) > 2.1      &&
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
MvaCategoryReader::UpdateTTHTag(double minDiphoMVALep, double minDiphoMVAHad)
{
  if (tthTag < 0) return;
  
  /// Similar logic here as for UpdateVHlepTag(..).
  bool isTTHLep = (tthTag > 1);
  bool isTTHHad = (tthTag % 2 == 1);
  
  if (ph1.pt < 60 * massOver120 || 
      ph2.pt < 30 * massOver120  ) {
    isTTHLep = false;
    isTTHHad = false;
  }
  
  if (diphoMVA < minDiphoMVALep) isTTHLep = false;
  if (diphoMVA < minDiphoMVAHad) isTTHHad = false;
  
  if      (isTTHLep) tthTag = 2;
  else if (isTTHHad) tthTag = 1;
  else               tthTag = 0;
} /// UpdateTTHTag


//------------------------------------------------------------------------------
void
MvaCategoryReader::UpdateVHHadTag(double minDiphoMVA)
{
  if (VHHadTag < 0) return;
  
  if (ph1.pt   < 60 * massOver120 || 
      ph2.pt   < 30 * massOver120 ||
      diphoMVA < minDiphoMVA        ) VHHadTag = 0;
} /// UpdateVHHadTag


//------------------------------------------------------------------------------
// The meat!
void
MvaCategoryReader::UpdateMvaCat7TeV(void)
{
  mvaCat = -999;
  if (!PassesPreselection()) return;
  if      (tthTag   == 2) mvaCat = kTTHLep;
  else if (tthTag   == 1) mvaCat = kTTHHad;
  else if (VHLepTag == 2) mvaCat = kVHLepTight;
  else if (VHLepTag == 1) mvaCat = kVHLepLoose;
  else if (dijetCat >= 0) mvaCat = kDijet0 + dijetCat;
  else if (VHMetTag == 1) mvaCat = kVHMet;
  else if (VHHadTag == 1) mvaCat = kVHHad;
  else if (inclCat  >= 0) mvaCat = kIncl0 + inclCat;
} /// UpdateMvaCat7TeV


//------------------------------------------------------------------------------
// More meat!
void
MvaCategoryReader::UpdateMvaCat8TeV(void)
{
  mvaCat = -999;
  if (!PassesPreselection()) return;
  if      (tthTag   == 2) mvaCat = kTTHLep;
  else if (VHLepTag == 2) mvaCat = kVHLepTight;
  else if (VHLepTag == 1) mvaCat = kVHLepLoose;
  else if (dijetCat >= 0) mvaCat = kDijet0 + dijetCat;
  else if (VHMetTag == 1) mvaCat = kVHMet;
  else if (tthTag   == 1) mvaCat = kTTHHad;
  else if (VHHadTag == 1) mvaCat = kVHHad;
  else if (inclCat  >= 0) mvaCat = kIncl0 + inclCat;
} /// UpdateMvaCat8TeV


//------------------------------------------------------------------------------
bool
MvaCategoryReader::PassesPreselection(void)
{
  bool passesEventFilter = true;
  if (fEventFilter) {
    passesEventFilter = fEventFilter->filter(run, lumi, evt);
  }
  return (passesEventFilter            &&
          100              < mass      && mass             < 180      &&
          40 * massOver120 < ph1.pt    && 30 * massOver120 < ph2.pt   &&
          -0.2             < ph1.idmva && -0.2             < ph2.idmva);
} /// PassesPreselection
