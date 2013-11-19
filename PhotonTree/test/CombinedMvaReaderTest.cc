#include <iostream> // std::cout
#include <utility>  // std::pair
#include <map>      // std::map
#include <cppunit/extensions/HelperMacros.h>
#include "MitHgg/PhotonTree/interface/CombinedMvaReader.h"
#include "MitHgg/PhotonTree/interface/TestTreeFactory.h"

using ::mithep::hgg::CombinedMvaReader;
using namespace std;
//------------------------------------------------------------------------------
/**
 * CombinedMvaReader test fixture
 */
class CombinedMvaReaderTest : public ::CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(CombinedMvaReaderTest);
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
  CombinedMvaReader *mva;
  EventMvaMap     mvaForRunEvent;
}; /// CombinedMvaReaderTest


//------------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(CombinedMvaReaderTest);

//------------------------------------------------------------------------------
void
CombinedMvaReaderTest::setUp(void)
{
  TTree *tree = ::mithep::hgg::TestTreeFactory::Create();
  mva      = new CombinedMvaReader(tree);
  /// Globe values from Francesco Micheli emailed on 19 Nov.
  mvaForRunEvent[RunEvent(200519, 68683)] = -0.994136;
  mvaForRunEvent[RunEvent(200519, 68687)] = -0.995291;
  mvaForRunEvent[RunEvent(200519, 68689)] = -0.997381;
  mvaForRunEvent[RunEvent(200519, 68690)] = -0.995763;
} /// setUp

                                            
//------------------------------------------------------------------------------
void
CombinedMvaReaderTest::tearDown(void)
{
  delete mva;
} /// tearDown

//------------------------------------------------------------------------------
void
CombinedMvaReaderTest::testWeightReading(void)
{
  Float_t delta = 0.5;
  std::cout << "\nCombinedMvaReaderTest::testWeightReading ... \n";
  for (int i=0; i < mva->GetEntries() && i < (int) mvaForRunEvent.size(); i++) {
    mva     ->GetEntry(i);
    RunEvent runEvent(mva->run, mva->evt);
    if (mvaForRunEvent.find(runEvent) == mvaForRunEvent.end()) {
      std::cout << "WARNING: CombinedMvaReaderTest::testWeightReading: Skipping"
                << " run "    << mva->run
                << ", event " << mva->evt << "...\n";
      continue;
    }
    Float_t expected = mvaForRunEvent[runEvent];
    std::cout << "   run:" << mva->run
              << "   lumi:" << mva->lumi
              << "   event:" << mva->evt
              << "   combinedMVA(Globe):" << expected
              << "   combinedMVA(MIT):" << mva->combinedMVA << "\n";
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mva->combinedMVA, delta);
  } /// Loop over entries.
} /// testWeightReading
