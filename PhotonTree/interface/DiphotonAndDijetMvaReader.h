// MitHgg/PhotonTree/interface/DiphotonAndDijetMvaReader.h - DiphotonAndDijetMvaReader class
// Reads dijet MVA weigths given a PhotonTree and path to the weights file.
// Author: J. Veverka
//-----------------------------------------------------------------------------

#ifndef MitHgg_PhotonTree_DiphotonAndDijetMvaReader_h
#define MitHgg_PhotonTree_DiphotonAndDijetMvaReader_h

#include "TMVA/Reader.h"
#include "MitHgg/PhotonTree/interface/DiphotonMvaReader.h"
#include "MitHgg/Tools/interface/PathMacros.h"

#define HGG_DEFAULT_DIJET_WEIGHTS_FILE src/MitHgg/PhotonTree/data/\
TMVA_dijet_sherpa_scalewt50_2evenb_powheg200_maxdPhi_oct9_Gradient.weights.xml

#define HGG_DEFAULT_DIJET_WEIGHTS_PATH \
  PREPEND_CMSSW_BASE_AND_STRINGIFY(HGG_DEFAULT_DIJET_WEIGHTS_FILE)

namespace mithep
{
  namespace hgg
  {
    class DiphotonAndDijetMvaReader : public ::mithep::hgg::DiphotonMvaReader
    {
    public:
      DiphotonAndDijetMvaReader(
        TTree      *iTree,
        EBeamEnergy iBeamEnergy=EBeamEnergy::k8TeV,
        const char *iDiphoWeights=HGG_DEFAULT_DIPHOTON_WEIGHTS_PATH,
        const char *iDijetWeights=HGG_DEFAULT_DIJET_WEIGHTS_PATH,
        bool        iDiphoUseSmearedMassError=true,
        Float_t     iDijetMaxDPhi=2.916);
      virtual ~DiphotonAndDijetMvaReader();
      
      void SetDijetMvaWeights(const char *path);
      void SetDijetMvaMaxDPhi(Float_t iMaxDPhi) {fDijetMaxDPhi = iMaxDPhi;}
      
      const char *GetDijetMvaWeights(void) {return fDijetWeights.Data();}
      Float_t     GetDijetMvaMaxDPhi(void) {return fDijetMaxDPhi       ;}

      Float_t dijet_DPhiTruncated;
      Float_t ptgg_over_mass;
      Float_t dijetMVA;

    protected:
      virtual void Update(void);
              void Init  (void);
      TString  fDijetWeights;
      Float_t       fDijetMaxDPhi;
      TMVA::Reader *fDijetMvaReader;
      ClassDef(DiphotonAndDijetMvaReader, 0)
    }; /// DiphotonAndDijetMvaReader
  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_DiphotonAndDijetMvaReader_h
