#ifndef MitHgg_PhotonTree_PhotonReader_h
#define MitHgg_PhotonTree_PhotonReader_h
#include "MitHgg/PhotonTree/interface/ObjectReader.tpp"
/// Provides mithep::PhotonTreeWriterPhoton<16>
#include "MitPhysics/Mods/interface/PhotonTreeWriter.h"
namespace mithep 
{
  namespace hgg 
  {
    
    typedef ObjectReader<
              ::mithep::PhotonTreeWriterPhoton<16>
              > PhotonReaderBase;
    
    //--------------------------------------------------------------------------
    class PhotonReader : public PhotonReaderBase 
    {      
      public:
        PhotonReader(TTree* iTree, const char *iPrefix);
        virtual ~PhotonReader() {}
        
        /// Generic string-based accessors
        Bool_t   & GetO(const char *name){return *(Bool_t   *) GetPtr(name);}
        Char_t   & GetB(const char *name){return *(Char_t   *) GetPtr(name);}
        UChar_t  & Getb(const char *name){return *(UChar_t  *) GetPtr(name);}
        Short_t  & GetS(const char *name){return *(Short_t  *) GetPtr(name);}
        UShort_t & Gets(const char *name){return *(UShort_t *) GetPtr(name);}
        Int_t    & GetI(const char *name){return *(Int_t    *) GetPtr(name);}
        UInt_t   & Geti(const char *name){return *(UInt_t   *) GetPtr(name);}
        Long64_t & GetL(const char *name){return *(Long64_t *) GetPtr(name);}
        ULong64_t& Getl(const char *name){return *(ULong64_t*) GetPtr(name);}
        Float_t  & GetF(const char *name){return *(Float_t  *) GetPtr(name);}
        Double_t & GetD(const char *name){return *(Double_t *) GetPtr(name);}
        
