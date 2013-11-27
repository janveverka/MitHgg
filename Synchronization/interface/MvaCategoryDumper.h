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
        void produceDump();
      protected:
        void dumpAllVariables(void);
        void dumpEventHeader(void);
        void dumpCategoryVariables(void);
        template <class T>
        void dumpVar(const char *name, T value, const char *suffix="\t");
    }; // MvaCategoryDumper
  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_MvaCategoryDumper_h
