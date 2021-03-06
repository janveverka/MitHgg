#ifndef MitHgg_PhotonTree_MvaCategoryDumper_h
#define MitHgg_PhotonTree_MvaCategoryDumper_h

#include <algorithm> // std::abs
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
        static EBeamEnergy GetBeamEnergy(const PSet &iConfig);
        static std::string GetEventsToSkip(const PSet &iConfig);
        void Init(const PSet &iConfig);
        bool PassDijetPreselection(void);
        void DumpAllVariables(void);
        void DumpEventHeader(void);
        void DumpPhotons(void);
        void DumpCategoryVariables(void);
        void DumpDiphotonVariables(void);
        void DumpVertexVariables(void);
        void DumpDiphotonMvaInputs(void);
        void DumpElectrons(void);
        void DumpMuons(void);
        void DumpJets(void);
        void DumpDijetVariables(void);
        void DumpPhoton(const char *prefix, PhotonReader &photon);
        void DumpPhotonIdMvaInputs(const char *prefix, PhotonReader &photon);
        bool IsUnset(float variable) {return std::abs(variable + 99) < 1e-4;}
        template <class T>
        void DumpVar(const char *name, T value, const char *suffix="\t");
        template <class T>
        void DumpVar(const char *prefix, const char *name, T value, 
                     const char *suffix="\t");
        int fMaxEntries;
    }; // MvaCategoryDumper
  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_MvaCategoryDumper_h
