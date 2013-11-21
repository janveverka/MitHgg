#include <iostream> // std::cout
#include <utility>  // std::pair
#include <map>      // std::map
#include <cppunit/extensions/HelperMacros.h>
#include "TMVA/Reader.h"
#include "MitHgg/PhotonTree/interface/TreeReader.h"
#include "MitHgg/PhotonTree/interface/DiphotonMvaReader.h"
#include "MitHgg/PhotonTree/interface/TestTreeFactory.h"

using ::mithep::hgg::DiphotonMvaReader;
using ::mithep::hgg::TreeReader;
using namespace std;
//------------------------------------------------------------------------------
/**
 * DiphotonMvaReader test fixture
 */
class DiphotonMvaReaderTest : public ::CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(DiphotonMvaReaderTest);
  CPPUNIT_TEST(testWeightReading);
  CPPUNIT_TEST_SUITE_END();

public:
  typedef std::pair<UInt_t, UInt_t> RunEvent;
  typedef std::map<RunEvent, Float_t> EventMvaMap;
  void setUp(void);
  void tearDown(void);

protected:
  void testWeightReading(void);

private:
  TreeReader        *tree          ;
  DiphotonMvaReader *actualMVA     ;
  EventMvaMap        mvaForRunEvent;
  Float_t            rVtxSigmaMoM  ;
  Float_t            wVtxSigmaMoM  ;
  Float_t            cosDPhi       ;
  Float_t            pho1_ptOverM  ;
  Float_t            pho2_ptOverM  ;
  Float_t            diphoMVA      ;
  TMVA::Reader *     expectedMVA   ;  
  bool          useSmearedMassError;
}; /// DiphotonMvaReaderTest


//------------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(DiphotonMvaReaderTest);

//------------------------------------------------------------------------------
void
DiphotonMvaReaderTest::setUp(void)
{
  TTree *tree1 = ::mithep::hgg::TestTreeFactory::Create();
  TTree *tree2 = static_cast<TTree*>(tree1->Clone());
  const char *weights = HGG_DEFAULT_DIPHOTON_WEIGHTS_PATH;
  bool useSmearedMassError = false; /// default is true
  tree      = new TreeReader       (tree1                              );
  actualMVA = new DiphotonMvaReader(tree2, weights, useSmearedMassError);

  expectedMVA = new TMVA::Reader("Silent");
  
  expectedMVA->AddVariable("masserrsmeared/mass"        , &rVtxSigmaMoM   );
  expectedMVA->AddVariable("masserrsmearedwrongvtx/mass", &wVtxSigmaMoM   );
  expectedMVA->AddVariable("vtxprob"                    , &tree->vtxprob  );
  expectedMVA->AddVariable("ph1.pt/mass"                , &pho1_ptOverM   );
  expectedMVA->AddVariable("ph2.pt/mass"                , &pho2_ptOverM   );
  expectedMVA->AddVariable("ph1.eta"                    , &tree->ph1.eta  );
  expectedMVA->AddVariable("ph2.eta"                    , &tree->ph2.eta  );
  expectedMVA->AddVariable("TMath::Cos(ph1.phi-ph2.phi)", &cosDPhi        );
  expectedMVA->AddVariable("ph1.idmva"                  , &tree->ph1.idmva);
  expectedMVA->AddVariable("ph2.idmva"                  , &tree->ph2.idmva);

  expectedMVA->BookMVA("BDTG", weights);
  
} /// setUp


//------------------------------------------------------------------------------
void
DiphotonMvaReaderTest::tearDown(void)
{
  delete tree;
  delete expectedMVA;
  delete actualMVA;
} /// tearDown

//------------------------------------------------------------------------------
void
DiphotonMvaReaderTest::testWeightReading(void)
{
  Float_t epsilon = 1e-6;
  for (int i=0; i < tree->GetEntries(); i++) {
    tree     ->GetEntry(i);
    actualMVA->GetEntry(i);
    
    if (useSmearedMassError) {
      rVtxSigmaMoM = tree->masserrsmeared / tree->mass;
      wVtxSigmaMoM = tree->masserrsmearedwrongvtx / tree->mass;
    } else {
      rVtxSigmaMoM = tree->masserr / tree->mass;
      wVtxSigmaMoM = tree->masserrwrongvtx / tree->mass;
    }
    cosDPhi      = TMath::Cos(tree->ph1.phi - tree->ph2.phi);
    pho1_ptOverM = tree->ph1.pt / tree->mass;
    pho2_ptOverM = tree->ph2.pt / tree->mass;
    
    Float_t expected = expectedMVA->EvaluateMVA("BDTG");
    Float_t actual   = actualMVA  ->diphoMVA;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, epsilon);
  } /// Loop over entries.
} /// testWeightReading

