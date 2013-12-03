#include <iostream>     // std::cout
#include "FWCore/Utilities/interface/Exception.h"
#include "MitHgg/PhotonTree/interface/CombinedMvaReader.h"

using ::mithep::hgg::CombinedMvaReader;

/// Make this a ROOT class
ClassImp(CombinedMvaReader)


//------------------------------------------------------------------------------
CombinedMvaReader::CombinedMvaReader(TTree *iTree,
                                     EBeamEnergy iBeamEnergy,
                                     const char *iDiphoWeights,
                                     const char *iDijetWeights,
                                     const char *iCombiWeights,
                                     bool iDiphoUseSmearedMassError,
                                     Float_t iDijetMaxDPhi) :
  DiphotonAndDijetMvaReader(iTree,
                            iBeamEnergy,
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
  switch (fBeamEnergy) {
    case EBeamEnergy::k7TeV:
      fCombiMvaReader->AddVariable("dipho_mva"              , &diphoMVA      );
      fCombiMvaReader->AddVariable("bdt_dijet_7TeV_ptrewght", &dijetMVA      );
      fCombiMvaReader->AddVariable("dipho_pt/mass"          , &ptgg_over_mass);
      break;
    case EBeamEnergy::k8TeV:
      fCombiMvaReader->AddVariable("dipho_mva"        , &diphoMVA      );
      fCombiMvaReader->AddVariable("bdt_dijet_maxdPhi", &dijetMVA      );
      fCombiMvaReader->AddVariable("dipho_pt/mass"    , &ptgg_over_mass);
      break;
    default:
      /// This should never happen!
      cms::Exception exception("BadEnum");
      exception << __FILE__ << ":" << __LINE__ << " in " << __FUNCTION__
                << ": Illegal EBeamEnergy enum value: " << fBeamEnergy;
      throw exception;
  } /// fBeamEnergy 
  fCombiMvaReader->BookMVA("BDTG", fCombiWeights.Data());
} /// Init


//------------------------------------------------------------------------------
void
CombinedMvaReader::Update(void)
{
  DiphotonAndDijetMvaReader::Update();
  combiMVA = fCombiMvaReader->EvaluateMVA("BDTG");
} /// Update
