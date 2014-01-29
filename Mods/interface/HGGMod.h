//--------------------------------------------------------------------------------------------------
// $Id: HGGMod.h,v 1.4 2012/03/31 23:27:24 paus Exp $
//
// HGGMod
//
// Higgs->gamma gamma analysis module which writes an ntuple containing the final kinematic
// variables needed for categorization and fits.  Some extra information related to primary vertex
// and conversion reconstruction is also stored.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------
#ifndef MITANA_PHYSICSMOD_HGGMod_H
#define MITANA_PHYSICSMOD_HGGMod_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/CollectionsFwd.h"
#include "MitAna/DataTree/interface/PileupInfoFwd.h"


class TH1D;
class TH2F;
class TNtuple;

namespace mithep 
{
  class HGGMod : public BaseMod
  {
    public:
      HGGMod(const char *name  = "HGGMod", 
	     const char *title = "Example analysis module with all branches");

    protected:
      void                     Begin();
      void                     Process();
      void                     SlaveBegin();
      void                     SlaveTerminate();
      void                     Terminate();

      void                     SetMCPartName(TString name)   { fMCPartName = name; }
      void                     SetTrackName(TString name)    { fTrackName = name; }
      void                     SetMuonName(TString name)     { fMuonName = name; }
      void                     SetElectronName(TString name) { fElectronName = name; }
      void                     SetJetName(TString name)      { fJetName = name; }
      void                     SetCorJetName(TString name)   { fCorJetName = name; }
      void                     SetElectronLooseName(TString name) { fElectronLooseName = name; }
      void                     SetElectronsFromBranch(Bool_t b) { fElectronsFromBranch = b; }
      void                     SetPhotonsFromBranch(Bool_t b) { fPhotonsFromBranch = b; }
      void                     SetPhotonName(TString name)   { fPhotonName = name; }      
      void                     SetMuonsFromBranch(Bool_t b)  { fMuonsFromBranch = b; }
      void                     SetMatchMC(Bool_t b)          { fMatchMC = b; }
      void                     SetMatchSim(Bool_t b)         { fMatchSim = b; }
      void                     SetUsePileupInfo(Bool_t b)    { fUsePileupInfo = b; }
      void                     SetSinglePhotonMode(Bool_t b) { fSinglePhotonMode = b; }
      void                     FillDiphotonTree(const Photon *g1, const Photon *g2) const;
      const MCParticle        *MatchMC(const Photon *ph) const;
      const MCParticle        *MatchMCPt(const Photon *ph) const;
      const MCParticle        *SimConv(const Electron *e) const;
      const MCParticle        *SimDalitz(const Electron *e) const;
      const MCParticle        *SimChargedPiKP(const Electron *e) const;
      const MCParticle        *SimHeavyFlavour(const Electron *e) const;
      const DecayParticle     *MatchConversion(const Photon *p) const;
      const Track             *MatchEcalTrack(const Photon *p) const;
      const PFJet             *MatchJet(const Photon *p, const PFJetCol *jets) const;
      const Electron          *MatchElectron(const Photon *p) const;
      const DecayParticle     *MatchElectronConversion(const Photon *p, const Electron *e) const;

      TString                  fMCPartName;         //name of particle collection
      TString                  fTrackName;          //name of track collection
      TString                  fMuonName;           //name of muon collection
      TString                  fElectronName;       //name of electron collection
      TString                  fElectronLooseName;  //name of loose electron collection (for Z veto)     
      TString                  fPFMetName;          //name of PFMet branch
      TString                  fCaloMetName;
      TString                  fPFCandidatesName;   //name of PFCandidate branch
      TString                  fPrimaryVertexName;  //name of PV collection
      TString                  fConversionName;
      TString                  fPhotonName;      
      TString                  fBeamSpotName;
      TString                  fEcalTrackName;
      TString                  fJetName;
      TString                  fCorJetName;
      TString                  fPileupInfoName;
      Bool_t                   fMuonsFromBranch;     //muons are loaded from a branch
      Bool_t                   fElectronsFromBranch; //electrons are loaded from a branch
      Bool_t                   fPhotonsFromBranch;
      Bool_t                   fMatchMC;
      Bool_t                   fMatchSim;
      Bool_t                   fUsePileupInfo;
      Bool_t                   fSinglePhotonMode;
      const MCParticleCol     *fParticles;           //!MCParticle branch
      const TrackCol          *fTracks;	             //!Track branch
      const MuonCol	      *fMuons;               //!Muon branch
      const ElectronCol	      *fElectrons;           //!Electron branch
      const ElectronCol       *fElectronsLoose;      //!loose electron branch
      const ElectronCol       *fRecoElectrons;       //!reconstructed electrons (for acceptance)
      const PFMetCol          *fPFMet;               //!PFMet branch
      const CaloMetCol        *fCaloMet;             //!
      const PFCandidateCol    *fPFCandidates;        //!PFCandidate branch
      const VertexCol         *fPrimaryVertex;       //!Primary vertex branch
      const DecayParticleCol  *fConversions;         //!
      const PhotonCol         *fPhotons;             //!
      const BeamSpotCol       *fBeamSpot;            //!
      const TrackCol          *fEcalTracks;          //!
      const PFJetCol          *fJets;                //!
      const PFJetCol          *fCorJets;             //!
      const PileupInfoCol     *fPileupInfo;          //!
      TNtuple                 *hHggNtuple;           //!
      TNtuple                 *hVtxNtuple;           //!

      ClassDef(HGGMod, 1) // Full example analysis module
  };
}
#endif
