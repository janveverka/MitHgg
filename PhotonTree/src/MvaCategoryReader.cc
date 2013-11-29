#include <algorithm>    // std::max
#include <iostream>     // std::cout
#include "TMath.h"
#include "MitHgg/PhotonTree/interface/MvaCategoryReader.h"
#include "MitCommon/MathTools/interface/MathUtils.h"

using ::mithep::MathUtils;
using ::mithep::hgg::MvaCategoryReader;

namespace default_8tev = ::mithep::hgg::mva_category_defaults_8tev;

/// Make this a ROOT class
ClassImp(MvaCategoryReader)

//------------------------------------------------------------------------------
MvaCategoryReader::MvaCategoryReader(TTree      *iTree                    ,
                                     const char *iDiphoWeights            ,
                                     const char *iDijetWeights            ,
                                     const char *iCombiWeights            ,
                                     bool        iDiphoUseSmearedMassError,
                                     Float_t     iDijetMaxDPhi            ) :
  CombinedMvaReader(iTree                    ,
                    iDiphoWeights            ,
                    iDijetWeights            ,
                    iCombiWeights            ,
                    iDiphoUseSmearedMassError,
                    iDijetMaxDPhi            ),
  massOver120(-99),
  VHMetTag(-1),
  dijetCat(-1),
  inclCat (-1),
  mvaCat  (-1),
  numInclCats (default_8tev::numInclCats ),
  numDijetCats(default_8tev::numDijetCats),
  kIncl0     (0                         ),
  kDijet0    (kIncl0      + numInclCats ),
  kVHLepTight(kDijet0     + numDijetCats),
  kVHLepLoose(kVHLepTight + 1           ),
  kVHMet     (kVHLepLoose + 1           ),
  kTTHLep    (kVHMet      + 1           ),
  kTTHHad    (kTTHLep     + 1           ),
  kVHHad     (kTTHHad     + 1           )
{
  Init();
} /// Ctor


//------------------------------------------------------------------------------
MvaCategoryReader::~MvaCategoryReader()
{} /// Dtor


//------------------------------------------------------------------------------
void
MvaCategoryReader::Init()
{
  SetDiphoMvaCuts(default_8tev::diphoMvaCuts);
  SetDijetMvaCuts(default_8tev::dijetMvaCuts);
  SetCombiMvaCuts(default_8tev::combiMvaCuts);
  Update();
} /// Init


