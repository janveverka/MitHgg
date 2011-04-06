//--------------------------------------------------------------------------------------------------
// $Id: PhotonIsoAreaMod.h,v 1.1 2011/01/24 14:57:09 paus Exp $
//
// PhotonIsoAreaMod
//
// This module compues photon eff from Z->mumugamma
//
// Authors: F,.Stoeckli
//--------------------------------------------------------------------------------------------------

#ifndef MITHGG_MODS_PHOTONISOAREAMOD_H
#define MITHGG_MODS_PHOTONISOAREAMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/CollectionsFwd.h"
#include "MitAna/DataTree/interface/PileupInfoCol.h"
#include "MitAna/DataCont/interface/Types.h"
#include "MitAna/DataTree/interface/PileupEnergyDensityCol.h"


class TH1D;
class TH2D;
class TNtuple;
class MCEventInfo;
class PileupEnergyDensityCol;
class PFCandidateCol;
class Vertex;
class BaseVertex;
class BeamSpotCol;

namespace mithep 
{
  class PhotonIsoAreaMod : public BaseMod
  {
  public:
    PhotonIsoAreaMod(const char *name  = "PhotonIsoAreaMod", 
		const char *title = "Photon Efficiency Analysis");

  protected:

    void                     Begin();
    void                     Process();
    void                     SlaveBegin();
    void                     SlaveTerminate();
    void                     Terminate();

    void                     SetTrigObjsName(const char *name) { fTrigObjsName = name; }
    void                     SetIsData(Bool_t b)               { fIsData = b; }
    void                     SetSkimming(Bool_t b)             { fSkimming = b; }
    void                     MatchObjectsToTrigger();   // create trig photon indices from sel.
    UInt_t                   PhotonTriggered(UInt_t i); // did this photon trigger

    void                     SetPhotonName(TString name)       { fPhotonName = name; }
    void                     SetPhotonsFromBranch(Bool_t b)    { fPhotonsFromBranch = b; }
    void                     ResetSelectedPhotons();

    void                     SetElectronName(TString name)       { fElectronName = name; }
    void                     SetElectronsFromBranch(Bool_t b)    { fElectronsFromBranch = b; }
    void                     ResetSelectedElectrons();
    
    void                     SetOverlapCut(double c)           { fOverlapCut = c; }
    Double_t                 getTrackIso(const Photon*, const BaseVertex*);    
    Double_t                 getTrackIso3(const Photon*, const BaseVertex*);    

    void                     SetPVName  (TString s) {fPVName   = s; fPVFromBranch   = false;};
    void                     SetPVNameDA(TString s) {fPVNameDA = s; fPVFromBranchDA = false;};
    void                     SetPVNameBS(TString s) {fPVNameBS = s; fPVFromBranchBS = false;};


    // Muon Collection & Cuts

    TString fTrackName;
    const TrackCol* fTracks;

    TString                  fTrigObjsName;             // name of trigger objects
    TString                  fMcEventInfoName;          // name of MC Event information

    TString fPVName;
    const VertexCol *fPV;
    Bool_t fPVFromBranch;

    TString fPVNameDA;
    const VertexCol *fPVDA;
    Bool_t fPVFromBranchDA;

    TString fPVNameBS;
    const VertexCol *fPVBS;
    Bool_t fPVFromBranchBS;

    const BeamSpotCol *fBeamspot;

    TString fElectronName;
    const ElectronCol* fElectrons;
    Bool_t fElectronsFromBranch;

    TString fPhotonName;
    const PhotonCol* fPhotons;
    Bool_t fPhotonsFromBranch;

    TString fMCParticleName;
    const MCParticleCol* fMCParticles;

    const MCEventInfo       *fMcEventInfo;              //! MC event information branch
    TString                  fPileUpName;
    const PileupInfoCol     *fPileUp;
    const PileupEnergyDensityCol     *fPileUpDen;
    TString                  fPileUpDenName;

    Bool_t                   fIsData;                   // looking at Data (or MC)
    Bool_t                   fSkimming;                 // are we skimming (skipping event)

    // general selection cuts
    double                   fOverlapCut;               // cut to allow for rejection of overlap

    // photon selection
    double                   fPhotonEtMin;              // minimal transverse energy for photon
    double                   fElectronEtMin;              // minimal transverse energy for photon
 
    std::vector<UInt_t>      fSelectedElectrons;    

    // SC selection
    std::vector<UInt_t>      fSelectedPhotons;
    std::vector<UInt_t>      fSelectedPV;    
    std::vector<UInt_t>      fTrigTightPhotons;         // photon indices, selected and triggered
    std::vector<UInt_t>      fTrigLoosePhotons;         // photon indices, selected and triggered

    std::vector<UInt_t>      fTrigElectrons;            // photon indices, selected and triggered


    const PFCandidateCol    *fCands;

    Bool_t                   fNoPixelSeedCut;           // request that there be no pixel seed?
    double                   fPhotonSigEeBarMax;        // maximal sigma(eta,eta), barrel
    double                   fPhotonSigEeEcpMax;        // maximal sigma(eta,eta), endcap
    double                   fPhotonIsoMax;             // maximal combined isolation
    double                   fPhotonR9Min;              // minimum R9 for cleaner photons
 
    std::vector<UInt_t>      fHiggsPhotons;        


    // The output Ntuple
    TNtuple* hPhIsoAreaTuple;
    TH1D* hNumPassTracks;
    TH1D* hPtPassTracks;
    TH1D* hNumFailTracks;
    TH1D* hPtFailTracks;

    ClassDef(PhotonIsoAreaMod, 1)
  };
}
#endif
