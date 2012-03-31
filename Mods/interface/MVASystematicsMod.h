//--------------------------------------------------------------------------------------------------
// $Id: MVASystematicsMod.h,v 1.1 2011/11/18 19:10:43 fabstoec Exp $
//
// MVASystematicsMod
//
// This module computes photon eff from Z->mumugamma
//
// Authors: F.Stoeckli
//--------------------------------------------------------------------------------------------------

#ifndef MITHGG_MODS_MVASYSTEMATICSMOD_H
#define MITHGG_MODS_MVASYSTEMATICSMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/MCParticleCol.h"

class TNtuple;

namespace mithep 
{
  class MVASystematicsMod : public BaseMod
  {
  public:
    MVASystematicsMod(const char *name  = "MVASystematicsMod", 
		      const char *title = "Photon Efficiency Analysis");


    // setting all the input Names
    void                 SetIsData      (bool b)           { fIsData = b; }
    void                 SetTupleName   (const char* c)    { fTupleName = c; }
    void                 FindHiggsPtAndY(Float_t& pt, Float_t& Y, Float_t& mass);

  protected:

    void                 Begin();
    void                 Process();
    void                 SlaveBegin();
    void                 SlaveTerminate();
    void                 Terminate();

    // Names for the input Collections
    TString              fMCParticleName;

    const MCParticleCol *fMCParticles;
    Bool_t               fIsData;                 // is it Data or MC?
    
    TString fTupleName;

    // The output Ntuple
    TNtuple*             hMVAtuple;

    ClassDef(MVASystematicsMod, 1)
  };
}
#endif
