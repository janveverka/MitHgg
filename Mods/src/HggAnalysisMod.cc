// $Id: HggAnalysisMod.cc,v 1.1 2011/06/08 14:27:54 fabstoec Exp $

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
#include "MitHgg/Mods/interface/HggAnalysisMod.h"
#include "MitAna/DataTree/interface/BeamSpotCol.h"
#include "MitAna/DataTree/interface/PFCandidateCol.h"

#include "MitPhysics/Utils/interface/IsolationTools.h"

using namespace mithep;

ClassImp(mithep::HggAnalysisMod)

//--------------------------------------------------------------------------------------------------
HggAnalysisMod::HggAnalysisMod(const char *name, const char *title) :
  BaseMod             (name,title),
  // initialize bambu objects
  fMcEventInfoName    (Names::gkMCEvtInfoBrn),

  fTrackName (Names::gkTrackBrn),
  fTracks(0),

  fTrigObjsName       (Names::gkHltObjBrn),
  fPVName (Names::gkPVBrn),
  fPV(0),
  fPVFromBranch(kTRUE),

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

  fConversions(0),

  fIsData             (kTRUE),
  fSkimming           (kFALSE),

  // scale removal
  fOverlapCut         (-1.0),
  // cuts for selection
  fPhotonEtMin        (10.0),

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
void HggAnalysisMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void HggAnalysisMod::Process()
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
  LoadEventObject(fPVName, fPV);
  LoadBranch(fPileUpDenName);
  LoadBranch("PFCandidates");
  LoadBranch(fTrackName);
  LoadBranch("BeamSpot");
  
  //----------------------------------------------------------------------------------
  
  Float_t numPU = -1.;
  Float_t numPV = -1.;
  if( !fIsData )
    numPU = (Float_t) fPileUp->At(0)->GetPU_NumInteractions();  
  
  Float_t _tRho = (Float_t) fPileUpDen->At(0)->Rho();
  
  const BaseVertex *bsp = dynamic_cast<const BaseVertex*>(fBeamspot->At(0));    

  /* -------------------------  
     Photon Selection
     We use all the photons coming as from the branch.
     To stear their selection (i.e. RECO/ID/ISO) use the PhotonIDMod.  */
  ResetSelectedPhotons();
  for (UInt_t i=0; i<fPhotons->GetEntries(); ++i) {
    const Photon *p = fPhotons->At(i);
    if (p->Et() > fPhotonEtMin) {
      fSelectedPhotons.push_back(i);
    }
  }
  
  if(fSelectedPhotons.size() < 2 ) return;
  
  MatchPhotonsToTrigger();

  // if we don't have both guys triggered (in Data!) return
  if ( fIsData ) {
    if(fTrigTightPhotons.size()<1 || fTrigLoosePhotons.size()<2) return;
  }
  
  // find the primary vertex (for now take the first one, later we should take the one 'mathing' the di-muon system
  if( fPV->GetEntries() == 0 ) return;  // NO PV found... ???
  numPV = (Float_t) fPV->GetEntries();
  
  // compute SuperCluster momentum (assuming zero mass)
  const BaseVertex* vtx = NULL ;

  Int_t tVtx = findClosestToTruth(fPV, fPhotons->At(fSelectedPhotons[0]));
  if ( tVtx > -1 ) vtx = dynamic_cast<const BaseVertex*>(fPV->At(tVtx));

  const Photon* _tPh1 = NULL;
  const Photon* _tPh2 = NULL;
  
  if( fIsData ) {
    if(fSelectedPhotons.size() > 0) {
      _tPh1 = fPhotons->At(fTrigTightPhotons[0]);
    }
    if(fSelectedPhotons.size() > 1) {
      _tPh2 = fPhotons->At(fTrigLoosePhotons[1]);
    }    
  } else {
    if(fSelectedPhotons.size() > 0) {
      _tPh1 = fPhotons->At(fSelectedPhotons[0]);
    }
    if(fSelectedPhotons.size() > 1) {
      _tPh2 = fPhotons->At(fSelectedPhotons[1]);
    }
  }

  Float_t _tPh1Et   = ( _tPh1 ? _tPh1->Et() : -1.);
  Float_t _tPh1Eta  = ( _tPh1 ? _tPh1->Eta() : -100.);
  Float_t _tPh1Phi  = ( _tPh1 ? _tPh1->Phi() : -100.);
  Float_t _tPh1E    = ( _tPh1 ? _tPh1->E() : -100.);
  Float_t _tPh1pz   = ( _tPh1 ? _tPh1->Pz() : -100.);
  Float_t _tPh1pt   = ( _tPh1 ? _tPh1->Pt() : -100.);
  Float_t _tPh1m    = ( _tPh1 ? _tPh1->Mass() : -100.);
  Float_t _tPh1HoE  = ( _tPh1 ? _tPh1->HadOverEm() : -100.);
  Float_t _tPh1R9   = ( _tPh1 ? _tPh1->R9() : -100.);
  Float_t _tPh1Tiso    = ( _tPh1 ? _tPh1->HollowConeTrkIsoDr04() : -100. );
  Float_t _tPh1NTiso    = ( _tPh1 ? IsolationTools::TrackIsolationNoPV(_tPh1, bsp, 0.4, 0.04, 0.0, 0.015, 0.1, TrackQuality::highPurity, fTracks ) : -100. );
  Float_t _tPh1JoshTiso    = ( _tPh1 ? IsolationTools::TrackIsolationNoPV(_tPh1, bsp, 0.4, 0.0, 0.0, 0.0, 0.1, TrackQuality::highPurity, fTracks, 0, fConversions) : -100. );
  Float_t _tPh1Tiso3    = ( _tPh1 ? _tPh1->HollowConeTrkIsoDr03() : -100. );
  Float_t _tPh1NTiso3    = ( _tPh1 ? IsolationTools::TrackIsolationNoPV(_tPh1, bsp, 0.3, 0.04, 0.0, 0.015, 0.1, TrackQuality::highPurity, fTracks ) : -100. );
  Float_t _tPh1JoshTiso3    = ( _tPh1 ? IsolationTools::TrackIsolationNoPV(_tPh1, bsp, 0.3, 0.0, 0.0, 0.0, 0.1, TrackQuality::highPurity, fTracks, 0, fConversions) : -100. );
  Float_t _tPh1Eiso    = ( _tPh1 ? _tPh1->EcalRecHitIsoDr04() : -100. );
  Float_t _tPh1Hiso    = ( _tPh1 ? _tPh1->HcalTowerSumEtDr04() : -100. );
  Float_t _tPh1Eiso3    = ( _tPh1 ? _tPh1->EcalRecHitIsoDr03() : -100. );
  Float_t _tPh1Hiso3    = ( _tPh1 ? _tPh1->HcalTowerSumEtDr03() : -100. );
  Float_t _tPh1SEE     = ( _tPh1 ? _tPh1->SCluster()->Seed()->CoviEtaiEta(): -100.);
  Float_t _tPh2Et   = ( _tPh2 ? _tPh2->Et() : -1.);
  Float_t _tPh2Eta  = ( _tPh2 ? _tPh2->Eta() : -100.);
  Float_t _tPh2Phi  = ( _tPh2 ? _tPh2->Phi() : -100.);
  Float_t _tPh2E    = ( _tPh2 ? _tPh2->E() : -100.);
  Float_t _tPh2pz   = ( _tPh2 ? _tPh2->Pz() : -100.);
  Float_t _tPh2pt   = ( _tPh2 ? _tPh2->Pt() : -100.);
  Float_t _tPh2m    = ( _tPh2 ? _tPh2->Mass() : -100.);
  Float_t _tPh2HoE  = ( _tPh2 ? _tPh2->HadOverEm() : -100.);
  Float_t _tPh2R9   = ( _tPh2 ? _tPh2->R9() : -100.);
  Float_t _tPh2Tiso    = ( _tPh2 ? _tPh2->HollowConeTrkIsoDr04() : -100. );
  Float_t _tPh2NTiso    = ( _tPh2 ? IsolationTools::TrackIsolationNoPV(_tPh2, bsp, 0.4, 0.04, 0.0, 0.015, 0.1, TrackQuality::highPurity, fTracks ) : -100. );
  Float_t _tPh2JoshTiso    = ( _tPh2 ? IsolationTools::TrackIsolationNoPV(_tPh2, bsp, 0.4, 0.0, 0.0, 0.0, 0.1, TrackQuality::highPurity, fTracks, 0, fConversions) : -100. );
  Float_t _tPh2Tiso3    = ( _tPh2 ? _tPh2->HollowConeTrkIsoDr03() : -100. );
  Float_t _tPh2NTiso3    = ( _tPh2 ? IsolationTools::TrackIsolationNoPV(_tPh2, bsp, 0.3, 0.04, 0.0, 0.015, 0.1, TrackQuality::highPurity, fTracks ) : -100. );
  Float_t _tPh2JoshTiso3    = ( _tPh2 ? IsolationTools::TrackIsolationNoPV(_tPh2, bsp, 0.3, 0.0, 0.0, 0.0, 0.1, TrackQuality::highPurity, fTracks, 0, fConversions) : -100. );
  Float_t _tPh2Eiso    = ( _tPh2 ? _tPh2->EcalRecHitIsoDr04() : -100. );
  Float_t _tPh2Hiso    = ( _tPh2 ? _tPh2->HcalTowerSumEtDr04() : -100. );
  Float_t _tPh2Eiso3    = ( _tPh2 ? _tPh2->EcalRecHitIsoDr03() : -100. );
  Float_t _tPh2Hiso3    = ( _tPh2 ? _tPh2->HcalTowerSumEtDr03() : -100. );
  Float_t _tPh2SEE     = ( _tPh2 ? _tPh2->SCluster()->Seed()->CoviEtaiEta() : -100.);
  
  Float_t _hmass       = ( _tPh1 && _tPh2 ? (_tPh1->Particle::Mom()+_tPh2->Particle::Mom()).M() : -100. );
  
  Float_t _pth = -100.;
  if( !fIsData ) _pth = findHiggsPt();
  
  Float_t fillEvent[] = { Float_t(fIsData),
			  _hmass   ,
			  _tPh1Et  ,
			  _tPh1Eta ,
			  _tPh1Phi ,
			  _tPh1E   ,
			  _tPh1pz  ,
			  _tPh1pt  ,
			  _tPh1m   ,
			  _tPh1HoE ,
			  _tPh1R9  ,
			  _tPh1Tiso,
			  _tPh1NTiso,
			  _tPh1JoshTiso,
			  _tPh1Tiso3,
			  _tPh1NTiso3,
			  _tPh1JoshTiso3,
			  _tPh1Eiso,
			  _tPh1Hiso,
			  _tPh1Eiso3,
			  _tPh1Hiso3,
			  _tPh1SEE ,
			  _tPh2Et  ,
			  _tPh2Eta ,
			  _tPh2Phi ,
			  _tPh2E   ,
			  _tPh2pz  ,
			  _tPh2pt  ,
			  _tPh2m   ,
			  _tPh2HoE ,
			  _tPh2R9  ,
			  _tPh2Tiso,
			  _tPh2NTiso,
			  _tPh2JoshTiso,
			  _tPh2Tiso3,
			  _tPh2NTiso3,
			  _tPh2JoshTiso3,
			  _tPh2Eiso,
			  _tPh2Hiso,
			  _tPh2Eiso3,
			  _tPh2Hiso3,
			  _tPh2SEE ,
			  (Float_t) numPU,
			  (Float_t) numPV,
			  _tRho,
			  _pth
  };
  
  
  hHggAnaTuple->Fill(fillEvent);
  
  return;
}

