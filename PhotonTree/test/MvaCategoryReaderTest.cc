#include <iostream> // std::cout
#include <utility>  // std::pair
#include <map>      // std::map
#include <cppunit/extensions/HelperMacros.h>
#include "MitHgg/PhotonTree/interface/MvaCategoryReader.h"
#include "MitHgg/PhotonTree/interface/TestTreeFactory.h"

using ::mithep::hgg::MvaCategoryReader;
using ::std::cout;

//------------------------------------------------------------------------------
/**
 * MvaCategoryReader test fixture
 */
class MvaCategoryReaderTest : public ::CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(MvaCategoryReaderTest);
  CPPUNIT_TEST(testReading);
  CPPUNIT_TEST_SUITE_END();

public:
  struct Tags {
    Int_t vhLep, vhMet, vhHad, tth, mvaCat;
    Tags(): vhLep(0), vhMet(0), vhHad(0), tth(0), mvaCat(0) {}
    Tags(Int_t vlep, Int_t vmet, Int_t vhad, Int_t tt, Int_t cat) :
      vhLep(vlep), vhMet(vmet), vhHad(vhad), tth(tt), mvaCat(cat)
      {}
  }; // Tags
  typedef std::pair<UInt_t, UInt_t> RunEvent;
  typedef std::map<RunEvent, Tags> EventTagMap;
  void setUp(void);
  void tearDown(void);

protected:
  void testReading(void);

private:
  MvaCategoryReader *reader;
  EventTagMap        tagsForRunEvent;
}; /// MvaCategoryReaderTest


//------------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(MvaCategoryReaderTest);

//------------------------------------------------------------------------------
void
MvaCategoryReaderTest::setUp(void)
{
  TTree *tree = ::mithep::hgg::TestTreeFactory::Create();
  reader      = new MvaCategoryReader(tree);
  /// MIT dump from Nov 14
  /// Tag order here (in the dump): vlep(6), vmet(7), vhad(8), tt(5), cat(4)
  tagsForRunEvent[RunEvent(200519, 68683)] = Tags(0 , 0, 0, 0, 0);
  tagsForRunEvent[RunEvent(200519, 68687)] = Tags(0 , 0, 0, 0, 0);
  tagsForRunEvent[RunEvent(200519, 68689)] = Tags(0 , 0, 0, 0, 0);
  tagsForRunEvent[RunEvent(200519, 68690)] = Tags(0 , 1, 0, 0, 9);
  tagsForRunEvent[RunEvent(200519, 68693)] = Tags(0 , 0, 0, 0, 0);
  tagsForRunEvent[RunEvent(200519, 68694)] = Tags(0 , 0, 0, 0, 0);
  tagsForRunEvent[RunEvent(200519, 68695)] = Tags(0 , 0, 0, 0, 0);
  tagsForRunEvent[RunEvent(200519, 68697)] = Tags(0 , 0, 0, 0, 0);
  tagsForRunEvent[RunEvent(200519, 68698)] = Tags(0 , 1, 0, 0, 9);
  tagsForRunEvent[RunEvent(200519, 68700)] = Tags(1 , 0, 0, 0, 8);
} /// setUp

                                            
//------------------------------------------------------------------------------
void
MvaCategoryReaderTest::tearDown(void)
{
  delete reader;
} /// tearDown

//------------------------------------------------------------------------------
void
MvaCategoryReaderTest::testReading(void)
{
  for (int i=0; i < reader->GetEntries() && i < (int) tagsForRunEvent.size();
       i++) {
    reader->GetEntry(i);
    RunEvent runEvent(reader->run, reader->evt);
    if (tagsForRunEvent.find(runEvent) == tagsForRunEvent.end()) {
      cout << "WARNING: MvaCategoryReaderTest::testReading: Skipping"
           << " run "    << reader->run
           << ", event " << reader->evt << "...\n";
      continue;
    }
    Tags expected = tagsForRunEvent[runEvent];
    CPPUNIT_ASSERT_EQUAL(expected.vhLep, reader->VHLepTag);
    CPPUNIT_ASSERT_EQUAL(expected.vhMet, reader->VHMetTag);
    CPPUNIT_ASSERT_EQUAL(expected.vhHad, reader->VHHadTag);
    CPPUNIT_ASSERT_EQUAL(expected.tth  , reader->tthTag  );
    // CPPUNIT_ASSERT_EQUAL(expected.cat  , reader->mvaCat  );
  } /// Loop over entries.
} /// testReading
