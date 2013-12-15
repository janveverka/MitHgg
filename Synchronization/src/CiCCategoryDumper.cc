#include <algorithm> // std::abs
#include <iostream>  // std::cout
#include <string>
#include "FWCore/Utilities/interface/Exception.h"
#include "MitHgg/Synchronization/interface/CiCCategoryDumper.h"
#include "MitHgg/PhotonTree/interface/TestTreeFactory.h"

using ::std::string;
using ::mithep::hgg::CiCCategoryDumper;
using ::mithep::hgg::PSet;

//------------------------------------------------------------------------------
CiCCategoryDumper::CiCCategoryDumper(TTree *tree, const PSet &iConfig) :
  CiCCategoryReader(
    tree                 ,
    GetBeamEnergy(iConfig)
  ),
  fMaxEntries(iConfig.getUntrackedParameter<int>("maxEntriesToProcess", -1))
{
  Init(iConfig);
} /// Ctor


//------------------------------------------------------------------------------
CiCCategoryDumper::~CiCCategoryDumper() {} /// Dtor


//------------------------------------------------------------------------------
CiCCategoryDumper::EBeamEnergy
CiCCategoryDumper::GetBeamEnergy(const PSet &iConfig)
{
  EBeamEnergy beamEnergy = EBeamEnergy::k8TeV;
  if (iConfig.existsAs<string>("beamEnergy")) {
    const string &iBeamEnergy = iConfig.getParameter<string>("beamEnergy");
    if (iBeamEnergy == "7TeV") {
      beamEnergy = EBeamEnergy::k7TeV;
    } else if (iBeamEnergy == "8TeV") {
      beamEnergy = EBeamEnergy::k8TeV;
    } else {
      /// This should never happen!
      cms::Exception exception("BadConfiguration|ParamberValue");
      exception << "Illegal value of beamEnergy: \"" << iBeamEnergy
                << "\". Must be one of \"7TeV\" or \"8TeV\"";
      throw exception;
    }
  } /// beamEnergy exists
  
  return beamEnergy;
} /// GetBeamEnergy


//------------------------------------------------------------------------------
void
CiCCategoryDumper::Init(const PSet &iConfig)
{
  /// Performance  
  fTree->SetBranchStatus("ph1.*seed"   , 0);
  fTree->SetBranchStatus("ph1.*bc2"    , 0);
  fTree->SetBranchStatus("ph1.*bclast" , 0);
  fTree->SetBranchStatus("ph1.*bclast2", 0);
  fTree->SetBranchStatus("ph1.conv*"   , 0);
  fTree->SetBranchStatus("ph1.pfsc*"   , 0);
  fTree->SetBranchStatus("ph1.pfcic4_*", 0);
  fTree->SetBranchStatus("ph1.idmva_*" , 0);
  fTree->SetBranchStatus("ph1.*bcs*"   , 0);
  fTree->SetBranchStatus("ph1.*psc*"   , 0);
  
  fTree->SetBranchStatus("ph2.*seed"   , 0);
  fTree->SetBranchStatus("ph2.*bc2"    , 0);
  fTree->SetBranchStatus("ph2.*bclast" , 0);
  fTree->SetBranchStatus("ph2.*bclast2", 0);
  fTree->SetBranchStatus("ph2.conv*"   , 0);
  fTree->SetBranchStatus("ph2.pfsc*"   , 0);
  fTree->SetBranchStatus("ph2.pfcic4_*", 0);
  fTree->SetBranchStatus("ph2.idmva_*" , 0);
  fTree->SetBranchStatus("ph2.*bcs*"   , 0);
  fTree->SetBranchStatus("ph2.*psc*"   , 0);
  
  fTree->SetCacheSize(5e8);
  fTree->AddBranchToCache("*");
  fTree->DropBranchFromCache("ph1.*");
  fTree->DropBranchFromCache("ph2.*");
  
  
} /// Init


//------------------------------------------------------------------------------
void
CiCCategoryDumper::ProduceDump()
{
  unsigned numEntriesToProcess = fTree->GetEntries();
  if (fMaxEntries > 0 && fMaxEntries < (int) numEntriesToProcess) {
    numEntriesToProcess = fMaxEntries;
  }
  for (unsigned iEntry=0; iEntry < numEntriesToProcess; iEntry++) {
    GetEntry(iEntry);
    if (cicCat < 0) continue;
    DumpAllVariables();
    std::cout << "\n";
  } /// Loop over entries
} /// ProduceDump


//------------------------------------------------------------------------------
void
CiCCategoryDumper::DumpAllVariables()
{
  DumpEventHeader();
  DumpPhotons();
  DumpCategoryVariables();
  DumpDiphotonVariables();
  DumpVertexVariables();
  DumpMuons();
  DumpElectrons();
  DumpJets();
  DumpDijetVariables();
} /// DumpAllVariables


//------------------------------------------------------------------------------
void
CiCCategoryDumper::DumpEventHeader()
{
  DumpVar("run"  , run );
  DumpVar("lumi" , lumi);
  DumpVar("event", evt );
} /// DumpEventHeader


//------------------------------------------------------------------------------
void
CiCCategoryDumper::DumpPhotons()
{
  /// Lead
  DumpPhoton("pho1_", ph1);
  /// Sublead
  DumpPhoton("pho2_", ph2);
} /// DumpPhotons


//------------------------------------------------------------------------------
void
CiCCategoryDumper::DumpCategoryVariables()
{
  DumpVar("cat"     , cicCat  );
  DumpVar("tth"     , tthTag  );
  DumpVar("vhLep"   , VHLepTag);
  DumpVar("vhMet"   , VHMetTag);
  DumpVar("vhHad"   , VHHadTag);
  DumpVar("numJets" , numJets );
  DumpVar("numBJets", numBJets);
} /// DumpCategoryVariables


