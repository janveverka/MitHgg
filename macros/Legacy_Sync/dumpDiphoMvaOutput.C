// Modified script dumpCiC_moriond.C from Mingiming, which in turn
// is a modified script from Josh for signal modeling
// TODO: 
// * Add per-photon variables bcInd, nBC, deltaEtaBCSC, deltaPhiBCSC
//   scInd
// * Add vertex variables
// * Fix ESEffSigmaRR in the barrel
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


// Declaration of functions.
//_____________________________________________________________________________
void dumpDiphoMvaOutput();
template <class T>
void dumpVar(const char *name, T value, bool appendTab=true);


//_____________________________________________________________________________
void dumpDiphoMvaOutput() {
  Float_t rVtxSigmaMoM = 0.0115566;
  Float_t wVtxSigmaMoM = 0.0271047;
  Float_t vtxprob      = 0.996821;
  Float_t pho1_ptOverM = 0.684849;
  Float_t pho2_ptOverM = 0.429963;
  Float_t teta1        = 1.96677;
  Float_t teta2        = 1.16757;
  Float_t cosDPhi      = -0.3613;
  Float_t idmva_1      = 0.0594218;
  Float_t idmva_2      = 0.179005;
  Float_t globe_bdt = 0.0511577;

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
  Float_t mit_bdt = reader->EvaluateMVA("BDTG");

  dumpVar("rVtxSigmaMoM", rVtxSigmaMoM);
  dumpVar("wVtxSigmaMoM", wVtxSigmaMoM);
  dumpVar("vtxprob     ", vtxprob     );
  dumpVar("pho1_ptOverM", pho1_ptOverM);
  dumpVar("pho1_ptOverM", pho1_ptOverM);
  dumpVar("teta1       ", teta1       );
  dumpVar("teta2       ", teta2       );
  dumpVar("cosDPhi     ", cosDPhi     );
  dumpVar("idmva_1     ", idmva_1     );
  dumpVar("idmva_2     ", idmva_2     );
  dumpVar("diphoMVA Globe", globe_bdt);
  dumpVar("diphoMVA MIT", mit_bdt);



 }
