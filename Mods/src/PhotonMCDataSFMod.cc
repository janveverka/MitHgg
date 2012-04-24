// $Id: PhotonMCDataSFMod.cc,v 1.2 2012/01/23 11:56:26 fabstoec Exp $

#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TNtuple.h>
#include <TRandom3.h>
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataTree/interface/TrackCol.h"
#include "MitAna/DataTree/interface/VertexCol.h"
#include "MitAna/DataTree/interface/PhotonCol.h"
#include "MitHgg/Mods/interface/PhotonMCDataSFMod.h"
#include "MitAna/DataTree/interface/BeamSpotCol.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "MitPhysics/Utils/interface/IsolationTools.h"
#include "MitPhysics/Utils/interface/MVATools.h"

using namespace mithep;

ClassImp(mithep::PhotonMCDataSFMod)

//--------------------------------------------------------------------------------------------------
PhotonMCDataSFMod::PhotonMCDataSFMod(const char *name, const char *title) :
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

  fBarrelSCName       (Names::gkBarrelSuperClusterBrn),
  fEndcapSCName       (Names::gkEndcapSuperClusterBrn),

  // -------------------------------------------
  fPhotonEtMin        (18.0),
  fElectronEtMin      (30.0),

  // SC selection
  fSCEtMin            (20.),
  fSCBarrelEtaMax     (1.4442),
  fSCEndcapEtaMax     (2.5),
  fSCEndcapEtaMin     (1.556),

  // ----------------------------------------
  // collections....
  fPhotons           (0),
  fElectrons         (0),
  fTracks            (0),
  fPileUpDen         (0),
  fPV                (0),
  fBeamspot          (0),
  fPileUp            (0),

  fBarrelSC          (0),
  fEndcapSC          (0),
  
  // -------------------------------------------
  fIsData            (false),
  fPhotonsFromBranch (true),  
  fEleFromBranch     (true),
  fPVFromBranch      (true),

  // ----------------------------------------------------
  // this is Trigger specifics. We need (in DATA only) the tag&probe guyas to match the tight and loose legs
  fTightTriggerLeg    (0),
  fLooseTriggerLeg    (0),

  // RNG needed for tga/probe decision
  rng(new TRandom3(0)),

  fTupleName("hPhEffTuple")
  
