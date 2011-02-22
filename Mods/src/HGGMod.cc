// $Id: HGGMod.cc,v 1.6 2010/03/12 13:51:26 bendavid Exp $

#include "MitHgg/Mods/interface/HGGMod.h"
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
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitPhysics/Utils/interface/ElectronTools.h"
#include "MitPhysics/Init/interface/ModNames.h"


using namespace mithep;

ClassImp(mithep::HGGMod)

//--------------------------------------------------------------------------------------------------
HGGMod::HGGMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fMCPartName(Names::gkMCPartBrn),
  fTrackName(Names::gkTrackBrn),
  fMuonName(Names::gkMuonBrn),
  fElectronName(Names::gkElectronBrn),
  fPFMetName("PFMet"),
  fCaloMetName("CorMuonMet"),
  fPFCandidatesName("PFCandidates"),
  fPrimaryVertexName("PrimaryVertexes"),
  fConversionName("MergedConversions"),
  fPhotonName("GoodPhotons"),
  fBeamSpotName  ("BeamSpot"),
  fEcalTrackName("ConversionOutInTracks"),
  fJetName("AKt5PFJets"),
  fCorJetName("AKt5PFJets"),
  fMuonsFromBranch(kTRUE),
  fElectronsFromBranch(kTRUE),
  fPhotonsFromBranch(kFALSE),
  fMatchMC(kFALSE),
  fMatchSim(kFALSE),
  fParticles(0),
  fTracks(0),
  fMuons(0),
  fElectrons(0),
  fJets(0),
  fCorJets(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void HGGMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void HGGMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and
  // fill the histograms.
  

  LoadEventObject(fPhotonName,fPhotons);
  
  if (fPhotons->GetEntries()<2) return;
  
  LoadEventObject(fPrimaryVertexName, fPrimaryVertex);
  LoadEventObject(fBeamSpotName, fBeamSpot);
  LoadEventObject(fJetName, fJets);
  LoadEventObject(fCorJetName, fCorJets);  

  
  //if (fPrimaryVertex->GetEntries()>1) return;

  LoadEventObject(fConversionName,fConversions);
  LoadEventObject(fEcalTrackName,fEcalTracks);


  if (fMatchMC)
    LoadEventObject(fMCPartName,fParticles);
  
  
  for (UInt_t i=0; i<fPhotons->GetEntries(); ++i) {

    const Photon *g1 = fPhotons->At(0);
    const Photon *g2 = fPhotons->At(1);

    const MCParticle *geng1 = 0;
    const MCParticle *geng2 = 0;
    const MCParticle *genh = 0;
    
    Double_t genpt1 = -99;
    Double_t geneta1 = -99;
    Double_t genphi1 = -99;
    Double_t gene1 = -99;
    
    Double_t genpt2 = -99;
    Double_t geneta2 = -99;
    Double_t genphi2 = -99;
    Double_t gene2 = -99;    
    
    Double_t genhpt = -99;
    Double_t genheta = -99;
    Double_t genhphi = -99;
    Double_t genhmass = -99;
    Double_t genhx = -99;
    Double_t genhy = -99;
    Double_t genhz = -99;

    ThreeVector vtx = fPrimaryVertex->At(0)->Position();
    ThreeVector genvtx;
    ThreeVector bestvtx;
    ThreeVector bestvtxc = fPrimaryVertex->At(0)->Position();
    ThreeVector bestvtxct = fPrimaryVertex->At(0)->Position();
    ThreeVector bestvtxt = fPrimaryVertex->At(0)->Position();
    ThreeVector bestvtxtc = fPrimaryVertex->At(0)->Position();
    ThreeVector bsvtx = fBeamSpot->At(0)->Position();
    
    UInt_t nvtx = fPrimaryVertex->GetEntries();
    UInt_t ngoodvtx = 0;
    for (UInt_t ivtx = 0; ivtx<fPrimaryVertex->GetEntries(); ++ivtx) {
      const Vertex *thevtx = fPrimaryVertex->At(ivtx);
      if (thevtx->Ndof()>=5 && TMath::Abs(thevtx->Position().Z())<24. && thevtx->Position().Rho()<2.) {
        ++ngoodvtx;
      }
    }

    if (fMatchMC) {
      geng1 = MatchMC(g1);
      geng2 = MatchMC(g2);
    }
    
    if (geng1 && geng1->Mother() && geng1->Mother()->AbsPdgId()==25) {
      genh = geng1->Mother();
    }
    
    if (geng1) {
      genpt1 = geng1->Pt();
      geneta1 = geng1->Eta();
      genphi1 = geng1->Phi();
      gene1 = geng1->E();
    }

    if (geng2) {
      genpt2 = geng2->Pt();
      geneta2 = geng2->Eta();
      genphi2 = geng2->Phi();
      gene2 = geng2->E();
    }
    
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
          bestvtx = rvtx->Position();
        }
      }
    }
    
    const PFJet *jet1 = MatchJet(g1,fJets);
    const PFJet *jet2 = MatchJet(g2,fJets);
    const PFJet *corjet1 = MatchJet(g1,fCorJets);
    const PFJet *corjet2 = MatchJet(g2,fCorJets);    
    
    Double_t jetpt1 = -99.;
    Double_t jeteta1 = -99.;
    Double_t jetphi1 = -99.;
    Double_t jete1 = -99.;
    
    if (jet1) {
      jetpt1 = jet1->Pt();
      jeteta1 = jet1->Eta();
      jetphi1 = jet1->Phi();
      jete1 = jet1->E();
    }
    
    Double_t jetpt2 = -99.;
    Double_t jeteta2 = -99.;
    Double_t jetphi2 = -99.;
    Double_t jete2 = -99.;
    
    if (jet2) {
      jetpt2 = jet2->Pt();
      jeteta2 = jet2->Eta();
      jetphi2 = jet2->Phi();
      jete2 = jet2->E();
    }    
    
    Double_t corjetpt1 = -99.;
    Double_t corjeteta1 = -99.;
    Double_t corjetphi1 = -99.;
    Double_t corjete1 = -99.;
    
    if (corjet1) {
      corjetpt1 = corjet1->Pt();
      corjeteta1 = corjet1->Eta();
      corjetphi1 = corjet1->Phi();
      corjete1 = corjet1->E();
    }
    
    Double_t corjetpt2 = -99.;
    Double_t corjeteta2 = -99.;
    Double_t corjetphi2 = -99.;
    Double_t corjete2 = -99.;
    
    if (corjet2) {
      corjetpt2 = corjet2->Pt();
      corjeteta2 = corjet2->Eta();
      corjetphi2 = corjet2->Phi();
      corjete2 = corjet2->E();
    }        
    

    Bool_t isconverted1 = kFALSE;
    Bool_t isconverted2 = kFALSE;
    

    UInt_t nelectrons1 = 0;
    UInt_t nelectrons2 = 0;
    
    Double_t convpt1 = -99.;    
    Double_t conveta1 = -99.;
    Double_t convphi1 = -99.;
    Double_t convp1 = -99.;
    
    
    Double_t convpt2 = -99.;
    Double_t conveta2 = -99.;
    Double_t convphi2 = -99.;
    Double_t convp2 = -99.;
    
    
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
    Double_t eoverp2 = -99.;
    
    Double_t deta1 = -99.;
    Double_t dphi1 = -99.;
    Double_t detacor1 = -99.;
    Double_t dphicor1 = -99.;
    Double_t detaconv1 = -99.;
    Double_t dphiconv1 = -99.;

    Double_t deta2 = -99.;
    Double_t dphi2 = -99.;
    Double_t detacor2 = -99.;
    Double_t dphicor2 = -99.;  
    Double_t detaconv2 = -99.;
    Double_t dphiconv2 = -99.;    
    
    Double_t cosdalpha1 = -99.;
    Double_t cosdalpha2 = -99.;
    Double_t cosdalphaboost1 = -99.;
    Double_t cosdalphaboost2 = -99.;
   
    ThreeVector norm1;
    ThreeVector norm2;
    ThreeVector normboost1;
    ThreeVector normboost2;

    FourVectorM hvector = g1->Mom() + g2->Mom();

    const DecayParticle *conv1 = MatchConversion(g1);
    const DecayParticle *conv2 = MatchConversion(g2);
    
    ThreeVector convpos1;
    ThreeVector convpos2;
    
