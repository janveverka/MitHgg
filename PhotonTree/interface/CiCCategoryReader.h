// MitHgg/PhotonTree/interface/CiCCategoryReader.h - CiCCategoryReader class
// Reads the photon tree and the CiC categories.
// Author: J. Veverka
//-----------------------------------------------------------------------------
#ifndef MitHgg_PhotonTree_CiCCategoryReader_h
#define MitHgg_PhotonTree_CiCCategoryReader_h

#include "MitHgg/PhotonTree/interface/TreeReader.h"

namespace mithep
{
  namespace hgg
  {
    //--------------------------------------------------------------------------
    class CiCCategoryReader : public TreeReader
    {
    public:
      enum EBeamEnergy {k7TeV, k8TeV};
      
      CiCCategoryReader(
        TTree      *iTree                                ,
        EBeamEnergy iBeamEnergy      = EBeamEnergy::k8TeV
        );
      virtual ~CiCCategoryReader();

      double massOver120;

      Int_t VHMetTag;
      Int_t dijetCat;
      Int_t inclCat ;
      Int_t cicCat  ;

    protected:
      void         Init                         (void);
      virtual void Update                       (void);
      void         UpdateCategoryDefinitions    (void);
      void         UpdateCategoryDefinitions7TeV(void);
      void         UpdateCategoryDefinitions8TeV(void);
      void         UpdateInclusiveCat           (void);
      void         UpdateDijetCat               (void);
      void         UpdateVHLepTag               (void);
      void         UpdateVHMetTag               (void);
      void         UpdateTTHTag                 (void);
      void         UpdateVHHadTag               (void);
      void         UpdateCiCCat                 (void);
      bool         PassesCommonVHMetTagCuts     (void);
      bool         PassesPreselection           (void);

      static const unsigned int kNumInclCats;
      static const unsigned int kNumDijetCats;

      EBeamEnergy fBeamEnergy;
      /// Category labels
      Int_t kIncl0, kDijet0;
      Int_t kVHLepTight, kVHLepLoose, kVHMet;
      Int_t kTTHLep, kTTHHad, kVHHad;

      ClassDef(CiCCategoryReader, 0)
    }; /// CiCCategoryReader

  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_CiCCategoryReader_h
