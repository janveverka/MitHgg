//--------------------------------------------------------------------------------------------------
// $Id: HggAnalysisMod.h,v 1.1 2011/01/24 14:57:09 paus Exp $
//
// HggAnalysisMod
//
// This module compues photon eff from Z->mumugamma
//
// Authors: F,.Stoeckli
//--------------------------------------------------------------------------------------------------

#ifndef MITHGG_MODS_HGGANALYSISMOD_H
#define MITHGG_MODS_HGGANALYSISMOD_H

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
  class HggAnalysisMod : public BaseMod
  {
  public:
    HggAnalysisMod(const char *name  = "HggAnalysisMod", 
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
    void                     MatchPhotonsToTrigger();   // create trig photon indices from sel.
    UInt_t                   PhotonTriggered(UInt_t i); // did this photon trigger

    void                     SetPhotonName(TString name)       { fPhotonName = name; }
    void                     SetPhotonsFromBranch(Bool_t b)    { fPhotonsFromBranch = b; }
    void                     ResetSelectedPhotons();
    
    void                     SetOverlapCut(double c)           { fOverlapCut = c; }

    void                     SetPVName  (TString s) {fPVName   = s; fPVFromBranch   = false;};
    Int_t                    findClosestToTruth(const VertexCol *, const Photon*);

    double findHiggsPt();
    // Muon Collection & Cuts

    TString fTrackName;
    const TrackCol* fTracks;

    TString                  fTrigObjsName;             // name of trigger objects
    TString                  fMcEventInfoName;          // name of MC Event information
    TString fPVName;
    const VertexCol *fPV;
    Bool_t fPVFromBranch;

    const BeamSpotCol *fBeamspot;

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

    const DecayParticleCol* fConversions;

    Bool_t                   fIsData;                   // looking at Data (or MC)
    Bool_t                   fSkimming;                 // are we skimming (skipping event)

    // general selection cuts
    double                   fOverlapCut;               // cut to allow for rejection of overlap

    // photon selection
    double                   fPhotonEtMin;              // minimal transverse energy for photon
 
    // SC selection
    std::vector<UInt_t>      fSelectedPhotons;
    std::vector<UInt_t>      fSelectedPV;    
    std::vector<UInt_t>      fTrigTightPhotons;         // photon indices, selected and triggered
    std::vector<UInt_t>      fTrigLoosePhotons;         // photon indices, selected and triggered


    const PFCandidateCol    *fCands;

    Bool_t                   fNoPixelSeedCut;           // request that there be no pixel seed?
    double                   fPhotonSigEeBarMax;        // maximal sigma(eta,eta), barrel
    double                   fPhotonSigEeEcpMax;        // maximal sigma(eta,eta), endcap
    double                   fPhotonIsoMax;             // maximal combined isolation
    double                   fPhotonR9Min;              // minimum R9 for cleaner photons
 
    std::vector<UInt_t>      fHiggsPhotons;        

    // The output Ntuple
    TNtuple* hHggAnaTuple;

    ClassDef(HggAnalysisMod, 1)
  };
}
#endif
