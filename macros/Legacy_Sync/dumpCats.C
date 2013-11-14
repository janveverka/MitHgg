// Modified script dumpCiC_moriond.C from Mingiming, which in turn
// is a modified script from Josh for signal modeling
// TODO: 
//   * Add mu1_phi, ele1_phi, mu2_*, ele2_*, numJets, numBJets
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "TMath.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TTree.h"
#include "TNtuple.h"
#include "RooDataSet.h"
#include "RooExponential.h"
#include "RooLandau.h"
#include "RooPlot.h"
#include "RooFit.h"
#include "RooAddPdf.h"
#include "RooGaussian.h"
#include "RooCBShape.h"
#include "RooFFTConvPdf.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooHistFunc.h"
#include "RooMoment.h"
#include "RooFitResult.h"
#include "RooExtendPdf.h"
#include "RooGenericPdf.h"
#include "RooBreitWigner.h"
#include "RooBifurGauss.h"
#include "RooProdPdf.h"      
#include "TArrow.h"

#include "RooCategory.h"
#include "RooSimultaneous.h"
#include "RooWorkspace.h"
#include "RooConstVar.h"
#include "TEfficiency.h"
#include "RooConstVar.h"
#include "RooAddition.h"

#include "TTreeFormula.h"
#include "TLatex.h"

#include "TString.h"

#include "RooHist.h"

#include "TMVA/Reader.h"
#include "TMVA/Tools.h"
#include "TMVA/Config.h"

#include "TRandom3.h"

#include "MitHgg/TreeWriter/interface/PhotonTree.h"

using namespace mithep::hgg;

enum Category {kIncl0      = 0, kIncl1, kIncl2, kIncl3,
               kDijet0     = 4, kDijet1, kDijet2,
               kVHLepTight = 7, kVHLepLoose,
               kVHMet      = 9,
               kTTHLep     = 10, kTTHHad,
               kVHHad      = 12};

// Declaration of functions.
//_____________________________________________________________________________
void dumpCats(bool debug = true,
              TString fileName =
//                   "/home/veverka/cms/hist/hgg-2013Final8TeV/merged/"
                  "/home/mingyang/cms/hist/hgg-2013Final8TeV/merged/"
                  "hgg-2013Final8TeV_s12-h120gg-vh-v7n_noskim.root");
// Please no extra white space!
void logThisEntry            (Int_t i, mithep::hgg::PhotonTree & gg);
void logPassedPreselection   ();
bool passesPreselection      (PhotonTree & gg);
void updateTTHTagConvention  (PhotonTree & gg);
void updateVHHadTagConvention(PhotonTree & gg);
void dumpRunLumiEvent        (PhotonTree & gg);
void dumpMetVariables        (PhotonTree & gg);
void dumpLeadingJetVariables (PhotonTree & gg);
void dumpTrailingJetVariables(PhotonTree & gg);
template <class T>
void dumpVar                 (const char *name, T value, bool appendTab=true);

