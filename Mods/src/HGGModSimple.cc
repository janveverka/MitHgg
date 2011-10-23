// $Id: HGGModSimple.cc,v 1.1 2011/04/13 12:14:27 bendavid Exp $

#include "MitHgg/Mods/interface/HGGModSimple.h"
#include <TH1D.h>
#include <TH2F.h>
#include <TNtuple.h>
#include "Math/GenVector/Boost.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataTree/interface/ElectronCol.h"
#include "MitAna/DataTree/interface/MCParticleCol.h"
#include "MitAna/DataTree/interface/MuonCol.h"
#include "MitAna/DataTree/interface/TrackCol.h"
#include "MitAna/DataTree/interface/PFMetCol.h"
#include "MitAna/DataTree/interface/CaloMetCol.h"
#include "MitAna/DataTree/interface/VertexCol.h"
#include "MitAna/DataTree/interface/PFCandidateCol.h"
#include "MitAna/DataTree/interface/DecayParticleCol.h"
#include "MitAna/DataTree/interface/StableData.h"
#include "MitAna/DataTree/interface/PhotonCol.h"
#include "MitAna/DataTree/interface/PFJetCol.h"
#include "MitAna/DataTree/interface/PileupInfoCol.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitPhysics/Utils/interface/ElectronTools.h"
#include "MitPhysics/Utils/interface/PhotonTools.h"
#include "MitPhysics/Utils/interface/IsolationTools.h"
#include "MitPhysics/Init/interface/ModNames.h"

using namespace mithep;

ClassImp(mithep::HGGModSimple)

//--------------------------------------------------------------------------------------------------
HGGModSimple::HGGModSimple(const char *name, const char *title) : 
  BaseMod(name,title),
  fMCPartName(Names::gkMCPartBrn),
  fTrackName(Names::gkTrackBrn),
  fElectronName(Names::gkElectronBrn),
  fPFMetName("PFMet"),
  fPrimaryVertexName(ModNames::gkGoodVertexesName),
  fConversionName("MergedConversions"),
  fPhotonName("GoodPhotons"),
  fBeamSpotName  ("BeamSpot"),
  fPileupInfoName("PileupInfo"),
  fPileUpDenName     (Names::gkPileupEnergyDensityBrn),
  fPrimaryVertexFromBranch(kFALSE),
  fPhotonsFromBranch(kFALSE),
  fMatchMC(kFALSE),
  fSinglePhotonMode(kFALSE),
  fParticles(0),
  fTracks(0),
  fElectrons(0),
  fPileupInfo(0),
  fPileUpDen(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void HGGModSimple::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void HGGModSimple::Process()
{
  // Process entries of the tree. For this module, we just load the branches and
  // fill the histograms.
  LoadEventObject(fPhotonName,fPhotons);
  
  if (fPhotons->GetEntries()<1)
    return;
  if ((!fSinglePhotonMode && fPhotons->GetEntries()<2))
    return;
  
  LoadEventObject(fPrimaryVertexName, fPrimaryVertex);
  LoadEventObject(fBeamSpotName, fBeamSpot);
  LoadEventObject(fConversionName,fConversions);
  LoadEventObject(fTrackName,    fTracks);
  LoadEventObject(fElectronName, fElectrons);
  LoadEventObject(fPileUpDenName,      fPileUpDen);
  

  if (fMatchMC) {
    LoadEventObject(fMCPartName,fParticles);
    LoadEventObject(fPileupInfoName, fPileupInfo);
  }

  if (fSinglePhotonMode) {
    for (UInt_t i=0; i<fPhotons->GetEntries(); ++i) {
      const Photon *g = fPhotons->At(i);
      FillDiphotonTree(g,g);
    }
  }
  else {
    const Photon *g1 = fPhotons->At(0);
    const Photon *g2 = fPhotons->At(1); 
    FillDiphotonTree(g1,g2);
  }
}



//--------------------------------------------------------------------------------------------------
void HGGModSimple::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here, we typically
  // initialize histograms and other analysis objects and request branches. For this module, we
  // request a branch of the MitTree.

  if (fMatchMC) {
    ReqEventObject(fMCPartName,   fParticles, kTRUE);
    ReqEventObject(fPileupInfoName, fPileupInfo, kTRUE);
  }
  
  ReqEventObject(fTrackName,    fTracks, kTRUE);
  ReqEventObject(fElectronName, fElectrons, kTRUE);
  ReqEventObject(fPFMetName,    fPFMet, kTRUE);
  ReqEventObject(fPrimaryVertexName, fPrimaryVertex, fPrimaryVertexFromBranch);
  ReqEventObject(fConversionName, fConversions, kTRUE);
  ReqEventObject(fPhotonName, fPhotons, fPhotonsFromBranch);
  ReqEventObject(fBeamSpotName, fBeamSpot, kTRUE);
  ReqEventObject(fPileUpDenName,      fPileUpDen,kTRUE);
  
  
  hHggNtuple = new TNtuple("hHggNtuple","hHggNtuple","hpt:heta:hphi:hmass:he:hpx:hpy:hpz:pt1:eta1:phi1:e1:px1:py1:pz1:pt2:eta2:phi2:e2:px2:py2:pz2:haspixelseed1:haspixelseed2:isconverted1:convpt1:conveta1:convphi1:convp1:vtxzconv1:vtxzconvsc1:vx1:vy1:vz1:vrho1:vphi1:ept11:ept21:isconverted2:convpt2:conveta2:convphi2:convp2:vtxzconv2:vtxzconvsc2:vx2:vy2:vz2:vrho2:vphi2:ept12:ept22:eoverp1:deta1:dphi1:eoverp2:deta2:dphi2:genpt1:geneta1:genphi1:gene1:genz1:genpt2:geneta2:genphi2:gene2:genz2:genhpt:genheta:genhphi:genhmass:genhx:genhy:genhz:vtxx:vtxy:vtxz:bsx:bsy:bsz:prob1:prob2:nhitsbeforevtx11:nhitsbeforevtx21:nhitsbeforevtx12:nhitsbeforevtx22:genvtxz:nvtx:deltar:r91:hovere1:sigietaieta1:sigiphiphi1:sigietaiphi1:iseb1:isee1:trkiso1:ecaliso1:hcaliso1:r92:hovere2:sigietaieta2:sigiphiphi2:sigietaiphi2:iseb2:isee2:trkiso2:ecaliso2:hcaliso2:ngenvtx:passelectronveto1:passelectronveto2:passelectronvetoconv1:passelectronvetoconv2:matchtrigger1:matchtrigger2:trkisonopv1:trkisonopv2:passconvid1:passconvid2:rho:genmatchvtxz:convvtxz:convmatchvtxz:hmassgen:hmassgenmatch:hmassconv:hmassconvmatch:hmassvtx:hmassbsvtx");
  AddOutput(hHggNtuple);
  
}

