// $Id: PhotonIsoAreaMod.cc,v 1.1 2011/04/06 15:59:54 fabstoec Exp $

#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TNtuple.h>
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataTree/interface/TrackCol.h"
#include "MitAna/DataTree/interface/VertexCol.h"
#include "MitAna/DataTree/interface/PhotonCol.h"
#include "MitHgg/Mods/interface/PhotonIsoAreaMod.h"
#include "MitAna/DataTree/interface/BeamSpotCol.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "MitPhysics/Utils/interface/IsolationTools.h"

using namespace mithep;

ClassImp(mithep::PhotonIsoAreaMod)

//--------------------------------------------------------------------------------------------------
PhotonIsoAreaMod::PhotonIsoAreaMod(const char *name, const char *title) :
  BaseMod             (name,title),

  // define all the Branches to load
  fPhotonBranchName  (Names::gkPhotonBrn),
  fElectronName      (Names::gkElectronBrn),
  fTrackBranchName   (Names::gkTrackBrn),
  fPileUpDenName     (Names::gkPileupEnergyDensityBrn),
  fPVName            (Names::gkPVBeamSpotBrn),
  fBeamspotName      (Names::gkBeamSpotBrn),
  // MC specific stuff...
  fPileUpName        (Names::gkPileupInfoBrn),
  fTrigObjsName      (Names::gkHltObjBrn),

  // -------------------------------------------
  fPhotonEtMin        (30.0),
  fElectronEtMin      (40.0),

  // ----------------------------------------
  // collections....
  fPhotons           (0),
  fElectrons         (0),
  fTracks            (0),
  fPileUpDen         (0),
  fPV                (0),
  fBeamspot          (0),
  fPileUp            (0),

  
  // -------------------------------------------
  fIsData            (false),
  fPhotonsFromBranch (true),  
  fEleFromBranch     (true),
  fPVFromBranch      (true),

  // ----------------------------------------------------
  // this is Trigger specifics. We need (in DATA only) the tag&probe guyas to match the tight and loose legs
  fTightTriggerLeg    ("none"),
  fLooseTriggerLeg    ("none")

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

  // ------------------------------------------------------------  
  // Process entries of the tree. 
  LoadEventObject(fPhotonBranchName,   fPhotons);
  LoadEventObject(fElectronName,       fElectrons);
  LoadEventObject(fTrackBranchName,    fTracks);
  LoadEventObject(fPileUpDenName,      fPileUpDen);
  LoadEventObject(fPVName,             fPV);    
  LoadEventObject(fBeamspotName,       fBeamspot);

  if( !fIsData )
    LoadBranch(fPileUpName);
  
  //----------------------------------------------------------------------------------


  Float_t numPU = -1.;
  Float_t numPV = -1.;

  if( !fIsData ) numPU = (Float_t) fPileUp->At(0)->GetPU_NumInteractions();  
  Float_t _tRho = (Float_t) fPileUpDen->At(0)->RhoRandomLowEta();
  numPV   = (Float_t) fPV  ->GetEntries();

  if( (int) numPV == 0 ) return;  // NO PV found... ???
  
  const BaseVertex *bsp = dynamic_cast<const BaseVertex*>(fBeamspot->At(0));      
  
  /* Run over the electrons a select them. We need at least one. */
  ResetSelectedElectronsAndPhotons();
  for (UInt_t i=0; i<fElectrons->GetEntries(); ++i) {
    const Electron *p = fElectrons->At(i);  
    if (p->Et() > fElectronEtMin) fSelectedElectrons.push_back(i);
  }
  for (UInt_t i=0; i<fPhotons->GetEntries(); ++i) {
    const Photon *p = fPhotons->At(i);
    if (p->Et() > fPhotonEtMin) fSelectedPhotons.push_back(i);
  }
  MatchObjectsToTrigger(fIsData);

  // we need at least one Tag and two probes (since the Tag HAS to be a probe...)
  if(fTrigElectrons.size() < 1 || fTrigPhotons.size() < 2) return;

  // if there's more than one tag candidate, choose randomly (tag has higher abs(eta))
  const Electron* theTag = fElectrons->At(fTrigElectrons[0]);
  double maxEta = theTag->AbsEta();
  for(UInt_t iE = 1; iE < fTrigElectrons.size(); ++iE) {
    const Electron *p = fElectrons->At(iE);
    if(p->AbsEta() > maxEta) {
      theTag = p;
      maxEta = p->AbsEta();
    }
  }

  // now pick the probe among the photons not macthing the Tag  
  const Photon* theProbe = NULL;
  for (UInt_t iP=0; iP<fTrigPhotons.size(); ++iP) {
    const Photon *p = fPhotons->At(fTrigPhotons[iP]);
    if(p->SCluster() == theTag->SCluster()) continue;
    theProbe = p;
    break;        // pick the highest Et one...
  }
  
  if(!theProbe) return;     // no Probe found... should never happen though....
  
  // compute SuperCluster momentum (assuming zero mass)
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
  Float_t _tPhEiso    = ( _tPh ? _tPh->EcalRecHitIsoDr04() : -100. );
  Float_t _tPhHiso    = ( _tPh ? _tPh->HcalTowerSumEtDr04() : -100. );
  Float_t _tPhIso     = ( _tPh ? _tPhTiso+_tPhEiso+_tPhHiso : -100. );

  Float_t _tPhNTiso    = ( _tPh ? IsolationTools::TrackIsolationNoPV(_tPh, bsp, 0.4, 0.04, 0.0, 0.015, 0.1, TrackQuality::highPurity, fTracks) : -100. );
  Float_t _tPhTiso3    = ( _tPh ? _tPh->HollowConeTrkIsoDr03() : -100. );
  Float_t _tPhEiso3   = ( _tPh ? _tPh->EcalRecHitIsoDr03() : -100. );
  Float_t _tPhHiso3    = ( _tPh ? _tPh->HcalTowerSumEtDr03() : -100. );
  Float_t _tPhIso3     = ( _tPh ? _tPhTiso3+_tPhEiso3+_tPhHiso3 : -100. );
  Float_t _tPhNTiso3    = ( _tPh ? IsolationTools::TrackIsolationNoPV(_tPh, bsp, 0.3, 0.04, 0.0, 0.015, 0.1, TrackQuality::highPurity, fTracks) : -100. );
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

  ReqEventObject(fPhotonBranchName,   fPhotons,    fPhotonsFromBranch);
  ReqEventObject(fTrackBranchName,    fTracks,     true);
  ReqEventObject(fElectronName,       fElectrons,  fEleFromBranch);  
  ReqEventObject(fPileUpDenName,      fPileUpDen,  true);
  ReqEventObject(fPVName,             fPV,         fPVFromBranch);
  ReqEventObject(fBeamspotName,       fBeamspot,   true);
  
  if (!fIsData)
    ReqBranch(fPileUpName,            fPileUp);

  
  hPhIsoAreaTuple = new TNtuple("hPhIsoAreaTuple","hPhIsoAreaTuple","isData:phEt:phEta:phPhi:phE:phPz:phPtt:phM:phHoE:phR9:phIso:phTiso:phNTiso:phEiso:phHiso:phIso3:phTiso3:phNTiso3:phEiso3:phHiso3:phSEE:elEt:elEta:elPhi:elE:elPz:elPt:elM:numPU:numPV:Rho:invMass");
  
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
void PhotonIsoAreaMod::ResetSelectedElectronsAndPhotons() {
  fSelectedPhotons.clear();
  fTrigPhotons.clear();
  fSelectedElectrons.clear();
  fTrigElectrons.clear();
  return;
}


