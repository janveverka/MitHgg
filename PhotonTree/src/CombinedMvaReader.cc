#include <iostream>     // std::cout
#include "MitHgg/PhotonTree/interface/CombinedMvaReader.h"

using ::mithep::hgg::CombinedMvaReader;

/// Make this a ROOT class
ClassImp(CombinedMvaReader)


//------------------------------------------------------------------------------
CombinedMvaReader::CombinedMvaReader(TTree *iTree,
                                     const char *iDiphoWeights,
                                     const char *iDijetWeights,
                                     const char *iCombiWeights,
                                     bool iDiphoUseSmearedMassError,
                                     Float_t iDijetMaxDPhi) :
  DiphotonAndDijetMvaReader(iTree,
                            iDiphoWeights,
                            iDijetWeights,
                            iDiphoUseSmearedMassError,
                            iDijetMaxDPhi             ),
  fCombiWeights            (iCombiWeights             ),
  fCombiMvaReader          (new TMVA::Reader("Silent"))
{
  Init();
} /// Ctor


//------------------------------------------------------------------------------
CombinedMvaReader::~CombinedMvaReader()
{
  delete fCombiMvaReader;
} /// Dtor


//------------------------------------------------------------------------------
void
CombinedMvaReader::SetCombiMvaWeights(const char *path)
{
  fCombiWeights = path;
  delete fCombiMvaReader;
  fCombiMvaReader = new TMVA::Reader("Silent");
  Init();
} /// SetCombiMvaWeights


//------------------------------------------------------------------------------
void
CombinedMvaReader::Init()
{
  fCombiMvaReader->AddVariable("dipho_mva"        , &diphoMVA      );
  fCombiMvaReader->AddVariable("bdt_dijet_maxdPhi", &dijetMVA      );
  fCombiMvaReader->AddVariable("dipho_pt/mass"    , &ptgg_over_mass);

  fCombiMvaReader->BookMVA("BDTG", fCombiWeights.Data());
} /// Init


//------------------------------------------------------------------------------
void
CombinedMvaReader::Update(void)
{
  DiphotonAndDijetMvaReader::Update();
  combiMVA = fCombiMvaReader->EvaluateMVA("BDTG");
} /// Update
