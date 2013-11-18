#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include "MitHgg/TreeWriter/interface/DiphotonMvaReader.h"
#include "MitHgg/TreeWriter/interface/PhotonTree.h"
#include "DummyTreeFactory.h"

using ::mithep::hgg::DiphotonMvaReader;
using ::mithep::hgg::PhotonTree;
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
  void setUp(void);
  void tearDown(void);

protected:
  void testWeightReading(void);

private:
  PhotonTree        *gg;
  DiphotonMvaReader *reader;
}; // DiphotonMvaReaderTest


//------------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(DiphotonMvaReaderTest);

//------------------------------------------------------------------------------
void
DiphotonMvaReaderTest::setUp(void)
{
//  TTree *tree = ::mithep::hgg::DummyTreeFactory::Create("/home/veverka/cms/cmssw/031/CMSSW_5_3_10_patch1/src/MitHgg/TreeWriter/data/hgg-2013Final8TeV_s12-h120gg-vh-v7n_head10.root", "hPhotonTree");
  TTree *tree = ::mithep::hgg::DummyTreeFactory::Create();
  gg     = new PhotonTree(tree);
  reader = new DiphotonMvaReader(*gg);
} // setUp


//------------------------------------------------------------------------------
void
DiphotonMvaReaderTest::tearDown(void)
{
  delete reader;
  delete gg;
} // tearDown

//------------------------------------------------------------------------------
void
DiphotonMvaReaderTest::testWeightReading(void)
{
  gg->GetEntry(0);
  reader->Update();  
  CPPUNIT_ASSERT_DOUBLES_EQUAL((Float_t)0.0975732, reader->diphoMVA, 1e-5);
} // testWeightReading

