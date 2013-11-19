#include <iostream>     // std::cout
#include "TMath.h"
#include "MitHgg/PhotonTree/interface/MvaCategoryReader.h"

using ::mithep::hgg::MvaCategoryReader;

/// Make this a ROOT class
ClassImp(MvaCategoryReader)


//------------------------------------------------------------------------------
MvaCategoryReader::MvaCategoryReader(TTree *iTree,
                                     const char *iCombinedWeights,
                                     const char *iDijetWeights   ,
                                     const char *iDiphotonWeights,
                                     Float_t iDijetMaxDPhi) :
  TreeReader       (iTree),
  CombinedMvaReader(iTree, iCombinedWeights, iDijetWeights, iDiphotonWeights,
                    iDijetMaxDPhi),
  VHMetTag(0),
  mvaCat(kIncl0)
{
  Init();
} /// Ctor


//------------------------------------------------------------------------------
MvaCategoryReader::~MvaCategoryReader()
{} /// Dtor


//------------------------------------------------------------------------------
Int_t
MvaCategoryReader::GetEntry(Long64_t entry, Int_t getall)
{
  Int_t bytesRead = fTree->GetEntry(entry, getall);
  CombinedMvaReader::Update();
  MvaCategoryReader::Update();
  return bytesRead;
} /// GetEntry


//------------------------------------------------------------------------------
void
MvaCategoryReader::Init()
{
  Update();
} /// Init


//------------------------------------------------------------------------------
void
MvaCategoryReader::Update(void)
{
  UpdateTTHTagConvention();
  UpdateVHHadTagConvention();
  UpdateVHMetTag();
} /// Update


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
void
MvaCategoryReader::UpdateVHMetTag(void)
{
  double dr1 = DeltaR(ph1.sceta, ph1.scphi, jetleadNoIDeta, jetleadNoIDphi);
  double dr2 = DeltaR(ph2.sceta, ph2.scphi, jetleadNoIDeta, jetleadNoIDphi);
  double dPhiMetGG  = DeltaPhi(phigg         , corrpfmetphi);
  double dPhiMetJet = DeltaPhi(jetleadNoIDphi, corrpfmetphi);

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
double
MvaCategoryReader::DeltaR(double eta1, double phi1, double eta2, double phi2)
{
  double deta   = eta1 - eta2;
  double dphi   = DeltaPhi(phi1, phi2);
  return TMath::Sqrt(deta * deta + dphi * dphi);
} /// DeltaR


//------------------------------------------------------------------------------
double
MvaCategoryReader::DeltaPhi(double phi1, double phi2)
{
  return TMath::ACos(TMath::Cos(phi1 - phi2));
} /// DeltaPhi