//------------------------------------------------------------------------------
void
CiCCategoryDumper::DumpDiphotonVariables()
{  
  if (IsUnset(costhetastar)) costhetastar = -999;
  else                       costhetastar = std::abs(costhetastar);
  
  if (IsUnset(bjetcsv     )) bjetcsv      = -999;
  
  DumpVar("mass"         , mass        );
  DumpVar("met"          , corrpfmet   );
  DumpVar("met_phi"      , corrpfmetphi);
  DumpVar("uncorrMet"    , pfmet       );
  DumpVar("uncorrMet_phi", pfmetphi    );
  DumpVar("cosThetaStar" , costhetastar);
  DumpVar("bjet_csv"     , bjetcsv     );
  DumpVar("rho"          , rho         );
} /// DumpDiphotonVariables


//------------------------------------------------------------------------------
void
CiCCategoryDumper::DumpVertexVariables()
{
  DumpVar("probmva"   , vtxprob );
  DumpVar("vertexId0" , vtxInd1 );
  DumpVar("vertexMva0", vtxMva1 );
  DumpVar("vertex_z"  , vtxMva1Z);  
} /// DumpVertexVariables
  
  
//------------------------------------------------------------------------------
void
CiCCategoryDumper::DumpMuons()
{
  if (mu2Pt < 0) {
    /// No dimuon present
    mu2Pt = mu2Eta = mu2Phi = -999;
    mu1Pt  = muonPt;
    mu1Eta = muonEta;
    mu1Phi = muonPhi;
  }
  
  /// No muon present
  if (mu1Pt < 0) {
    mu1Pt = mu1Eta = mu1Phi = -999;
  }
  
  /// Lead
  DumpVar("mu1_pt"  , mu1Pt );
  DumpVar("mu1_eta" , mu1Eta);
  DumpVar("mu1_phi" , mu1Phi);
  /// Sublead
  DumpVar("mu2_pt"  , mu2Pt );
  DumpVar("mu2_eta" , mu2Eta);
  DumpVar("mu2_phi" , mu2Phi);  
} /// DumpMuons


//------------------------------------------------------------------------------
void
CiCCategoryDumper::DumpElectrons()
{
  if (ele2Pt < 0) {
    /// No dielectron present
    ele2Pt = ele2Eta = ele2Phi = -999;
    ele1Pt  = elePt;
    ele1Eta = eleEta;
    ele1Phi = elePhi;
  }

  if (ele1Pt < 0) {
    /// No electron present
    eleIdMva = ele1Pt = ele1Eta = ele1Phi = -999;
  }
  
  /// Lead
  DumpVar("ele1_pt"  , ele1Pt );
  DumpVar("ele1_eta" , ele1Eta);
  DumpVar("ele1_phi" , ele1Phi);
  /// Sublead
  DumpVar("ele2_pt"  , ele2Pt );
  DumpVar("ele2_eta" , ele2Eta);
  DumpVar("ele2_phi" , ele2Phi);
} /// DumpElectrons


//------------------------------------------------------------------------------
void
CiCCategoryDumper::DumpJets(void)
{
  if (jet1pt < 0) jet1pt = jet1eta = jet1phi = -999;
  if (jet2pt < 0) jet2pt = jet2eta = jet2phi = -999;    
  
  /// Lead
  DumpVar("jet1_pt" , jet1pt );
  DumpVar("jet1_eta", jet1eta);
  DumpVar("jet1_phi", jet1phi);
  /// Sublead
  DumpVar("jet2_pt" , jet2pt );
  DumpVar("jet2_eta", jet2eta);
  DumpVar("jet2_phi", jet2phi); 
} /// DumpJets


//------------------------------------------------------------------------------
void
CiCCategoryDumper::DumpDijetVariables(void)
{
  float vbf_dEtaJJ = std::abs(jet1eta - jet2eta);
  if (jet1pt < 0 || jet2pt < 0) {
    dijetmass = zeppenfeld = dphidijetgg = vbf_dEtaJJ = -999;
  }  
  /// TODO: Add vhHad_mass_dijet to the tree writer
  float vhHad_mass_dijet = -999;  // not implemented yet

  DumpVar("vbf_massJJ"      , dijetmass  );
  DumpVar("vbf_Zeppenfeld"  , zeppenfeld );
  DumpVar("vbf_dPhiJJGG"    , dphidijetgg);
  DumpVar("vbf_dEtaJJ"      , vbf_dEtaJJ );
  
  DumpVar("vhHad_mass_dijet", vhHad_mass_dijet, ""); /// no trailing tab
} /// DumpDijetVariables


//------------------------------------------------------------------------------
void
CiCCategoryDumper::DumpPhoton(const char *prefix, PhotonReader &photon)
{
  DumpVar(prefix, "e"      , photon.e     );
  DumpVar(prefix, "eErr"   , photon.eerr * photon.e);
  DumpVar(prefix, "EnScale", photon.escale);
  DumpVar(prefix, "eta"    , photon.eta   );
  DumpVar(prefix, "phi"    , photon.phi   );
  DumpVar(prefix, "idMVA"  , photon.idmva );
  DumpVar(prefix, "r9"     , photon.r9    );
  
} /// DumpPhoton


//------------------------------------------------------------------------------
template <class T>
void
CiCCategoryDumper::DumpVar(const char *name, T value, const char *suffix)
{
  std::cout << name << ":" << value << suffix;
} /// DumpVar

//------------------------------------------------------------------------------
template <class T>
void
CiCCategoryDumper::DumpVar(const char *prefix, const char *name, T value,
                           const char *suffix)
{
  std::cout << prefix << name << ":" << value << suffix;
} /// DumpVar
