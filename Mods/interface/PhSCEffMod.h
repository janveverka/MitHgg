//--------------------------------------------------------------------------------------------------
// $Id: PhSCEffMod.h,v 1.3 2012/03/31 23:27:24 paus Exp $
//
// PhSCEffMod
//
// This module computes photon eff from Z->mumugamma
//
// Authors: F.Stoeckli
//--------------------------------------------------------------------------------------------------

#ifndef MITHGG_MODS_PHSCEFFMOD_H
#define MITHGG_MODS_PHSCEFFMOD_H

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
class TRandom3;

namespace mithep 
{
  class PhSCEffMod : public BaseMod
  {
  public:
    PhSCEffMod(const char *name  = "PhSCEffMod", 
	       const char *title = "Photon Efficiency Analysis");

  protected:

    void                     Begin();
    void                     Process();
    void                     SlaveBegin();
    void                     SlaveTerminate();
    void                     Terminate();

    void                     SetTrigObjsName(const char *name)   { fTrigObjsName   = name; }
    void                     SetIsData(Bool_t b)               { fIsData = b; }
    void                     SetSkimming(Bool_t b)             { fSkimming = b; }

    void                     SetPhotonName(TString name)       { fPhotonName = name; }
    void                     SetPhotonsFromBranch(Bool_t b)    { fPhotonsFromBranch = b; }
    void                     ResetSelectedPhotons();
    void                     ResetSelectedSC();      // reset stored SC indices


    void                     SetElectronName(TString name)       { fElectronName = name; }
    void                     SetElectronsFromBranch(Bool_t b)    { fElectronsFromBranch = b; }
    void                     ResetSelectedElectrons();
    
    void                     SetOverlapCut(double c)           { fOverlapCut = c; }

    void                     SetPVName  (TString s) {fPVName   = s; fPVFromBranch   = false;};

    void MatchElectronsToTightTrigger();
    void MatchSCToTrigger();
    double findHiggsPt();

    TString                  fTrigObjsName;            // name of the Electron Tag & Probe Trigger
    TString                  fMcEventInfoName;         // name of MC Event information

    TString                  fTrackName;
    const TrackCol          *fTracks;

    TString                  fMCParticleName;
    const MCParticleCol*     fMCParticles;

    TString                  fBarrelSCName;
    const SuperClusterCol   *fBarrelSC;
    TString                  fEndcapSCName;
    const SuperClusterCol   *fEndcapSC;

    TString                  fPVName;
    const VertexCol         *fPV;
    Bool_t                   fPVFromBranch;

    const BeamSpotCol       *fBeamspot;

    TString                  fElectronName;
    const ElectronCol*       fElectrons;
    Bool_t                   fElectronsFromBranch;

    TString                  fPhotonName;
    const PhotonCol*         fPhotons;
    Bool_t                   fPhotonsFromBranch;

    const MCEventInfo       *fMcEventInfo;              //! MC event information branch
    TString                  fPileUpName;
    const PileupInfoCol     *fPileUp;
    const PileupEnergyDensityCol *fPileUpDen;
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
    double                   fSCEtMin;
    double                   fSCBarrelEtaMax;
    double                   fSCEndcapEtaMax;
    double                   fSCEndcapEtaMin;

    // SC selection
    std::vector<UInt_t>      fSelectedPhotons;
    std::vector<UInt_t>      fSelectedPV;    
    std::vector<UInt_t>      fTrigPhotons;              // photon indices, selected and triggered
    std::vector<UInt_t>      fTrigElectrons;            // photon indices, selected and triggered
    std::vector<UInt_t>      fSelectedSCEB;
    std::vector<UInt_t>      fSelectedSCEE;
    std::vector<UInt_t>      fTrigSCEB;
    std::vector<UInt_t>      fTrigSCEE;

    // The output Ntuple
    TNtuple*                 hPhTrigEffTuple;
    TRandom3*                rng;

    ClassDef(PhSCEffMod, 1)
  };
}
#endif