//_____________________________________________________________________________
void dumpCats(bool debug, TString fileName) {
 
  TFile* file = TFile::Open(fileName.Data());

  // const char *treeName = "PhotonTreeWriterPresel";
  const char *dirName = "PhotonTreeWriterPreselNoSmear";
  TDirectory* theDir = (TDirectory*) file->FindObjectAny(dirName);
  TTree* theTree = (TTree*) theDir->Get("hPhotonTree");
  PhotonTree gg(theTree);

  Float_t  & mass = gg.mass;
  
  Int_t &  tth   = gg.tthTag;
  Int_t &  vhLep = gg.VHLepTag;
  Int_t &  vhHad = gg.VHHadTag;
  Int_t &  vbf   = gg.vbfTag;
  
  
  float ph1e, ph1pt, ph1eerr, ph1eerrsmeared, teta1, phi1;
  float ph2e, ph2pt, ph2eerr, ph2eerrsmeared, teta2, phi2;

  //theTree->SetBranchAddress("ph1.pt",&ph1pt);
  theTree->SetBranchAddress("ph1.e",&ph1e);
  theTree->SetBranchAddress("ph1.eerr",&ph1eerr);
  theTree->SetBranchAddress("ph1.eerrsmeared",&ph1eerrsmeared);
  theTree->SetBranchAddress("ph1.eta",&teta1);
  theTree->SetBranchAddress("ph1.phi",&phi1);

  // theTree->SetBranchAddress("ph2.pt",&ph2pt);
  theTree->SetBranchAddress("ph2.e",&ph2e);
  theTree->SetBranchAddress("ph2.eerr",&ph2eerr);
  theTree->SetBranchAddress("ph2.eerrsmeared",&ph2eerrsmeared);
  theTree->SetBranchAddress("ph2.eta",&teta2);
  theTree->SetBranchAddress("ph2.phi",&phi2);

  Float_t ele1_pt, ele1_eta;
  Float_t mu1_pt, mu1_eta;
  
  theTree->SetBranchAddress("elePt"  , &ele1_pt );
  theTree->SetBranchAddress("eleEta" , &ele1_eta);
  theTree->SetBranchAddress("muonPt" , &mu1_pt  );
  theTree->SetBranchAddress("muonEta", &mu1_eta );
  
  float jet1pt, jet1eta, jet1phi;
  float jet2pt, jet2eta, jet2phi;

  theTree->SetBranchAddress("jet1pt",&jet1pt);
  theTree->SetBranchAddress("jet1eta",&jet1eta);
  theTree->SetBranchAddress("jet1phi",&jet1phi);
  theTree->SetBranchAddress("jet2pt",&jet2pt);
  theTree->SetBranchAddress("jet2eta",&jet2eta);
  theTree->SetBranchAddress("jet2phi",&jet2phi);

  float masserr, masserrwvtx, masserr_ns, masserrwvtx_ns, vtxprob, idmva_1, idmva_2;
  theTree->SetBranchAddress("masserrsmeared",&masserr);
  theTree->SetBranchAddress("masserrsmearedwrongvtx",&masserrwvtx);
  theTree->SetBranchAddress("masserr",&masserr_ns);
  theTree->SetBranchAddress("masserrwrongvtx",&masserrwvtx_ns);
  theTree->SetBranchAddress("vtxprob",&vtxprob);
//   theTree->SetBranchAddress("ph1.idmva",&idmva_1);
//   theTree->SetBranchAddress("ph2.idmva",&idmva_2);

  // MET tag stuff
  float corrpfmet, corrpfmetphi, pfmet, pfmetphi;
  theTree->SetBranchAddress("corrpfmet",&corrpfmet);
  theTree->SetBranchAddress("corrpfmetphi",&corrpfmetphi);
  theTree->SetBranchAddress("pfmet",&pfmet);
  theTree->SetBranchAddress("pfmetphi",&pfmetphi);

  float phigg, jetleadNoIDpt, jetleadNoIDphi, jetleadNoIDeta;
  float ph1sceta, ph1scphi;
  float ph2sceta, ph2scphi;

  theTree->SetBranchAddress("phigg",&phigg);
  theTree->SetBranchAddress("jetleadNoIDpt",&jetleadNoIDpt);
  theTree->SetBranchAddress("jetleadNoIDphi",&jetleadNoIDphi);
  theTree->SetBranchAddress("jetleadNoIDeta",&jetleadNoIDeta);

  theTree->SetBranchAddress("ph1.sceta",&ph1sceta);
  theTree->SetBranchAddress("ph1.scphi",&ph1scphi);

  theTree->SetBranchAddress("ph2.sceta",&ph2sceta);
  theTree->SetBranchAddress("ph2.scphi",&ph2scphi);
 

  // Setup the diphoton BDT
  Float_t rVtxSigmaMoM, wVtxSigmaMoM, cosDPhi;
  Float_t pho1_ptOverM;
  Float_t pho2_ptOverM;
  Float_t diphoMVA;
  
  TMVA::Reader* reader = new TMVA::Reader();
  reader->AddVariable("masserrsmeared/mass"        , &rVtxSigmaMoM);
  reader->AddVariable("masserrsmearedwrongvtx/mass", &wVtxSigmaMoM);
  reader->AddVariable("vtxprob"                    , &vtxprob     );
  reader->AddVariable("ph1.pt/mass"                , &pho1_ptOverM);
  reader->AddVariable("ph2.pt/mass"                , &pho2_ptOverM);
  reader->AddVariable("ph1.eta"                    , &teta1       );
  reader->AddVariable("ph2.eta"                    , &teta2       );
  reader->AddVariable("TMath::Cos(ph1.phi-ph2.phi)", &cosDPhi     );
  reader->AddVariable("ph1.idmva"                  , &idmva_1     );
  reader->AddVariable("ph2.idmva"                  , &idmva_2     );
  const char *diphotonWeights = (
    "/home/veverka/cms/cmssw/031/CMSSW_5_3_10_patch1/src/MitPhysics/data/"
    "HggBambu_SMDipho_Oct01_redqcdweightallsigevenbkg_BDTG.weights.xml"
    );
  reader->BookMVA("BDTG", diphotonWeights);

  int eventCounter=0;

  // Loop over the entries.
  std::cout << "Looping over " << theTree->GetEntries() << " entries." << std::endl;
  for (int i=0; i < theTree->GetEntries(); ++i) {
   
    if (debug && eventCounter > 9) break;
    theTree->GetEntry(i);
    if (debug) logThisEntry(i, gg);
    if (passesPreselection(gg) == false) continue;
    eventCounter++;
    if (debug) logPassedPreselection();
    updateTTHTagConvention  (gg);
    updateVHHadTagConvention(gg);

    // MET category
    double dEtaJPh1 = ph1sceta - jetleadNoIDeta;
    double dPhiJPh1 = TMath::ACos(TMath::Cos(ph1scphi - jetleadNoIDphi));
    double dRJPh1 = TMath::Sqrt(TMath::Power(dEtaJPh1, 2) +
                                TMath::Power(dPhiJPh1, 2));
    double dEtaJPh2 = ph2sceta - jetleadNoIDeta;
    double dPhiJPh2 = TMath::ACos(TMath::Cos(ph2scphi - jetleadNoIDphi));
    double dRJPh2 = TMath::Sqrt(TMath::Power(dEtaJPh2, 2) +
                                TMath::Power(dPhiJPh2, 2));
    double dPhiMetGG = TMath::ACos(TMath::Cos(phigg - corrpfmetphi));
    double dPhiMetJet = TMath::ACos(
             TMath::Cos(TMath::Abs(jetleadNoIDphi - corrpfmetphi))
             );
    Int_t vhMet = 0;
    if (TMath::Abs(ph1sceta) < 1.4442 &&
        TMath::Abs(ph2sceta) < 1.4442 &&
        corrpfmet > 70. &&
        ph1pt/mass > 45./120. &&
        dPhiMetGG > 2.1 &&
        (
          jetleadNoIDpt < 50. ||
          dRJPh1 < 0.5 ||
          dRJPh2 < 0.5 ||
          dPhiMetJet < 2.7
        ) &&
        ph2pt > mass/4) {
       vhMet = 1;
    }

    // Calculate needed variables for the diphoMVA
    rVtxSigmaMoM = masserr_ns / mass;       // no smearing
    wVtxSigmaMoM = masserrwvtx_ns / mass;   // no smearing
    // rVtxSigmaMoM = masserr / mass;          // with smearing
    // wVtxSigmaMoM = masserrwvtx / mass;      // with smearing
    cosDPhi = TMath::Cos(phi1 - phi2);
    pho1_ptOverM = ph1pt / mass;
    pho2_ptOverM = ph2pt / mass;
    diphoMVA = reader->EvaluateMVA("BDTG");

  // Calculate the category in the correct order.
    Category cat = kIncl0;
    if      (tth   == 2) cat = kTTHLep;
    else if (vhLep == 2) cat = kVHLepTight;
    else if (vhLep == 1) cat = kVHLepLoose;
    else if (vbf   >  0) cat = kDijet0;
    else if (vhMet == 1) cat = kVHMet;
    else if (tth   == 1) cat = kTTHHad;
    else if (vhHad == 1) cat = kVHHad;

    dumpRunLumiEvent(gg);

    dumpVar("cat"                    , cat                 );
    dumpVar("tth"                    , tth                 );
    dumpVar("vhLep"                  , vhLep               );
    dumpVar("vhMet"                  , vhMet               );
    dumpVar("vhHad"                  , vhHad               );
    dumpVar("vbf"                    , vbf                 );

    // Leading Photon Variables
    dumpVar("pho1_e"                 , ph1e                   ); // 10
    dumpVar("pho1_eErr"              , ph1eerr                ); // 11
    dumpVar("pho1_eta"               , teta1                  ); //  8
    dumpVar("pho1_phi"               , phi1                   ); //  9
    dumpVar("pho1_idMVA"             , idmva_1                );

    // Trailing Photon Variables
    dumpVar("pho2_e"                 , ph2e                   ); // 36
    dumpVar("pho2_eErr"              , ph2eerr                ); // 37
    dumpVar("pho2_eta"               , teta2                  ); // 34
    dumpVar("pho2_phi"               , phi2                   ); // 35
    dumpVar("pho2_idMVA"             , idmva_2                );

    // Diphoton Variables
    dumpVar("mass"                   , mass                );
    dumpVar("diphoMVA"               , diphoMVA    );
    
    // Muon Variables
    if (mu1_pt < 0) {
      mu1_pt = -999;
      mu1_eta = -999;
    }
    dumpVar("mu1_pt"                , mu1_pt                 );
    dumpVar("mu1_eta"               , mu1_eta                );

    // Electron Variables
    if (ele1_pt < 0) {
      ele1_pt = -999;
      ele1_eta = -999;
    }
    dumpVar("ele1_pt"                , ele1_pt                 );
    dumpVar("ele1_eta"               , ele1_eta                );


    dumpLeadingJetVariables(gg);
    dumpTrailingJetVariables(gg);
    std::cout << std::endl;

  } // Loop over the tree entries.
  
  return;

} // void dumpMvaInputs(bool debug, TString fileName)

