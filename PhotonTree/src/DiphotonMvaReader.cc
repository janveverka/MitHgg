#include <iostream>
#include "TMath.h"
#include "MitHgg/PhotonTree/interface/DiphotonMvaReader.h"

using ::mithep::hgg::DiphotonMvaReader;
using ::mithep::hgg::TreeReader;

/// Make this a ROOT class
ClassImp(DiphotonMvaReader)


//------------------------------------------------------------------------------
DiphotonMvaReader::DiphotonMvaReader(TTree *tree, const char *weights,
                                     bool useSmearedMassError) :
  TreeReader(tree),
  fDiphotonWeights(weights),
  fUseSmearedMassError(useSmearedMassError),
  fDiphotonMvaReader(new TMVA::Reader("Silent"))
{
  Init();
} /// Ctor


//------------------------------------------------------------------------------
DiphotonMvaReader::~DiphotonMvaReader()
{
  delete fDiphotonMvaReader;
} /// Dtor


//------------------------------------------------------------------------------
void
DiphotonMvaReader::Init()
{
  fDiphotonMvaReader->AddVariable("masserrsmeared/mass"        , &rVtxSigmaMoM);
  fDiphotonMvaReader->AddVariable("masserrsmearedwrongvtx/mass", &wVtxSigmaMoM);
  fDiphotonMvaReader->AddVariable("vtxprob"                    , &vtxprob     );
  fDiphotonMvaReader->AddVariable("ph1.pt/mass"                , &pho1_ptOverM);
  fDiphotonMvaReader->AddVariable("ph2.pt/mass"                , &pho2_ptOverM);
  fDiphotonMvaReader->AddVariable("ph1.eta"                    , &ph1.eta     );
  fDiphotonMvaReader->AddVariable("ph2.eta"                    , &ph2.eta     );
  fDiphotonMvaReader->AddVariable("TMath::Cos(ph1.phi-ph2.phi)", &cosDPhi     );
  fDiphotonMvaReader->AddVariable("ph1.idmva"                  , &ph1.idmva   );
  fDiphotonMvaReader->AddVariable("ph2.idmva"                  , &ph2.idmva   );

  fDiphotonMvaReader->BookMVA("BDTG", fDiphotonWeights.Data());
} /// Init


//------------------------------------------------------------------------------
void
DiphotonMvaReader::Update(void)
{
  TreeReader::Update();
  /// Calculate derived diphoMVA inputs.
  if (fUseSmearedMassError) {
    rVtxSigmaMoM = masserrsmeared / mass;
    wVtxSigmaMoM = masserrsmearedwrongvtx / mass;
  } else {
    rVtxSigmaMoM = masserr / mass;
    wVtxSigmaMoM = masserrwrongvtx / mass;
  }
  cosDPhi      = TMath::Cos(ph1.phi - ph2.phi);
  pho1_ptOverM = ph1.pt / mass;
  pho2_ptOverM = ph2.pt / mass;
  diphoMVA = fDiphotonMvaReader->EvaluateMVA("BDTG");
} /// Update
