#include <iostream>
#include "MitHgg/PhotonTree/interface/TreeReader.h"
using ::mithep::hgg::TreeReader;
using ::mithep::hgg::TreeReaderConfiguration;
ClassImp(TreeReader)
ClassImp(TreeReaderConfiguration)

//------------------------------------------------------------------------------
TreeReader::TreeReader(TTree *iTree) : 
  DiphotonEventReader(iTree        ), 
  ph1                (iTree, "ph1."),
  ph2                (iTree, "ph2."),
  fTree              (iTree        ) 
{} /// Ctor


//------------------------------------------------------------------------------
TreeReader::TreeReader(const Configuration &iConfig) : 
  DiphotonEventReader(iConfig.tree        ), 
  ph1                (iConfig.tree, "ph1."),
  ph2                (iConfig.tree, "ph2."),
  fTree              (iConfig.tree        ) 
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