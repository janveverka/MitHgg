// $Id: PhSCEffMod.C,v 1.2 2011/04/28 19:40:54 fabstoec Exp $

#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TNtuple.h>
#include <TRandom3.h>
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataTree/interface/MCEventInfo.h"
#include "MitAna/DataTree/interface/MCParticleCol.h"
#include "MitAna/DataTree/interface/MuonCol.h"
#include "MitAna/DataTree/interface/TrackCol.h"
#include "MitAna/DataTree/interface/Vertex.h"
#include "MitAna/DataTree/interface/Photon.h"
#include "MitHgg/Mods/interface/PhSCEffMod.h"
#include "MitAna/DataTree/interface/BeamSpotCol.h"
#include "MitAna/DataTree/interface/PFCandidateCol.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "MitPhysics/Utils/interface/IsolationTools.h"

using namespace mithep;

ClassImp(mithep::PhSCEffMod)

//--------------------------------------------------------------------------------------------------
PhSCEffMod::PhSCEffMod(const char *name, const char *title) :
  BaseMod             (name,title),
  // initialize bambu objects
  fMcEventInfoName    (Names::gkMCEvtInfoBrn),

  fTrackName (Names::gkTrackBrn),
  fTracks(0),

  fMCParticleName(Names::gkMCPartBrn),
  fMCParticles(0),

  fBarrelSCName       (Names::gkBarrelSuperClusterBrn),
  fBarrelSC(0),
  fEndcapSCName       (Names::gkEndcapSuperClusterBrn),
  fEndcapSC(0),

  fTrigObjsName       (Names::gkHltObjBrn),

  fPVName("PrimaryVertexes"),
  fPV(0),
  fPVFromBranch(kTRUE),

  fElectronName       (Names::gkElectronBrn),
  fElectrons(0),
  fElectronsFromBranch(kTRUE),


  fPhotonName         (Names::gkPhotonBrn),
  fPhotons(0),
  fPhotonsFromBranch(kTRUE),
  
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
  fElectronEtMin      (30.0),

  // SC selection
  fSCEtMin (25.),
  fSCBarrelEtaMax (1.4442),
  fSCEndcapEtaMax (2.5),
  fSCEndcapEtaMin (1.556)

