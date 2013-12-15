#include <algorithm>    // std::min
#include <complex>      // std::abs
#include <iostream>     // std::cout
#include <cppunit/extensions/HelperMacros.h>
#include "MitHgg/PhotonTree/interface/TreeReader.h"
#include "MitHgg/PhotonTree/interface/DiphotonAndDijetMvaReader.h"
#include "MitHgg/PhotonTree/interface/TestTreeFactory.h"

using ::mithep::hgg::TreeReader;
using ::mithep::hgg::DiphotonAndDijetMvaReader;
using namespace std;
//------------------------------------------------------------------------------
/**
 * DiphotonAndDijetMvaReader test fixture
 */
class DiphotonAndDijetMvaReaderTest : public ::CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(DiphotonAndDijetMvaReaderTest);
  CPPUNIT_TEST(testWeightReading);
  CPPUNIT_TEST_SUITE_END();

public:
  typedef std::pair<UInt_t, UInt_t> RunEvent;
  typedef std::map<RunEvent, Float_t> EventMvaMap;
  void setUp(void);
  void tearDown(void);

protected:
  void testWeightReading(void);
  void setUpDijetMvaReader(void);
  void setUpDiphoMvaReader(void);
  void evaluateDijetMvaWeight(void);
  void evaluateDiphoMvaWeight(void);

private:
  TreeReader                *treeReader  ;
  DiphotonAndDijetMvaReader *actualReader;
  /// Diphoton MVA stuff
  Float_t            rVtxSigmaMoM  ;
  Float_t            wVtxSigmaMoM  ;
  Float_t            cosDPhi       ;
  Float_t            pho1_ptOverM  ;
  Float_t            pho2_ptOverM  ;
  Float_t            diphoMVA      ;
  bool    fDiphoUseSmearedMassError;
  TMVA::Reader *     fDiphoMvaReader;  
  /// Dijet MVA stuff
  Float_t dijet_DPhiTruncated;
  Float_t ptgg_over_mass;
  Float_t dijetMVA;
  TMVA::Reader *fDijetMvaReader;
}; /// DiphotonAndDijetMvaReaderTest


//------------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(DiphotonAndDijetMvaReaderTest);

//------------------------------------------------------------------------------
void
DiphotonAndDijetMvaReaderTest::setUp(void)
{
  TTree *tree1 = ::mithep::hgg::TestTreeFactory::Create();
  TTree *tree2 = static_cast<TTree*>(tree1->Clone());
  treeReader   = new TreeReader               (tree1);
  actualReader = new DiphotonAndDijetMvaReader(tree2);
  
  fDiphoUseSmearedMassError=false; /// default is true
  actualReader->SetDiphoMvaUseSmearedMassError(fDiphoUseSmearedMassError);
  
  setUpDiphoMvaReader();
  setUpDijetMvaReader();
} /// setUp


//------------------------------------------------------------------------------
void
DiphotonAndDijetMvaReaderTest::tearDown(void)
{
  delete treeReader;
  delete actualReader;
  delete fDijetMvaReader;
  delete fDiphoMvaReader;
} /// tearDown