//_____________________________________________________________________________
void logThisEntry(Int_t i, PhotonTree & gg)
{
  cout << "Processing entry " << i << " :" << endl
        << "    mass     : " << gg.mass          << endl
        << "    ph1.pt   : " << gg.ph1F("pt"   ) << endl
        << "    ph2.pt   : " << gg.ph2F("pt"   ) << endl
        << "    ph1.idmva: " << gg.ph1F("idmva") << endl
        << "    ph2.idmva: " << gg.ph2F("idmva") << endl;
}


//_____________________________________________________________________________
void logPassedPreselection()
{
  cout << "    ... passed preselection." << endl;
}


//_____________________________________________________________________________
bool passesPreselection(PhotonTree & gg)
{
  return (gg.mass > 100 &&
          gg.mass < 180 &&
          gg.ph1F("pt") > gg.mass/3 &&
          gg.ph2F("pt") > gg.mass/4 &&
          gg.ph1F("idmva") > -0.2 &&
          gg.ph2F("idmva") > -0.2);
}

//_____________________________________________________________________________
// To comply with the Hgg group convention that differs from ours at the
// moment.
void updateTTHTagConvention(PhotonTree & gg)
{
  if      (gg.tthTag   == 1) gg.tthTag = 2;
  else if (gg.tthTag   == 2) gg.tthTag = 1;
}


