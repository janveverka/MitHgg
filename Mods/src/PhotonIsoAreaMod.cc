// $Id: PhotonIsoAreaMod.cc,v 1.1 2011/01/24 14:57:09 paus Exp $

#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TNtuple.h>
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataTree/interface/MCEventInfo.h"
#include "MitAna/DataTree/interface/MCParticleCol.h"
#include "MitAna/DataTree/interface/MuonCol.h"
#include "MitAna/DataTree/interface/TrackCol.h"
#include "MitAna/DataTree/interface/Vertex.h"
#include "MitAna/DataTree/interface/Photon.h"
#include "MitHgg/Mods/interface/PhotonIsoAreaMod.h"
#include "MitAna/DataTree/interface/BeamSpotCol.h"
#include "MitAna/DataTree/interface/PFCandidateCol.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

using namespace mithep;

ClassImp(mithep::PhotonIsoAreaMod)

//--------------------------------------------------------------------------------------------------
PhotonIsoAreaMod::PhotonIsoAreaMod(const char *name, const char *title) :
  BaseMod             (name,title),
  // initialize bambu objects
  fMcEventInfoName    (Names::gkMCEvtInfoBrn),

  fTrackName (Names::gkTrackBrn),
  fTracks(0),

  fTrigObjsName       (Names::gkHltObjBrn),

  fPVName("PrimaryVertexes"),
  fPV(0),
  fPVFromBranch(kTRUE),

  fPVNameDA("DAPrimaryVertexes"),
  fPVDA(0),
  fPVFromBranchDA(kTRUE),

  fPVNameBS("DAPrimaryVertexesBS"),
  fPVBS(0),
  fPVFromBranchBS(kTRUE),

  fElectronName       (Names::gkElectronBrn),
  fElectrons(0),
  fElectronsFromBranch(kTRUE),

  fPhotonName         (Names::gkPhotonBrn),
  fPhotons(0),
  fPhotonsFromBranch(kTRUE),
  
  fMCParticleName(Names::gkMCPartBrn),
  fMCParticles(0),
  fMcEventInfo        (0),
  fPileUpName         ("PileupInfo"),
  fPileUp             (0),
  fPileUpDen          (0),
  fPileUpDenName      ("Rho"),

  fIsData             (kTRUE),
  fSkimming           (kFALSE),

  // scale removal
  fOverlapCut         (-1.0),
  // cuts for selection
  fPhotonEtMin        (30.0),
  fElectronEtMin      (40.0),
  

  fCands(0),

  fNoPixelSeedCut     (kTRUE),
  fPhotonSigEeBarMax  ( 0.0105),
  fPhotonSigEeEcpMax  ( 0.0300),
  fPhotonIsoMax       ( 0.30),
  fPhotonR9Min        (0.9)

