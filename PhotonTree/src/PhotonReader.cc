#include "MitHgg/PhotonTree/interface/PhotonReader.h"

using ::mithep::hgg::PhotonReader;
using ::mithep::hgg::PhotonReaderBase;

ClassImp(PhotonReader)
ClassImp(PhotonReaderBase)

//------------------------------------------------------------------------------
PhotonReader::PhotonReader(TTree *iTree, const char *prefix) :
  PhotonReaderBase(iTree, prefix),
  /// Initiliaze references to private members of PhotonTreeWriterPhoton.
  /// Auto-generated using 
  /// MitHgg/PhotonTree/scripts/dump-photon-reader-snippets.py
  hasphoton                  (Getb("hasphoton"                  )),
  index                      (Geti("index"                      )),
  e                          (GetF("e"                          )),
  pt                         (GetF("pt"                         )),
  eta                        (GetF("eta"                        )),
  phi                        (GetF("phi"                        )),
  r9                         (GetF("r9"                         )),
  e3x3                       (GetF("e3x3"                       )),
  e5x5                       (GetF("e5x5"                       )),
  scindex                    (Geti("scindex"                    )),
  sce                        (GetF("sce"                        )),
  scrawe                     (GetF("scrawe"                     )),
  scpse                      (GetF("scpse"                      )),
  scpssigmaxx                (GetF("scpssigmaxx"                )),
  scpssigmayy                (GetF("scpssigmayy"                )),
  sceta                      (GetF("sceta"                      )),
  scphi                      (GetF("scphi"                      )),
  scnclusters                (Geti("scnclusters"                )),
  scnhits                    (Geti("scnhits"                    )),
  scetawidth                 (GetF("scetawidth"                 )),
  scphiwidth                 (GetF("scphiwidth"                 )),
  hovere                     (GetF("hovere"                     )),
  hoveretower                (GetF("hoveretower"                )),
  sigietaieta                (GetF("sigietaieta"                )),
  isbarrel                   (GetO("isbarrel"                   )),
  isr9reco                   (GetO("isr9reco"                   )),
  isr9cat                    (GetO("isr9cat"                    )),
  eerr                       (GetF("eerr"                       )),
  eerrsmeared                (GetF("eerrsmeared"                )),
  esmearing                  (GetF("esmearing"                  )),
  escale                     (GetF("escale"                     )),
  idmva                      (GetF("idmva"                      )),
  ecalisodr03                (GetF("ecalisodr03"                )),
  hcalisodr03                (GetF("hcalisodr03"                )),
  trkisohollowdr03           (GetF("trkisohollowdr03"           )),
  ecalisodr04                (GetF("ecalisodr04"                )),
  hcalisodr04                (GetF("hcalisodr04"                )),
  trkisohollowdr04           (GetF("trkisohollowdr04"           )),
  trackiso1                  (GetF("trackiso1"                  )),
  trackiso2                  (GetF("trackiso2"                  )),
  combiso1                   (GetF("combiso1"                   )),
  combiso2                   (GetF("combiso2"                   )),
  passeleveto                (GetO("passeleveto"                )),
  eseed                      (GetF("eseed"                      )),
  etaseed                    (GetF("etaseed"                    )),
  phiseed                    (GetF("phiseed"                    )),
  ietaseed                   (GetI("ietaseed"                   )),
  iphiseed                   (GetI("iphiseed"                   )),
  ixseed                     (GetI("ixseed"                     )),
  iyseed                     (GetI("iyseed"                     )),
  etacryseed                 (GetF("etacryseed"                 )),
  phicryseed                 (GetF("phicryseed"                 )),
  xcryseed                   (GetF("xcryseed"                   )),
  ycryseed                   (GetF("ycryseed"                   )),
  thetaaxisseed              (GetF("thetaaxisseed"              )),
  phiaxisseed                (GetF("phiaxisseed"                )),
  sigietaietaseed            (GetF("sigietaietaseed"            )),
  sigiphiphiseed             (GetF("sigiphiphiseed"             )),
  covietaiphiseed            (GetF("covietaiphiseed"            )),
  e3x3seed                   (GetF("e3x3seed"                   )),
  e5x5seed                   (GetF("e5x5seed"                   )),
  emaxseed                   (GetF("emaxseed"                   )),
  e2ndseed                   (GetF("e2ndseed"                   )),
  etopseed                   (GetF("etopseed"                   )),
  ebottomseed                (GetF("ebottomseed"                )),
  eleftseed                  (GetF("eleftseed"                  )),
  erightseed                 (GetF("erightseed"                 )),
  e1x3seed                   (GetF("e1x3seed"                   )),
  e3x1seed                   (GetF("e3x1seed"                   )),
  e1x5seed                   (GetF("e1x5seed"                   )),
  e2x2seed                   (GetF("e2x2seed"                   )),
  e4x4seed                   (GetF("e4x4seed"                   )),
  e2x5maxseed                (GetF("e2x5maxseed"                )),
  e2x5topseed                (GetF("e2x5topseed"                )),
  e2x5bottomseed             (GetF("e2x5bottomseed"             )),
  e2x5leftseed               (GetF("e2x5leftseed"               )),
  e2x5rightseed              (GetF("e2x5rightseed"              )),
  xseedseed                  (GetF("xseedseed"                  )),
  yseedseed                  (GetF("yseedseed"                  )),
  zseedseed                  (GetF("zseedseed"                  )),
  nhitsseed                  (Geti("nhitsseed"                  )),
  ebc2                       (GetF("ebc2"                       )),
  etabc2                     (GetF("etabc2"                     )),
  phibc2                     (GetF("phibc2"                     )),
  ietabc2                    (GetI("ietabc2"                    )),
  iphibc2                    (GetI("iphibc2"                    )),
  ixbc2                      (GetI("ixbc2"                      )),
  iybc2                      (GetI("iybc2"                      )),
  etacrybc2                  (GetF("etacrybc2"                  )),
  phicrybc2                  (GetF("phicrybc2"                  )),
  xcrybc2                    (GetF("xcrybc2"                    )),
  ycrybc2                    (GetF("ycrybc2"                    )),
  thetaaxisbc2               (GetF("thetaaxisbc2"               )),
  phiaxisbc2                 (GetF("phiaxisbc2"                 )),
  sigietaietabc2             (GetF("sigietaietabc2"             )),
  sigiphiphibc2              (GetF("sigiphiphibc2"              )),
  covietaiphibc2             (GetF("covietaiphibc2"             )),
  e3x3bc2                    (GetF("e3x3bc2"                    )),
  e5x5bc2                    (GetF("e5x5bc2"                    )),
  emaxbc2                    (GetF("emaxbc2"                    )),
  e2ndbc2                    (GetF("e2ndbc2"                    )),
  etopbc2                    (GetF("etopbc2"                    )),
  ebottombc2                 (GetF("ebottombc2"                 )),
  eleftbc2                   (GetF("eleftbc2"                   )),
  erightbc2                  (GetF("erightbc2"                  )),
  e1x3bc2                    (GetF("e1x3bc2"                    )),
  e3x1bc2                    (GetF("e3x1bc2"                    )),
  e1x5bc2                    (GetF("e1x5bc2"                    )),
  e2x2bc2                    (GetF("e2x2bc2"                    )),
  e4x4bc2                    (GetF("e4x4bc2"                    )),
  e2x5maxbc2                 (GetF("e2x5maxbc2"                 )),
  e2x5topbc2                 (GetF("e2x5topbc2"                 )),
  e2x5bottombc2              (GetF("e2x5bottombc2"              )),
  e2x5leftbc2                (GetF("e2x5leftbc2"                )),
  e2x5rightbc2               (GetF("e2x5rightbc2"               )),
  xbc2bc2                    (GetF("xbc2bc2"                    )),
  ybc2bc2                    (GetF("ybc2bc2"                    )),
  zbc2bc2                    (GetF("zbc2bc2"                    )),
  nhitsbc2                   (Geti("nhitsbc2"                   )),
  ebclast                    (GetF("ebclast"                    )),
  etabclast                  (GetF("etabclast"                  )),
  phibclast                  (GetF("phibclast"                  )),
  ietabclast                 (GetI("ietabclast"                 )),
  iphibclast                 (GetI("iphibclast"                 )),
  ixbclast                   (GetI("ixbclast"                   )),
  iybclast                   (GetI("iybclast"                   )),
  etacrybclast               (GetF("etacrybclast"               )),
  phicrybclast               (GetF("phicrybclast"               )),
  xcrybclast                 (GetF("xcrybclast"                 )),
  ycrybclast                 (GetF("ycrybclast"                 )),
  thetaaxisbclast            (GetF("thetaaxisbclast"            )),
  phiaxisbclast              (GetF("phiaxisbclast"              )),
  sigietaietabclast          (GetF("sigietaietabclast"          )),
  sigiphiphibclast           (GetF("sigiphiphibclast"           )),
  covietaiphibclast          (GetF("covietaiphibclast"          )),
  e3x3bclast                 (GetF("e3x3bclast"                 )),
  e5x5bclast                 (GetF("e5x5bclast"                 )),
  nhitsbclast                (Geti("nhitsbclast"                )),
  ebclast2                   (GetF("ebclast2"                   )),
  etabclast2                 (GetF("etabclast2"                 )),
  phibclast2                 (GetF("phibclast2"                 )),
  ietabclast2                (GetI("ietabclast2"                )),
  iphibclast2                (GetI("iphibclast2"                )),
  ixbclast2                  (GetI("ixbclast2"                  )),
  iybclast2                  (GetI("iybclast2"                  )),
  etacrybclast2              (GetF("etacrybclast2"              )),
  phicrybclast2              (GetF("phicrybclast2"              )),
  xcrybclast2                (GetF("xcrybclast2"                )),
  ycrybclast2                (GetF("ycrybclast2"                )),
  thetaaxisbclast2           (GetF("thetaaxisbclast2"           )),
  phiaxisbclast2             (GetF("phiaxisbclast2"             )),
  sigietaietabclast2         (GetF("sigietaietabclast2"         )),
  sigiphiphibclast2          (GetF("sigiphiphibclast2"          )),
  covietaiphibclast2         (GetF("covietaiphibclast2"         )),
  e3x3bclast2                (GetF("e3x3bclast2"                )),
  e5x5bclast2                (GetF("e5x5bclast2"                )),
  nhitsbclast2               (Geti("nhitsbclast2"               )),
  ecor                       (GetF("ecor"                       )),
  ecorerr                    (GetF("ecorerr"                    )),
  ecorele                    (GetF("ecorele"                    )),
  ecoreleerr                 (GetF("ecoreleerr"                 )),
  etac                       (GetF("etac"                       )),
  etas                       (GetF("etas"                       )),
  etam                       (GetF("etam"                       )),
  phic                       (GetF("phic"                       )),
  phis                       (GetF("phis"                       )),
  phim                       (GetF("phim"                       )),
  xz                         (GetF("xz"                         )),
  xc                         (GetF("xc"                         )),
  xs                         (GetF("xs"                         )),
  xm                         (GetF("xm"                         )),
  yz                         (GetF("yz"                         )),
  yc                         (GetF("yc"                         )),
  ys                         (GetF("ys"                         )),
  ym                         (GetF("ym"                         )),
  hasconversion              (Getb("hasconversion"              )),
  convp                      (GetF("convp"                      )),
  convpt                     (GetF("convpt"                     )),
  conveta                    (GetF("conveta"                    )),
  convphi                    (GetF("convphi"                    )),
  convdeta                   (GetF("convdeta"                   )),
  convdphi                   (GetF("convdphi"                   )),
  convvtxrho                 (GetF("convvtxrho"                 )),
  convvtxz                   (GetF("convvtxz"                   )),
  convvtxphi                 (GetF("convvtxphi"                 )),
  convleadpt                 (GetF("convleadpt"                 )),
  convtrailpt                (GetF("convtrailpt"                )),
  convleadtrackpt            (GetF("convleadtrackpt"            )),
  convtrailtrackpt           (GetF("convtrailtrackpt"           )),
  haselectron                (Getb("haselectron"                )),
  eleisecaldriven            (Getb("eleisecaldriven"            )),
  eleistrackerdriven         (Getb("eleistrackerdriven"         )),
  elee                       (GetF("elee"                       )),
  elept                      (GetF("elept"                      )),
  eleeta                     (GetF("eleeta"                     )),
  elephi                     (GetF("elephi"                     )),
  elefbrem                   (GetF("elefbrem"                   )),
  eledeta                    (GetF("eledeta"                    )),
  eledphi                    (GetF("eledphi"                    )),
  elep                       (GetF("elep"                       )),
  elepin                     (GetF("elepin"                     )),
  elepout                    (GetF("elepout"                    )),
  haspfsc                    (Getb("haspfsc"                    )),
  pfsce                      (GetF("pfsce"                      )),
  pfscrawe                   (GetF("pfscrawe"                   )),
  pfsceta                    (GetF("pfsceta"                    )),
  pfscphi                    (GetF("pfscphi"                    )),
  pfscnclusters              (Geti("pfscnclusters"              )),
  pfscnhits                  (Geti("pfscnhits"                  )),
  pfscetawidth               (GetF("pfscetawidth"               )),
  pfscphiwidth               (GetF("pfscphiwidth"               )),
  pfscnpsclusters            (Geti("pfscnpsclusters"            )),
  ispromptgen                (Getb("ispromptgen"                )),
  gene                       (GetF("gene"                       )),
  genpt                      (GetF("genpt"                      )),
  geneta                     (GetF("geneta"                     )),
  genphi                     (GetF("genphi"                     )),
  genz                       (GetF("genz"                       )),
  pdgid                      (GetI("pdgid"                      )),
  motherpdgid                (GetI("motherpdgid"                )),
  pfcic4_tIso1               (GetF("pfcic4_tIso1"               )),
  pfcic4_tIso2               (GetF("pfcic4_tIso2"               )),
  pfcic4_tIso3               (GetF("pfcic4_tIso3"               )),
  pfcic4_covIEtaIEta         (GetF("pfcic4_covIEtaIEta"         )),
  pfcic4_HoE                 (GetF("pfcic4_HoE"                 )),
  pfcic4_R9                  (GetF("pfcic4_R9"                  )),
  pfcic4_wVtxInd             (GetF("pfcic4_wVtxInd"             )),
  pfcic4_ecalIso3            (GetF("pfcic4_ecalIso3"            )),
  pfcic4_ecalIso4            (GetF("pfcic4_ecalIso4"            )),
  pfcic4_trackIsoSel03       (GetF("pfcic4_trackIsoSel03"       )),
  pfcic4_trackIsoWorst04     (GetF("pfcic4_trackIsoWorst04"     )),
  pfcic4_combIso1            (GetF("pfcic4_combIso1"            )),
  pfcic4_combIso2            (GetF("pfcic4_combIso2"            )),
  idmva_tIso1abs             (GetF("idmva_tIso1abs"             )),
  idmva_tIso2abs             (GetF("idmva_tIso2abs"             )),
  idmva_tIso3abs             (GetF("idmva_tIso3abs"             )),
  idmva_absIsoEcal           (GetF("idmva_absIsoEcal"           )),
  idmva_absIsoHcal           (GetF("idmva_absIsoHcal"           )),
  idmva_CoviEtaiPhi          (GetF("idmva_CoviEtaiPhi"          )),
  idmva_s4ratio              (GetF("idmva_s4ratio"              )),
  idmva_GammaIso             (GetF("idmva_GammaIso"             )),
  idmva_ChargedIso_selvtx    (GetF("idmva_ChargedIso_selvtx"    )),
  idmva_ChargedIso_0p2_selvtx(GetF("idmva_ChargedIso_0p2_selvtx")),
  idmva_ChargedIso_worstvtx  (GetF("idmva_ChargedIso_worstvtx"  )),
  idmva_PsEffWidthSigmaRR    (GetF("idmva_PsEffWidthSigmaRR"    ))
  {} /// Ctor


//------------------------------------------------------------------------------
Char_t *
PhotonReader::GetPtr(const char *memberName)
{
  Char_t *address = (Char_t*) static_cast<WriterObjectType*>(this);
  Long_t offset = fOffsetForName[memberName];
  return address + offset;
} /// GetPtr
