#include <iostream>
#include "TMath.h"
#include "MitHgg/PhotonTree/interface/DiphotonMvaReader.h"

using ::mithep::hgg::DiphotonMvaReader;
using ::mithep::hgg::TreeReader;

/// Make this a ROOT class
ClassImp(DiphotonMvaReader)


//------------------------------------------------------------------------------
DiphotonMvaReader::DiphotonMvaReader(TTree *iTree, const char *iWeights) :
  TreeReader(iTree),
  fWeightsPath(iWeights),
  fMvaReader(new TMVA::Reader())
{
  Init();
} /// Ctor


//------------------------------------------------------------------------------
DiphotonMvaReader::~DiphotonMvaReader()
{} /// Dtor


//------------------------------------------------------------------------------
Int_t
DiphotonMvaReader::GetEntry(Long64_t entry, Int_t getall)
{
  Int_t ret = fTree->GetEntry(entry, getall);
  Update();
  return ret;
} /// GetEntry


//------------------------------------------------------------------------------
void
DiphotonMvaReader::Init()
{
  fMvaReader->AddVariable("masserrsmeared/mass"        , &rVtxSigmaMoM);
  fMvaReader->AddVariable("masserrsmearedwrongvtx/mass", &wVtxSigmaMoM);
  fMvaReader->AddVariable("vtxprob"                    , &vtxprob     );
  fMvaReader->AddVariable("ph1.pt/mass"                , &pho1_ptOverM);
  fMvaReader->AddVariable("ph2.pt/mass"                , &pho2_ptOverM);
  fMvaReader->AddVariable("ph1.eta"                    , &ph1.eta     );
  fMvaReader->AddVariable("ph2.eta"                    , &ph2.eta     );
  fMvaReader->AddVariable("TMath::Cos(ph1.phi-ph2.phi)", &cosDPhi     );
  fMvaReader->AddVariable("ph1.idmva"                  , &ph1.idmva   );
  fMvaReader->AddVariable("ph2.idmva"                  , &ph2.idmva   );

  std::cout << "\nINFO: mithep::hgg::DiphotonMvaReader::Init(): "
            << "Booking TMVA Reader ...\n";
  fMvaReader->BookMVA("BDTG", fWeightsPath.Data());
} /// Init


//------------------------------------------------------------------------------
void
DiphotonMvaReader::Update(void)
{
  /// Calculate derived diphoMVA inputs.
  // rVtxSigmaMoM = masserr_ns / mass;               /// no smearing
  // wVtxSigmaMoM = masserrwvtx_ns / mass;           /// no smearing
  rVtxSigmaMoM = masserrsmeared / mass;           /// with smearing
  wVtxSigmaMoM = masserrsmearedwrongvtx / mass;   /// with smearing
  cosDPhi      = TMath::Cos(ph1.phi - ph2.phi);
  pho1_ptOverM = ph1.pt / mass;
  pho2_ptOverM = ph2.pt / mass;
  diphoMVA = fMvaReader->EvaluateMVA("BDTG");
} /// Update
