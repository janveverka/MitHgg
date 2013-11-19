// MitHgg/PhotonTree/interface/MvaCategoryReader.h - MvaCategoryReader class
// Reads combined mva weigths given a PhotonTree and path to the weights file.
// Author: J. Veverka
//-----------------------------------------------------------------------------
#ifndef MitHgg_PhotonTree_MvaCategoryReader_h
#define MitHgg_PhotonTree_MvaCategoryReader_h

#include "TMVA/Reader.h"
#include "MitHgg/PhotonTree/interface/CombinedMvaReader.h"

namespace mithep
{
  namespace hgg
  {
    class MvaCategoryReader : public CombinedMvaReader
    {
    public:
      enum Category {kIncl0      = 0, kIncl1, kIncl2, kIncl3, kIncl4,
                     kDijet0     = 5, kDijet1, kDijet2,
                     kVHLepTight = 8, kVHLepLoose,
                     kVHMet      = 10,
                     kTTHLep     = 11, kTTHHad,
                     kVHHad      = 13};

      MvaCategoryReader(
        TTree *iTree,
        const char *iCombinedWeights = HGG_DEFAULT_COMBINED_WEIGHTS_PATH,
        const char *iDijetWeights    = HGG_DEFAULT_DIJET_WEIGHTS_PATH   ,
        const char *iDiphotonWeights = HGG_DEFAULT_DIPHOTON_WEIGHTS_PATH,
        Float_t iDijetMaxDPhi = 2.916
        );
      virtual ~MvaCategoryReader();
      virtual Int_t GetEntry(Long64_t entry = 0, Int_t getall = 0);

      Int_t    VHMetTag;
      Category mvaCat  ;

    protected:
      void Update(void);
      void Init(void);
      void UpdateTTHTagConvention(void);
      void UpdateVHHadTagConvention(void);
      void UpdateVHMetTag(void);
      static double DeltaR(double eta1, double phi1, double eta2, double phi2);
      static double DeltaPhi(double phi1, double phi2);
      ClassDef(MvaCategoryReader, 0)
    }; /// MvaCategoryReader
  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_MvaCategoryReader_h
