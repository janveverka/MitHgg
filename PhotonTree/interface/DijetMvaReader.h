// MitHgg/PhotonTree/interface/DijetMvaReader.h - DijetMvaReader class
// Reads dijet mva weigths given a PhotonTree and path to the weights file.
// Author: J. Veverka
//-----------------------------------------------------------------------------

#ifndef MitHgg_PhotonTree_DijetMvaReader_h
#define MitHgg_PhotonTree_DijetMvaReader_h

#include "TMVA/Reader.h"
#include "MitHgg/PhotonTree/interface/TreeReader.h"
#include "MitHgg/Tools/interface/PathMacros.h"

#define HGG_DEFAULT_DIJET_WEIGHTS_FILE src/MitHgg/PhotonTree/data/\
TMVA_dijet_sherpa_scalewt50_2evenb_powheg200_maxdPhi_oct9_Gradient.weights.xml

#define HGG_DEFAULT_DIJET_WEIGHTS_PATH \
  PREPEND_CMSSW_BASE_AND_STRINGIFY(HGG_DEFAULT_DIJET_WEIGHTS_FILE)

namespace mithep
{
  namespace hgg
  {
    class DijetMvaReader : virtual public TreeReader
    {
    public:
      DijetMvaReader(TTree *iTree,
                     const char *iWeights=HGG_DEFAULT_DIJET_WEIGHTS_PATH,
                     Float_t iMaxDPhi=2.916);
      virtual ~DijetMvaReader();
      virtual Int_t GetEntry(Long64_t entry = 0, Int_t getall = 0);

      Float_t dijet_DPhiTruncated;
      Float_t ptgg_over_mass;
      Float_t dijetMVA;

    protected:
      void Update(void);
      void Init(void);
      TString const fDijetWeights;
      Float_t fMaxDPhi;
      TMVA::Reader *fDijetMvaReader;
      ClassDef(DijetMvaReader, 0)
    }; /// DijetMvaReader
  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_DijetMvaReader_h
