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
  mvaForRunEvent[RunEvent(200519, 68683)] = -0.997401;
  mvaForRunEvent[RunEvent(200519, 68687)] = -0.977856;
  mvaForRunEvent[RunEvent(200519, 68689)] = -0.704309;
  mvaForRunEvent[RunEvent(200519, 68690)] = -0.963622;
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
  bool verbose = false;
  Float_t delta = 0.08;
  if (verbose) {
    std::cout << "\nDijetMvaReaderTest::testWeightReading ... \n";
  } /// verbose
  for (int i=0; i < mva->GetEntries() && i < (int) mvaForRunEvent.size(); i++) {
    mva     ->GetEntry(i);
    RunEvent runEvent(mva->run, mva->evt);
    if (mvaForRunEvent.find(runEvent) == mvaForRunEvent.end()) {
      if (verbose) {
        std::cout << "\nWARNING: DijetMvaReaderTest::testWeightReading: "
                  << "Skipping run "    << mva->run
                  << ", event " << mva->evt << "...\n";
      }
      continue;
    }
    Float_t expected = mvaForRunEvent[runEvent];
    if (verbose) {
      std::cout << "   run:"             << mva->run
                << "   lumi:"            << mva->lumi
                << "   event:"           << mva->evt
                << "   dijetMVA(Globe):" << expected
                << "   dijetMVA(MIT):"   << mva->dijetMVA << "\n";
    } /// verbose
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mva->dijetMVA, delta);
  } /// Loop over entries.
} /// testWeightReading