{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void PhotonIsoAreaMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void PhotonIsoAreaMod::Process()
{  
  IncNEventsProcessed();

  /* --------------------------
     Load Branches Needed...    */  
  if (! fIsData) {
    LoadBranch(fMcEventInfoName);
    if (fOverlapCut > 0. && fMcEventInfo->Scale() > fOverlapCut) {
      MDB(kModules,1)
	printf(" Reject event with scale %f  above cut: %f\n",fMcEventInfo->Scale(),fOverlapCut);
      return;
    }
    LoadBranch(fMCParticleName);
    LoadBranch(fPileUpName);
  }
  LoadEventObject(fPhotonName,fPhotons);
  LoadEventObject(fElectronName,fElectrons);
  LoadEventObject(fPVName, fPV);
  LoadEventObject(fPVNameDA, fPVDA);
  LoadEventObject(fPVNameBS, fPVBS);
  LoadBranch(fPileUpDenName);
  LoadBranch("PFCandidates");
  LoadBranch(fTrackName);
  LoadBranch("BeamSpot");
  
  //----------------------------------------------------------------------------------
  
  Float_t numPU = -1.;
  Float_t numPV = -1.;
  Float_t numPVDA = -1.;
  Float_t numPVBS = -1.;

  if( !fIsData )
    numPU = (Float_t) fPileUp->At(0)->GetPU_NumInteractions();  


  Float_t _tRho = (Float_t) fPileUpDen->At(0)->Rho();
  
  std::vector<Float_t> fPhotCandPtR2;
  std::vector<Float_t> fPhotCandPtR3;
  std::vector<Float_t> fPhotCandPtR4;
  std::vector<Float_t> fPhotCandPtR5;

  const BaseVertex *bsp = dynamic_cast<const BaseVertex*>(fBeamspot->At(0));    

  /* Run over the electrons a select them. We need at least one. */
  ResetSelectedElectrons();
  for (UInt_t i=0; i<fElectrons->GetEntries(); ++i) {
    const Electron *p = fElectrons->At(i);  
    if (p->Et() > fElectronEtMin) {
      fSelectedElectrons.push_back(i);
    }
  }
  
  if(fSelectedElectrons.size() == 0) return;
  if (fIsData) {
    MatchObjectsToTrigger();
    if(fTrigElectrons.size()<1) return;
  } else
    fTrigElectrons.push_back(fSelectedElectrons[0]);
  
  /* -------------------------  
     Photon Selection
     We use all the photons coming as from the branch.
     To stear their selection (i.e. RECO/ID/ISO) use the PhotonIDMod.  */

  ResetSelectedPhotons();
  for (UInt_t i=0; i<fPhotons->GetEntries(); ++i) {
    const Photon *p = fPhotons->At(i);
    if (p->Et() > fPhotonEtMin) {
      if(p->SCluster() == fElectrons->At(fTrigElectrons[0])->SCluster()) continue;
      fSelectedPhotons.push_back(i);
    }
  }
  
  if(fSelectedPhotons.size() == 0 ) return;

  // find the primary vertex (for now take the first one, later we should take the one 'mathing' the di-muon system
  if( fPV->GetEntries() == 0 ) return;  // NO PV found... ???

  numPV   = (Float_t) fPV  ->GetEntries();
  numPVDA = (Float_t) fPVDA->GetEntries();
  numPVBS = (Float_t) fPVBS->GetEntries();

  // compute SuperCluster momentum (assuming zero mass)
  const Vertex* vtx = fPV->At(0);

  const Electron* _tEl = fElectrons->At(fTrigElectrons[0]);
  const Photon*   _tPh = fPhotons->At(fSelectedPhotons[0]);
  
  Float_t _tPhEt   = ( _tPh ? _tPh->Et() : -1.);
  Float_t _tPhEta  = ( _tPh ? _tPh->Eta() : -100.);
  Float_t _tPhPhi  = ( _tPh ? _tPh->Phi() : -100.);
  Float_t _tPhE    = ( _tPh ? _tPh->E() : -100.);
  Float_t _tPhpz   = ( _tPh ? _tPh->Pz() : -100.);
  Float_t _tPhpt   = ( _tPh ? _tPh->Pt() : -100.);
  Float_t _tPhm    = ( _tPh ? _tPh->Mass() : -100.);
  Float_t _tPhHoE  = ( _tPh ? _tPh->HadOverEm() : -100.);
  Float_t _tPhR9   = ( _tPh ? _tPh->R9() : -100.);
  Float_t _tPhTiso    = ( _tPh ? _tPh->HollowConeTrkIsoDr04() : -100. );
  Float_t _tPhNTiso    = ( _tPh ? getTrackIso(_tPh, bsp) : -100. );
  Float_t _tPhEiso    = ( _tPh ? _tPh->EcalRecHitIsoDr04() : -100. );
  Float_t _tPhHiso    = ( _tPh ? _tPh->HcalTowerSumEtDr04() : -100. );
  Float_t _tPhIso     = ( _tPh ? _tPhTiso+_tPhEiso+_tPhHiso : -100. );
  Float_t _tPhTiso3    = ( _tPh ? _tPh->HollowConeTrkIsoDr03() : -100. );
  Float_t _tPhNTiso3    = ( _tPh ? getTrackIso3(_tPh, bsp) : -100. );
  Float_t _tPhEiso3   = ( _tPh ? _tPh->EcalRecHitIsoDr03() : -100. );
  Float_t _tPhHiso3    = ( _tPh ? _tPh->HcalTowerSumEtDr03() : -100. );
  Float_t _tPhIso3     = ( _tPh ? _tPhTiso3+_tPhEiso3+_tPhHiso3 : -100. );
  Float_t _tPhSEE     = ( _tPh ? _tPh->SCluster()->Seed()->CoviEtaiEta(): -100.);

  Float_t _tElEt   = ( _tEl ? _tEl->Et() : -1.);
  Float_t _tElEta  = ( _tEl ? _tEl->Eta() : -100.);
  Float_t _tElPhi  = ( _tEl ? _tEl->Phi() : -100.);
  Float_t _tElE    = ( _tEl ? _tEl->E() : -100.);
  Float_t _tElpz   = ( _tEl ? _tEl->Pz() : -100.);
  Float_t _tElpt   = ( _tEl ? _tEl->Pt() : -100.);
  Float_t _tElm    = ( _tEl ? _tEl->Mass() : -100.);
  
  Float_t _mass    = ( _tEl && _tPh ? (_tEl->Mom()+_tPh->Particle::Mom()).M() : -100.);

  Float_t fillEvent[] = { Float_t(fIsData),
			  _tPhEt  ,
			  _tPhEta ,
			  _tPhPhi ,
			  _tPhE   ,
			  _tPhpz  ,
			  _tPhpt  ,
			  _tPhm   ,
			  _tPhHoE ,
			  _tPhR9  ,
			  _tPhIso ,
			  _tPhTiso,
			  _tPhNTiso,
			  _tPhEiso,
			  _tPhHiso,
			  _tPhIso3 ,
			  _tPhTiso3,
			  _tPhNTiso3,
			  _tPhEiso3,
			  _tPhHiso3,
			  _tPhSEE ,
			  _tElEt  ,
			  _tElEta ,
			  _tElPhi ,
			  _tElE   ,
			  _tElpz  ,
			  _tElpt  ,
			  _tElm   ,
			  (Float_t) numPU,
			  (Float_t) numPV,
			  (Float_t) numPVDA,
			  (Float_t) numPVBS,
			  _tRho,
			  _mass
  };
  
  hPhIsoAreaTuple->Fill(fillEvent);

  return;
}

//--------------------------------------------------------------------------------------------------
void PhotonIsoAreaMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches or objects created by earlier modules.


  ReqEventObject(fPhotonName,fPhotons,fPhotonsFromBranch);
  ReqEventObject(fElectronName,fElectrons,fElectronsFromBranch);
  ReqEventObject(fPVName,   fPV,   fPVFromBranch);
  ReqEventObject(fPVNameDA, fPVDA, fPVFromBranchDA);
  ReqEventObject(fPVNameBS, fPVBS, fPVFromBranchBS);

  // Track Collection
  ReqBranch(fTrackName,fTracks);
  ReqBranch("BeamSpot",fBeamspot);

  // for MC only to adjust potential overlaps from generation
  if (! fIsData) {
    ReqBranch(Names::gkMCEvtInfoBrn,fMcEventInfo);
    printf(" Monte Carlo Information block %p\n",(void*) fMcEventInfo);
    printf(" --> this is no data. Access the McEventInfo.\n\n");
    ReqBranch(Names::gkMCPartBrn,fMCParticles);
    ReqBranch(fPileUpName, fPileUp);
  }
  else
    printf(" --> this is data. Drop the McEventInfo.\n\n");
  
  ReqBranch(fPileUpDenName,fPileUpDen);
  ReqBranch("PFCandidates",fCands);

  
  hPhIsoAreaTuple = new TNtuple("hPhEffTuple","hPhEffTuple","isData:phEt:phEta:phPhi:phE:phPz:phPtt:phM:phHoE:phR9:phIso:phTiso:phNTiso:phEiso:phHiso:phIso3:phTiso3:phNTiso3:phEiso3:phHiso3:phSEE:elEt:elEta:elPhi:elE:elPz:elPt:elM:numPU:numPV:numPVDA:numPVBS:Rho:invMass");
  
  AddOutput(hPhIsoAreaTuple);

}

