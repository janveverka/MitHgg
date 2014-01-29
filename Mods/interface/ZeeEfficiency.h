//--------------------------------------------------------------------------------------------------
// $Id: ZeeEfficiency.h,v 1.1 2011/01/24 14:57:09 paus Exp $
//
// ZeeEfficiency
//
// This module uses Electrons to measure the efficiency for electrons (and photons) using Z->ee
// events in a classical tag and probe style.
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITHGG_MODS_ZEEEFFICIENCY_H
#define MITHGG_MODS_ZEEEFFICIENCY_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/CollectionsFwd.h"

class TH1D;

namespace mithep 
{
  class ZeeEfficiency : public BaseMod
  {
  public:
    ZeeEfficiency(const char *name  = "ZeeEfficiency", 
		  const char *title = "Z To Electron Electron Analysis");
    
  protected:
    void                     Begin();
    void                     Process();
    void                     SlaveBegin();
    void                     SlaveTerminate();
    void                     Terminate();

    void                     SetTrigObjsName(const char *name) { fTrigObjsName = name; }
    void                     SetAllElecName(TString name)      { fAllElecName = name; }
    void                     SetSelectedElecName(TString name) { fSelectedElecName = name; }
    void                     SetIsData(Bool_t b)               { fIsData = b; }

    void                     MatchElecsToTrigger();     // create trig elec indices from sel.
    void                     ElecSelected(Electron *e); // was this electron selected?
    Bool_t                   ElecTriggered(UInt_t i);   // did this elec trigger

    TString                  fTrigObjsName;             // name of trigger objects
    TString                  fAllElecName;              // name of elec collection
    TString                  fSelectedElecName;         // name of elec collection
    const ElectronCol       *fElecs;                    //! elec from data stream
    Bool_t                   fIsData;                   // looking at Data (or MC)

    //// keep track of our elecs
    //std::vector<UInt_t>      fSelectedElecs;            // elec indices, selected
    //std::vector<UInt_t>      fTriggeredElecs;           // elec indices, selected and triggered

    //----------------------------------------------------------------------------------------------
    // histograms
    //----------------------------------------------------------------------------------------------
    // all elecs
    TH1D                    *hSele2ElecMass;     //! di-elec mass second electron was selected
    TH1D                    *hFail2ElecMass;     //! di-elec mass second electron failed
    
    ClassDef(ZeeEfficiency, 1) // Z to Electron Electron analysis
  };
}
#endif