        /// References to private data members of the PhotonTreeWriterPhoton.
        /// Auto-generated using 
        /// MitHgg/PhotonTree/scripts/dump-photon-reader-snippets.py
        UChar_t &hasphoton                  ;
        UInt_t  &index                      ;
        Float_t &e                          ;
        Float_t &pt                         ;
        Float_t &eta                        ;
        Float_t &phi                        ;
        Float_t &r9                         ;
        Float_t &e3x3                       ;
        Float_t &e5x5                       ;
        UInt_t  &scindex                    ;
        Float_t &sce                        ;
        Float_t &scrawe                     ;
        Float_t &scpse                      ;
        Float_t &scpssigmaxx                ;
        Float_t &scpssigmayy                ;
        Float_t &sceta                      ;
        Float_t &scphi                      ;
        UInt_t  &scnclusters                ;
        UInt_t  &scnhits                    ;
        Float_t &scetawidth                 ;
        Float_t &scphiwidth                 ;
        Float_t &hovere                     ;
        Float_t &hoveretower                ;
        Float_t &sigietaieta                ;
        Bool_t  &isbarrel                   ;
        Bool_t  &isr9reco                   ;
        Bool_t  &isr9cat                    ;
        Float_t &eerr                       ;
        Float_t &eerrsmeared                ;
        Float_t &esmearing                  ;
        Float_t &escale                     ;
        Float_t &idmva                      ;
        Float_t &ecalisodr03                ;
        Float_t &hcalisodr03                ;
        Float_t &trkisohollowdr03           ;
        Float_t &ecalisodr04                ;
        Float_t &hcalisodr04                ;
        Float_t &trkisohollowdr04           ;
        Float_t &trackiso1                  ;
        Float_t &trackiso2                  ;
        Float_t &combiso1                   ;
        Float_t &combiso2                   ;
        Bool_t  &passeleveto                ;
        Float_t &eseed                      ;
        Float_t &etaseed                    ;
        Float_t &phiseed                    ;
        Int_t   &ietaseed                   ;
        Int_t   &iphiseed                   ;
        Int_t   &ixseed                     ;
        Int_t   &iyseed                     ;
        Float_t &etacryseed                 ;
        Float_t &phicryseed                 ;
        Float_t &xcryseed                   ;
        Float_t &ycryseed                   ;
        Float_t &thetaaxisseed              ;
        Float_t &phiaxisseed                ;
        Float_t &sigietaietaseed            ;
        Float_t &sigiphiphiseed             ;
        Float_t &covietaiphiseed            ;
        Float_t &e3x3seed                   ;
        Float_t &e5x5seed                   ;
        Float_t &emaxseed                   ;
        Float_t &e2ndseed                   ;
        Float_t &etopseed                   ;
        Float_t &ebottomseed                ;
        Float_t &eleftseed                  ;
        Float_t &erightseed                 ;
        Float_t &e1x3seed                   ;
        Float_t &e3x1seed                   ;
        Float_t &e1x5seed                   ;
        Float_t &e2x2seed                   ;
        Float_t &e4x4seed                   ;
        Float_t &e2x5maxseed                ;
        Float_t &e2x5topseed                ;
        Float_t &e2x5bottomseed             ;
        Float_t &e2x5leftseed               ;
        Float_t &e2x5rightseed              ;
        Float_t &xseedseed                  ;
        Float_t &yseedseed                  ;
        Float_t &zseedseed                  ;
        UInt_t  &nhitsseed                  ;
        Float_t &ebc2                       ;
        Float_t &etabc2                     ;
        Float_t &phibc2                     ;
        Int_t   &ietabc2                    ;
        Int_t   &iphibc2                    ;
        Int_t   &ixbc2                      ;
        Int_t   &iybc2                      ;
        Float_t &etacrybc2                  ;
        Float_t &phicrybc2                  ;
        Float_t &xcrybc2                    ;
        Float_t &ycrybc2                    ;
        Float_t &thetaaxisbc2               ;
        Float_t &phiaxisbc2                 ;
        Float_t &sigietaietabc2             ;
        Float_t &sigiphiphibc2              ;
        Float_t &covietaiphibc2             ;
        Float_t &e3x3bc2                    ;
        Float_t &e5x5bc2                    ;
        Float_t &emaxbc2                    ;
        Float_t &e2ndbc2                    ;
        Float_t &etopbc2                    ;
        Float_t &ebottombc2                 ;
        Float_t &eleftbc2                   ;
        Float_t &erightbc2                  ;
        Float_t &e1x3bc2                    ;
        Float_t &e3x1bc2                    ;
        Float_t &e1x5bc2                    ;
        Float_t &e2x2bc2                    ;
        Float_t &e4x4bc2                    ;
        Float_t &e2x5maxbc2                 ;
        Float_t &e2x5topbc2                 ;
        Float_t &e2x5bottombc2              ;
        Float_t &e2x5leftbc2                ;
        Float_t &e2x5rightbc2               ;
        Float_t &xbc2bc2                    ;
        Float_t &ybc2bc2                    ;
        Float_t &zbc2bc2                    ;
        UInt_t  &nhitsbc2                   ;
        Float_t &ebclast                    ;
        Float_t &etabclast                  ;
        Float_t &phibclast                  ;
        Int_t   &ietabclast                 ;
        Int_t   &iphibclast                 ;
        Int_t   &ixbclast                   ;
        Int_t   &iybclast                   ;
        Float_t &etacrybclast               ;
        Float_t &phicrybclast               ;
        Float_t &xcrybclast                 ;
        Float_t &ycrybclast                 ;
        Float_t &thetaaxisbclast            ;
        Float_t &phiaxisbclast              ;
        Float_t &sigietaietabclast          ;
        Float_t &sigiphiphibclast           ;
        Float_t &covietaiphibclast          ;
        Float_t &e3x3bclast                 ;
        Float_t &e5x5bclast                 ;
        UInt_t  &nhitsbclast                ;
        Float_t &ebclast2                   ;
        Float_t &etabclast2                 ;
        Float_t &phibclast2                 ;
        Int_t   &ietabclast2                ;
        Int_t   &iphibclast2                ;
        Int_t   &ixbclast2                  ;
        Int_t   &iybclast2                  ;
        Float_t &etacrybclast2              ;
        Float_t &phicrybclast2              ;
        Float_t &xcrybclast2                ;
        Float_t &ycrybclast2                ;
        Float_t &thetaaxisbclast2           ;
        Float_t &phiaxisbclast2             ;
        Float_t &sigietaietabclast2         ;
        Float_t &sigiphiphibclast2          ;
        Float_t &covietaiphibclast2         ;
        Float_t &e3x3bclast2                ;
        Float_t &e5x5bclast2                ;
        UInt_t  &nhitsbclast2               ;
        Float_t &ecor                       ;
        Float_t &ecorerr                    ;
        Float_t &ecorele                    ;
        Float_t &ecoreleerr                 ;
        Float_t &etac                       ;
        Float_t &etas                       ;
        Float_t &etam                       ;
        Float_t &phic                       ;
        Float_t &phis                       ;
        Float_t &phim                       ;
        Float_t &xz                         ;
        Float_t &xc                         ;
        Float_t &xs                         ;
        Float_t &xm                         ;
        Float_t &yz                         ;
        Float_t &yc                         ;
        Float_t &ys                         ;
        Float_t &ym                         ;
        UChar_t &hasconversion              ;
        Float_t &convp                      ;
        Float_t &convpt                     ;
        Float_t &conveta                    ;
        Float_t &convphi                    ;
        Float_t &convdeta                   ;
        Float_t &convdphi                   ;
        Float_t &convvtxrho                 ;
        Float_t &convvtxz                   ;
        Float_t &convvtxphi                 ;
        Float_t &convleadpt                 ;
        Float_t &convtrailpt                ;
        Float_t &convleadtrackpt            ;
        Float_t &convtrailtrackpt           ;
        UChar_t &haselectron                ;
        UChar_t &eleisecaldriven            ;
        UChar_t &eleistrackerdriven         ;
        Float_t &elee                       ;
        Float_t &elept                      ;
        Float_t &eleeta                     ;
        Float_t &elephi                     ;
        Float_t &elefbrem                   ;
        Float_t &eledeta                    ;
        Float_t &eledphi                    ;
        Float_t &elep                       ;
        Float_t &elepin                     ;
        Float_t &elepout                    ;
        UChar_t &haspfsc                    ;
        Float_t &pfsce                      ;
        Float_t &pfscrawe                   ;
        Float_t &pfsceta                    ;
        Float_t &pfscphi                    ;
        UInt_t  &pfscnclusters              ;
        UInt_t  &pfscnhits                  ;
        Float_t &pfscetawidth               ;
        Float_t &pfscphiwidth               ;
        UInt_t  &pfscnpsclusters            ;
        UChar_t &ispromptgen                ;
        Float_t &gene                       ;
        Float_t &genpt                      ;
        Float_t &geneta                     ;
        Float_t &genphi                     ;
        Float_t &genz                       ;
        Int_t   &pdgid                      ;
        Int_t   &motherpdgid                ;
        Float_t &pfcic4_tIso1               ;
        Float_t &pfcic4_tIso2               ;
        Float_t &pfcic4_tIso3               ;
        Float_t &pfcic4_covIEtaIEta         ;
        Float_t &pfcic4_HoE                 ;
        Float_t &pfcic4_R9                  ;
        Float_t &pfcic4_wVtxInd             ;
        Float_t &pfcic4_ecalIso3            ;
        Float_t &pfcic4_ecalIso4            ;
        Float_t &pfcic4_trackIsoSel03       ;
        Float_t &pfcic4_trackIsoWorst04     ;
        Float_t &pfcic4_combIso1            ;
        Float_t &pfcic4_combIso2            ;
        Float_t &idmva_tIso1abs             ;
        Float_t &idmva_tIso2abs             ;
        Float_t &idmva_tIso3abs             ;
        Float_t &idmva_absIsoEcal           ;
        Float_t &idmva_absIsoHcal           ;
        Float_t &idmva_CoviEtaiPhi          ;
        Float_t &idmva_s4ratio              ;
        Float_t &idmva_GammaIso             ;
        Float_t &idmva_ChargedIso_selvtx    ;
        Float_t &idmva_ChargedIso_0p2_selvtx;
        Float_t &idmva_ChargedIso_worstvtx  ;
        Float_t &idmva_PsEffWidthSigmaRR    ;
      private:
        Char_t* GetPtr(const char *memberName);
        ClassDef(PhotonReader, 0)      
    }; /// PhotonReader
    
  } /// hgg
} /// mithep
#endif /// MitHgg_PhotonTree_PhotonReader_h
