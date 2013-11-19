#include "MitHgg/PhotonTree/interface/TreeReader.h"
using ::mithep::hgg::TreeReader;
ClassImp(TreeReader)

//------------------------------------------------------------------------------
TreeReader::TreeReader(TTree *iTree) : 
  DiphotonEventReader(iTree        ), 
  ph1                (iTree, "ph1."),
  ph2                (iTree, "ph2."),
  fTree              (iTree        ) 
{} /// Ctor


//------------------------------------------------------------------------------
Int_t
TreeReader::GetEntry(Long64_t entry, Int_t getall)
{
  return fTree->GetEntry(entry, getall);  
} /// GetEntry
