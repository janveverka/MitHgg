#ifndef MitHgg_PhotonTree_MvaCategoryDumper_h
#define MitHgg_PhotonTree_MvaCategoryDumper_h

#include "MitHgg/Tools/interface/PSet.h"
#include "MitHgg/PhotonTree/interface/MvaCategoryReader.h"

namespace mithep
{
  namespace hgg
  {
    //--------------------------------------------------------------------------
    class MvaCategoryDumper : public MvaCategoryReader
    {
      public:
        MvaCategoryDumper(TTree *iTree, const PSet &iConfig);
        virtual ~MvaCategoryDumper();
        void ProduceDump();
      protected:
        void Init(const PSet &iConfig);
        bool PassPreselection(void);
        void DumpAllVariables(void);
        void DumpEventHeader(void);
        void DumpPhotons(void);
        void DumpCategoryVariables(void);
        void DumpDiphotonVariables(void);
        void DumpElectrons(void);
        void DumpMuons(void);
        void DumpJets(void);
        template <class T>
        void DumpVar(const char *name, T value, const char *suffix="\t");
    }; // MvaCategoryDumper
  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_MvaCategoryDumper_h