//--------------------------------------------------------------------------------------------------
void HGGModSimple::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void HGGModSimple::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do anything here.
}


void HGGModSimple::FillDiphotonTree(const Photon *g1, const Photon *g2) const {
 
  //get trigger object collection if trigger matching is enabled
  
  const TriggerObjectCol *trigObjs = 0;  
  if (fApplyTriggerMatching) {
    trigObjs = GetHLTObjects(fTrigObjectsName);
  }    
  
  FourVectorM hvector = g1->Mom() + g2->Mom();
  
  const MCParticle *geng1 = 0;
  const MCParticle *geng2 = 0;
  const MCParticle *genh  = 0;
  
  Double_t genpt1   = -99;
  Double_t geneta1  = -99;
  Double_t genphi1  = -99;
  Double_t gene1    = -99;
  Double_t genz1    = -99;
  
  Double_t genpt2   = -99;
  Double_t geneta2  = -99;
  Double_t genphi2  = -99;
  Double_t gene2    = -99;    
  Double_t genz2    = -99;    
  
  Double_t genhpt   = -99;
  Double_t genheta  = -99;
  Double_t genhphi  = -99;
  Double_t genhmass = -99;
  Double_t genhx    = -99;
  Double_t genhy    = -99;
  Double_t genhz    = -99;
  
  ThreeVector vtx          = fPrimaryVertex->At(0)->Position();
  ThreeVector genvtx;
  ThreeVector genmatchvtx;
  ThreeVector bsvtx        = fBeamSpot->At(0)->Position();
  ThreeVector convvtx      = fPrimaryVertex->At(0)->Position();
  ThreeVector convmatchvtx = fPrimaryVertex->At(0)->Position();
  
  UInt_t nvtx = fPrimaryVertex->GetEntries();
  
  Int_t ngenvtx = -99;
  if (fMatchMC) {
    ngenvtx = 1 + fPileupInfo->At(0)->GetPU_NumInteractions();
  }
  
  if (fMatchMC) {
    geng1 = MatchMC(g1);
    geng2 = MatchMC(g2);
  }
  
  //  if (!geng1 && !geng2) printf("Double fake!!!\n");
  
  if (geng1 && geng1->Mother() && geng1->Mother()->AbsPdgId()==25) {
    genh = geng1->Mother();
  }
  
  if (geng1) {
    genpt1 = geng1->Pt();
    geneta1 = geng1->Eta();
    genphi1 = geng1->Phi();
    gene1 = geng1->E();
    if (geng1->Mother()) {
      genz1 = geng1->Mother()->DecayVertex().Z();
    }
  }
  
  if (geng2) {
    genpt2 = geng2->Pt();
    geneta2 = geng2->Eta();
    genphi2 = geng2->Phi();
    gene2 = geng2->E();
    if (geng2->Mother()) {
      genz2 = geng2->Mother()->DecayVertex().Z();
    }
  }
  
  const Vertex *closestVtx = 0;    
  if (genh) {
    genhpt = genh->Pt();
    genheta = genh->Eta();
    genhphi = genh->Phi();
    genhmass = genh->Mass();
    genhx = genh->DecayVertex().X();
    genhy = genh->DecayVertex().Y();
    genhz = genh->DecayVertex().Z();      
    
    genvtx.SetXYZ(genhx,genhx,genhz);
    Double_t mindeltaz = 999.;
    for (UInt_t ivtx = 0; ivtx<fPrimaryVertex->GetEntries(); ++ivtx) {
      const Vertex *rvtx = fPrimaryVertex->At(ivtx);
      if (TMath::Abs(genhz - rvtx->Position().Z()) < mindeltaz) {
	mindeltaz = TMath::Abs(genhz - rvtx->Position().Z());
	genmatchvtx = rvtx->Position();
	closestVtx = rvtx;
      }
    }
  }
  
  Bool_t isconverted1 = kFALSE;
  Bool_t isconverted2 = kFALSE;
  
  //UInt_t nelectrons1 = 0;
  //UInt_t nelectrons2 = 0;
  
  Double_t convpt1 = -99.;    
  Double_t conveta1 = -99.;
  Double_t convphi1 = -99.;
  Double_t convp1 = -99.;
  
  Double_t vtxzconv1 = -99;
  //Double_t vtxzconverr1 = -99;    
  Double_t vtxzconvsc1 = -99;
  //Double_t vtxzconvscconstraint1 = -99;
  //Double_t vtxzconvscconstrainterr1 = -99;
  
  Double_t convpt2 = -99.;
  Double_t conveta2 = -99.;
  Double_t convphi2 = -99.;
  Double_t convp2 = -99.;
  
  Double_t vtxzconv2 = -99;
  //Double_t vtxzconverr2 = -99;    
  Double_t vtxzconvsc2 = -99;
  //Double_t vtxzconvscconstraint2 = -99;
  //Double_t vtxzconvscconstrainterr2 = -99;
  
  
  Double_t ept11 = -99.;
  Double_t ept21 = -99.;
  Double_t ept12 = -99.;
  Double_t ept22 = -99.;
  
  Double_t prob1 = -99.;
  Double_t prob2 = -99.;
  Double_t nhitsbeforevtx11 = -99.;
  Double_t nhitsbeforevtx21 = -99.;
  Double_t nhitsbeforevtx12 = -99.;
  Double_t nhitsbeforevtx22 = -99.;
  
  
  Double_t eoverp1 = -99.;
  //Double_t etoverptdir1 = -99.;
  //Double_t etoverptvtx1 = -99.;
  
  Double_t eoverp2 = -99.;
  //Double_t etoverptdir2 = -99.;
  //Double_t etoverptvtx2 = -99.;   
  
  Double_t deta1 = -99.;
  Double_t dphi1 = -99.;
  //Double_t detacor1 = -99.;
  //Double_t dphicor1 = -99.;
  //Double_t detaconv1 = -99.;
  //Double_t dphiconv1 = -99.;
  
  Double_t deta2 = -99.;
  Double_t dphi2 = -99.;
  //Double_t detacor2 = -99.;
  //Double_t dphicor2 = -99.;  
  //Double_t detaconv2 = -99.;
  //Double_t dphiconv2 = -99.;    
  
  //Double_t cosdalpha1 = -99.;
  //Double_t cosdalpha2 = -99.;
  //Double_t cosdalphaboost1 = -99.;
  //Double_t cosdalphaboost2 = -99.;
  
  ThreeVector norm1;
  ThreeVector norm2;
  ThreeVector normboost1;
  ThreeVector normboost2;
  
  const DecayParticle *conv1 = PhotonTools::MatchedConversion(g1,fConversions,fBeamSpot->At(0));
  const DecayParticle *conv2 = PhotonTools::MatchedConversion(g2,fConversions,fBeamSpot->At(0));
  
  Bool_t passelectronveto1 = PhotonTools::PassElectronVeto(g1,fElectrons);
  Bool_t passelectronveto2 = PhotonTools::PassElectronVeto(g2,fElectrons);
  
  Bool_t passelectronvetoconv1 = PhotonTools::PassElectronVetoConvRecovery(g1,fElectrons,fConversions,fBeamSpot->At(0));
  Bool_t passelectronvetoconv2 = PhotonTools::PassElectronVetoConvRecovery(g2,fElectrons,fConversions,fBeamSpot->At(0));
  
  Bool_t matchtrigger1 = kFALSE;
  Bool_t matchtrigger2 = kFALSE;
  
  if (fApplyTriggerMatching && trigObjs) {
    matchtrigger1 = PhotonTools::PassTriggerMatching(g1,trigObjs);
    matchtrigger2 = PhotonTools::PassTriggerMatching(g2,trigObjs);
  }
  
  Double_t trkisonopv1 = IsolationTools::TrackIsolationNoPV(g1, fBeamSpot->At(0), 0.4, 0.04, 0.0, 0.015, 0.1, TrackQuality::highPurity, fTracks);
  Double_t trkisonopv2 = IsolationTools::TrackIsolationNoPV(g2, fBeamSpot->At(0), 0.4, 0.04, 0.0, 0.015, 0.1, TrackQuality::highPurity, fTracks);
  
  Bool_t passconvid1 = PhotonTools::PassConversionId(g1,conv1);
  Bool_t passconvid2 = PhotonTools::PassConversionId(g2,conv2);
  
  Double_t rho = -99.;
  if (fPileUpDen->GetEntries())
    rho = fPileUpDen->At(0)->Rho();
  
  ThreeVector convpos1;
  ThreeVector convpos2;
  
  
  if (conv1) {
    isconverted1 = kTRUE;
    convpos1 = conv1->Position();
    const DaughterData *electron = conv1->DaughterDat(0);
    const DaughterData *positron = conv1->DaughterDat(1);
    
    prob1 = conv1->Prob();
    nhitsbeforevtx11 = dynamic_cast<const StableData*>(conv1->DaughterDat(0))->NHitsBeforeVtx();
    nhitsbeforevtx21 = dynamic_cast<const StableData*>(conv1->DaughterDat(1))->NHitsBeforeVtx();
    
    convpt1 = conv1->Pt();
    conveta1 = conv1->Eta();
    convphi1 = conv1->Phi();
    convp1 = conv1->P();
    
    vtxzconv1 = conv1->DzCorrected(fBeamSpot->At(0))+fBeamSpot->At(0)->Z();
    vtxzconvsc1 = conv1->Z0EcalVtx(fBeamSpot->At(0)->Position(), ThreeVector(g1->SCluster()->Point()));
    
    eoverp1 = g1->SCluster()->Energy()/conv1->P();
    
    ThreeVector dirconv = ThreeVector(g1->SCluster()->Point()) - conv1->Position();
    deta1 = dirconv.Eta() - conv1->Eta();
    dphi1 = MathUtils::DeltaPhi(dirconv.Phi(),conv1->Phi());    
    
    if (electron->Pt()>positron->Pt()) {
      ept11 = electron->Pt();
      ept21 = positron->Pt();
    }
    else {
      ept11 = positron->Pt();
      ept21 = electron->Pt();
    }
    
    
  }
  
  if (conv2) {
    isconverted2 = kTRUE;
    convpos2 = conv2->Position();
    const DaughterData *electron = conv2->DaughterDat(0);
    const DaughterData *positron = conv2->DaughterDat(1);
    
    prob2 = conv2->Prob();
    nhitsbeforevtx12 = dynamic_cast<const StableData*>(conv2->DaughterDat(0))->NHitsBeforeVtx();
    nhitsbeforevtx22 = dynamic_cast<const StableData*>(conv2->DaughterDat(1))->NHitsBeforeVtx();
    
    convpt2 = conv2->Pt();
    conveta2 = conv2->Eta();
    convphi2 = conv2->Phi();
    convp2 = conv2->P();
    
    vtxzconv2 = conv2->DzCorrected(fBeamSpot->At(0))+fBeamSpot->At(0)->Z();
    vtxzconvsc2 = conv2->Z0EcalVtx(fBeamSpot->At(0)->Position(), ThreeVector(g2->SCluster()->Point()));
    
    eoverp2 = g2->SCluster()->Energy()/conv2->P();
    
    ThreeVector dirconv = ThreeVector(g2->SCluster()->Point()) - conv2->Position();
    deta2 = dirconv.Eta() - conv2->Eta();
    dphi2 = MathUtils::DeltaPhi(dirconv.Phi(),conv2->Phi());    
    
    if (electron->Pt()>positron->Pt()) {
      ept12 = electron->Pt();
      ept22 = positron->Pt();
    }
    else {
      ept12 = positron->Pt();
      ept22 = electron->Pt();
    }
    
    
  }
  
  const DecayParticle *bestconv = 0;
  if (isconverted1 && isconverted2) {
    if (TMath::Abs(conv1->Position().Rho()) < TMath::Abs(conv2->Position().Rho()))
      bestconv = conv1;
    else
      bestconv = conv2;
  }
  else if (isconverted1) {
    bestconv = conv1;
  }
  else if (isconverted2) {
    bestconv = conv2;
  }
  
  if (bestconv) {
    Double_t zconv = 0.;
    const Photon *gbestconv = 0;
    if (bestconv==conv1)
      gbestconv = g1;
    else if (bestconv==conv2)
      gbestconv = g2;
    
    Bool_t usetrackz = ((gbestconv->IsEB() && bestconv->Position().Rho()<15.0) ||
			(gbestconv->IsEE() && TMath::Abs(bestconv->Position().Z())<100.0));
    
    if (usetrackz)
      zconv = bestconv->DzCorrected(fBeamSpot->At(0)) + fBeamSpot->At(0)->Z();
    else
      zconv = bestconv->Z0EcalVtx(fBeamSpot->At(0)->Position(), ThreeVector(gbestconv->SCluster()->Point()));
    convvtx.SetXYZ(fBeamSpot->At(0)->X(),fBeamSpot->At(0)->Y(),zconv);
    
    Double_t mindz = 999.;
    for (UInt_t ivtx=0; ivtx<fPrimaryVertex->GetEntries(); ++ivtx) {
      const Vertex *thevtx = fPrimaryVertex->At(ivtx);
      Double_t dz;
      if (usetrackz)
	dz = TMath::Abs(bestconv->DzCorrected(thevtx));
      else
	dz = TMath::Abs(bestconv->Z0EcalVtx(thevtx->Position(),ThreeVector(gbestconv->SCluster()->Point())) - thevtx->Z());
      if (dz < mindz) {
	mindz = dz;
	convmatchvtx = thevtx->Position();
      }
    }
  }
  
  FourVectorM hvectorgen       = g1->MomVtx(genvtx) + g2->MomVtx(genvtx);    
  FourVectorM hvectorgenmatch  = g1->MomVtx(genmatchvtx) + g2->MomVtx(genmatchvtx);
  FourVectorM hvectorconv      = g1->MomVtx(convvtx) + g2->MomVtx(convvtx);
  FourVectorM hvectorconvmatch = g1->MomVtx(convmatchvtx) + g2->MomVtx(convmatchvtx);
  FourVectorM hvectorvtx       = g1->MomVtx(vtx) + g2->MomVtx(vtx);
  FourVectorM hvectorbsvtx     = g1->MomVtx(bsvtx) + g2->MomVtx(bsvtx);
  
  Float_t fillGen[] = { hvector.Pt(),
			hvector.Eta(),
			hvector.Phi(),
			hvector.M(),
			hvector.E(),
			hvector.Px(),
			hvector.Py(),
			hvector.Pz(),
			g1->Pt(),
			g1->Eta(),
			g1->Phi(),
			g1->E(),
			g1->Px(),
			g1->Py(),
			g1->Pz(),
			g2->Pt(),
			g2->Eta(),
			g2->Phi(),
			g2->E(),
			g2->Px(),
			g2->Py(),
			g2->Pz(),
			g1->HasPixelSeed(),
			g2->HasPixelSeed(),
			isconverted1,
			convpt1,
			conveta1,
			convphi1,
			convp1,
			vtxzconv1,
			vtxzconvsc1,
			convpos1.X(),
			convpos1.Y(),
			convpos1.Z(),
			convpos1.Rho(),
			convpos1.Phi(),
			ept11,
			ept21,
			isconverted2,
			convpt2,
			conveta2,
			convphi2,
			convp2,
			vtxzconv2,
			vtxzconvsc2,
			convpos2.X(),
			convpos2.Y(),
			convpos2.Z(),
			convpos2.Rho(),
			convpos2.Phi(),
			ept12,
			ept22,
			eoverp1,
			deta1,
			dphi1,
			eoverp2,
			deta2,
			dphi2,
			genpt1,
			geneta1,
			genphi1,
			gene1,
			genz1,
			genpt2,
			geneta2,
			genphi2,
			gene2,
			genz2,
			genhpt,
			genheta,
			genhphi,
			genhmass,
			genhx,
			genhy,
			genhz,
			fPrimaryVertex->At(0)->X(),
			fPrimaryVertex->At(0)->Y(),
			fPrimaryVertex->At(0)->Z(),
			fBeamSpot->At(0)->X(),
			fBeamSpot->At(0)->Y(),
			fBeamSpot->At(0)->Z(),
			prob1,
			prob2,
			nhitsbeforevtx11,
			nhitsbeforevtx21,
			nhitsbeforevtx12,
			nhitsbeforevtx22,
			genvtx.Z(),
			nvtx,
			MathUtils::DeltaR(g1,g2),
			g1->R9(),
			g1->HadOverEm(),
			g1->CoviEtaiEta(),
			g1->SCluster()->Seed()->CoviPhiiPhi(),
			g1->SCluster()->Seed()->CoviEtaiPhi(),
			g1->IsEB(),
			g1->IsEE(),
			g1->HollowConeTrkIsoDr04(),
			g1->EcalRecHitIsoDr04(),
			g1->HcalTowerSumEtDr04(),
			g2->R9(),
			g2->HadOverEm(),
			g2->CoviEtaiEta(),
			g2->SCluster()->Seed()->CoviPhiiPhi(),
			g2->SCluster()->Seed()->CoviEtaiPhi(),
			g2->IsEB(),
			g2->IsEE(),
			g2->HollowConeTrkIsoDr04(),
			g2->EcalRecHitIsoDr04(),
			g2->HcalTowerSumEtDr04(),
			ngenvtx,
			passelectronveto1,
			passelectronveto2,
			passelectronvetoconv1,
			passelectronvetoconv2,
			matchtrigger1,
			matchtrigger2,
			trkisonopv1,
			trkisonopv2,
			passconvid1,
			passconvid2,
			rho,
			genmatchvtx.Z(),
			convvtx.Z(),
			convmatchvtx.Z(),
			hvectorgen.M(),
			hvectorgenmatch.M(),
			hvectorconv.M(),
			hvectorconvmatch.M(),
			hvectorvtx.M(),
			hvectorbsvtx.M()
  };

  hHggNtuple->Fill(fillGen);
  
}