//--------------------------------------------------------------------------------------------------
void HggAnalysisMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches or objects created by earlier modules.


  ReqEventObject(fPhotonName,fPhotons,fPhotonsFromBranch);
  ReqEventObject(fPVName, fPV, fPVFromBranch);

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

  ReqBranch("MergedConversions",fConversions);
  
  hHggAnaTuple = new TNtuple("hHggAnaTuple","hHggAnaTuple","isData:hmass:ph1et:ph1Eta:ph1phi:ph1E:ph1pz:ph1pt:ph1m:ph1HoE:ph1R9:ph1Tiso:ph1NTiso:ph1JoshTIso:ph1Tiso3:ph1NTiso3:ph1JoshTIso3:ph1Eiso:ph1Hiso:ph1Eiso3:ph1Hiso3:ph1SEE:ph2et:ph2Eta:ph2phi:ph2E:ph2pz:ph2pt:ph2m:ph2HoE:ph2R9:ph2Tiso:ph2NTiso:ph2JoshTIso:ph2Tiso3:ph2NTiso3:ph2JoshTIso3:ph2Eiso:ph2Hiso:ph2Eiso3:ph2Hiso3:ph2SEE:numPU:numPV:Rho:pth");
  
  AddOutput(hHggAnaTuple);

}

