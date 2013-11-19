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
  fWeightsPath(iWeights),
  fMaxDPhi(iMaxDPhi),
  fMvaReader(new TMVA::Reader())
{
  Init();
} /// Ctor


//------------------------------------------------------------------------------
DijetMvaReader::~DijetMvaReader()
{} /// Dtor


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
  fMvaReader->AddVariable("dijet_leadEta"        , &jet1eta            );
  fMvaReader->AddVariable("dijet_subleadEta"     , &jet2eta            );
  fMvaReader->AddVariable("dijet_LeadJPt"        , &jet1pt             );
  fMvaReader->AddVariable("dijet_SubJPt"         , &jet2pt             );
  fMvaReader->AddVariable("dijet_Zep"            , &zeppenfeld         );
  fMvaReader->AddVariable("min(dijet_dPhi,2.916)", &dijet_DPhiTruncated);
  fMvaReader->AddVariable("dijet_Mjj"            , &dijetmass          );
  fMvaReader->AddVariable("dipho_pt/mass"        , &ptgg_over_mass     );

  std::cout << "\nINFO: mithep::hgg::DijetMvaReader::Init(): "
            << "Booking TMVA Reader ...\n";
  fMvaReader->BookMVA("BDTG", fWeightsPath.Data());
} /// Init


//------------------------------------------------------------------------------
void
DijetMvaReader::Update(void)
{
  /// Calculate derived diphoMVA inputs.
  dijet_DPhiTruncated = std::min(std::abs(dphidijetgg), fMaxDPhi);
  ptgg_over_mass = ptgg / mass;
  dijetMVA = fMvaReader->EvaluateMVA("BDTG");
} /// Update
