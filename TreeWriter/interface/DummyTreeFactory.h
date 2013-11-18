#ifndef MITHGG_TREEWRITER_TEST_DUMMYTREEFACTORY_H
#define MITHGG_TREEWRITER_TEST_DUMMYTREEFACTORY_H

#ifndef CMSSW_BASE
#define CMSSW_BASE /home/veverka/cms/cmssw/031/CMSSW_5_3_10_patch1
#endif // CMSSW_BASE

#define DEFAULT_FILE_NAME src/MitHgg/TreeWriter/data/hgg-2013Final8TeV_s12-h120gg-vh-v7n_head10.root

namespace mithep
{
  namespace hgg
  {
    struct DummyTreeFactory
    { 
      static TTree* Create(
        const char *fname="CMSSW_BASE/DEFAULT_FILE_NAME",
        const char *tname="hPhotonTree"
      );      
    }; // DummyTreeFactory
  } // hgg
} // mithep

#endif // MITHGG_TREEWRITER_TEST_DUMMYTREEFACTORY_H
