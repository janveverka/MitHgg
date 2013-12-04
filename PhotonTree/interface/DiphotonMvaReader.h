// MitHgg/PhotonTree/interface/DiphotonMvaReader.h - DiphotonMvaReader class
// Reads diphoton mva weigths given a PhotonTree and path to the weights file.
// Author: J. Veverka
//-----------------------------------------------------------------------------

#ifndef MitHgg_PhotonTree_DiphotonMvaReader_h
#define MitHgg_PhotonTree_DiphotonMvaReader_h

#include "TMVA/Reader.h"
#include "MitHgg/PhotonTree/interface/TreeReader.h"
#include "MitHgg/Tools/interface/PathMacros.h"
// #include "MitHgg/Tools/interface/PSet.h"

#define HGG_DEFAULT_DIPHOTON_WEIGHTS_FILE src/MitPhysics/data/\
HggBambu_SMDipho_Oct01_redqcdweightallsigevenbkg_BDTG.weights.xml

#define HGG_DEFAULT_DIPHOTON_WEIGHTS_PATH \
  PREPEND_CMSSW_BASE_AND_STRINGIFY(HGG_DEFAULT_DIPHOTON_WEIGHTS_FILE)

namespace mithep
{
  namespace hgg
  {
    //--------------------------------------------------------------------------
    class DiphotonMvaReader : public TreeReader
    {
      public:
        enum EBeamEnergy {k7TeV, k8TeV};
        
        DiphotonMvaReader (
          TTree       *tree,
          EBeamEnergy  iBeamEnergy=EBeamEnergy::k8TeV,
          const char  *iDiphoWeights=HGG_DEFAULT_DIPHOTON_WEIGHTS_PATH,
          bool         iDiphoUseSmearedMassError=true,
          const char  *iDiphoTmvaOptions="Silent"
        );
        virtual ~DiphotonMvaReader();

        void SetDiphoMvaUseSmearedMassError(bool b) 
        {
          fDiphoUseSmearedMassError = b;
        }      
        void SetDiphoMvaWeigths(const char *path);

        bool GetDiphoMvaUseSmearedMassError(void)
        {
          return fDiphoUseSmearedMassError;
        }
        const char* GetDiphoMvaWeights(void) {return fDiphoWeights.Data();}
        EBeamEnergy GetBeamEnergy(void) {return fBeamEnergy;}

        Float_t rVtxSigmaMoM;
        Float_t wVtxSigmaMoM;
        Float_t cosDPhi     ;
        Float_t pho1_ptOverM;
        Float_t pho2_ptOverM;
        Float_t diphoMVA    ;

      protected:
        virtual void Update(void);
                void Init  (void);
        EBeamEnergy   fBeamEnergy;
        TString       fDiphoWeights;
        bool          fDiphoUseSmearedMassError;
        TString       fDiphoTmvaOption;
        TMVA::Reader *fDiphoMvaReader;
        ClassDef(DiphotonMvaReader, 0)
    }; /// DiphotonMvaReader
        
  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_DiphotonMvaReader_h