{
  
  rng = new TRandom3(0);

  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void PhSCEffMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void PhSCEffMod::Process()
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
  LoadBranch(fPileUpDenName);
  LoadBranch(fTrackName);
  LoadBranch("BeamSpot");
  
  LoadBranch(fBarrelSCName);
  LoadBranch(fEndcapSCName);

  //----------------------------------------------------------------------------------
  
  Float_t numPU = -1.;
  Float_t numPV = -1.;

  const Electron* _tEl = NULL;
  if( !fIsData )
    numPU = (Float_t) fPileUp->At(0)->GetPU_NumInteractions();  
  Float_t _tRho = (Float_t) fPileUpDen->At(0)->Rho();  
  const BaseVertex *bsp = dynamic_cast<const BaseVertex*>(fBeamspot->At(0));    

  // find all the SC
  ResetSelectedSC();
  for(UInt_t i=0; i<fBarrelSC->GetEntries(); ++i) {
    const SuperCluster *sc = fBarrelSC->At(i);
    if ( fabs(sc->Eta()) > fSCBarrelEtaMax ||
	 sc->Et() < fSCEtMin ) continue;    
    fSelectedSCEB.push_back(i);
  }

  for(UInt_t i=0; i<fEndcapSC->GetEntries(); ++i) {
    const SuperCluster *sc = fEndcapSC->At(i);
    if ( fabs(sc->Eta()) > fSCEndcapEtaMax ||
 	 fabs(sc->Eta()) < fSCEndcapEtaMin ||
 	 sc->Et() < fSCEtMin ) continue;
    fSelectedSCEE.push_back(i);
  }
  
  if (fIsData)
    MatchSCToTrigger();
  else {
    for (UInt_t i=0; i<fSelectedSCEB.size(); ++i) {
      fTrigSCEB.push_back(fSelectedSCEB[i]);
    }
    for (UInt_t i=0; i<fSelectedSCEE.size(); ++i) {
      fTrigSCEE.push_back(fSelectedSCEE[i]);
    }
  }


  if(fTrigSCEB.size()+fTrigSCEE.size() !=2 ) return;
  
  ResetSelectedPhotons();
  for (UInt_t i=0; i<fPhotons->GetEntries(); ++i) {
    const Photon *p = fPhotons->At(i);
    if (p->Et() > fPhotonEtMin) {
      fSelectedPhotons.push_back(i);
    }
  }
  
  /* Run over the electrons a select them. We need at least one. */
  ResetSelectedElectrons();
  for (UInt_t i=0; i<fElectrons->GetEntries(); ++i) {
    const Electron *p = fElectrons->At(i);  
    if (p->Et() > fElectronEtMin) {
      fSelectedElectrons.push_back(i);
    }
  }
  
  if(fSelectedElectrons.size() == 0) return;
  
  // match the electrons to the Electron Triggers
  if (fIsData)
    MatchElectronsToTightTrigger();
  else {
    for (UInt_t i=0; i<fSelectedElectrons.size(); ++i) {
      fTrigElectrons.push_back(fSelectedElectrons[i]);
    }
  }

  if(fTrigElectrons.size() == 0 ) return;      

  // decide which one is tag
  const SuperCluster* tTag = NULL;
  const SuperCluster* tProbe = NULL;
  if(fTrigSCEB.size() > 0) {
    tTag = fBarrelSC->At(fTrigSCEB[0]);
    if(fTrigSCEB.size() > 1)
      tProbe = fBarrelSC->At(fTrigSCEB[1]);
    else {
      if(fTrigSCEE.size() > 0)
	tProbe = fEndcapSC->At(fTrigSCEE[0]);	
    }
  } else {
    if(fTrigSCEE.size() > 0) {
      tTag = fEndcapSC->At(fTrigSCEE[0]);
      if(fTrigSCEE.size() > 1) {
	tProbe = fEndcapSC->At(fTrigSCEE[1]);
      }
    }
  }
  
  if(!tTag || !tProbe) {
    std::cout<<" ERROR "<<std::endl;
    return;
  }

  if (rng->Uniform() < 0.5) {
    const SuperCluster* tmp = tProbe;
    tProbe = tTag;
    tTag   = tmp;
  }
  
  // test the tag to pass electron selections
  bool tagPassEleOfflineAndTightEleLeg = false;
  for(UInt_t kk=0; kk<fTrigElectrons.size(); ++kk) {
    if(tTag == fElectrons->At(fTrigElectrons[kk])->SCluster()) {
      _tEl = fElectrons->At(fTrigElectrons[kk]);
      tagPassEleOfflineAndTightEleLeg = true;
      break;
    }
  }
  
  if (!tagPassEleOfflineAndTightEleLeg) return;

  // check iof the probe is a ID'd Photon
  Int_t probePass = -1;
  for(UInt_t kk=0; kk<fSelectedPhotons.size(); ++kk) {
    if(tProbe == fPhotons->At(fSelectedPhotons[kk])->SCluster()) {
      probePass = true;
      break;
    }
  }
  

  // find the primary vertex (for now take the first one, later we should take the one 'mathing' the di-muon system
  if( fPV->GetEntries() == 0 ) return;  // NO PV found... ???
  numPV   = (Float_t) fPV  ->GetEntries();

  // compute SuperCluster momentum (assuming zero mass)
  const Vertex* vtx = fPV->At(0);
  FourVectorM tProbe_M;  
  ThreeVectorC tProbe_P  = tProbe->Point() - vtx->Position();
  tProbe_P = tProbe_P/tProbe_P.R();
  tProbe_M.SetXYZT(tProbe->Energy()*tProbe_P.X(), tProbe->Energy()*tProbe_P.Y(), tProbe->Energy()*tProbe_P.Z(), tProbe->Energy());

  // compute the same for the electron
  FourVectorM tTag_M;  
  ThreeVectorC tTag_P  = tTag->Point() - vtx->Position();
  tTag_P = tTag_P/tTag_P.R();
  tTag_M.SetXYZT(tTag->Energy()*tTag_P.X(), tTag->Energy()*tTag_P.Y(), tTag->Energy()*tTag_P.Z(), tTag->Energy());

  Float_t _pth = -1;
  if( !fIsData )
    _pth = findHiggsPt();
  
  const SuperCluster*   _tPh = tProbe;
  
  Float_t _tPhEt   = tProbe_M.Et() ;
  Float_t _tPhEta  = tProbe_M.Eta();
  Float_t _tPhPhi  = tProbe_M.Phi();
  Float_t _tPhE    = tProbe_M.E()  ;
  Float_t _tPhpz   = tProbe_M.Pz() ;
  Float_t _tPhpt   = tProbe_M.Pt() ;
  Float_t _tPhm    = tProbe_M.M()  ;
  Float_t _tPhSEE  = _tPh->Seed()->CoviEtaiEta();

  Float_t _tElEt   = ( _tEl ? _tEl->Et() : -1.);
  Float_t _tElEta  = ( _tEl ? _tEl->Eta() : -100.);
  Float_t _tElPhi  = ( _tEl ? _tEl->Phi() : -100.);
  Float_t _tElE    = ( _tEl ? _tEl->E() : -100.);
  Float_t _tElpz   = ( _tEl ? _tEl->Pz() : -100.);
  Float_t _tElpt   = ( _tEl ? _tEl->Pt() : -100.);
  Float_t _tElm    = ( _tEl ? _tEl->Mass() : -100.);
  
  Float_t _mass    = ( _tEl ? (_tEl->Mom()+tProbe_M).M() : -100.);
  Float_t _mass2   = (tTag_M+tProbe_M).M();
  
  Float_t fillEvent[] = { Float_t(fIsData),
			  _tPhEt  ,
			  _tPhEta ,
			  _tPhPhi ,
			  _tPhE   ,
			  _tPhpz  ,
			  _tPhpt  ,
			  _tPhm   ,
			  _tPhSEE ,
			  (Float_t) probePass,
			  _tElEt  ,
			  _tElEta ,
			  _tElPhi ,
			  _tElE   ,
			  _tElpz  ,
			  _tElpt  ,
			  _tElm   ,
			  (Float_t) numPU,
			  (Float_t) numPV,
			  _tRho,
			  _mass,
			  _mass2,
			  _pth
  };
  
  hPhTrigEffTuple->Fill(fillEvent);

  return;
}

