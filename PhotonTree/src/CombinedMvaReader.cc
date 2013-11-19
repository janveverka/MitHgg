#include <iostream>     // std::cout
#include "MitHgg/PhotonTree/interface/CombinedMvaReader.h"

using ::mithep::hgg::CombinedMvaReader;

/// Make this a ROOT class
ClassImp(CombinedMvaReader)


//------------------------------------------------------------------------------
CombinedMvaReader::CombinedMvaReader(TTree *iTree,
                                     const char *iCombinedWeights,
                                     const char *iDijetWeights   ,
                                     const char *iDiphotonWeights,
                                     Float_t iDijetMaxDPhi) :
  TreeReader        (iTree                              ),
  DijetMvaReader    (iTree, iDijetWeights, iDijetMaxDPhi),
  DiphotonMvaReader (iTree, iDiphotonWeights            ),
  fCombinedWeights  (iCombinedWeights                   ),
  fCombinedMvaReader(new TMVA::Reader()                 )
{
  Init();
} /// Ctor


//------------------------------------------------------------------------------
CombinedMvaReader::~CombinedMvaReader()
{
  delete fCombinedMvaReader;
} /// Dtor


//------------------------------------------------------------------------------
Int_t
CombinedMvaReader::GetEntry(Long64_t entry, Int_t getall)
{
  Int_t bytesRead = fTree->GetEntry(entry, getall);
  DiphotonMvaReader::Update();
  DijetMvaReader   ::Update();
  CombinedMvaReader::Update();
  return bytesRead;
} /// GetEntry


//------------------------------------------------------------------------------
void
CombinedMvaReader::Init()
{
  fCombinedMvaReader->AddVariable("dipho_mva"        , &diphoMVA      );
  fCombinedMvaReader->AddVariable("bdt_dijet_maxdPhi", &dijetMVA      );
  fCombinedMvaReader->AddVariable("dipho_pt/mass"    , &ptgg_over_mass);

  std::cout << "\nINFO: mithep::hgg::CombinedMvaReader::Init(): "
            << "Booking TMVA Reader ...\n";
  fCombinedMvaReader->BookMVA("BDTG", fCombinedWeights.Data());
} /// Init


//------------------------------------------------------------------------------
void
CombinedMvaReader::Update(void)
{
  combinedMVA = fCombinedMvaReader->EvaluateMVA("BDTG");
} /// Update
