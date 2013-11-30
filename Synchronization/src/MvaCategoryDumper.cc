#include <iostream>  // std::cout
#include <string>
#include "MitHgg/Synchronization/interface/MvaCategoryDumper.h"
#include "MitHgg/PhotonTree/interface/TestTreeFactory.h"

using ::mithep::hgg::MvaCategoryDumper;
using ::mithep::hgg::PSet;

//------------------------------------------------------------------------------
MvaCategoryDumper::MvaCategoryDumper(TTree *tree, const PSet &iConfig) :
  MvaCategoryReader(
    tree,
    iConfig.getParameter<std::string>("diphotonMvaWeights").c_str(),
    iConfig.getParameter<std::string>("dijetMvaWeights"   ).c_str(),
    iConfig.getParameter<std::string>("combinedMvaWeights").c_str(),
    iConfig.getParameter<bool  >("diphotonMvaUseSmearedMassError"),
    iConfig.getParameter<double>("dijetMvaMaxDPhi")
  )
{
  Init(iConfig);
} /// Ctor


//------------------------------------------------------------------------------
MvaCategoryDumper::~MvaCategoryDumper() {} /// Dtor


//------------------------------------------------------------------------------
void
MvaCategoryDumper::Init(const PSet &iConfig) 
{
  typedef std::vector<double> vdouble;
  if (iConfig.existsAs<vdouble>("diphotonBdtBoundaries")) {
    SetDiphoMvaCuts(iConfig.getParameter<vdouble>("diphotonBdtBoundaries"));
  }
  if (iConfig.existsAs<vdouble>("dijetBdtBoundaries")) {
    SetDijetMvaCuts(iConfig.getParameter<vdouble>("dijetBdtBoundaries"));
  }
  if (iConfig.existsAs<vdouble>("combinedBdtBoundaries")) {
    SetCombiMvaCuts(iConfig.getParameter<vdouble>("combinedBdtBoundaries"));
  }
  
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
MvaCategoryDumper::ProduceDump()
{
  for (unsigned iEntry=0; iEntry < fTree->GetEntries(); iEntry++) {
    GetEntry(iEntry);
    if (mvaCat < 0) continue;
    DumpAllVariables();
    std::cout << "\b\n";
  } /// Loop over entries
} /// ProduceDump


//------------------------------------------------------------------------------
void
MvaCategoryDumper::DumpAllVariables()
{
  DumpEventHeader();
  DumpPhotons();
  DumpCategoryVariables();
  DumpDiphotonVariables();
  DumpMuons();
  DumpElectrons();
  DumpJets();
} /// DumpAllVariables


//------------------------------------------------------------------------------
void
MvaCategoryDumper::DumpEventHeader()
{
  DumpVar("run"  , run );
  DumpVar("lumi" , lumi);
  DumpVar("event", evt );
} /// DumpEventHeader


//------------------------------------------------------------------------------
void
MvaCategoryDumper::DumpPhotons()
{
  /// Lead
  DumpVar("pho1_e"    , ph1.e    );
  DumpVar("pho1_eErr" , ph1.eerr );
  DumpVar("pho1_eta"  , ph1.eta  );
  DumpVar("pho1_phi"  , ph1.phi  );
  DumpVar("pho1_idMVA", ph1.idmva);
  /// Sublead
  DumpVar("pho2_e"    , ph2.e    );
  DumpVar("pho2_eErr" , ph2.eerr );
  DumpVar("pho2_eta"  , ph2.eta  );
  DumpVar("pho2_phi"  , ph2.phi  );
  DumpVar("pho2_idMVA", ph2.idmva);
} /// DumpPhotons


//------------------------------------------------------------------------------
void
MvaCategoryDumper::DumpCategoryVariables()
{
  DumpVar("cat"     , mvaCat  );
  DumpVar("tth"     , tthTag  );
  DumpVar("vhLep"   , VHLepTag);
  DumpVar("vhMet"   , VHMetTag);
  DumpVar("vhHad"   , VHHadTag);
  DumpVar("numJets" , numJets );
  DumpVar("numBJets", numBJets);
} /// DumpCategoryVariables


//------------------------------------------------------------------------------
void
MvaCategoryDumper::DumpDiphotonVariables()
{
  if (dijetCat < 0 ) dijetMVA = combiMVA = -999;
  if (VHHadTag <= 0) costhetastar = -999;
  
  DumpVar("mass"         , mass        );
  DumpVar("met"          , corrpfmet   );
  DumpVar("met_phi"      , corrpfmetphi);
  DumpVar("uncorrMet"    , pfmet       );
  DumpVar("uncorrMet_phi", pfmetphi    );
  DumpVar("diphoMVA"     , diphoMVA    );
  DumpVar("dijetMVA"     , dijetCat    );
  DumpVar("combiMVA"     , dijetCat    );
  DumpVar("cosThetaStar" , VHHadTag    );
} /// DumpDiphotonVariables


//------------------------------------------------------------------------------
void
MvaCategoryDumper::DumpMuons()
{
  /// Lead
  if (mu2Pt < 0) {
    /// No dimuon in this event
    mu2Pt = mu2Eta = mu2Phi = -999;
    mu1Pt  = muonPt;
    mu1Eta = muonEta;
    mu1Phi = muonPhi;
  }
  
  if (mu1Pt < 0) {
    mu1Pt = mu1Eta = mu1Phi = -999;
  }
  
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
MvaCategoryDumper::DumpElectrons()
{
  /// Lead
  if (ele2Pt > 0) {
    /// We have a dielectron
    DumpVar("ele1_pt"  , ele1Pt );
    DumpVar("ele1_eta" , ele1Eta);
    DumpVar("ele1_phi" , ele1Phi);
  } else {
    DumpVar("ele1_pt"  , elePt );
    DumpVar("ele1_eta" , eleEta);
    DumpVar("ele1_phi" , elePhi);
  }
  /// Sublead
  DumpVar("ele2_pt"  , ele2Pt );
  DumpVar("ele2_eta" , ele2Eta);
  DumpVar("ele2_phi" , ele2Phi);
} /// DumpElectrons


//------------------------------------------------------------------------------
void
MvaCategoryDumper::DumpJets()
{
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
template <class T>
void
MvaCategoryDumper::DumpVar(const char *name, T value, const char *suffix)
{
  std::cout << name << ":" << value << suffix;
} /// DumpVar
