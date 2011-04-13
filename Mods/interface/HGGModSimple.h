//--------------------------------------------------------------------------------------------------
// $Id: HGGModSimple.h,v 1.7 2010/03/12 13:51:26 bendavid Exp $
//
// HGGModSimple
//
// Higgs->gamma gamma analysis module which writes an ntuple
// containing the final kinematic variables needed for categorization and fits.
// Some extra information related to primary vertex and
// conversion reconstruction is also stored.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_PHYSICSMOD_HGGModSimple_H
#define MITANA_PHYSICSMOD_HGGModSimple_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/CollectionsFwd.h"
#include "MitAna/DataTree/interface/PileupInfoFwd.h"
#include "MitAna/DataTree/interface/PileupEnergyDensityCol.h"


class TH1D;
class TH2F;
class TNtuple;

namespace mithep 
{
  class HGGModSimple : public BaseMod
  {
    public:
      HGGModSimple(const char *name="HGGModSimple", 
                     const char *title="Example analysis module with all branches");

    protected:
      void                     Begin();
      void                     Process();
      void                     SlaveBegin();
      void                     SlaveTerminate();
      void                     Terminate();

      void                     SetMCPartName(TString name)   { fMCPartName = name; }
      void                     SetTrackName(TString name)    { fTrackName = name; }
      void                     SetElectronName(TString name) { fElectronName = name; }
      void                     SetPhotonsFromBranch(Bool_t b) { fPhotonsFromBranch = b; }
      void                     SetPhotonName(TString name)   { fPhotonName = name; }      
      void                     SetTriggerObjectsName(const char *n)   { fTrigObjectsName = n;       }      
      void                     SetMatchMC(Bool_t b)          { fMatchMC = b; }
      void                     SetApplyTriggerMatching(Bool_t b) { fApplyTriggerMatching = b; }      
      void                     SetSinglePhotonMode(Bool_t b) { fSinglePhotonMode = b; }
      void                     FillDiphotonTree(const Photon *g1, const Photon *g2) const;
      const MCParticle        *MatchMC(const Photon *ph) const;      


      TString                  fMCPartName;         //name of particle collection
      TString                  fTrackName;          //name of track collection
      TString                  fElectronName;       //name of electron collection
      TString                  fPFMetName;          //name of PFMet branch
      TString                  fCaloMetName;
      TString                  fPFCandidatesName;    //name of PFCandidate branch
      TString                  fPrimaryVertexName;  //name of PV collection
      TString                  fConversionName;
      TString                  fPhotonName;      
      TString                  fBeamSpotName;
      TString                  fPileupInfoName;
      TString                  fTrigObjectsName;        //name of trigger object collection      
      TString                  fPileUpDenName;        //name of the PU density collection            
      Bool_t                   fPrimaryVertexFromBranch;      
      Bool_t                   fPhotonsFromBranch;
      Bool_t                   fMatchMC;
      Bool_t                   fApplyTriggerMatching;
      Bool_t                   fSinglePhotonMode;
      const MCParticleCol     *fParticles;          //!MCParticle branch
      const TrackCol          *fTracks;	            //!Track branch
      const ElectronCol	      *fElectrons;          //!Electron branch
      const PFMetCol          *fPFMet;              //!PFMet branch
      const VertexCol         *fPrimaryVertex;      //!
      const DecayParticleCol  *fConversions;        //!
      const PhotonCol         *fPhotons;            //!
      const BeamSpotCol       *fBeamSpot;           //!
      const PileupInfoCol     *fPileupInfo;         //!
      const PileupEnergyDensityCol *fPileUpDen;  //!rho branch      
      TNtuple                 *hHggNtuple;//!


      ClassDef(HGGModSimple, 1) // Full example analysis module
  };
}
#endif