//_____________________________________________________________________________
// Poor man's merging of the to b-tag and no-b-tag categories
void updateVHHadTagConvention(PhotonTree & gg)
{
    if (gg.VHHadTag == 2) gg.VHHadTag = 1;
}


//_____________________________________________________________________________
void dumpRunLumiEvent(PhotonTree & gg)
{
  dumpVar("run"   , gg.run );
  dumpVar("lumi"  , gg.lumi);
  dumpVar("event" , gg.evt );
}


//_____________________________________________________________________________
void dumpMetVariables(PhotonTree & gg)
{
  dumpVar("met"          , gg.corrpfmet   );
  dumpVar("met_phi"      , gg.corrpfmetphi);
  dumpVar("uncorrMet"    , gg.pfmet       );
  dumpVar("uncorrMet_phi", gg.pfmetphi    );
}


//_____________________________________________________________________________
void dumpLeadingJetVariables(PhotonTree & gg)
{
  if (gg.jet1pt < 0) {
    gg.jet1pt  = -999;
    gg.jet1eta = -999;
  }
  dumpVar("jet1_pt" , gg.jet1pt );
  dumpVar("jet1_eta", gg.jet1eta);
  dumpVar("jet1_phi", gg.jet1phi);
}


//_____________________________________________________________________________
void dumpTrailingJetVariables(PhotonTree & gg)
{
  if (gg.jet2pt < 0) {
    gg.jet2pt  = -999;
    gg.jet2eta = -999;
  }
  dumpVar("jet2_pt" , gg.jet2pt );
  dumpVar("jet2_eta", gg.jet2eta);
  dumpVar("jet2_phi", gg.jet2phi);
}


//_____________________________________________________________________________
template <class T>
void dumpVar(const char *name, T value, bool appendTab) {
  std::cout << name << ":" << value;
  if (appendTab) {
    std::cout << "\t";
  }
}
