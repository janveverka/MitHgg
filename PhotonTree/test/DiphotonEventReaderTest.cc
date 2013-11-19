/**
 * \file MitHgg/PhotonTree/test/DiphotonEventReaderTest.cc
 * \class DiphotonEventReaderTest
 * \brief Unit test of the DiphotonEventReader class
 * \author Jan Veverka, MIT, jan.veverka@cern.ch
 * \date 18 November 2013
 */

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include "MitHgg/PhotonTree/interface/DiphotonEventReader.h"
#include "MitHgg/PhotonTree/interface/TestTreeFactory.h"

#define CPPUNIT_ASSERT_INEQUAL(a, b) \
  CPPUNIT_ASSERT_ASSERTION_FAIL(CPPUNIT_ASSERT_EQUAL(a, b))

using ::mithep::hgg::DiphotonEventReader;

//------------------------------------------------------------------------------
/**
 * PhotonTree test fixture
 */
class DiphotonEventReaderTest : public ::CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(DiphotonEventReaderTest);
  CPPUNIT_TEST(testReading);
  CPPUNIT_TEST(testBufferModification);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp(void);
  void tearDown(void);

protected:
  void testReading(void);
  void testBufferModification(void);

private:
  TTree               *tree;
  TTree               *tclone;
  DiphotonEventReader *event;
  UChar_t              evtcat;
  UInt_t               evt;
  Float_t              mass;
  Double_t             cosdphiMetgg;
}; /// DiphotonEventReaderTest


//------------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(DiphotonEventReaderTest);

//------------------------------------------------------------------------------
void
DiphotonEventReaderTest::setUp(void)
{
  tree = mithep::hgg::TestTreeFactory::Create();
  tree->SetBranchAddress("evtcat"       , &evtcat       );
  tree->SetBranchAddress("evt"          , &evt          );
  tree->SetBranchAddress("mass"         , &mass         );
  tree->SetBranchAddress("cosdphiMetgg" , &cosdphiMetgg );
  tclone = static_cast<TTree*>(tree->Clone());
  event = new DiphotonEventReader(tclone);
} /// setUp


//------------------------------------------------------------------------------
void
DiphotonEventReaderTest::tearDown(void)
{
  delete event;
} /// tearDown


//------------------------------------------------------------------------------
void
DiphotonEventReaderTest::testReading(void)
{
  for (Int_t i=0; i<tree->GetEntries(); i++) {
    CPPUNIT_ASSERT_EQUAL(tree->GetEntry(i), tclone->GetEntry(i));
    CPPUNIT_ASSERT_EQUAL(evtcat           , event->evtcat      );
    CPPUNIT_ASSERT_EQUAL(evt              , event->evt         );
    CPPUNIT_ASSERT_EQUAL(mass             , event->mass        );
    CPPUNIT_ASSERT_EQUAL(cosdphiMetgg     , event->cosdphiMetgg);
  };
} /// testReading


//------------------------------------------------------------------------------
void
DiphotonEventReaderTest::testBufferModification(void)
{
  for (Int_t i=0; i < tree->GetEntries() - 1; i++) {
    /// Load different entries for expected and actual trees.
    tree->GetEntry(i);
    tclone->GetEntry(i + 1);

    /// Make sure that we have a significant number of different values
    /// for expected and actual variables.
    CPPUNIT_ASSERT_INEQUAL(evt         , event->evt         );
    CPPUNIT_ASSERT_INEQUAL(mass        , event->mass        );
    CPPUNIT_ASSERT_INEQUAL(cosdphiMetgg, event->cosdphiMetgg);
      
    /// Set the actual values in the PhotonTree buffers to the expected ones.
    event->evtcat             = evtcat       ;
    event->evt                = evt          ;
    event->mass               = mass         ;
    event->cosdphiMetgg       = cosdphiMetgg ;

    /// Test that the actual and expected values agree.
    CPPUNIT_ASSERT_EQUAL(evtcat      , event->evtcat      );
    CPPUNIT_ASSERT_EQUAL(evt         , event->evt         );
    CPPUNIT_ASSERT_EQUAL(mass        , event->mass        );
    CPPUNIT_ASSERT_EQUAL(cosdphiMetgg, event->cosdphiMetgg);
  } /// Loop over entries.
} /// testBufferModification