//--------------------------------------------------------------------------------------------------
void PhSCEffMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches or objects created by earlier modules.


  ReqEventObject(fPhotonName,fPhotons,fPhotonsFromBranch);
  ReqEventObject(fElectronName,fElectrons,fElectronsFromBranch);
  ReqEventObject(fPVName,   fPV,   fPVFromBranch);

  // SC collections
  ReqBranch(fBarrelSCName,fBarrelSC);
  ReqBranch(fEndcapSCName,fEndcapSC);

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
  
  hPhTrigEffTuple = new TNtuple("hPhEffTuple","hPhEffTuple","isData:phEt:phEta:phPhi:phE:phPz:phPtt:phM:phSEE:phTrig:elEt:elEta:elPhi:elE:elPz:elPt:elM:numPU:numPV:Rho:invMass:invMass2:pth");
  
  AddOutput(hPhTrigEffTuple);

}

//--------------------------------------------------------------------------------------------------
void PhSCEffMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void PhSCEffMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void PhSCEffMod::ResetSelectedPhotons() {
  fSelectedPhotons.clear();
  fTrigPhotons.clear();
  return;
}

//--------------------------------------------------------------------------------------------------
void PhSCEffMod::ResetSelectedSC() {
  fSelectedSCEB.clear();
  fSelectedSCEE.clear();
  fTrigSCEB.clear();
  fTrigSCEE.clear();
  return;
}


//--------------------------------------------------------------------------------------------------
void PhSCEffMod::ResetSelectedElectrons() {
  fSelectedElectrons.clear();
  fTrigElectrons.clear();
  return;
}


//--------------------------------------------------------------------------------------------------

void PhSCEffMod::MatchElectronsToTightTrigger()
{
  const TriggerObjectCol *tos = GetHLTObjects(fTrigObjsName);
  
  if (! tos) return;             // this can only happen if HLTMod::SetAbortIfNotAccepted(kFALSE) was called

    
  UInt_t nEnts = tos->GetEntries();
  for (UInt_t i=0; i<fSelectedElectrons.size(); i++) {
    const Electron *p = fElectrons->At(fSelectedElectrons[i]);
    for (UInt_t j=0; j<nEnts; ++j) {
      const TriggerObject *to = tos->At(j);
      Double_t delR = MathUtils::DeltaR(p->SCluster()->Point(),to->Mom());
      if( delR < 0.2 ) {
	TString _tight  = "hltEle32CaloIdLCaloIsoVLSC17PixelMatchFilter";
	if(!strcmp(to->ModuleName(),_tight.Data())) {
	  fTrigElectrons.push_back(fSelectedElectrons[i]);
	}
      }
    }
  }
  return;
}


void PhSCEffMod::MatchSCToTrigger()
{
  const TriggerObjectCol *tos   = GetHLTObjects(fTrigObjsName);  
  if (! tos) return;             // this can only happen if HLTMod::SetAbortIfNotAccepted(kFALSE) was called
    
  UInt_t nEnts = tos->GetEntries();
  for (UInt_t i=0; i<fSelectedSCEB.size(); i++) {
    const SuperCluster *p = fBarrelSC->At(fSelectedSCEB[i]);
    for (UInt_t j=0; j<nEnts; ++j) {
      const TriggerObject *to = tos->At(j);
      Double_t delR = MathUtils::DeltaR(p->Point(),to->Mom());
      if( delR < 0.2 ) {
	TString _loose  = "hltEle32CaloIdLCaloIsoVLSC17HEDoubleFilter";
	if(!strcmp(to->ModuleName(),_loose.Data())) {
	  fTrigSCEB.push_back(fSelectedSCEB[i]);
	}
      }
    }
  }
  for (UInt_t i=0; i<fSelectedSCEE.size(); i++) {
    const SuperCluster *p = fEndcapSC->At(fSelectedSCEE[i]);
    for (UInt_t j=0; j<nEnts; ++j) {
      const TriggerObject *to = tos->At(j);
      Double_t delR = MathUtils::DeltaR(p->Point(),to->Mom());
      if( delR < 0.2 ) {
	TString _loose  = "hltEle32CaloIdLCaloIsoVLSC17HEDoubleFilter";
	if(!strcmp(to->ModuleName(),_loose.Data())) {
	  fTrigSCEE.push_back(fSelectedSCEE[i]);
	}
      }
    }
  }


  return;
}


double PhSCEffMod::findHiggsPt() {

  // loop over all GEN particles and look for status 1 photons
  for(UInt_t i=0; i<fMCParticles->GetEntries(); ++i) {
    const MCParticle* p = fMCParticles->At(i);
    if( !(p->Is(MCParticle::kH)) ) continue;
    return p->Pt();
  }

  return -1.0;
 }
