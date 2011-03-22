// $Id: PhotonEffMod.cc,v 1.1 2011/01/24 14:57:09 paus Exp $

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

#include "MitHgg/Mods/interface/PhotonEffMod.h"

using namespace mithep;

ClassImp(mithep::PhotonEffMod)

//--------------------------------------------------------------------------------------------------
PhotonEffMod::PhotonEffMod(const char *name, const char *title) :
  BaseMod             (name,title),
  // initialize bambu objects
  fMuonName           (Names::gkMuonBrn),
  fMuonsFromBranch    (kTRUE),
  fMcEventInfoName    (Names::gkMCEvtInfoBrn),
  fBarrelSCName       (Names::gkBarrelSuperClusterBrn),
  fBarrelSC(0),
  fEndcapSCName       (Names::gkEndcapSuperClusterBrn),
  fEndcapSC(0),

  fPVName (Names::gkPVBrn),
  fPV(0),
  fPVFromBranch(kTRUE),

  fPhotonName         (Names::gkPhotonBrn),
  fPhotons(0),
  fPhotonsFromBranch(kTRUE),

  fTrackName (Names::gkTrackBrn),
  fTracks(0),

  fMCParticleName(Names::gkMCPartBrn),
  fMCParticles(0),

  fMuons              (0),
  fMcEventInfo        (0),

  fPileUpName         ("PileupInfo"),
  fPileUp             (0),

  fIsData             (kTRUE),
  fSkimming           (kFALSE),

  // scale removal
  fOverlapCut         (-1.0),

  // cuts for selection
  fMuonEtMin          (10.0),
  
  // cuts for selection
  fPhotonEtMin        (0.0),

  // SC selection
  fSCEtMin (10.),
  fSCBarrelEtaMax (1.4442),
  fSCEndcapEtaMax (2.5),
  fSCEndcapEtaMin (1.556)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void PhotonEffMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void PhotonEffMod::Process()
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
    // load pile-up info
    LoadBranch(fPileUpName);
  }
  LoadEventObject(fMuonName,fMuons);
  LoadEventObject(fPhotonName,fPhotons);
  LoadBranch(fBarrelSCName);
  LoadBranch(fEndcapSCName);
  LoadEventObject(fPVName, fPV);
  LoadBranch(fTrackName);
  // -------------------------

  
  Float_t numPU = -1.;
  Float_t numPV = -1.;
  if( !fIsData )
    numPU = (Float_t) fPileUp->At(0)->GetPU_NumInteractions();
  

  /* -------------------------  
     Photon Selection
     We use all the photons coming as from the branch.
     To stear their selection (i.e. RECO/ID/ISO) use the PhotonIDMod.  */
  ResetSelectedPhotons();
  for (UInt_t i=0; i<fPhotons->GetEntries(); ++i) {
    const Photon *p = fPhotons->At(i);
    if (p->Et() > fPhotonEtMin) fSelectedPhotons.push_back(i);
  }
  // -------------------------  
  
  /* -------------------------  
     Muon Selection
     We store the selected muons (hardest & second hardes). 
     Define the muons to be selected from the MuonIDMod. */
  ResetSelectedMuons();
  for (UInt_t i=0; i<fMuons->GetEntries(); ++i) {
    const Muon *p = fMuons->At(i);
    if (p->Pt() > fMuonEtMin )
      fSelectedMuons.push_back(i);
  }
  // if we don't have two muons, we skip the event
  if(fSelectedMuons.size()<2) return;
  // -------------------------  


  /* -------------------------  
     SuperCluster Selection
     loop over the SCs and store the selected ones */
  
  ResetSelectedSC();
  const SuperCluster* hardSC = NULL;
  const SuperCluster* closeSC = NULL;
  Bool_t hardIsBarrel = false;
  Bool_t closeIsBarrel = false;
  for(UInt_t i=0; i<fBarrelSC->GetEntries(); ++i) {
    const SuperCluster *sc = fBarrelSC->At(i);

    if ( fabs(sc->Eta()) > fSCBarrelEtaMax ||
	 sc->Et() < fSCEtMin ) continue;
    
    fSelectedSC.push_back(i);

    // check if it is the hardest
    if(!hardSC || ( hardSC->Et() < sc->Et() ) ) {
      hardSC = sc;
      hardIsBarrel = true;
    }
    // check if it's the closest
    if( !closeSC || 
	( 
	 getMinDR(sc,      fMuons->At(fSelectedMuons[0]), fMuons->At(fSelectedMuons[1])) 
	 < 
	 getMinDR(closeSC, fMuons->At(fSelectedMuons[0]), fMuons->At(fSelectedMuons[1])) 
	 )
	) {
      closeSC = sc;
      closeIsBarrel = true;
    }
  }
  

  for(UInt_t i=0; i<fEndcapSC->GetEntries(); ++i) {
    const SuperCluster *sc = fEndcapSC->At(i);
    if ( fabs(sc->Eta()) > fSCEndcapEtaMax ||
 	 fabs(sc->Eta()) < fSCEndcapEtaMin ||
 	 sc->Et() < fSCEtMin ) continue;

    fSelectedSC.push_back(i);
    
    // check if it is the hardest
    if(!hardSC || ( hardSC->Et() < sc->Et() ) ) {
      hardSC = sc;
      hardIsBarrel = false;
    }
    // check if it's the closest
    if( !closeSC || 
	( 
	 getMinDR(sc,      fMuons->At(fSelectedMuons[0]), fMuons->At(fSelectedMuons[1])) 
	 < 
	 getMinDR(closeSC, fMuons->At(fSelectedMuons[0]), fMuons->At(fSelectedMuons[1])) 
	 )
	) {
      closeSC = sc;
      closeIsBarrel = false;
    }
  }


  // find the primary vertex (for now take the first one, later we should take the one 'mathing' the di-muon system
  if( fPV->GetEntries() == 0 ) return;  // NO PV found... ???
  numPV = (Float_t) fPV->GetEntries();
  
  // compute SuperCluster momentum (assuming zero mass)
  const Vertex* vtx = fPV->At(0);
  FourVectorM hardSC_M;
  FourVectorM closeSC_M;
  
  if(hardSC) {
    ThreeVectorC  hardSC_P  = hardSC->Point() - vtx->Position();
    hardSC_P = hardSC_P/hardSC_P.R();
    hardSC_M.SetXYZT(hardSC->Energy()*hardSC_P.X(), hardSC->Energy()*hardSC_P.Y(), hardSC->Energy()*hardSC_P.Z(), hardSC->Energy());
  }
  if(closeSC) {
    ThreeVectorC  closeSC_P  = closeSC->Point() - vtx->Position();
    closeSC_P = closeSC_P/closeSC_P.R();
    closeSC_M.SetXYZT(closeSC->Energy()*closeSC_P.X(), closeSC->Energy()*closeSC_P.Y(), closeSC->Energy()*closeSC_P.Z(), closeSC->Energy());
  }

  
  // loop over the selected photons and fund match
  const Photon* hardPh = NULL;
  const Photon* closePh = NULL;
  for(unsigned int k=0; k<fSelectedPhotons.size(); ++k) {
    const Photon* p = fPhotons->At(fSelectedPhotons[k]);
    const SuperCluster* sc = p->SCluster();
    if(hardSC == sc) {
      hardPh = p;
      if(closePh) break;
    }
    if(closeSC == sc) {
      closePh = p;
      if(hardPh) break;
    }    
  }
  

  // check if the SC matches an FSR GEN photon
  bool hardIsFSR =false;
  bool closeIsFSR =false;
  const MCParticle* matchHard  = NULL;
  const MCParticle* matchClose = NULL;
  if( !fIsData && hardSC  ) matchHard  = findMatch(hardSC,  hardIsFSR);
  if( !fIsData && closeSC ) matchClose = findMatch(closeSC, closeIsFSR);


  // Time to fill the information...
  const Muon* _mu1 = fMuons->At(fSelectedMuons[0]);
  const Muon* _mu2 = fMuons->At(fSelectedMuons[1]);

  FourVectorM muM = (_mu1->Mom()+_mu2->Mom()); 
  Float_t muMass = muM.M();  
  
  Float_t hardMass  = -100.;
  Float_t closeMass = -100.;
  if(hardSC)  hardMass  = (muM + hardSC_M).M();
  if(closeSC) closeMass = (muM + closeSC_M).M();


  // Hard SC found
  Float_t hardSCEt   = ( hardSC ? hardSC->Et() : -1.);
  Float_t hardSCE    = ( hardSC ? hardSC->Energy() : -1.);
  Float_t hardSCpz   = ( hardSC ? hardSC_M.Pz() : -1.);
  Float_t hardSCpt   = ( hardSC ? hardSC_M.Pt() : -1.);
  Float_t hardSCm    = ( hardSC ? hardSC_M.M() : -1.);
  Float_t hardSCEta  = ( hardSC ? hardSC->Eta() : -100.);
  Float_t hardSCPhi  = ( hardSC ? hardSC->Phi() : -100.);
  Float_t hardSCTiso = ( hardSC ? isTrackIsolated(hardSC, _mu1, _mu2) : -100.);
  Float_t hardMinDR   = ( hardSC ? getMinDR(hardSC, _mu1, _mu2) : -100.);

  // Close SC found
  Float_t closeSCEt   = ( closeSC ? closeSC->Et() : -1.);
  Float_t closeSCE    = ( closeSC ? closeSC->Energy() : -1.);
  Float_t closeSCpz   = ( closeSC ? closeSC_M.Pz() : -1.);
  Float_t closeSCpt   = ( closeSC ? closeSC_M.Pt() : -1.);
  Float_t closeSCm    = ( closeSC ? closeSC_M.M() : -1.);
  Float_t closeSCEta  = ( closeSC ? closeSC->Eta() : -100.);
  Float_t closeSCPhi  = ( closeSC ? closeSC->Phi() : -100.);
  Float_t closeSCTiso = ( closeSC ? isTrackIsolated(closeSC, _mu1, _mu2) : -100.);
  Float_t closeMinDR  = ( closeSC ? getMinDR(closeSC, _mu1, _mu2) : -100.);

  // Hard Photon found
  Float_t hardPhEt   = ( hardPh ? hardPh->Et() : -1.);
  Float_t hardPhEta  = ( hardPh ? hardPh->Eta() : -100.);
  Float_t hardPhPhi  = ( hardPh ? hardPh->Phi() : -100.);
  Float_t hardPhE    = ( hardPh ? hardPh->E() : -100.);
  Float_t hardPhpz   = ( hardPh ? hardPh->Pz() : -100.);
  Float_t hardPhpt   = ( hardPh ? hardPh->Pt() : -100.);
  Float_t hardPhm    = ( hardPh ? hardPh->Mass() : -100.);
  Float_t hardPhHoE  = ( hardPh ? hardPh->HadOverEm() : -100.);
  Float_t hardPhR9   = ( hardPh ? hardPh->R9() : -100.);
  Float_t hardPhTiso    = ( hardPh ? hardPh->HollowConeTrkIsoDr04() : -100. );
  Float_t hardPhEiso    = ( hardPh ? hardPh->EcalRecHitIsoDr04() : -100. );
  Float_t hardPhHiso    = ( hardPh ? hardPh->HcalTowerSumEtDr04() : -100. );
  Float_t hardPhIso     = ( hardPh ? hardPhTiso+hardPhEiso+hardPhHiso : -100. );

  // Close Photon found
  Float_t closePhEt   = ( closePh ? closePh->Et() : -1.);
  Float_t closePhEta  = ( closePh ? closePh->Eta() : -100.);
  Float_t closePhPhi  = ( closePh ? closePh->Phi() : -100.);
  Float_t closePhE    = ( closePh ? closePh->E() : -100.);
  Float_t closePhpz   = ( closePh ? closePh->Pz() : -100.);
  Float_t closePhpt   = ( closePh ? closePh->Pt() : -100.);
  Float_t closePhm    = ( closePh ? closePh->Mass() : -100.);
  Float_t closePhHoE  = ( closePh ? closePh->HadOverEm() : -100.);
  Float_t closePhR9   = ( closePh ? closePh->R9() : -100.);
  Float_t closePhTiso    = ( closePh ? closePh->HollowConeTrkIsoDr04() : -100. );
  Float_t closePhEiso    = ( closePh ? closePh->EcalRecHitIsoDr04() : -100. );
  Float_t closePhHiso    = ( closePh ? closePh->HcalTowerSumEtDr04() : -100. );
  Float_t closePhIso     = ( closePh ? closePhTiso+closePhEiso+closePhHiso : -100. );


  // take as close to SC muon the first muon by default. Will stay this in case these is no SC found...
  const Muon* _muCloseToClose = _mu1;
  const Muon* _muCloseToHard  = _mu1;
  
  if(closeSC)
    _muCloseToClose = getCloserMuon(closeSC, _mu1, _mu2);
  if(hardSC)
    _muCloseToHard  = getCloserMuon(hardSC, _mu1, _mu2);
  
  Float_t fillEvent[] = { Float_t(fIsData),
			  _mu1->Pt(),
			  _mu1->Eta(),
			  _mu1->Phi(),
			  _mu1->E(),
			  _mu1->Pz(),
			  _mu1->IsoR03SumPt() + _mu1->IsoR03EmEt() + _mu1->IsoR03HadEt(),
			  _mu1->IsoR03SumPt(),
			  _mu1->IsoR03EmEt(),
			  _mu1->IsoR03HadEt(),
			  _mu1->HadEnergy(),
			  _mu1->HadS9Energy(),
			  _mu1->EmEnergy(),
			  _mu1->EmS9Energy(),
			  _mu1->HoEnergy(),
			  _mu1->HoS9Energy(),
			  _mu2->Pt(),
			  _mu2->Eta(),
			  _mu2->Phi(),
			  _mu2->E(),
			  _mu2->Pz(),
			  _mu2->IsoR03SumPt() + _mu2->IsoR03EmEt() + _mu2->IsoR03HadEt(),
			  _mu2->IsoR03SumPt(),
			  _mu2->IsoR03EmEt(),
			  _mu2->IsoR03HadEt(),
			  _mu2->HadEnergy(),
			  _mu2->HadS9Energy(),
			  _mu2->EmEnergy(),
			  _mu2->EmS9Energy(),
			  _mu2->HoEnergy(),
			  _mu2->HoS9Energy(),
			  closeSCEt,
			  closeSCEta,
			  closeSCPhi,
			  closeSCE,
			  closeSCpz,
			  closeSCpt,
			  closeSCm,
			  closeSCTiso,
			  closeMinDR,
			  Float_t(closeIsFSR),
			  Float_t(closeIsBarrel),
			  hardSCEt,
			  hardSCEta,
			  hardSCPhi,
			  hardSCE,
			  hardSCpz,
			  hardSCpt,
			  hardSCm,
			  hardSCTiso,
			  Float_t(hardIsFSR),
			  Float_t(hardIsBarrel),
			  hardMinDR,
			  closePhEt,
			  closePhEta,
			  closePhPhi,
			  closePhE,
			  closePhpz,
			  closePhpt,
			  closePhm,
			  closePhHoE,
			  closePhR9,
			  closePhIso,
			  closePhTiso,
			  closePhEiso,
			  closePhHiso,
			  hardPhEt,
			  hardPhEta,
			  hardPhPhi,
			  hardPhE,
			  hardPhpz,
			  hardPhpt,
			  hardPhm,
			  hardPhHoE,
			  hardPhR9,
			  hardPhIso,
			  hardPhTiso,
			  hardPhEiso,
			  hardPhHiso,
			  _muCloseToClose->Pt(),
			  _muCloseToClose->Eta(),
			  _muCloseToClose->Phi(),
			  _muCloseToClose->E(),
			  _muCloseToClose->IsoR03SumPt() + _muCloseToClose->IsoR03EmEt() + _muCloseToClose->IsoR03HadEt(),
			  _muCloseToClose->IsoR03SumPt(),
			  _muCloseToClose->IsoR03EmEt(),
			  _muCloseToClose->IsoR03HadEt(),
			  _muCloseToClose->HadEnergy(),
			  _muCloseToClose->HadS9Energy(),
			  _muCloseToClose->EmEnergy(),
			  _muCloseToClose->EmS9Energy(),
			  _muCloseToClose->HoEnergy(),
			  _muCloseToClose->HoS9Energy(),
			  _muCloseToHard->Pt(),
			  _muCloseToHard->Eta(),
			  _muCloseToHard->Phi(),
			  _muCloseToHard->E(),
			  _muCloseToHard->IsoR03SumPt() + _muCloseToHard->IsoR03EmEt() + _muCloseToHard->IsoR03HadEt(),
			  _muCloseToHard->IsoR03SumPt(),
			  _muCloseToHard->IsoR03EmEt(),
			  _muCloseToHard->IsoR03HadEt(),
			  _muCloseToHard->HadEnergy(),
			  _muCloseToHard->HadS9Energy(),
			  _muCloseToHard->EmEnergy(),
			  _muCloseToHard->EmS9Energy(),
			  _muCloseToHard->HoEnergy(),
			  _muCloseToHard->HoS9Energy(),
			  muMass,
			  closeMass,
			  hardMass,
			  numPU,
			  numPV
  };

  hPhEffTuple->Fill(fillEvent);

  return;
}

