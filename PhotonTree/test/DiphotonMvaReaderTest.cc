#include <iostream> // std::cout
#include <utility>  // std::pair
#include <map>      // std::map
#include <cppunit/extensions/HelperMacros.h>
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
  TreeReader        *diphoton;
  DiphotonMvaReader *mva;
  EventMvaMap       mvaForRunEvent;
}; /// DiphotonMvaReaderTest


//------------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(DiphotonMvaReaderTest);

//------------------------------------------------------------------------------
void
DiphotonMvaReaderTest::setUp(void)
{
  TTree *tree1 = ::mithep::hgg::TestTreeFactory::Create();
  TTree *tree2 = static_cast<TTree*>(tree1->Clone());
  diphoton = new TreeReader       (tree1);
  mva      = new DiphotonMvaReader(tree2);

  /// For $ head mit_inputvariables_wzh120_oct18.txt | awk '{print $1, $3, $68}'
  mvaForRunEvent[RunEvent(200519, 68683)] =  0.0975732;
  mvaForRunEvent[RunEvent(200519, 68687)] = -0.416784 ;
  mvaForRunEvent[RunEvent(200519, 68689)] =  0.437828 ;
  mvaForRunEvent[RunEvent(200519, 68690)] =  0.677142 ;
  mvaForRunEvent[RunEvent(200519, 68693)] =  0.0798072;
  mvaForRunEvent[RunEvent(200519, 68694)] = -0.136305 ;
  mvaForRunEvent[RunEvent(200519, 68695)] =  0.148157 ;
  mvaForRunEvent[RunEvent(200519, 68697)] = -0.0837085;
  mvaForRunEvent[RunEvent(200519, 68698)] =  0.373162 ;
} /// setUp


//------------------------------------------------------------------------------
void
DiphotonMvaReaderTest::tearDown(void)
{
  delete diphoton;
  delete mva;
} /// tearDown

//------------------------------------------------------------------------------
void
DiphotonMvaReaderTest::testWeightReading(void)
{
  bool verbose = false;
  Float_t epsilon = 1e-5;
  for (int i=0; i < diphoton->GetEntries(); i++) {
    diphoton->GetEntry(i);
    mva     ->GetEntry(i);
    RunEvent runEvent(mva->run, mva->evt);
    if (mvaForRunEvent.find(runEvent) == mvaForRunEvent.end()) {
      if (verbose) {
        std::cout << "\nWARNING: DiphotonMvaReaderTest::testWeightReading: "
                  << "Skipping run "    << mva->run
                  << ", event " << mva->evt << "...\n";
      }
      continue;
    }
    Float_t expected = mvaForRunEvent[runEvent];
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mva->diphoMVA, epsilon);
  } /// Loop over entries.
} /// testWeightReading

