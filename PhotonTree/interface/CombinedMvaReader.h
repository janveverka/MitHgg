// MitHgg/PhotonTree/interface/CombinedMvaReader.h - CombinedMvaReader class
// Reads combined mva weigths given a PhotonTree and path to the weights file.
// Author: J. Veverka
//-----------------------------------------------------------------------------
/**
  <Variables NVar="3">
    <Variable VarIndex="0" Expression="dipho_mva"         ... />
    <Variable VarIndex="1" Expression="bdt_dijet_maxdPhi" ... />
    <Variable VarIndex="2" Expression="dipho_pt/mass"     ... />
  </Variables>
  diphoMVA
  dijetMVA
  ptgg_over_mass
*/

#ifndef MitHgg_PhotonTree_CombinedMvaReader_h
#define MitHgg_PhotonTree_CombinedMvaReader_h

#include "TMVA/Reader.h"
#include "MitHgg/PhotonTree/interface/DiphotonMvaReader.h"
#include "MitHgg/PhotonTree/interface/DijetMvaReader.h"
#include "MitHgg/Tools/interface/PathMacros.h"

#define HGG_DEFAULT_COMBINED_WEIGHTS_FILE src/MitHgg/PhotonTree/data/\
TMVA_vbf_dijet_dipho_evenbkg_scaledwt50_maxdPhi_Gradient.weights.xml

#define HGG_DEFAULT_COMBINED_WEIGHTS_PATH \
  PREPEND_CMSSW_BASE_AND_STRINGIFY(HGG_DEFAULT_COMBINED_WEIGHTS_FILE)

namespace mithep
{
  namespace hgg
  {
    class CombinedMvaReader : virtual public DijetMvaReader,
                              virtual public DiphotonMvaReader
    {
    public:
      CombinedMvaReader(
        TTree *iTree,
        const char *iCombinedWeights = HGG_DEFAULT_COMBINED_WEIGHTS_PATH,
        const char *iDijetWeights    = HGG_DEFAULT_DIJET_WEIGHTS_PATH   ,
        const char *iDiphotonWeights = HGG_DEFAULT_DIPHOTON_WEIGHTS_PATH,
        Float_t iDijetMaxDPhi = 2.916
        );
      virtual ~CombinedMvaReader();
      virtual Int_t GetEntry(Long64_t entry = 0, Int_t getall = 0);

      Float_t combinedMVA;

    protected:
      void Update(void);
      void Init(void);
      TString const fCombinedWeights;
      TMVA::Reader *fCombinedMvaReader;
      ClassDef(CombinedMvaReader, 0)
    }; /// CombinedMvaReader
  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_CombinedMvaReader_h