//--------------------------------------------------------------------------------------------------
void PhotonIsoAreaMod::MatchObjectsToTrigger(bool isData)
{

  // for MC assume always accept for both legs...
  if(!isData) {

    for (UInt_t i=0; i<fSelectedElectrons.size(); i++)
      fTrigElectrons.push_back(fSelectedElectrons[i]);
    for (UInt_t i=0; i<fSelectedPhotons.size(); i++)
      fTrigPhotons.push_back(fSelectedPhotons[i]);

  } else {

    // For Data, dio the trigger matching
    const TriggerObjectCol *tos = GetHLTObjects(fTrigObjsName);
    if (! tos)             // this can only happen if HLTMod::SetAbortIfNotAccepted(kFALSE) was called
      return;

    UInt_t nEnts = tos->GetEntries();
    for (UInt_t j=0; j<nEnts; ++j) {
      const TriggerObject *to = tos->At(j);
      // -----------------------------------------------------------
      // match the selected electrons to tight trigger leg (for TAG)
      for (UInt_t i=0; i<fSelectedElectrons.size(); i++) {
	const Electron *p = fElectrons->At(fSelectedElectrons[i]);
	if( MathUtils::DeltaR(p->SCluster()->Point(),to->Mom()) < 0.2 ) {
	  if(!strcmp(to->ModuleName(),fTightTriggerLeg.Data()) || !strcmp(fTightTriggerLeg.Data(),"none"))
	    fTrigElectrons.push_back(fSelectedElectrons[i]);
	}
      }
      // -----------------------------------------------------------
      // match the selected photons to loose trigger leg (for PROBE)
      for (UInt_t i=0; i<fSelectedPhotons.size(); i++) {
	const Photon *p = fPhotons->At(fSelectedPhotons[i]);
	if( MathUtils::DeltaR(p->SCluster()->Point(),to->Mom()) < 0.2 ) {
	  if(!strcmp(to->ModuleName(),fLooseTriggerLeg.Data()) || !strcmp(fLooseTriggerLeg.Data(),"none"))
	    fTrigPhotons.push_back(fSelectedPhotons[i]);
	}
      }
    }
  }
  
  return;
}
