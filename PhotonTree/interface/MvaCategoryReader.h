// MitHgg/PhotonTree/interface/MvaCategoryReader.h - MvaCategoryReader class
// Reads combined mva weigths given a PhotonTree and path to the weights file.
// Author: J. Veverka
//-----------------------------------------------------------------------------
#ifndef MitHgg_PhotonTree_MvaCategoryReader_h
#define MitHgg_PhotonTree_MvaCategoryReader_h

#include <vector>
#include "TMVA/Reader.h"
#include "MitHgg/PhotonTree/interface/CombinedMvaReader.h"

namespace mithep
{
  namespace hgg
  {
    //--------------------------------------------------------------------------
    class MvaCategoryReader : public CombinedMvaReader
    {
    public:
      MvaCategoryReader(
        TTree *iTree,
        const char *iCombiWeights = HGG_DEFAULT_COMBINED_WEIGHTS_PATH,
        const char *iDijetWeights = HGG_DEFAULT_DIJET_WEIGHTS_PATH   ,
        const char *iDiphoWeights = HGG_DEFAULT_DIPHOTON_WEIGHTS_PATH,
        Float_t     iDijetMaxDPhi = 2.916
        );
      virtual ~MvaCategoryReader();
      virtual Int_t GetEntry(Long64_t entry = 0, Int_t getall = 0);

      void SetDiphoMvaCuts(const std::vector<float> & cuts);
      void SetDijetMvaCuts(const std::vector<float> & cuts);
      void SetCombiMvaCuts(const std::vector<float> & cuts);
      const std::vector<float>& DiphoMvaCuts() const {return diphoMvaCuts;}

      Int_t VHMetTag;
      Int_t dijetCat;
      Int_t inclCat ;
      Int_t mvaCat  ;

    protected:
      void Init(void);
      void Update(void);
      void UpdateCategoryDefinitions(void);
      void UpdateInclusiveCat(void);
      void UpdateDijetCat(void);
      void UpdateVHMetTag(void);
      void UpdateTTHTagConvention(void);
      void UpdateVHHadTagConvention(void);
      void UpdateMvaCat(void);
      static double DeltaR(double eta1, double phi1, double eta2, double phi2);
      static double DeltaPhi(double phi1, double phi2);

      std::vector<float> diphoMvaCuts;
      std::vector<float> dijetMvaCuts;
      std::vector<float> combiMvaCuts;

      int numInclCats;
      int numDijetCats;

      /// Category labels
      Int_t kIncl0, kDijet0;
      Int_t kVHLepTight, kVHLepLoose, kVHMet;
      Int_t kTTHLep, kTTHHad, kVHHad;

      ClassDef(MvaCategoryReader, 0)
    }; /// MvaCategoryReader


    //--------------------------------------------------------------------------
    namespace mva_category_defaults_8tev_jim
    {
      const unsigned int numInclCats  = 5;
      const unsigned int numDijetCats = 3;

      const float initDiphoMvaCuts[numInclCats ] = {0.915, 0.75, 0.52, 0.13,
                                                   -0.4};
      const float initDijetMvaCuts[numDijetCats] = {0.64 , 0.64, 0.47};
      const float initCombiMvaCuts[numDijetCats] = {0.906, 0.74, 0.5 };

      const std::vector<float> diphoMvaCuts(initDiphoMvaCuts,
                                            initDiphoMvaCuts + numInclCats );
      const std::vector<float> dijetMvaCuts(initDijetMvaCuts,
                                            initDijetMvaCuts + numDijetCats);
      const std::vector<float> combiMvaCuts(initCombiMvaCuts,
                                            initCombiMvaCuts + numDijetCats);
    } /// mva_category_defaults_8tev_jim


    //--------------------------------------------------------------------------
    namespace mva_category_defaults_8tev = mva_category_defaults_8tev_jim;


  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_MvaCategoryReader_h