//--------------------------------------------------------------------------------------------------
void HggAnalysisMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void HggAnalysisMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void HggAnalysisMod::ResetSelectedPhotons() {
  fSelectedPhotons.clear();
  fTrigTightPhotons.clear();
  fTrigLoosePhotons.clear();
  return;
}


//--------------------------------------------------------------------------------------------------
void HggAnalysisMod::MatchPhotonsToTrigger()
{
  // Reset the selected and trigger matched photons

  // access all trigger objects
  const TriggerObjectCol *tos = GetHLTObjects(fTrigObjsName);
  if (! tos)             // this can only happen if HLTMod::SetAbortIfNotAccepted(kFALSE) was called
    return;
  UInt_t nEnts = tos->GetEntries();
  
  // loop through our selected photons
  for (UInt_t i=0; i<fSelectedPhotons.size(); i++) {
    const Photon *p = fPhotons->At(fSelectedPhotons[i]);
    UInt_t matched = 0;
    // loop through all trigger objects and try to find a match
    for (UInt_t j=0; j<nEnts; ++j) {
      const TriggerObject *to = tos->At(j);
      Double_t delR = MathUtils::DeltaR(p->SCluster()->Point(),to->Mom());
      if( delR < 0.2 ) {
        matched++;
	TString _tight  = "hltPhoton26IsoVLTrackIsoFilter";
	TString _loose  = "hltDoubleIsoEG18HELastFilter";
	if(!strcmp(to->ModuleName(),_tight.Data())) {
	  fTrigTightPhotons.push_back(fSelectedPhotons[i]);
	}
	if(!strcmp(to->ModuleName(),_loose.Data())) {
	  fTrigLoosePhotons.push_back(fSelectedPhotons[i]);
	}
      }
    }
    // add to our trigger collection if it was matched
    if (matched > 0) {      
      fTrigTightPhotons.push_back(fSelectedPhotons[i]);
      // print warning if desired
    }
    if (matched > 1 && false) {
      MDB(kGeneral,1) {
	//printf(" Ambiguos match found: %d\n",matched);
	//printf(" Photon:  %8.3f %8.3f %8.3f\n",p->Eta(), p->Phi(), p->Pt());
	for (UInt_t j=0; j<nEnts; ++j) {
	  const TriggerObject *to = tos->At(j);
	  //printf(" TrigObj: %8.3f %8.3f %8.3f\n",to->Eta(),to->Phi(),to->Pt());
	  }
      }
    }
  }
  
  return;
}