{

  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void PhotonMCDataSFMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void PhotonMCDataSFMod::Process()
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

  LoadBranch(fBarrelSCName);
  LoadBranch(fEndcapSCName);

  if( !fIsData )
    LoadBranch(fPileUpName);
  
  //----------------------------------------------------------------------------------

  Float_t numPU = -1.;
  Float_t numPV = -1.;
  
  if( !fIsData ) numPU = (Float_t) fPileUp->At(0)->GetPU_NumInteractions();  
  Float_t _tRho = (Float_t) fPileUpDen->At(0)->RhoRandomLowEta();
  numPV   = (Float_t) fPV  ->GetEntries();

  if( (int) numPV == 0 ) return;  // NO PV found... ???
  // compute SuperCluster momentum (assuming zero mass)
  const Vertex* vtx = fPV->At(0);
  
  //const BaseVertex *bsp = dynamic_cast<const BaseVertex*>(fBeamspot->At(0));      
  
  // first we look for all the SuperClusters
  ResetSelectedStuff();
  
  for(UInt_t i=0; i<fBarrelSC->GetEntries(); ++i) {
    const SuperCluster *sc = fBarrelSC->At(i);
    if ( fabs(sc->Eta()) > fSCBarrelEtaMax || sc->Et() < fSCEtMin ) continue;    
    fSelectedSC_EB.push_back(i);
  }
  for(UInt_t i=0; i<fEndcapSC->GetEntries(); ++i) {
    const SuperCluster *sc = fEndcapSC->At(i);
    if ( fabs(sc->Eta()) > fSCEndcapEtaMax || fabs(sc->Eta()) < fSCEndcapEtaMin ||
 	 sc->Et() < fSCEtMin ) continue;
    fSelectedSC_EE.push_back(i);
  }
  
  // check if we have two SuperClusters... if not we can skip the event (to save time)
  if(fSelectedSC_EB.size()+fSelectedSC_EE.size() < 2) return;
  
  /* Run over the electrons a select them. We need at least one. */
  for (UInt_t i=0; i<fElectrons->GetEntries(); ++i) {
    const Electron *p = fElectrons->At(i);  
    if (p->Et() > fElectronEtMin) fSelectedElectrons.push_back(i);
  }
  MatchObjectsToTrigger(fIsData);

  // we need at least one Tag and two probes (since the Tag HAS to be a probe...)
  if(fTrigElectrons.size() < 1 || (fTrigSC_EB.size()+fTrigSC_EE.size()) < 2) return;
  
  for (UInt_t i=0; i<fPhotons->GetEntries(); ++i) {
    const Photon *p = fPhotons->At(i);
    if (p->Et() > fPhotonEtMin) fSelectedPhotons.push_back(i);
  }
  
  // decide which one is tag... this could be improved (FIX-ME)
  const SuperCluster* tTag = NULL;
  const SuperCluster* tProbe = NULL;
  if(fTrigSC_EB.size() > 0) {
    tTag = fBarrelSC->At(fTrigSC_EB[0]);
    if(fTrigSC_EB.size() > 1)
      tProbe = fBarrelSC->At(fTrigSC_EB[1]);
    else {
      if(fTrigSC_EE.size() > 0)
	tProbe = fEndcapSC->At(fTrigSC_EE[0]);	
    }
  } else {
    if(fTrigSC_EE.size() > 0) {
      tTag = fEndcapSC->At(fTrigSC_EE[0]);
      if(fTrigSC_EE.size() > 1) {
	tProbe = fEndcapSC->At(fTrigSC_EE[1]);
      }
    }
  }
  
  if(!tTag || !tProbe) {
    std::cout<<" ERROR in PhotonMCDataSFMod: No TAG and no PROBE found."<<std::endl;
    return;
  }
  if (rng->Uniform() < 0.5) {
    const SuperCluster* tmp = tProbe;
    tProbe = tTag;
    tTag   = tmp;
  }

  const Electron* _tEl = NULL;  
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
      probePass = 1.;
      break;
    }
  }
  
  // compute the probe momentum wr to the chosen Vtx
  FourVectorM tProbe_M;  
  ThreeVectorC tProbe_P  = tProbe->Point() - vtx->Position();
  tProbe_P = tProbe_P/tProbe_P.R();
  tProbe_M.SetXYZT(tProbe->Energy()*tProbe_P.X(), tProbe->Energy()*tProbe_P.Y(), tProbe->Energy()*tProbe_P.Z(), tProbe->Energy());

  // compute the probe momentum wr to the chosen Vtx
  FourVectorM tTag_M;  
  ThreeVectorC tTag_P  = tTag->Point() - vtx->Position();
  tTag_P = tTag_P/tTag_P.R();
  tTag_M.SetXYZT(tTag->Energy()*tTag_P.X(), tTag->Energy()*tTag_P.Y(), tTag->Energy()*tTag_P.Z(), tTag->Energy());

  
  const SuperCluster*   _tPh   = tProbe;
  const SuperCluster*   _tagPh = tTag;
  
  Float_t _tPhEt   = tProbe_M.Et() ;
  Float_t _tPhEta  = tProbe_M.Eta();
  Float_t _tPhPhi  = tProbe_M.Phi();
  Float_t _tPhE    = tProbe_M.E()  ;
  Float_t _tPhpz   = tProbe_M.Pz() ;
  Float_t _tPhpt   = tProbe_M.Pt() ;
  Float_t _tPhm    = tProbe_M.M()  ;
  Float_t _tPhSEE  = _tPh->Seed()->CoviEtaiEta();
  Float_t _tPhR9   = _tPh->Seed()->E3x3()/_tPh->RawEnergy();

  Float_t _tagPhEt   = tProbe_M.Et() ;
  Float_t _tagPhEta  = tProbe_M.Eta();
  Float_t _tagPhPhi  = tProbe_M.Phi();
  Float_t _tagPhE    = tProbe_M.E()  ;
  Float_t _tagPhpz   = tProbe_M.Pz() ;
  Float_t _tagPhpt   = tProbe_M.Pt() ;
  Float_t _tagPhm    = tProbe_M.M()  ;
  Float_t _tagPhSEE  = _tagPh->Seed()->CoviEtaiEta();
  Float_t _tagPhR9   = _tagPh->Seed()->E3x3()/_tagPh->RawEnergy();
  
  Float_t _tElEt   = ( _tEl ? _tEl->Et() : -1.);
  Float_t _tElEta  = ( _tEl ? _tEl->Eta() : -100.);
  Float_t _tElPhi  = ( _tEl ? _tEl->Phi() : -100.);
  Float_t _tElE    = ( _tEl ? _tEl->E() : -100.);
  Float_t _tElpz   = ( _tEl ? _tEl->Pz() : -100.);
  Float_t _tElpt   = ( _tEl ? _tEl->Pt() : -100.);
  Float_t _tElm    = ( _tEl ? _tEl->Mass() : -100.);
  
  Float_t _mass    = ( _tEl ? (_tEl->Mom()+tProbe_M).M() : -100.);
  Float_t _mass_dp = (tTag_M+tProbe_M).M();


  Float_t fillEvent[] = { Float_t(fIsData),
			  _tPhEt  ,
			  _tPhEta ,
			  _tPhPhi ,
			  _tPhE   ,
			  _tPhpz  ,
			  _tPhpt  ,
			  _tPhm   ,
			  _tPhSEE ,
			  _tPhR9  ,
			  _tagPhEt  ,
			  _tagPhEta ,
			  _tagPhPhi ,
			  _tagPhE   ,
			  _tagPhpz  ,
			  _tagPhpt  ,
			  _tagPhm   ,
			  _tagPhSEE ,
			  _tagPhR9  ,
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
			  _mass_dp
  };
  
  hPhTrigEffTuple->Fill(fillEvent);


  return;
}