//------------------------------------------------------------------------------
void
DiphotonAndDijetMvaReaderTest::testWeightReading(void)
{
  bool verbose = false;
  Float_t delta = 0.08;
  if (verbose) {
    std::cout << "\nDiphotonAndDijetMvaReaderTest::testWeightReading ... \n";
  } /// verbose
  for (int i=0; i < treeReader->GetEntries(); i++) {
    treeReader  ->GetEntry(i);
    actualReader->GetEntry(i);
    evaluateDijetMvaWeight();
    evaluateDiphoMvaWeight();
    if (verbose) {
      std::cout << "   run:"             << actualReader->run
                << "   lumi:"            << actualReader->lumi
                << "   event:"           << actualReader->evt
                << "   dijetMVA(MIT):"   << actualReader->dijetMVA << "\n";
    } /// verbose
    CPPUNIT_ASSERT_DOUBLES_EQUAL(diphoMVA, actualReader->diphoMVA, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(dijetMVA, actualReader->dijetMVA, delta);
  } /// Loop over entries.
} /// testWeightReading


//------------------------------------------------------------------------------
void
DiphotonAndDijetMvaReaderTest::setUpDijetMvaReader(void)
{
  fDijetMvaReader = new TMVA::Reader("Silent");

  fDijetMvaReader->AddVariable("dijet_leadEta"       , &treeReader->jet1eta   );
  fDijetMvaReader->AddVariable("dijet_subleadEta"    , &treeReader->jet2eta   );
  fDijetMvaReader->AddVariable("dijet_LeadJPt"       , &treeReader->jet1pt    );
  fDijetMvaReader->AddVariable("dijet_SubJPt"        , &treeReader->jet2pt    );
  fDijetMvaReader->AddVariable("dijet_Zep"           , &treeReader->zeppenfeld);
  fDijetMvaReader->AddVariable("min(dijet_dPhi,2.916)", &dijet_DPhiTruncated  );
  fDijetMvaReader->AddVariable("dijet_Mjj"           , &treeReader->dijetmass );
  fDijetMvaReader->AddVariable("dipho_pt/mass"       , &ptgg_over_mass        );

  fDijetMvaReader->BookMVA("BDTG", actualReader->GetDijetMvaWeights());
} /// setUpDijetMvaReader  


//------------------------------------------------------------------------------
void
DiphotonAndDijetMvaReaderTest::setUpDiphoMvaReader(void)
{
  fDiphoMvaReader = new TMVA::Reader("Silent");  
  TMVA::Reader &myReader = *fDiphoMvaReader;

  myReader.AddVariable("masserrsmeared/mass"        , &rVtxSigmaMoM         );
  myReader.AddVariable("masserrsmearedwrongvtx/mass", &wVtxSigmaMoM         );
  myReader.AddVariable("vtxprob"                    , &treeReader->vtxprob  );
  myReader.AddVariable("ph1.pt/mass"                , &pho1_ptOverM         );
  myReader.AddVariable("ph2.pt/mass"                , &pho2_ptOverM         );
  myReader.AddVariable("ph1.eta"                    , &treeReader->ph1.eta  );
  myReader.AddVariable("ph2.eta"                    , &treeReader->ph2.eta  );
  myReader.AddVariable("TMath::Cos(ph1.phi-ph2.phi)", &cosDPhi              );
  myReader.AddVariable("ph1.idmva"                  , &treeReader->ph1.idmva);
  myReader.AddVariable("ph2.idmva"                  , &treeReader->ph2.idmva);

  myReader.BookMVA("BDTG", actualReader->GetDiphoMvaWeights());
} /// setUpDiphoMvaReader


//------------------------------------------------------------------------------
void
DiphotonAndDijetMvaReaderTest::evaluateDijetMvaWeight(void)
{
  dijet_DPhiTruncated = std::min(std::abs(treeReader->dphidijetgg), 
                                 actualReader->GetDijetMvaMaxDPhi());
  ptgg_over_mass = treeReader->ptgg / treeReader->mass;
  dijetMVA = fDijetMvaReader->EvaluateMVA("BDTG");
} /// evaluateDijetMvaWeight


//------------------------------------------------------------------------------
void
DiphotonAndDijetMvaReaderTest::evaluateDiphoMvaWeight(void)
{
  if (fDiphoUseSmearedMassError) {
    rVtxSigmaMoM = treeReader->masserrsmeared;
    wVtxSigmaMoM = treeReader->masserrsmearedwrongvtx;
  } else {
    rVtxSigmaMoM = treeReader->masserr;
    wVtxSigmaMoM = treeReader->masserrwrongvtx;
  }
  cosDPhi      = TMath::Cos(treeReader->ph1.phi - treeReader->ph2.phi);
  pho1_ptOverM = treeReader->ph1.pt / treeReader->mass;
  pho2_ptOverM = treeReader->ph2.pt / treeReader->mass;
  diphoMVA     = fDiphoMvaReader->EvaluateMVA("BDTG");  
} /// evaluateDiphoMvaWeight
