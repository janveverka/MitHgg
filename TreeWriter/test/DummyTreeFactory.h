#ifndef MITHGG_TREEWRITER_TEST_DUMMYTREEFACTORY_H
#define MITHGG_TREEWRITER_TEST_DUMMYTREEFACTORY_H

#include "TFile.h"
#include "TTree.h"

#include "MitHgg/TreeWriter/interface/PathMacros.h"

#define HGG_DEFAULT_DUMMY_TREE_FILE \
  src/MitHgg/TreeWriter/data/hgg-2013Final8TeV_s12-h120gg-vh-v7n_head10.root
#define HGG_DEFAULT_DUMMY_TREE_PATH \
  PREPEND_CMSSW_BASE_AND_STRINGIFY(HGG_DEFAULT_DUMMY_TREE_FILE)
#define HGG_DEFAULT_DUMMY_TREE_NAME "hPhotonTree"

namespace mithep
{
  namespace hgg
  {
    struct DummyTreeFactory
    { 
      static TTree* Create(const char *fname=HGG_DEFAULT_DUMMY_TREE_PATH,
                           const char *tname=HGG_DEFAULT_DUMMY_TREE_NAME) {
        TFile *file = TFile::Open(fname);
        return static_cast<TTree*>(file->Get(tname));
      } // Create      
    }; // DummyTreeFactory
  } // hgg
} // mithep

#endif // MITHGG_TREEWRITER_TEST_DUMMYTREEFACTORY_H