//--------------------------------------------------------------------------------------------------
void PhotonEffMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches or objects created by earlier modules.

  // request the following Event Objects (often branches, but could be locally created ones)
  ReqEventObject(fMuonName,fMuons,fMuonsFromBranch);

  ReqEventObject(fPhotonName,fPhotons,fPhotonsFromBranch);

  // SC collections
  ReqBranch(fBarrelSCName,fBarrelSC);
  ReqBranch(fEndcapSCName,fEndcapSC);

  // PV collection
  ReqEventObject(fPVName, fPV, fPVFromBranch);

  // Track Collection
  ReqBranch(fTrackName,fTracks);

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
  
  
  hPhEffTuple = new TNtuple("hPhEffTuple","hPhEffTuple","isData:mu1pt:mu1eta:mu1phi:mu1E:mu1pz:mu1iso:mu1isoT:mu1isoE:mu1IsoH:mu1Had:mu1Had9:mu1Em:mu1Em9:mu1HoE:mu1HoE9:mu2pt:mu2eta:mu2phi:mu2E:mu2pz:mu2iso:mu2isoT:mu2isoE:mu2IsoH:mu2Had:mu2Had9:mu2Em:mu2Em9:mu2HoE:mu2HoE9:scCet:scCeta:scCphi:scCE:scCpz:scCpt:scCm:scCisoT:scCFSR:scCbarrel:scCDR:scHet:scHeta:scHphi:scHE:scHpz:scHpt:scHm:scHisoT:scHFSR:scHbarrel:scHDR:phCet:phCEta:phCphi:phCE:phCpz:phCpt:phCm:phCHoE:phCR9:phCiso:phCTiso:phCEiso:phCHiso:phHet:phHEta:phHphi:phHE:phHpz:phHpt:phHm:phHHoE:phHR9:phHiso:phHTiso:phHEiso:phHHiso:muCloseToClosept:muCloseToCloseeta:muCloseToClosephi:muCloseToCloseE:muCloseToCloseiso:muCloseToCloseisoT:muCloseToCloseisoE:muCloseToCloseIsoH:muCloseToCloseHad:muCloseToCloseHad9:muCloseToCloseEm:muCloseToCloseEm9:muCloseToCloseHoE:muCloseToCloseHoE9:muCloseToHardpt:muCloseToHardeta:muCloseToHardphi:muCloseToHardE:muCloseToHardiso:muCloseToHardisoT:muCloseToHardisoE:muCloseToHardIsoH:muCloseToHardHad:muCloseToHardHad9:muCloseToHardEm:muCloseToHardEm9:muCloseToHardHoE:muCloseToHardHoE9:massMuMu:massMuMuCloseSC:massMuMuHardSC:numPU:numPV");

  AddOutput(hPhEffTuple);
}

