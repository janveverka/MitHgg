// $Id: HggAnalysisMod.cc,v 1.3 2011/07/08 17:55:21 fabstoec Exp $

#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TNtuple.h>
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataTree/interface/PhotonCol.h"
#include "MitHgg/Mods/interface/HggAnalysisMod.h"

using namespace mithep;

ClassImp(mithep::HggAnalysisMod)

//--------------------------------------------------------------------------------------------------
HggAnalysisMod::HggAnalysisMod(const char *name, const char *title) :
  BaseMod             (name,title),

  fPhotonName         (Names::gkPhotonBrn),
  fPhotons            (0),
  fPhotonsFromBranch  (kTRUE),
  
  fSelectedPhotons    (0)

{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void HggAnalysisMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void HggAnalysisMod::Process()
{  
  IncNEventsProcessed();
  LoadEventObject(fPhotonName,fPhotons);

  ResetSelectedPhotons();
  for (UInt_t i=0; i<fPhotons->GetEntries(); ++i)
    fSelectedPhotons.push_back(i);
  
  if(fSelectedPhotons.size() < 2 ) return;
  
  const Photon* _tPh1 = fPhotons->At(fSelectedPhotons[0]);
  const Photon* _tPh2 = fPhotons->At(fSelectedPhotons[1]);
  
  Float_t _hmass       = ( _tPh1 && _tPh2 ? (_tPh1->Particle::Mom()+_tPh2->Particle::Mom()).M() : -100. );  
  hHggAnaMass->Fill(_hmass);
  
  return;
}

//--------------------------------------------------------------------------------------------------
void HggAnalysisMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches or objects created by earlier modules.

  ReqEventObject(fPhotonName,fPhotons,fPhotonsFromBranch);
  
  hHggAnaMass = new TH1D("hHggAnaMass","",100,100.,150.);  
  AddOutput(hHggAnaMass);

}

//--------------------------------------------------------------------------------------------------
void HggAnalysisMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void HggAnalysisMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void HggAnalysisMod::ResetSelectedPhotons() {
  fSelectedPhotons.clear();
  return;
}
