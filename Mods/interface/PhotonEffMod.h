//--------------------------------------------------------------------------------------------------
// $Id: PhotonEffMod.h,v 1.1 2011/01/24 14:57:09 paus Exp $
//
// PhotonEffMod
//
// This module compues photon eff from Z->mumugamma
//
// Authors: F,.Stoeckli
//--------------------------------------------------------------------------------------------------

#ifndef MITHGG_MODS_PHOTONEFFMOD_H
#define MITHGG_MODS_PHOTONEFFMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/CollectionsFwd.h"
#include "MitAna/DataTree/interface/PileupInfoCol.h"

class TH1D;
class TH2D;
class TNtuple;
class MCEventInfo;

namespace mithep 
{
  class PhotonEffMod : public BaseMod
  {
  public:
    PhotonEffMod(const char *name  = "PhotonEffMod", 
		const char *title = "Photon Efficiency Analysis");
    
  protected:
    void                     Begin();
    void                     Process();
    void                     SlaveBegin();
    void                     SlaveTerminate();
    void                     Terminate();

    void                     SetMuonName(TString name)         { fMuonName = name; }
    void                     SetMuonsFromBranch(Bool_t b)      { fMuonsFromBranch = b; }
    void                     SetIsData(Bool_t b)               { fIsData = b; }
    void                     SetSkimming(Bool_t b)             { fSkimming = b; }

    void                     SetPhotonName(TString name)       { fPhotonName = name; }
    void                     SetPhotonsFromBranch(Bool_t b)    { fPhotonsFromBranch = b; }

    void                     ResetSelectedMuons();      // reset stored muon indices
    void                     ResetSelectedSC();      // reset stored SC indices
    void                     ResetSelectedPhotons();
    
    void                     SetOverlapCut(double c)           { fOverlapCut = c; }

    double isTrackIsolated(const SuperCluster*, const Muon*, const Muon*);
    const MCParticle* findMatch(const SuperCluster*, bool&);
    double getMinDR(const SuperCluster*, const Muon*, const Muon*);
    const Muon* getCloserMuon(const SuperCluster*, const Muon*, const Muon*);

    // Muon Collection & Cuts
    TString                  fMuonName;                 // name of muon collection
    Bool_t                   fMuonsFromBranch;          // muons are loaded from a branch
    

    TString                  fMcEventInfoName;          // name of MC Event information

    TString                  fBarrelSCName;
    const SuperClusterCol   *fBarrelSC;
    TString                  fEndcapSCName;
    const SuperClusterCol   *fEndcapSC;

    TString fPVName;
    const VertexCol *fPV;
    Bool_t fPVFromBranch;

    TString fPhotonName;
    const PhotonCol* fPhotons;
    Bool_t fPhotonsFromBranch;

    TString fTrackName;
    const TrackCol* fTracks;

    TString fMCParticleName;
    const MCParticleCol* fMCParticles;

    const MuonCol           *fMuons;                    //! muon from data stream
    const MCEventInfo       *fMcEventInfo;              //! MC event information branch

    TString                  fPileUpName;
    const PileupInfoCol     *fPileUp;

    Bool_t                   fIsData;                   // looking at Data (or MC)
    Bool_t                   fSkimming;                 // are we skimming (skipping event)

    // general selection cuts
    double                   fOverlapCut;               // cut to allow for rejection of overlap

    // muon selection cuts
    double                   fMuonEtMin;                // minimal transverse energy for muon
    
    // photon selection
    double                   fPhotonEtMin;              // minimal transverse energy for photon
 
    // SC selection
    double                   fSCEtMin;
    double                   fSCBarrelEtaMax;
    double                   fSCEndcapEtaMax;
    double                   fSCEndcapEtaMin;

    // keep track of our muons
    std::vector<UInt_t>      fSelectedMuons;            // muon indices, selected
    std::vector<UInt_t>      fSelectedSC;
    std::vector<UInt_t>      fSelectedPhotons;
    std::vector<UInt_t>      fSelectedPV;
    

    // The output Ntuple
    TNtuple* hPhEffTuple;

    ClassDef(PhotonEffMod, 1)
  };
}
#endif
