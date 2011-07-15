//--------------------------------------------------------------------------------------------------
// $Id: PhotonMCDataSFMod.h,v 1.1 2011/04/06 15:59:54 fabstoec Exp $
//
// PhotonMCDataSFMod
//
// This module compues photon eff from Z->mumugamma
//
// Authors: F,.Stoeckli
//--------------------------------------------------------------------------------------------------

#ifndef MITHGG_MODS_PHOTONMCDATASFMOD_H
#define MITHGG_MODS_PHOTONMCDATASFMOD_H

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
  class PhotonMCDataSFMod : public BaseMod
  {
  public:
    PhotonMCDataSFMod(const char *name  = "PhotonMCDataSFMod", 
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

    void                SetIsData(bool b) { fIsData = b; }

    // set Trigger Leg Strings
    void                AddTriggerLegs(const char* tight, const char* loose) {
      fTightTriggerLeg.push_back(tight);
      fLooseTriggerLeg.push_back(loose);
    }

    void                SetTrigObjsName(const char* n) { fTrigObjsName = n; }

    void                SetSCEtMin(double a)           { fSCEtMin = a; }
    void                SetSCBarrelEtaMax(double a)    { fSCBarrelEtaMax = a; }
    void                SetSCEndcapEtaMax(double a)    { fSCEndcapEtaMax = a; }
    void                SetSCEndcapEtaMin(double a)    { fSCEndcapEtaMin = a; }

    void                SetTupleName(const char* c)    { fTupleName = c; }

  protected:

    void                     Begin();
    void                     Process();
    void                     SlaveBegin();
    void                     SlaveTerminate();
    void                     Terminate();

    void                     MatchObjectsToTrigger(bool isData);   // create trig photon indices from sel.

    void                     ResetSelectedStuff();

    // Names for the input Collections
    TString             fPhotonBranchName;
    TString             fElectronName;
    TString             fConversionName;
    TString             fTrackBranchName;
    TString             fPileUpDenName;    
    TString             fPVName;
    TString             fBeamspotName;
    TString             fPFCandName;
    TString             fMCParticleName;
    TString             fPileUpName;
    TString             fTrigObjsName;

    TString             fBarrelSCName;
    TString             fEndcapSCName;

    // photon selection
    double                   fPhotonEtMin;              // minimal transverse energy for photon
    double                   fElectronEtMin;              // minimal transverse energy for photon
    // SC selection
    double                   fSCEtMin;
    double                   fSCBarrelEtaMax;
    double                   fSCEndcapEtaMax;
    double                   fSCEndcapEtaMin;

    const PhotonCol              *fPhotons;
    const ElectronCol            *fElectrons;
    const TrackCol               *fTracks;
    const PileupEnergyDensityCol *fPileUpDen;
    const VertexCol              *fPV;
    const BeamSpotCol            *fBeamspot;
    const PileupInfoCol          *fPileUp;    

    const SuperClusterCol        *fBarrelSC;
    const SuperClusterCol        *fEndcapSC;

    // is it Data or MC?
    Bool_t              fIsData;
    
    // in case there's some PV pre-selection
    Bool_t              fPhotonsFromBranch;
    Bool_t              fEleFromBranch;
    Bool_t              fPVFromBranch;
 
    // Trigger strings, need to be provided by USER
    std::vector<TString>             fTightTriggerLeg;
    std::vector<TString>             fLooseTriggerLeg;

    std::vector<UInt_t>      fSelectedElectrons;
    std::vector<UInt_t>      fSelectedSC_EB;
    std::vector<UInt_t>      fSelectedSC_EE;
    std::vector<UInt_t>      fSelectedPhotons;
    std::vector<UInt_t>      fTrigElectrons;
    std::vector<UInt_t>      fTrigSC_EB;
    std::vector<UInt_t>      fTrigSC_EE;

    TRandom3* rng;

    TString fTupleName;

    // The output Ntuple
    TNtuple* hPhTrigEffTuple;
    ClassDef(PhotonMCDataSFMod, 1)
  };
}
#endif
