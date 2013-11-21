// MitHgg/PhotonTree/interface/DiphotonMvaReader.h - DiphotonMvaReader class
// Reads diphoton mva weigths given a PhotonTree and path to the weights file.
// Author: J. Veverka
//-----------------------------------------------------------------------------

#ifndef MitHgg_PhotonTree_DiphotonMvaReader_h
#define MitHgg_PhotonTree_DiphotonMvaReader_h

#include "TMVA/Reader.h"
#include "MitHgg/PhotonTree/interface/TreeReader.h"
#include "MitHgg/Tools/interface/PathMacros.h"

#define HGG_DEFAULT_DIPHOTON_WEIGHTS_FILE src/MitPhysics/data/\
HggBambu_SMDipho_Oct01_redqcdweightallsigevenbkg_BDTG.weights.xml

#define HGG_DEFAULT_DIPHOTON_WEIGHTS_PATH \
  PREPEND_CMSSW_BASE_AND_STRINGIFY(HGG_DEFAULT_DIPHOTON_WEIGHTS_FILE)

namespace mithep
{
  namespace hgg
  {
    class DiphotonMvaReader : public TreeReader
    {
    public:
      DiphotonMvaReader (TTree *tree,
                         const char *weights=HGG_DEFAULT_DIPHOTON_WEIGHTS_PATH,
                         bool useSmearedMassError=true);
      virtual ~DiphotonMvaReader();

      Float_t rVtxSigmaMoM;
      Float_t wVtxSigmaMoM;
      Float_t cosDPhi     ;
      Float_t pho1_ptOverM;
      Float_t pho2_ptOverM;
      Float_t diphoMVA    ;

    protected:
      virtual void Update(void);
      void Init(void);
      TString const fDiphotonWeights;
      bool fUseSmearedMassError;
      TMVA::Reader *fDiphotonMvaReader;
      ClassDef(DiphotonMvaReader, 0)
    }; /// DiphotonMvaReader
  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_DiphotonMvaReader_h
