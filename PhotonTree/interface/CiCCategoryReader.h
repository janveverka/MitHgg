// MitHgg/PhotonTree/interface/CiCCategoryReader.h - CiCCategoryReader class
// Reads the photon tree and the CiC categories.
// Author: J. Veverka
//-----------------------------------------------------------------------------
#ifndef MitHgg_PhotonTree_CiCCategoryReader_h
#define MitHgg_PhotonTree_CiCCategoryReader_h

#include <string>
#include "MitHgg/PhotonTree/interface/TreeReader.h"
#include "MitHgg/Tools/interface/EventFilterFromListStandAlone.h"

namespace mithep
{
  namespace hgg
  {
    //--------------------------------------------------------------------------
    class CiCCategoryReader : public TreeReader
    {
    public:
      enum EBeamEnergy         {k7TeV  , k8TeV};
      enum EEcalFiducialRegion {eBarrel, eEndcaps, eGap};
      
      CiCCategoryReader(
        TTree      *iTree                                ,
        EBeamEnergy iBeamEnergy      = EBeamEnergy::k8TeV,
        const char *eventsToSkip     = ""
        );
      virtual ~CiCCategoryReader();

      double massOver120;

      Int_t VHMetTag;
      Int_t dijetCat;
      Int_t inclCat ;
      Int_t cicCat  ;
      EEcalFiducialRegion ph1_ecalRegion;
      EEcalFiducialRegion ph2_ecalRegion;

    protected:
      void         Init                         (void);
      EEcalFiducialRegion GetEcalFiducialRegion (float scEta);
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
      void         UpdateCiCCat7TeV             (void);
      void         UpdateCiCCat8TeV             (void);
      bool         PassesCommonVHMetTagCuts     (void);
      bool         PassesPreselection           (void);

      static const unsigned int kNumInclCats;
      static const unsigned int kNumDijetCats;

      EBeamEnergy fBeamEnergy;
      /// Category labels
      Int_t kIncl0, kDijet0;
      Int_t kVHLepTight, kVHLepLoose, kVHMet;
      Int_t kTTHLep, kTTHHad, kVHHad;

      std::string fEventsToSkip;
      EventFilterFromListStandAlone *fEventFilter;

      ClassDef(CiCCategoryReader, 0)
    }; /// CiCCategoryReader

  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_CiCCategoryReader_h