//--------------------------------------------------------------------------------------------------
void PhotonEffMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void PhotonEffMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void PhotonEffMod::ResetSelectedSC() {
  fSelectedSC.clear();
  return;
}

void PhotonEffMod::ResetSelectedPhotons() {
  fSelectedPhotons.clear();
  return;
}


void PhotonEffMod::ResetSelectedMuons()
{
  // Reset the seleted and trigger matched muons

  fSelectedMuons.clear();
  return;
}

//--------------------------------------------------------------------------------------------------
double PhotonEffMod::isTrackIsolated(const SuperCluster* sc, const Muon* m1, const Muon* m2) {
  
  // loop over all tracks
  double tPt = 0.;
  for(UInt_t i=0; i<fTracks->GetEntries(); ++i) {
    const Track* t = fTracks->At(i);
    if (t == m1->TrackerTrk() || t == m2->TrackerTrk()) continue;
    if(t->Pt()<0.1) continue;
    double dR=(sc->Eta()-t->Eta())*(sc->Eta()-t->Eta());
    double dPhi = fabs((sc->Phi() - t->Phi()));
    if(dPhi>M_PI) dPhi = 2*M_PI - dPhi;
    dR += dPhi*dPhi;
    dR=sqrt(dR);
    if(dR < 0.4) tPt += t->Pt();
  }
  return (tPt/sc->Et());
}

