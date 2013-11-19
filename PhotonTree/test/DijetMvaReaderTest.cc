#include <iostream> // std::cout
#include <utility>  // std::pair
#include <map>      // std::map
#include <cppunit/extensions/HelperMacros.h>
#include "MitHgg/PhotonTree/interface/DijetMvaReader.h"
#include "MitHgg/PhotonTree/interface/TestTreeFactory.h"

using ::mithep::hgg::DijetMvaReader;
using namespace std;
//------------------------------------------------------------------------------
/**
 * DijetMvaReader test fixture
 */
class DijetMvaReaderTest : public ::CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(DijetMvaReaderTest);
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
  DijetMvaReader *mva;
  EventMvaMap     mvaForRunEvent;
}; /// DijetMvaReaderTest


//------------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(DijetMvaReaderTest);

//------------------------------------------------------------------------------
void
DijetMvaReaderTest::setUp(void)
{
  TTree *tree = ::mithep::hgg::TestTreeFactory::Create();
  mva      = new DijetMvaReader(tree);
  /// Dummy values
  mvaForRunEvent[RunEvent(200519, 68683)] = 0.;
  mvaForRunEvent[RunEvent(200519, 68687)] = 0.;
  mvaForRunEvent[RunEvent(200519, 68689)] = 0.;
} /// setUp


//------------------------------------------------------------------------------
void
DijetMvaReaderTest::tearDown(void)
{
  delete mva;
} /// tearDown

//------------------------------------------------------------------------------
void
DijetMvaReaderTest::testWeightReading(void)
{
  Float_t epsilon = 1;
  std::cout << "\nDijetMvaReaderTest::testWeightReading ... \n";
  for (int i=0; i < mva->GetEntries(); i++) {
    mva     ->GetEntry(i);
    std::cout << "    run:" << mva->run
              << "\t lumi:" << mva->lumi
              << "\t event:" << mva->evt
              << "\t dijetMVA:" << mva->dijetMVA << "\n";
    RunEvent runEvent(mva->run, mva->evt);
    if (mvaForRunEvent.find(runEvent) == mvaForRunEvent.end()) {
/*      std::cout << "WARNING: DijetMvaReaderTest::testWeightReading: Skipping"
                << " run "    << mva->run
                << ", event " << mva->evt << "...\n";*/
      continue;
    }
    Float_t expected = mvaForRunEvent[runEvent];
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mva->dijetMVA, epsilon);
  } /// Loop over entries.
} /// testWeightReading
