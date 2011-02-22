//--------------------------------------------------------------------------------------------------
// $Id: HGGSimMod.h,v 1.7 2010/03/12 13:51:26 bendavid Exp $
//
// HGGSimMod
//
// This TAM module shows how to use TAM. It takes a couple of branches
// and produces a pt and eta distribution.
//
// More information about TAM in general can be found at
// http://www.cmsaf.mit.edu/twiki/bin/view/Software/TAM
//
// Authors: C.Loizides, J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_PHYSICSMOD_HGGSimMod_H
#define MITANA_PHYSICSMOD_HGGSimMod_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/CollectionsFwd.h"

class TH1D;
class TH2F;
class TNtuple;

namespace mithep 
{
  class HGGSimMod : public BaseMod
  {
    public:
      HGGSimMod(const char *name="HGGSimMod", 
                     const char *title="Example analysis module with all branches");

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
      void                     SetElectronLooseName(TString name) { fElectronLooseName = name; }
      void                     SetElectronsFromBranch(Bool_t b) { fElectronsFromBranch = b; }
      void                     SetMuonsFromBranch(Bool_t b)  { fMuonsFromBranch = b; }
      void                     SetMatchWZ(Bool_t b)          { fMatchWZ = b; }
      void                     SetMatchSim(Bool_t b)         { fMatchSim = b; }
      const MCParticle        *MatchWZ(const Electron *e) const;
      const MCParticle        *SimConv(const Electron *e) const;
      const MCParticle        *SimDalitz(const Electron *e) const;
      const MCParticle        *SimChargedPiKP(const Electron *e) const;
      const MCParticle        *SimHeavyFlavour(const Electron *e) const;






      TString                  fMCPartName;         //name of particle collection
      TString                  fTrackName;          //name of track collection
      TString                  fMuonName;           //name of muon collection
      TString                  fElectronName;       //name of electron collection
      TString                  fElectronLooseName;       //name of loose electron collection (for Z veto)     
      TString                  fPFMetName;          //name of PFMet branch
      TString                  fCaloMetName;
      TString                  fPFCandidatesName;    //name of PFCandidate branch
      TString                  fPrimaryVertexName;  //name of PV collection
      TString                  fConversionName;
      Bool_t                   fMuonsFromBranch;    //muons are loaded from a branch
      Bool_t                   fElectronsFromBranch; //electrons are loaded from a branch
      Bool_t                   fMatchWZ;
      Bool_t                   fMatchSim;
      const MCParticleCol     *fParticles;          //!MCParticle branch
      const TrackCol          *fTracks;	            //!Track branch
      const MuonCol	      *fMuons;              //!Muon branch
      const ElectronCol	      *fElectrons;          //!Electron branch
      const ElectronCol       *fElectronsLoose;     //!loose electron branch
      const ElectronCol       *fRecoElectrons;      //!reconstructed electrons (for acceptance)
      const PFMetCol          *fPFMet;              //!PFMet branch
      const CaloMetCol        *fCaloMet;            //!
      const PFCandidateCol    *fPFCandidates;       //!PFCandidate branch
      const VertexCol         *fPrimaryVertex;      //!Primary vertex branch
      const DecayParticleCol  *fConversions;
      TNtuple                 *hGenNtuple;//!


      ClassDef(HGGSimMod, 1) // Full example analysis module
  };
}
#endif