//     FourVector mom1cor(g1->Mom());
//     FourVector mom2cor(g2->Mom());
    FourVector mom1conv(g1->Mom());
    FourVector mom2conv(g2->Mom());
    FourVector mom1convcor(g1->Mom());
    FourVector mom2convcor(g2->Mom());
    
    ThreeVector vtxcor = fPrimaryVertex->At(0)->Position();
    ThreeVector vtxcorct = fPrimaryVertex->At(0)->Position();
    ThreeVector vtxcort = fPrimaryVertex->At(0)->Position();
    ThreeVector vtxcortc = fPrimaryVertex->At(0)->Position();

    ThreeVector vtxcor1 = fPrimaryVertex->At(0)->Position();
    ThreeVector vtxcor2 = fPrimaryVertex->At(0)->Position();
    Double_t vtxcorz1 = vtxcor1.Z();
    Double_t vtxcorz2 = vtxcor2.Z();
    ThreeVector vtxcort1 = fPrimaryVertex->At(0)->Position();
    ThreeVector vtxcort2 = fPrimaryVertex->At(0)->Position();

    
    ROOT::Math::Boost hboost(hvector.BoostToCM().X(),hvector.BoostToCM().Y(),hvector.BoostToCM().Z());
    FourVector g1boost = hboost(FourVector(g1->Mom()));
    FourVector g2boost = hboost(FourVector(g2->Mom()));
    Double_t deltarboost = MathUtils::DeltaR(hvector,g1boost);
    Double_t cosdthetaboost = ThreeVector(g1boost).Unit().Dot(ThreeVector(hvector).Unit());

    if (conv1) {
      isconverted1 = kTRUE;
      convpos1 = conv1->Position();
      const DaughterData *electron = conv1->DaughterDat(0);
      const DaughterData *positron = conv1->DaughterDat(1);
      
      mom1conv.SetXYZT(conv1->Px()*g1->E()/conv1->P(),conv1->Py()*g1->E()/conv1->P(),conv1->Pz()*g1->E()/conv1->P(),g1->E());
      mom1convcor.SetXYZT(conv1->Px()*g1->E()/conv1->P(),conv1->Py()*g1->E()/conv1->P(),conv1->Pz()*g1->E()/conv1->P(),g1->E());
      
      vtxcor1.SetXYZ(fBeamSpot->At(0)->X(),fBeamSpot->At(0)->Y(),conv1->DzCorrected(fBeamSpot->At(0))+fBeamSpot->At(0)->Z());
      vtxcorz1 = vtxcor.Z();
      
      ThreeVector dirvtxcor = (ThreeVector(g1->SCluster()->Point()) - vtxcor).Unit();
      //mom1cor.SetXYZT(dirvtxcor.X()*g1->E(),dirvtxcor.Y()*g1->E(),dirvtxcor.Z()*g1->E(),g1->E());

      prob1 = conv1->Prob();
      nhitsbeforevtx11 = dynamic_cast<const StableData*>(conv1->DaughterDat(0))->NHitsBeforeVtx();
      nhitsbeforevtx21 = dynamic_cast<const StableData*>(conv1->DaughterDat(1))->NHitsBeforeVtx();

      
      convpt1 = conv1->Pt();
      conveta1 = conv1->Eta();
      convphi1 = conv1->Phi();
      convp1 = conv1->P();
            
      eoverp1 = g1->SCluster()->Energy()/conv1->P();
      
      ThreeVector dirvtx = ThreeVector(g1->SCluster()->Point()) - fPrimaryVertex->At(0)->Position();
      deta1 = dirvtx.Eta() - conv1->Eta();
      dphi1 = MathUtils::DeltaPhi(dirvtx.Phi(),conv1->Phi());

      detacor1 = dirvtxcor.Eta() - conv1->Eta();
      dphicor1 = MathUtils::DeltaPhi(dirvtxcor.Phi(),conv1->Phi());     
      
      ThreeVector dirconv = ThreeVector(g1->SCluster()->Point()) - conv1->Position();
      detaconv1 = dirconv.Eta() - conv1->Eta();
      dphiconv1 = MathUtils::DeltaPhi(dirconv.Phi(),conv1->Phi());    
      
      if (electron->Pt()>positron->Pt()) {
        ept11 = electron->Pt();
        ept21 = positron->Pt();
      }
      else {
        ept11 = positron->Pt();
        ept21 = electron->Pt();
      }

      ThreeVector momelectron(electron->Px(),electron->Py(),electron->Pz());
      ThreeVector mompositron(positron->Px(),positron->Py(),positron->Pz());

      cosdalpha1 = momelectron.Unit().Dot(mompositron.Unit());
      norm1 = momelectron.Cross(mompositron).Unit();


      FourVector electronboost = hboost(FourVector(electron->Mom()));
      FourVector positronboost = hboost(FourVector(positron->Mom()));

      ThreeVector momelectronboost(electronboost.Px(),electronboost.Py(),electronboost.Pz());
      ThreeVector mompositronboost(positronboost.Px(),positronboost.Py(),positronboost.Pz());

      cosdalphaboost1 = momelectronboost.Unit().Dot(mompositronboost.Unit());
      normboost1 = momelectronboost.Cross(mompositronboost).Unit();

    }

    if (conv2) {
      isconverted2 = kTRUE;
      convpos2 = conv2->Position();      
      const DaughterData *electron = conv2->DaughterDat(0);
      const DaughterData *positron = conv2->DaughterDat(1);

      mom2conv.SetXYZT(conv2->Px()*g2->E()/conv2->P(),conv2->Py()*g2->E()/conv2->P(),conv2->Pz()*g2->E()/conv2->P(),g2->E());
      mom2convcor.SetXYZT(conv2->Px()*g2->E()/conv2->P(),conv2->Py()*g2->E()/conv2->P(),conv2->Pz()*g2->E()/conv2->P(),g2->E());
      
      vtxcor2.SetXYZ(fBeamSpot->At(0)->X(),fBeamSpot->At(0)->Y(),conv2->DzCorrected(fBeamSpot->At(0))+fBeamSpot->At(0)->Z());
      vtxcorz2 = vtxcor.Z();
      
      ThreeVector dirvtxcor = (ThreeVector(g2->SCluster()->Point()) - vtxcor).Unit();
      //mom2cor.SetXYZT(dirvtxcor.X()*g2->E(),dirvtxcor.Y()*g2->E(),dirvtxcor.Z()*g2->E(),g2->E());
      
      prob2 = conv2->Prob();
      nhitsbeforevtx12 = dynamic_cast<const StableData*>(conv2->DaughterDat(0))->NHitsBeforeVtx();
      nhitsbeforevtx22 = dynamic_cast<const StableData*>(conv2->DaughterDat(1))->NHitsBeforeVtx();


      convpt2 = conv2->Pt();
      conveta2 = conv2->Eta();
      convphi2 = conv2->Phi();
      convp2 = conv2->P();      
      
      eoverp2 = g2->SCluster()->Energy()/conv2->P();
      
      ThreeVector dirvtx = ThreeVector(g2->SCluster()->Point()) - fPrimaryVertex->At(0)->Position();
      deta2 = dirvtx.Eta() - conv2->Eta();
      dphi2 = MathUtils::DeltaPhi(dirvtx.Phi(),conv2->Phi());

      detacor2 = dirvtxcor.Eta() - conv2->Eta();
      dphicor2 = MathUtils::DeltaPhi(dirvtxcor.Phi(),conv2->Phi());        
      
      ThreeVector dirconv = ThreeVector(g2->SCluster()->Point()) - conv2->Position();
      detaconv2 = dirconv.Eta() - conv2->Eta();
      dphiconv2 = MathUtils::DeltaPhi(dirconv.Phi(),conv2->Phi());        
      
      if (electron->Pt()>positron->Pt()) {
        ept12 = electron->Pt();
        ept22 = positron->Pt();
      }
      else {
        ept12 = positron->Pt();
        ept22 = electron->Pt();
      }

      ThreeVector momelectron(electron->Px(),electron->Py(),electron->Pz());
      ThreeVector mompositron(positron->Px(),positron->Py(),positron->Pz());

      cosdalpha2 = momelectron.Unit().Dot(mompositron.Unit());
      norm2 = momelectron.Cross(mompositron).Unit();

      FourVector electronboost = hboost(FourVector(electron->Mom()));
      FourVector positronboost = hboost(FourVector(positron->Mom()));

      ThreeVector momelectronboost(electronboost.Px(),electronboost.Py(),electronboost.Pz());
      ThreeVector mompositronboost(positronboost.Px(),positronboost.Py(),positronboost.Pz());

      cosdalphaboost2 = momelectronboost.Unit().Dot(mompositronboost.Unit());
      normboost2 = momelectronboost.Cross(mompositronboost).Unit();

    }
    
    Bool_t hastrack1 = kFALSE;    
    Bool_t hastrack2 = kFALSE;  
    Double_t trackpt1 = -99.;
    Double_t tracketa1 = -99.;
    Double_t trackphi1 = -99.;
    UInt_t tracknhits1 = -99;
    Double_t trackdeta1 = -99.;
    Double_t trackdphi1 = -99.;
    Double_t trackpt2 = -99.;
    Double_t tracketa2 = -99.;
    Double_t trackphi2 = -99.;
    UInt_t tracknhits2 = -99;
    Double_t trackdeta2 = -99.;
    Double_t trackdphi2 = -99.;

    const Track *track1 = MatchEcalTrack(g1);    
    const Track *track2 = MatchEcalTrack(g2);    

    if (track1) {
      hastrack1 = kTRUE;
      trackpt1 = track1->Pt();
      tracketa1 = track1->Eta();
      trackphi1 = track1->Phi();
      tracknhits1 = track1->NHits();
      
      vtxcort1.SetXYZ(fBeamSpot->At(0)->X(),fBeamSpot->At(0)->Y(),track1->DzCorrected(*fBeamSpot->At(0))+fBeamSpot->At(0)->Z());
      ThreeVector dirts = ThreeVector(g1->SCluster()->Point()) - vtxcort1;
      trackdeta1 = track1->Eta() - dirts.Eta();
      trackdphi1 = MathUtils::DeltaPhi(track1->Phi(),dirts.Phi());

    }

    if (track2) {
      hastrack2 = kTRUE;
      trackpt2 = track2->Pt();
      tracketa2 = track2->Eta();
      trackphi2 = track2->Phi();
      tracknhits2 = track2->NHits();
      
      vtxcort2.SetXYZ(fBeamSpot->At(0)->X(),fBeamSpot->At(0)->Y(),track2->DzCorrected(*fBeamSpot->At(0))+fBeamSpot->At(0)->Z());
      ThreeVector dirts = ThreeVector(g2->SCluster()->Point()) - vtxcort2;
      trackdeta2 = track2->Eta() - dirts.Eta();
      trackdphi2 = MathUtils::DeltaPhi(track2->Phi(),dirts.Phi());

    }

    const DecayParticle *bestconv = 0;
    if (isconverted1 && isconverted2) {
      if ( TMath::Abs(conv1->Position().Rho()) < TMath::Abs(conv2->Position().Rho()) )
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

    const Track *besttrack = 0;
    if (hastrack1 && hastrack2) {
      if ( track1->DszErr()*track1->P()/track1->Pt() > track1->DszErr()*track1->P()/track1->Pt() )
        besttrack = track1;
      else
        besttrack = track2;
    }
    else if (hastrack1) {
      besttrack = track1;
    }
    else if (hastrack2) {
      besttrack = track2;
    }

    Double_t besttracknpixelhits = -99.;
    Double_t besttracknstereohits = -99.;
    Double_t besttrackdzerr = -99.;
    Double_t besttrackdeta = -99.;
    Double_t besttrackdphi = -99.;

    if (besttrack) {
      vtxcort.SetXYZ(fBeamSpot->At(0)->X(),fBeamSpot->At(0)->Y(),besttrack->DzCorrected(*fBeamSpot->At(0))+fBeamSpot->At(0)->Z());
      vtxcortc = vtxcort;

      if (besttrack==track1) {
        besttrackdeta = trackdeta1;
        besttrackdphi = trackdphi1;
      }
      else {
        besttrackdeta = trackdeta2;
        besttrackdphi = trackdphi2;
      }
 
      Double_t mindz = 999.;
      for (UInt_t ivtx=0; ivtx<fPrimaryVertex->GetEntries(); ++ivtx) {
        Double_t dz = TMath::Abs(besttrack->DzCorrected(*fPrimaryVertex->At(ivtx)));
        if ( dz < mindz ) {
          mindz = dz;
          bestvtxt = fPrimaryVertex->At(ivtx)->Position();
          bestvtxtc = bestvtxt;
        }
      }

      besttracknpixelhits = besttrack->NPixelHits();
      besttracknstereohits = besttrack->NStereoHits();
      besttrackdzerr = besttrack->DszErr()*besttrack->P()/besttrack->Pt();
    }

    Double_t bestconvdeta = -99.;
    Double_t bestconvdphi = -99.;

    if (bestconv) {
      vtxcor.SetXYZ(fBeamSpot->At(0)->X(),fBeamSpot->At(0)->Y(),bestconv->DzCorrected(fBeamSpot->At(0))+fBeamSpot->At(0)->Z());
      vtxcortc = vtxcor;

      if (bestconv==conv1) {
        bestconvdeta = detaconv1;
        bestconvdphi = dphiconv1;
      }
      else {
        bestconvdeta = detaconv2;
        bestconvdphi = dphiconv2;
      }

      Double_t mindz = 999.;
      for (UInt_t ivtx=0; ivtx<fPrimaryVertex->GetEntries(); ++ivtx) {
        Double_t dz = TMath::Abs(bestconv->DzCorrected(fPrimaryVertex->At(ivtx)));
        if ( dz < mindz ) {
          mindz = dz;
          bestvtxc = fPrimaryVertex->At(ivtx)->Position();
          bestvtxtc = bestvtxc;
        }
      }

      const Track *bestct = 0;
      const Track *bestct1 = dynamic_cast<const StableParticle*>(bestconv->Daughter(0))->Trk();
      const Track *bestct2 = dynamic_cast<const StableParticle*>(bestconv->Daughter(1))->Trk();
      if (bestct1->Pt()>bestct2->Pt()) {
        bestct = bestct1;
      }
      else {
        bestct = bestct2;
      }
      vtxcorct.SetXYZ(fBeamSpot->At(0)->X(),fBeamSpot->At(0)->Y(),bestct->DzCorrected(*fBeamSpot->At(0))+fBeamSpot->At(0)->Z());
      mindz = 999.;
      for (UInt_t ivtx=0; ivtx<fPrimaryVertex->GetEntries(); ++ivtx) {
        Double_t dz = TMath::Abs(bestct->DzCorrected(*fPrimaryVertex->At(ivtx)));
        if ( dz < mindz ) {
          mindz = dz;
          bestvtxct = fPrimaryVertex->At(ivtx)->Position();
        }
      }

    }

    ThreeVector dirvtxcor1 = (ThreeVector(g1->SCluster()->Point()) - vtxcor).Unit();
    ThreeVector dirvtxcorct1 = (ThreeVector(g1->SCluster()->Point()) - vtxcorct).Unit();
    ThreeVector dirvtxcort1 = (ThreeVector(g1->SCluster()->Point()) - vtxcort).Unit();
    ThreeVector dirvtxcortc1 = (ThreeVector(g1->SCluster()->Point()) - vtxcortc).Unit();
    ThreeVector dirvtx1 = (ThreeVector(g1->SCluster()->Point()) - vtx).Unit();
    ThreeVector dirgenvtx1 = (ThreeVector(g1->SCluster()->Point()) - genvtx).Unit();
    ThreeVector dirbestvtx1 = (ThreeVector(g1->SCluster()->Point()) - bestvtx).Unit();
    ThreeVector dirbestvtxc1 = (ThreeVector(g1->SCluster()->Point()) - bestvtxc).Unit();
    ThreeVector dirbestvtxt1 = (ThreeVector(g1->SCluster()->Point()) - bestvtxt).Unit();
    ThreeVector dirbestvtxtc1 = (ThreeVector(g1->SCluster()->Point()) - bestvtxtc).Unit();
    ThreeVector dirbestvtxct1 = (ThreeVector(g1->SCluster()->Point()) - bestvtxct).Unit();
    ThreeVector dirbsvtx1 = (ThreeVector(g1->SCluster()->Point()) - bsvtx).Unit();
    FourVector mom1cor(dirvtxcor1.X()*g1->E(),dirvtxcor1.Y()*g1->E(),dirvtxcor1.Z()*g1->E(),g1->E());
    FourVector mom1corct(dirvtxcorct1.X()*g1->E(),dirvtxcorct1.Y()*g1->E(),dirvtxcorct1.Z()*g1->E(),g1->E());
    FourVector mom1cort(dirvtxcort1.X()*g1->E(),dirvtxcort1.Y()*g1->E(),dirvtxcort1.Z()*g1->E(),g1->E());
    FourVector mom1cortc(dirvtxcortc1.X()*g1->E(),dirvtxcortc1.Y()*g1->E(),dirvtxcortc1.Z()*g1->E(),g1->E());
    FourVector mom1vtx(dirvtx1.X()*g1->E(),dirvtx1.Y()*g1->E(),dirvtx1.Z()*g1->E(),g1->E());
    FourVector mom1genvtx(dirgenvtx1.X()*g1->E(),dirgenvtx1.Y()*g1->E(),dirgenvtx1.Z()*g1->E(),g1->E());
    FourVector mom1bestvtx(dirbestvtx1.X()*g1->E(),dirbestvtx1.Y()*g1->E(),dirbestvtx1.Z()*g1->E(),g1->E());
    FourVector mom1bestvtxc(dirbestvtxc1.X()*g1->E(),dirbestvtxc1.Y()*g1->E(),dirbestvtxc1.Z()*g1->E(),g1->E());
    FourVector mom1bestvtxct(dirbestvtxct1.X()*g1->E(),dirbestvtxct1.Y()*g1->E(),dirbestvtxct1.Z()*g1->E(),g1->E());
    FourVector mom1bestvtxt(dirbestvtxt1.X()*g1->E(),dirbestvtxt1.Y()*g1->E(),dirbestvtxt1.Z()*g1->E(),g1->E());
    FourVector mom1bestvtxtc(dirbestvtxtc1.X()*g1->E(),dirbestvtxtc1.Y()*g1->E(),dirbestvtxtc1.Z()*g1->E(),g1->E());
    FourVector mom1bsvtx(dirbsvtx1.X()*g1->E(),dirbsvtx1.Y()*g1->E(),dirbsvtx1.Z()*g1->E(),g1->E());

    
    ThreeVector dirvtxcor2 = (ThreeVector(g2->SCluster()->Point()) - vtxcor).Unit();
    ThreeVector dirvtxcorct2 = (ThreeVector(g2->SCluster()->Point()) - vtxcorct).Unit();
    ThreeVector dirvtxcort2 = (ThreeVector(g2->SCluster()->Point()) - vtxcort).Unit();
    ThreeVector dirvtxcortc2 = (ThreeVector(g2->SCluster()->Point()) - vtxcortc).Unit();
    ThreeVector dirvtx2 = (ThreeVector(g2->SCluster()->Point()) - vtx).Unit();
    ThreeVector dirgenvtx2 = (ThreeVector(g2->SCluster()->Point()) - genvtx).Unit();
    ThreeVector dirbestvtx2 = (ThreeVector(g2->SCluster()->Point()) - bestvtx).Unit();
    ThreeVector dirbestvtxc2 = (ThreeVector(g2->SCluster()->Point()) - bestvtxc).Unit();
    ThreeVector dirbestvtxt2 = (ThreeVector(g2->SCluster()->Point()) - bestvtxt).Unit();
    ThreeVector dirbestvtxtc2 = (ThreeVector(g2->SCluster()->Point()) - bestvtxtc).Unit();
    ThreeVector dirbestvtxct2 = (ThreeVector(g2->SCluster()->Point()) - bestvtxct).Unit();
    ThreeVector dirbsvtx2 = (ThreeVector(g2->SCluster()->Point()) - bsvtx).Unit();
    FourVector mom2cor(dirvtxcor2.X()*g2->E(),dirvtxcor2.Y()*g2->E(),dirvtxcor2.Z()*g2->E(),g2->E());
    FourVector mom2corct(dirvtxcorct2.X()*g2->E(),dirvtxcorct2.Y()*g2->E(),dirvtxcorct2.Z()*g2->E(),g2->E());
    FourVector mom2cort(dirvtxcort2.X()*g2->E(),dirvtxcort2.Y()*g2->E(),dirvtxcort2.Z()*g2->E(),g2->E());
    FourVector mom2cortc(dirvtxcortc2.X()*g2->E(),dirvtxcortc2.Y()*g2->E(),dirvtxcortc2.Z()*g2->E(),g2->E());
    FourVector mom2vtx(dirvtx2.X()*g2->E(),dirvtx2.Y()*g2->E(),dirvtx2.Z()*g2->E(),g2->E());
    FourVector mom2genvtx(dirgenvtx2.X()*g2->E(),dirgenvtx2.Y()*g2->E(),dirgenvtx2.Z()*g2->E(),g2->E());
    FourVector mom2bestvtx(dirbestvtx2.X()*g2->E(),dirbestvtx2.Y()*g2->E(),dirbestvtx2.Z()*g2->E(),g2->E());
    FourVector mom2bestvtxc(dirbestvtxc2.X()*g2->E(),dirbestvtxc2.Y()*g2->E(),dirbestvtxc2.Z()*g2->E(),g2->E());
    FourVector mom2bestvtxct(dirbestvtxct2.X()*g2->E(),dirbestvtxct2.Y()*g2->E(),dirbestvtxct2.Z()*g2->E(),g2->E());
    FourVector mom2bestvtxt(dirbestvtxt2.X()*g2->E(),dirbestvtxt2.Y()*g2->E(),dirbestvtxt2.Z()*g2->E(),g2->E());
    FourVector mom2bestvtxtc(dirbestvtxtc2.X()*g2->E(),dirbestvtxtc2.Y()*g2->E(),dirbestvtxtc2.Z()*g2->E(),g2->E());
    FourVector mom2bsvtx(dirbsvtx2.X()*g2->E(),dirbsvtx2.Y()*g2->E(),dirbsvtx2.Z()*g2->E(),g2->E());

    


//     if (!conv1 && conv2) {
//       ThreeVector dirvtxcor = (ThreeVector(g1->SCluster()->Point()) - vtxcor).Unit();
//       mom1cor.SetXYZT(dirvtxcor.X()*g1->E(),dirvtxcor.Y()*g1->E(),dirvtxcor.Z()*g1->E(),g1->E());
//     }
//     
//     if (conv1 && !conv2) {
//       ThreeVector dirvtxcor = (ThreeVector(g2->SCluster()->Point()) - vtxcor).Unit();
//       mom2cor.SetXYZT(dirvtxcor.X()*g2->E(),dirvtxcor.Y()*g2->E(),dirvtxcor.Z()*g2->E(),g2->E());
//     }    
 
    Double_t hmassconv = (mom1conv+mom2conv).M();   
    Double_t hmassconvcor = (mom1convcor+mom2convcor).M();
    Double_t hmasscor = (mom1cor+mom2cor).M();
    Double_t hmasscorct = (mom1corct+mom2corct).M();
    Double_t hmasscort = (mom1cort + mom2cort).M();
    Double_t hmasscortc = (mom1cortc + mom2cortc).M();
    Double_t hmassvtx = (mom1vtx + mom2vtx).M();
    Double_t hmassgenvtx = (mom1genvtx + mom2genvtx).M();
    Double_t hmassbestvtx = (mom1bestvtx + mom2bestvtx).M();
    Double_t hmassbestvtxc = (mom1bestvtxc + mom2bestvtxc).M();
    Double_t hmassbestvtxct = (mom1bestvtxct + mom2bestvtxct).M();
    Double_t hmassbestvtxt = (mom1bestvtxt + mom2bestvtxt).M();
    Double_t hmassbestvtxtc = (mom1bestvtxtc + mom2bestvtxtc).M();
    Double_t hmassbsvtx = (mom1bsvtx + mom2bsvtx).M();

    

    Double_t cosdbeta = norm1.Dot(norm2);
    Double_t cosdbetaboost = normboost1.Dot(normboost2);

    
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
                          g1boost.Pt(),
                          g1boost.Eta(),
                          g1boost.Phi(),
                          g1boost.E(),
                          g1boost.Px(),
                          g1boost.Py(),
                          g1boost.Pz(),
                          g2->Pt(),
                          g2->Eta(),
                          g2->Phi(),
                          g2->E(),
                          g2->Px(),
                          g2->Py(),
                          g2->Pz(),
                          g2boost.Pt(),
                          g2boost.Eta(),
                          g2boost.Phi(),
                          g2boost.E(),
                          g2boost.Px(),
                          g2boost.Py(),
                          g2boost.Pz(),
                          g1->HasPixelSeed(),
                          g2->HasPixelSeed(),
                          isconverted1,
                          convpt1,
                          conveta1,
                          convphi1,
                          convp1,
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
                          convpos2.X(),
                          convpos2.Y(),
                          convpos2.Z(),
                          convpos2.Rho(),
                          convpos2.Phi(),
                          ept12,
                          ept22,
                          cosdalpha1,
                          cosdalpha2,
                          cosdbeta,
                          cosdalphaboost1,
                          cosdalphaboost2,
                          cosdbetaboost,
                          (g1->Mom() + g2->Mom()).Pt(),
                          eoverp1,
                          deta1,
                          dphi1,
                          detacor1,
                          dphicor1,
                          detaconv1,
                          dphiconv1,
                          eoverp2,
                          deta2,
                          dphi2,
                          detacor2,
                          dphicor2, 
                          detaconv2,
                          dphiconv2,
                          hastrack1,
                          hastrack2,
                          trackpt1,
                          tracketa1,
                          trackphi1,
                          tracknhits1,
                          trackpt2,
                          tracketa2,
                          trackphi2,
                          tracknhits2,
                          hmassconv,
                          hmassconvcor,
                          hmasscor,
                          hmasscorct,
                          hmasscort,
                          hmasscortc,
                          hmassvtx,
                          hmassgenvtx,
                          hmassbestvtx,
                          hmassbestvtxc,
                          hmassbestvtxct,
                          hmassbestvtxt,
                          hmassbestvtxtc,
                          hmassbsvtx,
                          genpt1,
                          geneta1,
                          genphi1,
                          gene1,
                          genpt2,
                          geneta2,
                          genphi2,
                          gene2,
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
                          vtxcorz1,
                          vtxcorz2,
                          prob1,
                          prob2,
                          nhitsbeforevtx11,
                          nhitsbeforevtx21,
                          nhitsbeforevtx12,
                          nhitsbeforevtx22,
                          vtxcor.Z(),
                          vtxcorct.Z(),
                          vtxcort.Z(),
                          vtxcortc.Z(),
                          genvtx.Z(),
                          bestvtx.Z(),
                          bestvtxc.Z(),
                          bestvtxct.Z(),
                          bestvtxt.Z(),
                          bestvtxtc.Z(),
                          bsvtx.Z(),
                          deltarboost,
                          cosdthetaboost,
                          besttracknpixelhits,
                          besttracknstereohits,
                          besttrackdzerr,
                          besttrackdeta,
                          besttrackdphi,
                          bestconvdeta,
                          bestconvdphi,
                          nvtx,
                          ngoodvtx,
                          g1->IsConverted(),
                          g2->IsConverted(),
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
                          jetpt1,
                          jeteta1,
                          jetphi1,
                          jete1,
                          jetpt2,
                          jeteta2,
                          jetphi2,
                          jete2,
                          corjetpt1,
                          corjeteta1,
                          corjetphi1,
                          corjete1,
                          corjetpt2,
                          corjeteta2,
                          corjetphi2,
                          corjete2                          
                          };
                          
    hHggNtuple->Fill(fillGen);
    break;
  }
  

}

