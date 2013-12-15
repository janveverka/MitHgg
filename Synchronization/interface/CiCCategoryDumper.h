#ifndef MitHgg_PhotonTree_CiCCategoryDumper_h
#define MitHgg_PhotonTree_CiCCategoryDumper_h

#include <algorithm> // std::abs
#include "MitHgg/Tools/interface/PSet.h"
#include "MitHgg/PhotonTree/interface/CiCCategoryReader.h"

namespace mithep
{
  namespace hgg
  {
    //--------------------------------------------------------------------------
    class CiCCategoryDumper : public CiCCategoryReader
    {
      public:
        CiCCategoryDumper(TTree *iTree, const PSet &iConfig);
        virtual ~CiCCategoryDumper();
        void ProduceDump();
      protected:
        static EBeamEnergy GetBeamEnergy(const PSet &iConfig);
        void Init(const PSet &iConfig);
        bool PassDijetPreselection(void);
        void DumpAllVariables(void);
        void DumpEventHeader(void);
        void DumpPhotons(void);
        void DumpCategoryVariables(void);
        void DumpDiphotonVariables(void);
        void DumpVertexVariables(void);
        void DumpElectrons(void);
        void DumpMuons(void);
        void DumpJets(void);
        void DumpDijetVariables(void);
        void DumpPhoton(const char *prefix, PhotonReader &photon);
        bool IsUnset(float variable) {return std::abs(variable + 99) < 1e-4;}
        template <class T>
        void DumpVar(const char *name, T value, const char *suffix="\t");
        template <class T>
        void DumpVar(const char *prefix, const char *name, T value, 
                     const char *suffix="\t");
        int fMaxEntries;
    }; // CiCCategoryDumper
  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_CiCCategoryDumper_h
