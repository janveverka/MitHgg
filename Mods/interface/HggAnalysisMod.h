//--------------------------------------------------------------------------------------------------
// $Id: HggAnalysisMod.h,v 1.2 2011/07/08 17:55:21 fabstoec Exp $
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
#include "MitAna/DataCont/interface/Types.h"

class TH1D;

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
    
    void                     SetPhotonName(TString name)       { fPhotonName = name; }
    void                     SetPhotonsFromBranch(Bool_t b)    { fPhotonsFromBranch = b; }
    void                     ResetSelectedPhotons();

    TString          fPhotonName;
    const PhotonCol* fPhotons;
    Bool_t           fPhotonsFromBranch;

    std::vector<UInt_t> fSelectedPhotons;

    // The output Ntuple
    TH1D* hHggAnaMass;
    
    ClassDef(HggAnalysisMod, 1)
      };
}
#endif
