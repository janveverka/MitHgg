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
      typedef std::vector<double> vdouble;
      MvaCategoryReader(
        TTree      *iTree                                               ,
        EBeamEnergy iBeamEnergy      = EBeamEnergy::k8TeV               ,
        const char *iDiphoWeights    = HGG_DEFAULT_DIPHOTON_WEIGHTS_PATH,
        const char *iDijetWeights    = HGG_DEFAULT_DIJET_WEIGHTS_PATH   ,
        const char *iCombiWeights    = HGG_DEFAULT_COMBINED_WEIGHTS_PATH,
        bool        iDiphoUseSmearedMassError = true                    ,
        Float_t     iDijetMaxDPhi    = 2.916                            ,
        const char *iDiphoTmvaOption = "Silent"                         ,
        const char *iDijetTmvaOption = "Silent"                         ,
        const char *iCombiTmvaOption = "Silent"
        );
      virtual ~MvaCategoryReader();

      void SetDiphoMvaCuts(const vdouble & cuts);
      void SetDijetMvaCuts(const vdouble & cuts);
      void SetCombiMvaCuts(const vdouble & cuts);
      const vdouble& DiphoMvaCuts() const {return diphoMvaCuts;}
      const vdouble& DijetMvaCuts() const {return dijetMvaCuts;}
      const vdouble& CombiMvaCuts() const {return combiMvaCuts;}

      double massOver120;
      Int_t VHMetTag;
      Int_t dijetCat;
      Int_t inclCat ;
      Int_t mvaCat  ;

    protected:
      void         Init                         (void                   );
      virtual void Update                       (void                   );
      void         Update7TeV                   (void                   );
      void         Update8TeV                   (void                   );
      void         UpdateCategoryDefinitions    (void                   );
      void         UpdateCategoryDefinitions7TeV(void                   );
      void         UpdateCategoryDefinitions8TeV(void                   );
      void         UpdateInclusiveCat           (void                   );
      void         UpdateDijetCat               (void                   );
      void         UpdateVHLepTag               (double minDiphoMVALoose,
                                                 double minDiphoMVATight);
      void         UpdateVHMetTag               (double minDiphoMVA     );
      void         UpdateTTHTag                 (double minDiphoMVALep  ,
                                                 double minDiphoMVAHad  );
      void         UpdateVHHadTag               (double minDiphoMVA     );
      void         UpdateMvaCat                 (void                   );
      bool         PassesCommonVHMetTagCuts     (void                   );
      bool         PassesPreselection           (void                   );

      vdouble diphoMvaCuts;
      vdouble dijetMvaCuts;
      vdouble combiMvaCuts;

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

      const double initDiphoMvaCuts[numInclCats ] = {0.915, 0.75, 0.52, 0.13,
                                                   -0.4};
      const double initDijetMvaCuts[numDijetCats] = {0.64 , 0.64, 0.47};
      const double initCombiMvaCuts[numDijetCats] = {0.906, 0.74, 0.5 };

      const std::vector<double> diphoMvaCuts(initDiphoMvaCuts,
                                             initDiphoMvaCuts + numInclCats );
      const std::vector<double> dijetMvaCuts(initDijetMvaCuts,
                                             initDijetMvaCuts + numDijetCats);
      const std::vector<double> combiMvaCuts(initCombiMvaCuts,
                                             initCombiMvaCuts + numDijetCats);
    } /// mva_category_defaults_8tev_jim


    //--------------------------------------------------------------------------
    namespace mva_category_defaults_8tev = mva_category_defaults_8tev_jim;


  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_MvaCategoryReader_h