//--------------------------------------------------------------------------------------------------
void PhotonIsoAreaMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void PhotonIsoAreaMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void PhotonIsoAreaMod::ResetSelectedPhotons() {
  fSelectedPhotons.clear();
  fTrigTightPhotons.clear();
  fTrigLoosePhotons.clear();
  return;
}

//--------------------------------------------------------------------------------------------------
void PhotonIsoAreaMod::ResetSelectedElectrons() {
  fSelectedElectrons.clear();
  fTrigElectrons.clear();
  return;
}



//--------------------------------------------------------------------------------------------------
void PhotonIsoAreaMod::MatchObjectsToTrigger()
{
  
  // access all trigger objects
  const TriggerObjectCol *tos = GetHLTObjects(fTrigObjsName);
  if (! tos)             // this can only happen if HLTMod::SetAbortIfNotAccepted(kFALSE) was called
    return;

  UInt_t nEnts = tos->GetEntries();
  // loop through our selected photons
  Bool_t foundMatch=false;
  for (UInt_t i=0; i<fSelectedElectrons.size(); i++) {
    const Electron *p = fElectrons->At(fSelectedElectrons[i]);
    for (UInt_t j=0; j<nEnts; ++j) {
      const TriggerObject *to = tos->At(j);
      if (fabs(p->Eta()-to->Eta())          < 0.10 &&
          fabs(p->Phi()-to->Phi())          < 0.05 &&
          fabs((p->Pt() -to->Pt())/p->Pt()) < 0.05   ) {
	TString _tight  = "hltEle32CaloIdLCaloIsoVLSC17PixelMatchFilter";
	if(!strcmp(to->ModuleName(),_tight.Data())) {
	  fTrigElectrons.push_back(fSelectedElectrons[i]);
	  foundMatch=true;
	}
      }
    }
    if(foundMatch) break;
  }
  
  return;
}

