/**
 * \file MitHgg/TreeWriter/test/PhotonTreeReaderDiphotonEventTest.cc
 * \class PhotonTreeReaderDiphotonEventTest
 * \brief Unit test of the PhotonTreeReaderDiphotonEvent class
 * \author Jan Veverka, MIT, jan.veverka@cern.ch
 * \date 18 November 2013
 */

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include "MitHgg/TreeWriter/interface/PhotonTreeReaderDiphotonEvent.h"
#include "MitHgg/TreeWriter/test/DummyTreeFactory.h"

#define CPPUNIT_ASSERT_INEQUAL(a, b) \
  CPPUNIT_ASSERT_ASSERTION_FAIL(CPPUNIT_ASSERT_EQUAL(a, b))

using ::mithep::hgg::PhotonTreeReaderDiphotonEvent;

//------------------------------------------------------------------------------
/**
 * PhotonTree test fixture
 */
class PhotonTreeReaderDiphotonEventTest : public ::CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(PhotonTreeReaderDiphotonEventTest);
  CPPUNIT_TEST(testTreeReading);
  CPPUNIT_TEST(testBufferModification);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp(void);
  void tearDown(void);

protected:
  void testTreeReading(void);
  void testBufferModification(void);

private:
  TTree                         *tree;
  TTree                         *tclone;
  PhotonTreeReaderDiphotonEvent *event;
  UChar_t                        evtcat;
  UInt_t                         evt;
  Float_t                        mass;
  Double_t                       cosdphiMetgg;
}; // PhotonTreeReaderDiphotonEventTest


//------------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(PhotonTreeReaderDiphotonEventTest);

//------------------------------------------------------------------------------
void
PhotonTreeReaderDiphotonEventTest::setUp(void)
{
  tree = mithep::hgg::DummyTreeFactory::Create();
  tree->SetBranchAddress("evtcat"       , &evtcat       );
  tree->SetBranchAddress("evt"          , &evt          );
  tree->SetBranchAddress("mass"         , &mass         );
  tree->SetBranchAddress("cosdphiMetgg" , &cosdphiMetgg );
  tclone = static_cast<TTree*>(tree->Clone());
  event = new PhotonTreeReaderDiphotonEvent(tclone);
} // setUp


//------------------------------------------------------------------------------
void
PhotonTreeReaderDiphotonEventTest::tearDown(void)
{
  delete event;
} // tearDown


//------------------------------------------------------------------------------
void
PhotonTreeReaderDiphotonEventTest::testTreeReading(void)
{
  for (Int_t i=0; i<tree->GetEntries(); i++) {
    CPPUNIT_ASSERT_EQUAL(tree->GetEntry(i), tclone->GetEntry(i));
    CPPUNIT_ASSERT_EQUAL(evtcat           , event->evtcat      );
    CPPUNIT_ASSERT_EQUAL(evt              , event->evt         );
    CPPUNIT_ASSERT_EQUAL(mass             , event->mass        );
    CPPUNIT_ASSERT_EQUAL(cosdphiMetgg     , event->cosdphiMetgg);
  };
} // testTreeReading


//------------------------------------------------------------------------------
void
PhotonTreeReaderDiphotonEventTest::testBufferModification(void)
{
  /// Load different entries for expected and actual trees.
  tree->GetEntry(1);
  tclone->GetEntry(0);

  /// Make sure that we have a significant number of different values
  /// for expected and actual variables.
  CPPUNIT_ASSERT_INEQUAL(evtcat      , event->evtcat      );
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
} // testBufferModification
