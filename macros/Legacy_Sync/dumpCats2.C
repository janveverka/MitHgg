// Modified script dumpCiC_moriond.C from Mingiming, which in turn
// is a modified script from Josh for signal modeling
// TODO: 
//   * Add mu1_phi, ele1_phi, mu2_*, ele2_*, numJets, numBJets
// Added to PhotonTreeWriter:
//   * mu1_phi, ele1_phi
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


#include "TString.h"

#include "MitHgg/PhotonTree/interface/MvaCategoryReader.h"

// Declaration of functions.
//_____________________________________________________________________________
void dumpCats(bool debug = true,
              TString fileName =
//                   "/home/veverka/cms/hist/hgg-2013Final8TeV/merged/"
                  "/home/mingyang/cms/hist/hgg-2013Final8TeV/merged/"
                  "hgg-2013Final8TeV_s12-h120gg-vh-v7n_noskim.root");
template <class T>
void dumpVar(const char *name, T value, bool appendTab=true);


//_____________________________________________________________________________
void dumpCats(bool debug, TString fileName) {
 
  TFile* file = TFile::Open(fileName.Data());

  // const char *treeName = "PhotonTreeWriterPresel";
  const char *treeName = "PhotonTreeWriterPreselNoSmear";
  TDirectory* theDir = (TDirectory*) file->FindObjectAny(treeName);
  TTree* theTree = (TTree*) theDir->Get("hPhotonTree");
  const char *combiWeights = "/home/veverka/cms/cmssw/031/CMSSW_5_3_10_patch1/"
    "src/MitHgg/PhotonTree/data/"
    "TMVA_vbf_dijet_dipho_evenbkg_scaledwt50_maxdPhi_Gradient.weights.xml";
  const char *dijetWeights = "/home/veverka/cms/cmssw/031/CMSSW_5_3_10_patch1/"
    "src/MitHgg/PhotonTree/data/"
    "TMVA_dijet_sherpa_scalewt50_2evenb_powheg200_maxdPhi_oct9_Gradient.weights.xml";
  const char *diphoWeights = "/home/veverka/cms/cmssw/031/CMSSW_5_3_10_patch1/"
    "src/MitPhysics/data/"
    "HggBambu_SMDipho_Oct01_redqcdweightallsigevenbkg_BDTG.weights.xml";
  mithep::hgg::MvaCategoryReader gg(theTree, combiWeights, dijetWeights,
                                    diphoWeights);


  int eventCounter=0;

  // Loop over the entries.
  std::cout << "Looping over " << theTree->GetEntries() << " entries." << std::endl;
  for (int i=0; i < theTree->GetEntries(); ++i) {
   
    if ((eventCounter > 9 || i > 20) && debug ) break;

    gg.GetEntry(i);

    if (debug) {
      cout << "Processing entry " << i << " :" << endl
           << "    run:      "  << gg.run        << endl
           << "    lumi:     "  << gg.lumi       << endl
           << "    event:    "  << gg.evt        << endl
           << "    mass:     "  << gg.mass       << endl
           << "    ph1pt:    "  << gg.ph1.pt     << endl
           << "    ph2pt:    "  << gg.ph2.pt     << endl
           << "    idmva_1:  "  << gg.ph1.idmva  << endl
           << "    idmva_2:  "  << gg.ph2.idmva  << endl
           << "    diphoMVA: "  << gg.diphoMVA   << endl;
    }

    bool passPreselection = (gg.mass > 100 &&
                             gg.mass < 180 &&
                             gg.ph1.pt > gg.mass/3 &&
                             gg.ph2.pt > gg.mass/4 &&
                             gg.ph1.idmva > -0.2 &&
                             gg.ph2.idmva > -0.2 &&
                             gg.diphoMVA > 0.4);

    if (passPreselection == false) {
      if (debug) {
        cout << "    passPreselection: " << passPreselection << endl;
      }
      continue;
    }

    if (debug) {
      cout << "    ... passed preselection." << endl;
    }

    eventCounter++;

    // Event Variables
    dumpVar("run"                    , gg.run                    ); //  1
    dumpVar("lumi"                   , gg.lumi                   ); //  2
    dumpVar("event"                  , gg.evt                    ); //  3

    
    dumpVar("cat"                    , gg.mvaCat              );
    dumpVar("tth"                    , gg.tthTag              );
    dumpVar("vhLep"                  , gg.VHLepTag            );
    dumpVar("vhMet"                  , gg.VHMetTag            );
    dumpVar("vhHad"                  , gg.VHHadTag            );
    dumpVar("vbf"                    , gg.dijetCat            );


//     // Leading Photon Variables
//     dumpVar("pho1_e"                 , ph1e                   ); // 10
//     dumpVar("pho1_eErr"              , ph1eerr                ); // 11
//     dumpVar("pho1_eta"               , teta1                  ); //  8
//     dumpVar("pho1_phi"               , phi1                   ); //  9
//     dumpVar("pho1_idMVA"             , idmva_1                );
// 
//     // Trailing Photon Variables
//     dumpVar("pho2_e"                 , ph2e                   ); // 36
//     dumpVar("pho2_eErr"              , ph2eerr                ); // 37
//     dumpVar("pho2_eta"               , teta2                  ); // 34
//     dumpVar("pho2_phi"               , phi2                   ); // 35
//     dumpVar("pho2_idMVA"             , idmva_2                );
// 
//     // Diphoton Variables
//     dumpVar("mass"                   , mass                   );
//     dumpVar("met"                    , corrpfmet              );
//     dumpVar("met_phi"                , corrpfmetphi           );
//     dumpVar("uncorrMet"              , pfmet                  );
//     dumpVar("uncorrMet_phi"          , pfmetphi               );
//     dumpVar("diphoMVA"               , diphoMVA               );
// 
//     // Muon Variables
//     if (mu1_pt < 0) {
//       mu1_pt = -999;
//       mu1_eta = -999;
//     }
//     dumpVar("mu1_pt"                , mu1_pt                 );
//     dumpVar("mu1_eta"               , mu1_eta                );
// 
//     // Electron Variables
//     if (ele1_pt < 0) {
//       ele1_pt = -999;
//       ele1_eta = -999;
//     }
//     dumpVar("ele1_pt"                , ele1_pt                 );
//     dumpVar("ele1_eta"               , ele1_eta                );
// 
//     
//     // Leading Jet Variables
//     if (jet1pt < 0) {
//       jet1pt = -999;
//       jet1eta = -999;
//     }
//     dumpVar("jet1_pt"                , jet1pt                 ); // 69
//     dumpVar("jet1_eta"               , jet1eta                ); // 70
//     dumpVar("jet1_phi"               , jet1phi                ); // 70
// 
//     // Trailing Jet Variables
//     if (jet2pt < 0) {
//       jet2pt = -999;
//       jet2eta = -999;
//     }
//     dumpVar("jet2_pt"                , jet2pt                 ); // 72
//     dumpVar("jet2_eta"               , jet2eta                ); // 73
//     dumpVar("jet2_phi"               , jet2phi                ); // 70

    std::cout << std::endl;
  } // Loop over the tree entries.
  
  return;

} // void dumpMvaInputs(bool debug, TString fileName)


//_____________________________________________________________________________
template <class T>
void dumpVar(const char *name, T value, bool appendTab) {
  std::cout << name << ":" << value;
  if (appendTab) {
    std::cout << "\t";
  }
}
