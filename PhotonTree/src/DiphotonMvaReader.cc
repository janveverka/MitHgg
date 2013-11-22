#include <iostream>
#include "TMath.h"
#include "MitHgg/PhotonTree/interface/DiphotonMvaReader.h"

using ::mithep::hgg::DiphotonMvaReader;
using ::mithep::hgg::TreeReader;

/// Make this a ROOT class
ClassImp(DiphotonMvaReader)


//------------------------------------------------------------------------------
DiphotonMvaReader::DiphotonMvaReader(TTree *tree, const char *iDiphoWeights,
                                     bool iDiphoUseSmearedMassError) :
  TreeReader(tree),
  fDiphoWeights(iDiphoWeights),
  fDiphoUseSmearedMassError(iDiphoUseSmearedMassError),
  fDiphoMvaReader(new TMVA::Reader("Silent"))
{
  Init();
} /// Ctor


//------------------------------------------------------------------------------
DiphotonMvaReader::~DiphotonMvaReader()
{
  delete fDiphoMvaReader;
} /// Dtor


//------------------------------------------------------------------------------
void
DiphotonMvaReader::Init()
{
  fDiphoMvaReader->AddVariable("masserrsmeared/mass"        , &rVtxSigmaMoM);
  fDiphoMvaReader->AddVariable("masserrsmearedwrongvtx/mass", &wVtxSigmaMoM);
  fDiphoMvaReader->AddVariable("vtxprob"                    , &vtxprob     );
  fDiphoMvaReader->AddVariable("ph1.pt/mass"                , &pho1_ptOverM);
  fDiphoMvaReader->AddVariable("ph2.pt/mass"                , &pho2_ptOverM);
  fDiphoMvaReader->AddVariable("ph1.eta"                    , &ph1.eta     );
  fDiphoMvaReader->AddVariable("ph2.eta"                    , &ph2.eta     );
  fDiphoMvaReader->AddVariable("TMath::Cos(ph1.phi-ph2.phi)", &cosDPhi     );
  fDiphoMvaReader->AddVariable("ph1.idmva"                  , &ph1.idmva   );
  fDiphoMvaReader->AddVariable("ph2.idmva"                  , &ph2.idmva   );

  fDiphoMvaReader->BookMVA("BDTG", fDiphoWeights.Data());
} /// Init


//------------------------------------------------------------------------------
void
DiphotonMvaReader::Update(void)
{
  TreeReader::Update();
  /// Calculate derived diphoMVA inputs.
  if (fDiphoUseSmearedMassError) {
    rVtxSigmaMoM = masserrsmeared / mass;
    wVtxSigmaMoM = masserrsmearedwrongvtx / mass;
  } else {
    rVtxSigmaMoM = masserr / mass;
    wVtxSigmaMoM = masserrwrongvtx / mass;
  }
  cosDPhi      = TMath::Cos(ph1.phi - ph2.phi);
  pho1_ptOverM = ph1.pt / mass;
  pho2_ptOverM = ph2.pt / mass;
  diphoMVA     = fDiphoMvaReader->EvaluateMVA("BDTG");
} /// Update
