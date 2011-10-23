//--------------------------------------------------------------------------------------------------
// $Id: PhotonIsoAreaMod.h,v 1.2 2011/07/15 17:26:34 fabstoec Exp $
//
// PhotonIsoAreaMod
//
// This module computes photon isolation area.
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


    // setting all the input Names
    void                SetInputPhotonsName(const char *n){ fPhotonBranchName= n;        }
    void                SetPhotonsFromBranch(bool b)      { fPhotonsFromBranch = b;      }
    void                SetTrackName(const char *n)       { fTrackBranchName = n;        }
    void                SetElectronName(const char *n)    { fElectronName = n;           }
    void                SetEleFromBranch(bool b)          { fEleFromBranch = b;          }
    void                SetPUDensityName(const char *n)   { fPileUpDenName = n;          }
    void                SetPVName(const char *n)          { fPVName = n;                 }
    void                SetPVFromBranch(bool b)           { fPVFromBranch = b;           }
    void                SetPUInfoName(const char *n)      { fPileUpName = n;             }
    void                SetBeamspotName(const char *n)    { fBeamspotName = n;           }

    void                SetIsData(bool b)                 { fIsData = b; }
    void                SetTrigObjsName(const char* n)    { fTrigObjsName = n; }
    void                SetTriggerLegs(const char* tight, const char* loose) {
                                                             fTightTriggerLeg = tight;
							     fLooseTriggerLeg = loose;   }


  protected:
    void                          Begin();
    void                          Process();
    void                          SlaveBegin();
    void                          SlaveTerminate();
    void                          Terminate();
    void                          MatchObjectsToTrigger(bool isData);
    void                          ResetSelectedElectronsAndPhotons();

    // Names for the input Collections
    TString                       fPhotonBranchName;
    TString                       fElectronName;
    TString                       fConversionName;
    TString                       fTrackBranchName;
    TString                       fPileUpDenName;    
    TString                       fPVName;
    TString                       fBeamspotName;
    TString                       fPFCandName;
    TString                       fMCParticleName;
    TString                       fPileUpName;
    TString                       fTrigObjsName;
			          
    // photon selection	          
    double                        fPhotonEtMin;              // minimal transverse energy for photon
    double                        fElectronEtMin;              // minimal transverse energy for photon

    const PhotonCol              *fPhotons;
    const ElectronCol            *fElectrons;
    const TrackCol               *fTracks;
    const PileupEnergyDensityCol *fPileUpDen;
    const VertexCol              *fPV;
    const BeamSpotCol            *fBeamspot;
    const PileupInfoCol          *fPileUp;    

    // is it Data or MC?
    Bool_t                        fIsData;
    
    // in case there's some PV pre-selection
    Bool_t                        fPhotonsFromBranch;
    Bool_t                        fEleFromBranch;
    Bool_t                        fPVFromBranch;
 
    // Trigger strings, need to be provided by USER
    TString                       fTightTriggerLeg;
    TString                       fLooseTriggerLeg;

    std::vector<UInt_t>           fSelectedElectrons;
    std::vector<UInt_t>           fSelectedPhotons;
    std::vector<UInt_t>           fTrigElectrons;
    std::vector<UInt_t>           fTrigPhotons;

    // The output Ntuple
    TNtuple* hPhIsoAreaTuple;
    ClassDef(PhotonIsoAreaMod, 1)
  };
}
#endif
