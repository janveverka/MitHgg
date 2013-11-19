// MitHgg/PhotonTree/interface/DijetMvaReader.h - DijetMvaReader class
// Reads dijet mva weigths given a PhotonTree and path to the weights file.
// Author: J. Veverka
//-----------------------------------------------------------------------------
/**
<Variables NVar="8">
    <Variable VarIndex="0" Expression="dijet_leadEta" Label="dijet_leadEta" Title="dijet_leadEta" Unit="" Internal="dijet_leadEta" Type="F" Min="-4.69994450e+00" Max="4.69988489e+00"/>
    jet1eta
    <Variable VarIndex="1" Expression="dijet_subleadEta" Label="dijet_subleadEta" Title="dijet_subleadEta" Unit="" Internal="dijet_subleadEta" Type="F" Min="-4.69991112e+00" Max="4.69987011e+00"/>
    jet2eta
    <Variable VarIndex="2" Expression="dijet_LeadJPt" Label="dijet_LeadJPt" Title="dijet_LeadJPt" Unit="" Internal="dijet_LeadJPt" Type="F" Min="1.50000381e+01" Max="1.15414673e+03"/>
    jet1pt
    <Variable VarIndex="3" Expression="dijet_SubJPt" Label="dijet_SubJPt" Title="dijet_SubJPt" Unit="" Internal="dijet_SubJPt" Type="F" Min="1.00000286e+01" Max="8.05402710e+02"/>
    jet2pt
    <Variable VarIndex="4" Expression="dijet_Zep" Label="dijet_Zep" Title="dijet_Zep" Unit="" Internal="dijet_Zep" Type="F" Min="9.23505286e-06" Max="1.21754856e+01"/>
    zeppenfeld
    <Variable VarIndex="5" Expression="min(dijet_dPhi,2.916)" Label="min(dijet_dPhi,2.916)" Title="min(dijet_dPhi,2.916)" Unit="" Internal="min_dijet_dPhi,2.916_" Type="F" Min="1.04706294e-06" Max="2.91599989e+00"/>
    <Variable VarIndex="6" Expression="dijet_Mjj" Label="dijet_Mjj" Title="dijet_Mjj" Unit="" Internal="dijet_Mjj" Type="F" Min="8.30823040e+00" Max="4.62011377e+03"/>
    dijetmass
    <Variable VarIndex="7" Expression="dipho_pt/mass" Label="dipho_pt/mass" Title="dipho_pt/mass" Unit="" Internal="dipho_pt_D_mass" Type="F" Min="5.85410511e-04" Max="8.18329144e+00"/>
    ptgg
  </Variables>
jet1eta
jet2eta
jet1pt
jet2pt
zeppenfeld
dijetmass
ptgg
*/

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
      TMVA::Reader* MvaReader() {return fMvaReader;}

      Float_t dijet_DPhiTruncated;
      Float_t ptgg_over_mass;
      Float_t dijetMVA;

    protected:
      void Update(void);
      void Init(void);
      TString const fWeightsPath;
      Float_t fMaxDPhi;
      TMVA::Reader *fMvaReader;
      ClassDef(DijetMvaReader, 0)
    }; /// DijetMvaReader
  } /// hgg
} /// mithep

#endif // MitHgg_PhotonTree_DijetMvaReader_h