//--------------------------------------------------------------------------------------------------
void HGGMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  if (fMatchMC) {
    ReqEventObject(fMCPartName,   fParticles, kTRUE);
  }
  
  ReqEventObject(fTrackName,    fTracks, kTRUE);
  ReqEventObject(fMuonName,     fMuons, fMuonsFromBranch);
  ReqEventObject(fElectronName, fElectrons, fElectronsFromBranch);
  ReqEventObject(fElectronLooseName, fElectronsLoose, kFALSE);  
  ReqEventObject("Electrons", fRecoElectrons, kTRUE);  
  ReqEventObject(fPFMetName,    fPFMet, kTRUE);
  ReqEventObject(fCaloMetName,    fCaloMet, kTRUE);
  ReqEventObject(fPFCandidatesName, fPFCandidates, kTRUE);
  ReqEventObject(fPrimaryVertexName, fPrimaryVertex, kTRUE);
  ReqEventObject(fConversionName, fConversions, kTRUE);
  ReqEventObject(fEcalTrackName, fEcalTracks, kTRUE);
  ReqEventObject(fPhotonName, fPhotons, fPhotonsFromBranch);
  ReqEventObject(fBeamSpotName, fBeamSpot, kTRUE);
  ReqEventObject(fJetName, fJets, kTRUE);
  ReqEventObject(fCorJetName, fCorJets, kTRUE);

  

  
  hHggNtuple = new TNtuple("hHggNtuple","hHggNtuple","hpt:heta:hphi:hmass:he:hpx:hpy:hpz:pt1:eta1:phi1:e1:px1:py1:pz1:ptboost1:etaboost1:phiboost1:eboost1:pxboost1:pyboost1:pzboost1:pt2:eta2:phi2:e2:px2:py2:pz2:ptboost2:etaboost2:phiboost2:eboost2:pxboost2:pyboost2:pzboost2:haspixelseed1:haspixelseed2:isconverted1:convpt1:conveta1:convphi1:convp1:vx1:vy1:vz1:vrho1:vphi1:ept11:ept21:isconverted2:convpt2:conveta2:convphi2:convp2:vx2:vy2:vz2:vrho2:vphi2:ept12:ept22:cosdalpha1:cosdalpha2:cosdbeta:cosdalphaboost1:cosdalphaboost2:cosdbetaboost:dipt:eoverp1:deta1:dphi1:detacor1:dphicor1:detaconv1:dphiconv1:eoverp2:deta2:dphi2:detacor2:dphicor2:detaconv2:dphiconv2:hastrack1:hastrack2:trackpt1:tracketa1:trackphi1:tracknhits1:trackpt2:tracketa2:trackphi2:tracknhits2:hmassconv:hmassconvcor:hmasscor:hmasscorct:hmasscort:hmasscortc:hmassvtx:hmassgenvtx:hmassbestvtx:hmassbestvtxc:hmassbestvtxct:hmassbestvtxt:hmassbestvtxtc:hmassbsvtx:genpt1:geneta1:genphi1:gene1:genpt2:geneta2:genphi2:gene2:genhpt:genheta:genhphi:genhmass:genhx:genhy:genhz:vtxx:vtxy:vtxz:bsx:bsy:bsz:vtxcorz1:vtxcorz2:prob1:prob2:nhitsbeforevtx11:nhitsbeforevtx21:nhitsbeforevtx12:nhitsbeforevtx22:vtxcorz:vtxcorctz:vtxcortz:vtxcortcz:genvtxz:bestvtxz:bestvtxcz:bestvtxctz:bestvtxtz:bestvtxtcz:bsvtxz:deltarboost:cosdthetaboost:besttracknpixelhits:besttracknstereohits:besttrackdzerr:besttrackdeta:besttrackdphi:bestconvdeta:bestconvdphi:nvtx:ngoodvtx:hasconvtracks1:hasconvtracks2:deltar:r91:hovere1:sigietaieta1:sigiphiphi1:sigietaiphi1:iseb1:isee1:trkiso1:ecaliso1:hcaliso1:r92:hovere2:sigietaieta2:sigiphiphi2:sigietaiphi2:iseb2:isee2:trkiso2:ecaliso2:hcaliso2:jetpt1:jeteta1:jetphi1:jete1:jetpt2:jeteta2:jetphi2:jete2:corjetpt1:corjeteta1:corjetphi1:corjete1:corjetpt2:corjeteta2:corjetphi2:corjete2");
  AddOutput(hHggNtuple);
}