//--------------------------------------------------------------------------------------------------
double PhotonEffMod::getMinDR(const SuperCluster* sc, const Muon* m1, const Muon* m2) {
  double dR1=(sc->Eta()-m1->Eta())*(sc->Eta()-m1->Eta());
  double dPhi = fabs(sc->Phi() - m1->Phi());
  if(dPhi>M_PI) dPhi = 2*M_PI - dPhi;
  dR1 += dPhi*dPhi;
  dR1=sqrt(dR1);

  double dR2=(sc->Eta()-m2->Eta())*(sc->Eta()-m2->Eta());
  dPhi = fabs(sc->Phi() - m2->Phi());
  if(dPhi>M_PI) dPhi = 2*M_PI - dPhi;
  dR2 += dPhi*dPhi;
  dR2=sqrt(dR2);

  if (dR2 > dR1) return dR1;
  else return dR2;
}

//--------------------------------------------------------------------------------------------------
const Muon* PhotonEffMod::getCloserMuon(const SuperCluster* sc, const Muon* m1, const Muon* m2) {
  double dR1=(sc->Eta()-m1->Eta())*(sc->Eta()-m1->Eta());
  double dPhi = fabs(sc->Phi() - m1->Phi());
  if(dPhi>M_PI) dPhi = 2*M_PI - dPhi;
  dR1 += dPhi*dPhi;
  dR1=sqrt(dR1);
  
  double dR2=(sc->Eta()-m2->Eta())*(sc->Eta()-m2->Eta());
  dPhi = fabs(sc->Phi() - m2->Phi());
  if(dPhi>M_PI) dPhi = 2*M_PI - dPhi;
  dR2 += dPhi*dPhi;
  dR2=sqrt(dR2);
  
  if (dR2 > dR1) return m1;
  else return m2;
}


//--------------------------------------------------------------------------------------------------
const MCParticle* PhotonEffMod::findMatch(const SuperCluster* sc, bool& isFSR) {

  const MCParticle* _ph = NULL;
  // loop over all GEN particles and look for status 1 photons
  for(UInt_t i=0; i<fMCParticles->GetEntries(); ++i) {
    const MCParticle* p = fMCParticles->At(i);
    if( p->Status() != 1 || !(p->Is(MCParticle::kGamma)) || p->Pt()<2. ) continue;
    if(fabs((p->Pt()-sc->Et())/sc->Et()) > 0.5) continue;
    if(fabs(p->Eta()-sc->Eta()) > 0.1) continue;
    if(fabs(p->Phi()-sc->Phi()) > 0.1) continue;
    _ph = p;
    break;
  }
  if( !_ph ) return NULL;

  // check if it's FSR from muon
  const MCParticle* mother = _ph->Mother();
  while (mother->Is(MCParticle::kGamma) ) mother = mother->Mother();
  isFSR=(mother->Is(MCParticle::kMu));
  return _ph;

 }