//------------------------------------------------------------------------------
void
MvaCategoryReader::Update(void)
{
  CombinedMvaReader::Update();
  massOver120 = mass / 120.;
  UpdateCategoryDefinitions();
  UpdateInclusiveCat();
  UpdateDijetCat();
  UpdateVHMetTag();
  UpdateTTHTagConvention();
  UpdateVHHadTagConvention();
  UpdateMvaCat();
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
MvaCategoryReader::UpdateCategoryDefinitions()
{
  numInclCats  = diphoMvaCuts.size();
  numDijetCats = std::max(dijetMvaCuts.size(), combiMvaCuts.size());
  kDijet0     = kIncl0      + numInclCats ; // 4-7
  kVHLepTight = kDijet0     + numDijetCats; // 8
  kVHLepLoose = kVHLepTight + 1           ; // 9
  kVHMet      = kVHLepLoose + 1           ; // 10
  kTTHLep     = kVHMet      + 1           ; // 11
  kTTHHad     = kTTHLep     + 1           ; // 12
  kVHHad      = kTTHHad     + 1           ; // 13
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
MvaCategoryReader::UpdateVHMetTag(void)
{
  double jet_phi = jetleadNoIDphi;
  double jet_eta = jetleadNoIDeta;
  double dr1 = MathUtils::DeltaR(ph1.scphi, ph1.sceta, jet_phi, jet_eta);
  double dr2 = MathUtils::DeltaR(ph2.scphi, ph2.sceta, jet_phi, jet_eta);
  double dPhiMetGG  = MathUtils::DeltaPhi((Double_t) phigg  ,
                                          (Double_t) corrpfmetphi);
  double dPhiMetJet = MathUtils::DeltaPhi((Double_t) jet_phi,
                                          (Double_t) corrpfmetphi);

  if (TMath::Abs(ph1.sceta) < 1.4442   &&
      TMath::Abs(ph2.sceta) < 1.4442   &&
      corrpfmet             > 70.      &&
      diphoMVA              > 0.       &&
      pho1_ptOverM          > 45./120. &&
      pho2_ptOverM          > 30./120. &&
      dPhiMetGG             > 2.1      &&
      (jetleadNoIDpt < 50. || dr1 < 0.5 || dr2 < 0.5 || dPhiMetJet < 2.7)) {
    VHMetTag = 1;
  } else {
    VHMetTag = 0;
  }
} /// UpdateVHMetTag


//------------------------------------------------------------------------------
// To comply with the Hgg group convention that differs from ours at the
// moment.
// TODO:
// Check the pt of the photons, see L2064 and L2065 of the Hgg AN
//   double reducedMass = fDiphotonEvent->mass / 120.;
//   if (phHard->Pt() < 60. * reducedMass) return;
//   if (phSoft->Pt() < 30. * reducedMass && !fIsCutBased) return;
//   if (phSoft->Pt() < 25.               &&  fIsCutBased) return;
// Check the Photon ID for ttH lep, see L2071-2072 of the Hgg AN
//     bool passPhotonID = false;
//     if (fIsCutBased) {
//       passPhotonID = true;
//     } else {
//       passPhotonID = (phHard->IdMva() > -0.6 &&
//                       phSoft->IdMva() > -0.6);
//     }
// Check the Photon ID for ttH had, see L2092-2093 of the Hgg AN
//     bool passPhotonID = false;
//     if (fIsCutBased) {
//       passPhotonID = true;
//     } else {
//       passPhotonID = (phHard->IdMva() > -0.2 &&
//                       phSoft->IdMva() > -0.2);
//     }

void
MvaCategoryReader::UpdateTTHTagConvention(void)
{
  if      (tthTag   == 1) tthTag = 2;
  else if (tthTag   == 2) tthTag = 1;
} /// UpdateTTHTagConvention


//------------------------------------------------------------------------------
// Poor man's merging of the to b-tag and no-b-tag categories
// TODO: See L2007-2013 of the Hgg AN 2013/253 v3
// phHard->Pt() > 60. * reducedMass &&
// (fIsCutBased ? phSoft->Pt() > 25. : phSoft->Pt() > 30. * reducedMass) &&
// 100. < mass && mass < 180. &&
void
MvaCategoryReader::UpdateVHHadTagConvention(void)
{
  if (VHHadTag == 2) VHHadTag = 1;
} /// UpdateVHHadTagConvention


// TODO: Update VH lep tag
//   Float_t reducedMass = fDiphotonEvent->mass / 120.;
//   bool passesPhotonCuts = (
//     phHard->Pt() > 45 * reducedMass &&
//     (fIsCutBased ? phSoft->Pt() > 25 : phSoft->Pt() > 30 * reducedMass) &&
//     (fIsCutBased ? true              : phHard->IdMva() > -0.2) &&
//     (fIsCutBased ? true              : phSoft->IdMva() > -0.2)
//   );
//   if (not passesPhotonCuts) return;


//------------------------------------------------------------------------------
// The meat!
void
MvaCategoryReader::UpdateMvaCat(void)
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
} /// UpdateMvaCat

//------------------------------------------------------------------------------
bool
MvaCategoryReader::PassesPreselection(void)
{
  return (100              < mass      && mass             < 180      &&
          40 * massOver120 < ph1.pt    && 30 * massOver120 < ph2.pt   &&
          -0.2             < ph1.idmva && -0.2             < ph2.idmva);
} /// PassesPreselection