const MCParticle *HGGModSimple::MatchMC(const Photon *ph) const
{
  //printf("searching for mc match\n");
  //const MCParticle *match = 0;
  for (UInt_t i=0; i<fParticles->GetEntries(); ++i) {
    const MCParticle *p = fParticles->At(i);
//     if (p->AbsPdgId()==22 && p->IsGenerated() && MathUtils::DeltaPhi(*ph,*p) < 0.3 && p->Mother()) {
//       printf("Mother pdg id = %i\n",p->Mother()->PdgId());
//     }
//     if (p->AbsPdgId()==22 && p->IsGenerated() && p->Mother() && (p->Mother()->AbsPdgId()==25 || p->Mother()->AbsPdgId()<=21)) {
//       printf("prompt photon pt = %5f, eta = %5f, mother pdgid = %i\n",p->Pt(),p->Eta(),p->Mother()->PdgId());
//     }

    if (p->AbsPdgId()==22 && p->IsGenerated() &&
	MathUtils::DeltaPhi(*ph,*p) < 0.3 && p->Mother() &&
	(p->Mother()->AbsPdgId()==25 || p->Mother()->AbsPdgId()<=21)) {
      //printf("MCMatch: mother pdgid = %i",p->Mother()->PdgId());
      //match = p;
      return p;
    }
  }
  
  return 0;
  
}
