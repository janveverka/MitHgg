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
                                     const char *iCombiWeights            ,
                                     const char *iDijetWeights            ,
                                     const char *iDiphoWeights            ,
                                     bool        iDiphoUseSmearedMassError,
                                     Float_t     iDijetMaxDPhi            ) :
  CombinedMvaReader(iTree, iCombiWeights, iDijetWeights, iDiphoWeights,
                    iDiphoUseSmearedMassError, iDijetMaxDPhi),
  VHMetTag(0),
  dijetCat(-1),
  inclCat(-1),
  mvaCat(-1),
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
MvaCategoryReader::SetDiphoMvaCuts(const std::vector<float> & cuts)
{
  diphoMvaCuts = cuts;
  numInclCats  = cuts.size();
  UpdateCategoryDefinitions();
} /// SetDiphoMvaCuts


//------------------------------------------------------------------------------
void
MvaCategoryReader::SetDijetMvaCuts(const std::vector<float> & cuts)
{
  dijetMvaCuts = cuts;
  numDijetCats = cuts.size();
  UpdateCategoryDefinitions();
} /// SetDijetMvaCuts


//------------------------------------------------------------------------------
void
MvaCategoryReader::SetCombiMvaCuts(const std::vector<float> & cuts)
{
  combiMvaCuts = cuts;
  numDijetCats = cuts.size();
  UpdateCategoryDefinitions();
} /// SetCombiMvaCuts


//------------------------------------------------------------------------------
void
MvaCategoryReader::UpdateCategoryDefinitions()
{
  kDijet0     = kIncl0      + numInclCats ;
  kVHLepTight = kDijet0     + numDijetCats;
  kVHLepLoose = kVHLepTight + 1           ;
  kVHMet      = kVHLepLoose + 1           ;
  kTTHLep     = kVHMet      + 1           ;
  kTTHHad     = kTTHLep     + 1           ;
  kVHHad      = kTTHHad     + 1           ;
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
  assert(dijetMvaCuts.size() == combiMvaCuts.size());

  if (vbfTag <= 0) {
    dijetCat = -1;
    return;
  }

  for (dijetCat=0; dijetCat < numDijetCats; dijetCat++) {
    if (dijetMVA > dijetMvaCuts[dijetCat] &&
        combiMVA > combiMvaCuts[dijetCat]) {
      break;
    }
  }

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
void
MvaCategoryReader::UpdateTTHTagConvention(void)
{
  if      (tthTag   == 1) tthTag = 2;
  else if (tthTag   == 2) tthTag = 1;
} /// UpdateTTHTagConvention


//------------------------------------------------------------------------------
// Poor man's merging of the to b-tag and no-b-tag categories
void
MvaCategoryReader::UpdateVHHadTagConvention(void)
{
  if (VHHadTag == 2) VHHadTag = 1;
} /// UpdateVHHadTagConvention


//------------------------------------------------------------------------------
// The meat!
void
MvaCategoryReader::UpdateMvaCat(void)
{
  if      (tthTag   == 2) mvaCat = kTTHLep;
  else if (VHLepTag == 2) mvaCat = kVHLepTight;
  else if (VHLepTag == 1) mvaCat = kVHLepLoose;
  else if (dijetCat >= 0) mvaCat = kDijet0 + dijetCat;
  else if (VHMetTag == 1) mvaCat = kVHMet;
  else if (tthTag   == 1) mvaCat = kTTHHad;
  else if (VHHadTag == 1) mvaCat = kVHHad;
  else if (inclCat  >= 0) mvaCat = kIncl0 + inclCat;
  else                    mvaCat = -999;
} /// UpdateMvaCat
