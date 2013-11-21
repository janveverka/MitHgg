#include <iostream>
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
  Int_t numBytesRead = fTree->GetEntry(entry, getall);
  Update();
  return numBytesRead;
} /// GetEntry

//------------------------------------------------------------------------------
void
TreeReader::Update()
{
  /// There are no derived variables to be updated in this class.  The purpose 
  /// of this method is for the derived classes, like for example the 
  /// DiphotonMvaReader, to reimplement it.  We define Update here
  /// here, so that the derived classes don't need to reimplemnt GetEntry.
}