#include <algorithm>    // std::min
#include <complex>      // std::abs
#include <iostream>     // std::cout
#include "MitHgg/PhotonTree/interface/DiphotonAndDijetMvaReader.h"

using ::mithep::hgg::DiphotonAndDijetMvaReader;
using ::mithep::hgg::DiphotonMvaReader;

/// Make this a ROOT class
ClassImp(DiphotonAndDijetMvaReader)


//------------------------------------------------------------------------------
DiphotonAndDijetMvaReader::DiphotonAndDijetMvaReader(
  TTree *iTree, 
  const char *iDiphoWeights,
  const char *iDijetWeights,
  bool        iDiphoUseSmearedMassError,
  Float_t     iDijetMaxDPhi
) :
  DiphotonMvaReader         (iTree, iDiphoWeights, iDiphoUseSmearedMassError),
  fDijetWeights             (iDijetWeights),
  fDijetMaxDPhi             (iDijetMaxDPhi),
  fDijetMvaReader(new TMVA::Reader("Silent"))
{
  Init();
} /// Ctor


//------------------------------------------------------------------------------
DiphotonAndDijetMvaReader::~DiphotonAndDijetMvaReader()
{
  delete fDijetMvaReader;
} /// Dtor


//------------------------------------------------------------------------------
void
DiphotonAndDijetMvaReader::SetDijetMvaWeights(const char *path)
{
  fDijetWeights = path;
  delete fDijetMvaReader;
  fDijetMvaReader = new TMVA::Reader("Silent");
  Init();
} /// SetDijetMvaWeights


//------------------------------------------------------------------------------
void
DiphotonAndDijetMvaReader::Init()
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
DiphotonAndDijetMvaReader::Update(void)
{
  DiphotonMvaReader::Update();
  /// Calculate derived diphoMVA inputs.
  dijet_DPhiTruncated = std::min(std::abs(dphidijetgg), fDijetMaxDPhi);
  ptgg_over_mass = ptgg / mass;
  dijetMVA = fDijetMvaReader->EvaluateMVA("BDTG");
} /// Update
