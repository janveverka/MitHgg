// MitHgg/PhotonTree/interface/TreeReader.h - TreeReader class
// Holds a pointer to the photon tree. Derives from DiphotonEventReader 
// and holds two PhotonReader instances - one for each photon.
// Author: J. Veverka
//-----------------------------------------------------------------------------
#ifndef MitHgg_PhotonTree_TreeReader_h
#define MitHgg_PhotonTree_TreeReader_h

#include "TTree.h"
#include "MitHgg/PhotonTree/interface/PhotonReader.h"
#include "MitHgg/PhotonTree/interface/DiphotonEventReader.h"

namespace mithep
{
  namespace hgg
  {
    class TreeReader : public DiphotonEventReader
    {
      public:
        TreeReader (TTree *iTree);
        virtual ~TreeReader() {}

        virtual Int_t GetEntry(Long64_t entry = 0, Int_t getall = 0);
        virtual Long64_t GetEntries() const {return fTree->GetEntries();}

        PhotonReader ph1;
        PhotonReader ph2;

      protected:
        TTree *fTree;
        ClassDef(TreeReader, 0)
    }; /// TreeReader
  } /// hgg
} /// mithep

#endif /// MitHgg_PhotonTree_DiphotonEventReader_h
