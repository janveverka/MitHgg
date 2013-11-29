// MitHgg/PhotonTree/interface/CombinedMvaReader.h - CombinedMvaReader class
// Reads combined mva weigths given a PhotonTree and path to the weights file.
// Author: J. Veverka
//-----------------------------------------------------------------------------
#ifndef MitHgg_PhotonTree_CombinedMvaReader_h
#define MitHgg_PhotonTree_CombinedMvaReader_h

#include "TMVA/Reader.h"
#include "MitHgg/PhotonTree/interface/DiphotonAndDijetMvaReader.h"
#include "MitHgg/Tools/interface/PathMacros.h"

#define HGG_DEFAULT_COMBINED_WEIGHTS_FILE src/MitHgg/PhotonTree/data/\
TMVA_vbf_dijet_dipho_evenbkg_scaledwt50_maxdPhi_Gradient.weights.xml

#define HGG_DEFAULT_COMBINED_WEIGHTS_PATH \
  PREPEND_CMSSW_BASE_AND_STRINGIFY(HGG_DEFAULT_COMBINED_WEIGHTS_FILE)

namespace mithep
{
  namespace hgg
  {
    class CombinedMvaReader : public DiphotonAndDijetMvaReader
    {
    public:
      CombinedMvaReader(
        TTree      *iTree,
        const char *iDiphoWeights = HGG_DEFAULT_DIPHOTON_WEIGHTS_PATH,
        const char *iDijetWeights = HGG_DEFAULT_DIJET_WEIGHTS_PATH   ,
        const char *iCombiWeights = HGG_DEFAULT_COMBINED_WEIGHTS_PATH,
        bool        iDiphoUseSmearedMassError=true,
        Float_t     iDijetMaxDPhi = 2.916
        );
      virtual ~CombinedMvaReader();

      void        SetCombiMvaWeights(const char *path);
      const char* GetCombiMvaWeights(void) {return fCombiWeights.Data();}

      Float_t combiMVA;

    protected:
      virtual void  Update(void);
      void          Init  (void);

      TString       fCombiWeights;
      TMVA::Reader *fCombiMvaReader;

      ClassDef(CombinedMvaReader, 0)
    }; /// CombinedMvaReader
  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_CombinedMvaReader_h