//--------------------------------------------------------------------------------------------------
void HGGMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void HGGMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}

const MCParticle *HGGMod::MatchMC(const Photon *ph) const
{
  for (UInt_t i=0; i<fParticles->GetEntries(); ++i) {
    const MCParticle *p = fParticles->At(i);
    if ( p->AbsPdgId()==22 && p->IsGenerated() && MathUtils::DeltaPhi(*ph,*p) < 0.3 && p->Mother() && (p->Mother()->AbsPdgId()==25 || p->Mother()->AbsPdgId()<=21) )
      return p;
  }
  
  return 0;
  
}

const MCParticle *HGGMod::SimConv(const Electron *e) const
{

  const MCParticle *p = e->GsfTrk()->MCPart();
  if (!p) return 0;

  if (p->AbsPdgId() != 11) return 0;

  const MCParticle *m = p->DistinctMother();

  if (!m) return 0;

  if (!m->PdgId()==22) return 0;

  return m;

  
}

const MCParticle *HGGMod::SimDalitz(const Electron *e) const
{

  const MCParticle *p = e->GsfTrk()->MCPart();
  if (!p) return 0;

  if (p->AbsPdgId() != 11) return 0;

  const MCParticle *m = p->DistinctMother();

  if (!m) return 0;

  if (!m->PdgId()==111) return 0;

  return m;

  
}