//--------------------------------------------------------------------------------------------------
UInt_t PhotonIsoAreaMod::PhotonTriggered(UInt_t iPhoton)
{
  // Determine whether the requested photon index is in the triggered photon indices

  // per default the photon did not trigger
  UInt_t triggered = 0;

  // loop through loose leg triggered photon indices
  for (UInt_t i=0; i<fTrigLoosePhotons.size(); i++) {
    if (fTrigLoosePhotons[i] == iPhoton) {
      triggered += 1;
      break;
    }
  }

  
  // loop through triggered photon indices
  for (UInt_t i=0; i<fTrigTightPhotons.size(); i++) {
    if (fTrigTightPhotons[i] == iPhoton) {
      triggered += 2;
      break;
    }
  }
  
  return triggered;
}

//--------------------------------------------------------------------------------------------------
Double_t PhotonIsoAreaMod::getTrackIso(const Photon* p, const BaseVertex* bsp) {
  
  // loop over all tracks
  Double_t tPt = 0.;
  for(UInt_t i=0; i<fTracks->GetEntries(); ++i) {
    const Track* t = fTracks->At(i);
    if ( ! t->Quality().Quality(TrackQuality::highPurity ) || fabs(t->D0Corrected( *bsp) ) > 0.1) continue;
    Double_t dR=MathUtils::DeltaR(t->Mom(),p->Particle::Mom());
    Double_t dEta = fabs(t->Eta()-p->Eta());
    if(dR < 0.4 && dR > 0.04 && dEta > 0.015) tPt += t->Pt();
  }
  return tPt;
}


Double_t PhotonIsoAreaMod::getTrackIso3(const Photon* p, const BaseVertex* bsp) {
  
  // loop over all tracks
  Double_t tPt = 0.;
  for(UInt_t i=0; i<fTracks->GetEntries(); ++i) {
    const Track* t = fTracks->At(i);
    if ( ! t->Quality().Quality(TrackQuality::highPurity ) || fabs(t->D0Corrected( *bsp) ) > 0.1) continue;
    Double_t dR=MathUtils::DeltaR(t->Mom(),p->Particle::Mom());
    Double_t dEta = fabs(t->Eta()-p->Eta());
    if(dR < 0.3 && dR > 0.04 && dEta > 0.015) tPt += t->Pt();
  }
  return tPt;
}
