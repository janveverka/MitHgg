#ifndef MitHgg_PhotonTree_TestTreeFactory_h
#define MitHgg_PhotonTree_TestTreeFactory_h

#include <assert.h>
#include "TFile.h"
#include "TTree.h"

#include "MitHgg/Tools/interface/PathMacros.h"

#define HGG_DEFAULT_TEST_TREE_FILE \
  src/MitHgg/PhotonTree/data/hgg-2013Final8TeV_s12-h120gg-vh-v7n_head10.root
#define HGG_DEFAULT_TEST_TREE_PATH \
  PREPEND_CMSSW_BASE_AND_STRINGIFY(HGG_DEFAULT_TEST_TREE_FILE)
#define HGG_DEFAULT_TEST_TREE_NAME "hPhotonTree"

namespace mithep
{
  namespace hgg
  {
    struct TestTreeFactory
    { 
      static TTree* Create(const char *fname=HGG_DEFAULT_TEST_TREE_PATH,
                           const char *tname=HGG_DEFAULT_TEST_TREE_NAME) 
      {
        TFile *file = TFile::Open(fname);
        assert(file != 0);
        TTree *tree = static_cast<TTree*>(file->Get(tname));
        assert(tree != 0);
        return tree;
      } /// Create      
    }; /// TestTreeFactory
  } /// hgg
} /// mithep

#endif /// MitHgg_PhotonTree_TestTreeFactory_h