const MCParticle *HGGMod::SimChargedPiKP(const Electron *e) const
{

  const MCParticle *p = e->GsfTrk()->MCPart();
  if (!p) return 0;

  if ( ! (p->AbsPdgId() == 211 || p->AbsPdgId()==321 || p->AbsPdgId()==2212) ) return 0;

  return p;

  
}

const MCParticle *HGGMod::SimHeavyFlavour(const Electron *e) const
{

  const MCParticle *p = e->GsfTrk()->MCPart();
  if (!p) return 0;

  if (p->AbsPdgId() != 11) return 0;

  const MCParticle *m = p->DistinctMother();

  if (!m) return 0;

  if (m->PdgId()==111 || m->PdgId()==22 || m->AbsPdgId()==24 || m->AbsPdgId()==23) return 0;

  return m;

  
}

const DecayParticle *HGGMod::MatchConversion(const Photon *p) const
{
  const DecayParticle *match = 0;
  Double_t besteoverp = -99.;
  
  for (UInt_t i=0; i<fConversions->GetEntries(); ++i) {
    const DecayParticle *c = fConversions->At(i);
    //if ( c->Quality().Quality(ConversionQuality::arbitratedMerged) &&  c->Quality().Quality(ConversionQuality::highPurity) && MathUtils::DeltaR(p,c)<0.3 )
    //if ( c->Quality().Quality(ConversionQuality::arbitratedMerged) && c->Prob()>1e-6 && MathUtils::DeltaR(p,c)<0.3 )   
//    if (c->Quality().Quality(ConversionQuality::arbitratedMerged) && c->Prob()>0. && MathUtils::DeltaR(p,c)<0.3 ) {
   // if (c->Quality().Quality(ConversionQuality::arbitratedEcalSeeded) && c->Prob()>0. && MathUtils::DeltaR(p,c)<0.3 ) {
    if ( c->Prob()>1e-6 && MathUtils::DeltaR(p,c)<0.3 ) {  
      if (TMath::Abs(1.-p->SCluster()->Energy()/c->P()) < TMath::Abs(1.-besteoverp)) {
        besteoverp = p->SCluster()->Energy()/c->P();
        match = c;
      }
    }
  }
  
  return match;
  
}

const Track *HGGMod::MatchEcalTrack(const Photon *p) const
{
  const Track *match = 0;
  Double_t besteoverp = -99.;
  
  for (UInt_t i=0; i<fEcalTracks->GetEntries(); ++i) {
    const Track *t = fEcalTracks->At(i);
    if ( MathUtils::DeltaR(p,t)<0.3 ) {
    
      if (TMath::Abs(1.-p->SCluster()->Energy()/t->P()) < TMath::Abs(1.-besteoverp)) {
        besteoverp = p->SCluster()->Energy()/t->P();
        match = t;
      }
    }
  }
  
  return match;
  
}

const PFJet *HGGMod::MatchJet(const Photon *p, const PFJetCol *jets) const
{
  const PFJet *match = 0;
  Double_t mindr = 999.;
  
  for (UInt_t i=0; i<jets->GetEntries(); ++i) {
    const PFJet *j = jets->At(i);
    double dr = MathUtils::DeltaR(p,j);
    if ( dr<mindr ) {
      mindr = dr;
      match = j;
    }
  }
  
  return match;
  
}