//--------------------------------------------------------------------------------------------------
UInt_t HggAnalysisMod::PhotonTriggered(UInt_t iPhoton)
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
Int_t HggAnalysisMod::findClosestToTruth(const VertexCol* vtxs, const Photon* p) {
  
  if( fIsData ) return -1;
  if( vtxs->GetEntries() == 0 ) return -1;
  if( !p ) return -1;
  
  // find match with photon
  const MCParticle* theM = NULL;
  for(UInt_t k=0; k<fMCParticles->GetEntries(); ++k) {    
    const MCParticle* mc = fMCParticles->At(k);
    if( mc->Status() != 1 || !(mc->Is(MCParticle::kGamma))) continue;
    if(fabs((p->Pt()-mc->Pt())/p->Pt()) > 0.5) continue;
    if(fabs(p->Eta()-mc->Eta()) > 0.1) continue;
    if(fabs(p->Phi()-mc->Phi()) > 0.1) continue;
    theM = mc;
    break;
  }
  if( !theM ) return 0;
  
  // check if it's FSR from muon
  const MCParticle* mother = theM->Mother();
  while (mother->Is(MCParticle::kGamma) ) mother = mother->Mother();
  BaseVertex MCvtx(mother->DecayVertex());
  
  Int_t closeInd = -1;
  Double_t closeDZ = 1000000.;
  
  for(UInt_t i =0; i < vtxs->GetEntries(); ++i) {
    const BaseVertex* vtx = vtxs->At(i);
    Double_t delZ = fabs(vtx->Z() - MCvtx.Z());
    if(delZ < closeDZ) {
      closeDZ = delZ;
      closeInd = i;
    }
  }

  //std::cout<<" closes Vtx is dZ = "<<closeDZ<<std::endl;
  return closeInd;
  
}

double HggAnalysisMod::findHiggsPt() {

  // loop over all GEN particles and look for status 1 photons
  for(UInt_t i=0; i<fMCParticles->GetEntries(); ++i) {
    const MCParticle* p = fMCParticles->At(i);
    if( !(p->Is(MCParticle::kH)) ) continue;
    return p->Pt();
  }

  return -1.0;
 }
