#include <iostream>     // std::cout
#include <algorithm>    // std::min
#include <complex>      // std::abs
#include "MitHgg/PhotonTree/interface/DijetMvaReader.h"

using ::mithep::hgg::DijetMvaReader;
using ::mithep::hgg::TreeReader;

/// Make this a ROOT class
ClassImp(DijetMvaReader)


//------------------------------------------------------------------------------
DijetMvaReader::DijetMvaReader(TTree *iTree, const char *iWeights,
                               Float_t iMaxDPhi) :
  TreeReader(iTree),
  fDijetWeights(iWeights),
  fMaxDPhi(iMaxDPhi),
  fDijetMvaReader(new TMVA::Reader("Silent"))
{
  Init();
} /// Ctor


//------------------------------------------------------------------------------
DijetMvaReader::~DijetMvaReader()
{
  delete fDijetMvaReader;
} /// Dtor


//------------------------------------------------------------------------------
Int_t
DijetMvaReader::GetEntry(Long64_t entry, Int_t getall)
{
  Int_t bytesRead = fTree->GetEntry(entry, getall);
  Update();
  return bytesRead;
} /// GetEntry


//------------------------------------------------------------------------------
void
DijetMvaReader::Init()
{
  fDijetMvaReader->AddVariable("dijet_leadEta"        , &jet1eta            );
  fDijetMvaReader->AddVariable("dijet_subleadEta"     , &jet2eta            );
  fDijetMvaReader->AddVariable("dijet_LeadJPt"        , &jet1pt             );
  fDijetMvaReader->AddVariable("dijet_SubJPt"         , &jet2pt             );
  fDijetMvaReader->AddVariable("dijet_Zep"            , &zeppenfeld         );
  fDijetMvaReader->AddVariable("min(dijet_dPhi,2.916)", &dijet_DPhiTruncated);
  fDijetMvaReader->AddVariable("dijet_Mjj"            , &dijetmass          );
  fDijetMvaReader->AddVariable("dipho_pt/mass"        , &ptgg_over_mass     );

  fDijetMvaReader->BookMVA("BDTG", fDijetWeights.Data());
} /// Init


//------------------------------------------------------------------------------
void
DijetMvaReader::Update(void)
{
  /// Calculate derived diphoMVA inputs.
  dijet_DPhiTruncated = std::min(std::abs(dphidijetgg), fMaxDPhi);
  ptgg_over_mass = ptgg / mass;
  dijetMVA = fDijetMvaReader->EvaluateMVA("BDTG");
} /// Update