//--------------------------------------------------------------------------------------------------
void PhotonMCDataSFMod::SlaveBegin()
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
  
  // SC collections
  ReqBranch(fBarrelSCName,fBarrelSC);
  ReqBranch(fEndcapSCName,fEndcapSC);

  if (!fIsData)
    ReqBranch(fPileUpName,            fPileUp);

  hPhTrigEffTuple = new TNtuple(fTupleName.Data(),fTupleName.Data(),"isData:phEt:phEta:phPhi:phE:phPz:phPtt:phM:phSEE:phR9:tagEt:tagEta:tagPhi:tagE:tagPz:tagPtt:tagM:tagSEE:tagR9:phTrig:elEt:elEta:elPhi:elE:elPz:elPt:elM:numPU:numPV:Rho:invMass:invMassDP");
  
  AddOutput(hPhTrigEffTuple);
}

//--------------------------------------------------------------------------------------------------
void PhotonMCDataSFMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void PhotonMCDataSFMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void PhotonMCDataSFMod::ResetSelectedStuff() {
  fSelectedPhotons.clear();
  fSelectedSC_EB.clear();
  fSelectedSC_EE.clear();
  fTrigSC_EB.clear();
  fTrigSC_EE.clear();
  fSelectedElectrons.clear();
  fTrigElectrons.clear();
  return;
}


//--------------------------------------------------------------------------------------------------
void PhotonMCDataSFMod::MatchObjectsToTrigger(bool isData)
{

  // for MC assume always accept for both legs...
  if(!isData || true) {

    for (UInt_t i=0; i<fSelectedElectrons.size(); i++)
      fTrigElectrons.push_back(fSelectedElectrons[i]);
    for (UInt_t i=0; i<fSelectedSC_EB.size(); i++)
      fTrigSC_EB.push_back(fSelectedSC_EB[i]);
    for (UInt_t i=0; i<fSelectedSC_EE.size(); i++)
      fTrigSC_EE.push_back(fSelectedSC_EE[i]);
    
    const TriggerObjectCol *tos = GetHLTObjects(fTrigObjsName);
    if (! tos)             // this can only happen if HLTMod::SetAbortIfNotAccepted(kFALSE) was called
      return;
    
    UInt_t nEnts = tos->GetEntries();
    for (UInt_t j=0; j<nEnts; ++j) {
      const TriggerObject *to = tos->At(j);
      std::cout<<to->ModuleName()<<std::endl;
    }

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
	  for(unsigned int kk=0; kk<fTightTriggerLeg.size(); ++kk) {
	    if(!strcmp(to->ModuleName(),fTightTriggerLeg[kk].Data())) {
	      fTrigElectrons.push_back(fSelectedElectrons[i]);
	      break;
	    }
	  }
	}
      }
      // -----------------------------------------------------------
      // match the selected photons to loose trigger leg (for PROBE)
      for (UInt_t i=0; i<fSelectedSC_EB.size(); i++) {
	const SuperCluster *p = fBarrelSC->At(fSelectedSC_EB[i]);
	if( MathUtils::DeltaR(p->Point(),to->Mom()) < 0.2 ) {
	  for(unsigned int kk=0; kk<fLooseTriggerLeg.size(); ++kk) {
	    if( !strcmp(to->ModuleName(),fLooseTriggerLeg[kk].Data()) ) {
	      fTrigSC_EB.push_back(fSelectedSC_EB[i]);
	      break;
	    }
	  }
	}
      }
      for (UInt_t i=0; i<fSelectedSC_EE.size(); i++) {
	const SuperCluster *p = fEndcapSC->At(fSelectedSC_EE[i]);
	if( MathUtils::DeltaR(p->Point(),to->Mom()) < 0.2 ) {
	  for(unsigned int kk=0; kk<fLooseTriggerLeg.size(); ++kk) {
	    if( !strcmp(to->ModuleName(),fLooseTriggerLeg[kk].Data()) ) {
	      fTrigSC_EE.push_back(fSelectedSC_EE[i]);
	      break;
	    }
	  }
	}
      }
    }
  }
  
  return;
}
