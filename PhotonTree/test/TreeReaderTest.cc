/**
 * \file MitHgg/PhotonTree/test/TreeReaderTest.cc
 * \class TreeReaderTest
 * \brief Unit test of the PhotonTreeReader class
 * \author Jan Veverka, MIT, jan.veverka@cern.ch
 * \date 19 November 2013
 */

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include "MitHgg/PhotonTree/interface/TreeReader.h"
#include "MitHgg/PhotonTree/interface/TestTreeFactory.h"

#define CPPUNIT_ASSERT_INEQUAL(a, b) \
  CPPUNIT_ASSERT_ASSERTION_FAIL(CPPUNIT_ASSERT_EQUAL(a, b))

using ::mithep::hgg::TreeReader;

//------------------------------------------------------------------------------
/**
 * PhotonTree test fixture
 */
class TreeReaderTest : public ::CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TreeReaderTest);
  CPPUNIT_TEST(testReading);
  CPPUNIT_TEST(testBufferModification);
  CPPUNIT_TEST(testBufferModificationWithRefs);
  CPPUNIT_TEST(testConfigCtor);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp(void);
  void tearDown(void);

protected:
  void testReading(void);
  void testBufferModification(void);
  void testBufferModificationWithRefs(void);
  void testConfigCtor(void);

private:
  TTree      *tree;
  TreeReader *reader;
  UChar_t    evtcat;
  UInt_t     evt;
  Float_t    mass;
  Double_t   cosdphiMetgg;
  UChar_t    ph1_hasphoton, ph2_hasphoton;
  UInt_t     ph1_index    , ph2_index    ;
  Float_t    ph1_e        , ph2_e        ;
  Float_t    ph1_pt       , ph2_pt       ;
  Float_t    ph1_idmva    , ph2_idmva    ;
  Bool_t     ph1_isbarrel , ph2_isbarrel ;
}; // TreeReaderTest

//------------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(TreeReaderTest);

//------------------------------------------------------------------------------
void
TreeReaderTest::setUp(void)
{
  tree = mithep::hgg::TestTreeFactory::Create();
  TTree *tclone  = static_cast<TTree*>(tree->Clone());
  tree->SetBranchAddress("evtcat"       , &evtcat       );
  tree->SetBranchAddress("evt"          , &evt          );
  tree->SetBranchAddress("mass"         , &mass         );
  tree->SetBranchAddress("cosdphiMetgg" , &cosdphiMetgg );
  tree->SetBranchAddress("ph1.hasphoton", &ph1_hasphoton);
  tree->SetBranchAddress("ph2.hasphoton", &ph2_hasphoton);
  tree->SetBranchAddress("ph1.index"    , &ph1_index    );
  tree->SetBranchAddress("ph2.index"    , &ph2_index    );
  tree->SetBranchAddress("ph1.e"        , &ph1_e        );
  tree->SetBranchAddress("ph2.e"        , &ph2_e        );
  tree->SetBranchAddress("ph1.pt"       , &ph1_pt       );
  tree->SetBranchAddress("ph2.pt"       , &ph2_pt       );
  tree->SetBranchAddress("ph1.idmva"    , &ph1_idmva    );
  tree->SetBranchAddress("ph2.idmva"    , &ph2_idmva    );
  tree->SetBranchAddress("ph1.isbarrel" , &ph1_isbarrel );
  tree->SetBranchAddress("ph2.isbarrel" , &ph2_isbarrel );
  reader  = new TreeReader(tclone);
} // TreeReaderTest::setUp(void)


//------------------------------------------------------------------------------
void
TreeReaderTest::tearDown(void)
{
  delete reader;
} // TreeReaderTest::tearDown(void)


//------------------------------------------------------------------------------
void
TreeReaderTest::testReading(void)
{
  for (Int_t i=0; i<tree->GetEntries(); i++) {
    CPPUNIT_ASSERT_EQUAL(tree->GetEntry(i), reader->GetEntry(i)          );
    CPPUNIT_ASSERT_EQUAL(evtcat           , reader->evtcat               );
    CPPUNIT_ASSERT_EQUAL(evt              , reader->evt                  );
    CPPUNIT_ASSERT_EQUAL(mass             , reader->mass                 );
    CPPUNIT_ASSERT_EQUAL(cosdphiMetgg     , reader->cosdphiMetgg         );
    CPPUNIT_ASSERT_EQUAL(ph1_e            , reader->ph1.GetF("e")        );
    CPPUNIT_ASSERT_EQUAL(ph2_e            , reader->ph2.GetF("e")        );
    CPPUNIT_ASSERT_EQUAL(ph1_pt           , reader->ph1.GetF("pt")       );
    CPPUNIT_ASSERT_EQUAL(ph2_pt           , reader->ph2.GetF("pt")       );
    CPPUNIT_ASSERT_EQUAL(ph1_idmva        , reader->ph1.GetF("idmva")    );
    CPPUNIT_ASSERT_EQUAL(ph2_idmva        , reader->ph2.GetF("idmva")    );
    CPPUNIT_ASSERT_EQUAL(ph1_hasphoton    , reader->ph1.Getb("hasphoton"));
    CPPUNIT_ASSERT_EQUAL(ph2_hasphoton    , reader->ph2.Getb("hasphoton"));
    CPPUNIT_ASSERT_EQUAL(ph1_index        , reader->ph1.Geti("index")    );
    CPPUNIT_ASSERT_EQUAL(ph2_index        , reader->ph2.Geti("index")    );
    CPPUNIT_ASSERT_EQUAL(ph1_isbarrel     , reader->ph1.GetO("isbarrel") );
    CPPUNIT_ASSERT_EQUAL(ph2_isbarrel     , reader->ph2.GetO("isbarrel") );
    CPPUNIT_ASSERT_EQUAL(ph1_e            , reader->ph1.e                );
    CPPUNIT_ASSERT_EQUAL(ph2_e            , reader->ph2.e                );
    CPPUNIT_ASSERT_EQUAL(ph1_pt           , reader->ph1.pt               );
    CPPUNIT_ASSERT_EQUAL(ph2_pt           , reader->ph2.pt               );
    CPPUNIT_ASSERT_EQUAL(ph1_idmva        , reader->ph1.idmva            );
    CPPUNIT_ASSERT_EQUAL(ph2_idmva        , reader->ph2.idmva            );
    CPPUNIT_ASSERT_EQUAL(ph1_hasphoton    , reader->ph1.hasphoton        );
    CPPUNIT_ASSERT_EQUAL(ph2_hasphoton    , reader->ph2.hasphoton        );
    CPPUNIT_ASSERT_EQUAL(ph1_index        , reader->ph1.index            );
    CPPUNIT_ASSERT_EQUAL(ph2_index        , reader->ph2.index            );
    CPPUNIT_ASSERT_EQUAL(ph1_isbarrel     , reader->ph1.isbarrel         );
    CPPUNIT_ASSERT_EQUAL(ph2_isbarrel     , reader->ph2.isbarrel         );
  };
} /// testReading


//------------------------------------------------------------------------------
void
TreeReaderTest::testBufferModification(void)
{
  for (Int_t i=0; i < tree->GetEntries() - 1; i++) {  
    /// Load different entries for expected and actual trees.
    tree->GetEntry(i);
    reader->GetEntry(i + 1);

    /// Make sure that we have a significant number of different values
    /// for expected and actual variables.
    CPPUNIT_ASSERT_INEQUAL(evt         , reader->evt              );
    CPPUNIT_ASSERT_INEQUAL(mass        , reader->mass             );
    CPPUNIT_ASSERT_INEQUAL(cosdphiMetgg, reader->cosdphiMetgg     );
    CPPUNIT_ASSERT_INEQUAL(ph1_e       , reader->ph1.GetF("e")    );
    CPPUNIT_ASSERT_INEQUAL(ph2_e       , reader->ph2.GetF("e")    );
    CPPUNIT_ASSERT_INEQUAL(ph1_pt      , reader->ph1.GetF("pt")   );
    CPPUNIT_ASSERT_INEQUAL(ph2_pt      , reader->ph2.GetF("pt")   );
    CPPUNIT_ASSERT_INEQUAL(ph1_idmva   , reader->ph1.GetF("idmva"));
    CPPUNIT_ASSERT_INEQUAL(ph2_idmva   , reader->ph2.GetF("idmva"));
      
    /// Skipping variables that will be the same for many entries:
    /// ph{1,2}_hasphoton, ph{1,2}_index, ph{1,2}_isbarrel

    /// Set the actual values in the PhotonTree buffers to the expected ones.
    reader->evtcat                = evtcat       ;
    reader->evt                   = evt          ;
    reader->mass                  = mass         ;
    reader->cosdphiMetgg          = cosdphiMetgg ;
    reader->ph1.GetF("e")         = ph1_e        ;
    reader->ph2.GetF("e")         = ph2_e        ;
    reader->ph1.GetF("pt")        = ph1_pt       ;
    reader->ph2.GetF("pt")        = ph2_pt       ;
    reader->ph1.GetF("idmva")     = ph1_idmva    ;
    reader->ph2.GetF("idmva")     = ph2_idmva    ;
    reader->ph1.Getb("hasphoton") = ph1_hasphoton;
    reader->ph2.Getb("hasphoton") = ph2_hasphoton;
    reader->ph1.Geti("index")     = ph1_index    ;
    reader->ph2.Geti("index")     = ph2_index    ;
    reader->ph1.GetO("isbarrel")  = ph1_isbarrel ;
    reader->ph2.GetO("isbarrel")  = ph2_isbarrel ;

    /// Test that the actual and expected values agree.
    CPPUNIT_ASSERT_EQUAL(evtcat       , reader->evtcat               );
    CPPUNIT_ASSERT_EQUAL(evt          , reader->evt                  );
    CPPUNIT_ASSERT_EQUAL(mass         , reader->mass                 );
    CPPUNIT_ASSERT_EQUAL(cosdphiMetgg , reader->cosdphiMetgg         );
    CPPUNIT_ASSERT_EQUAL(ph1_e        , reader->ph1.GetF("e")        );
    CPPUNIT_ASSERT_EQUAL(ph2_e        , reader->ph2.GetF("e")        );
    CPPUNIT_ASSERT_EQUAL(ph1_pt       , reader->ph1.GetF("pt")       );
    CPPUNIT_ASSERT_EQUAL(ph2_pt       , reader->ph2.GetF("pt")       );
    CPPUNIT_ASSERT_EQUAL(ph1_idmva    , reader->ph1.GetF("idmva")    );
    CPPUNIT_ASSERT_EQUAL(ph2_idmva    , reader->ph2.GetF("idmva")    );
    CPPUNIT_ASSERT_EQUAL(ph1_hasphoton, reader->ph1.Getb("hasphoton"));
    CPPUNIT_ASSERT_EQUAL(ph2_hasphoton, reader->ph2.Getb("hasphoton"));
    CPPUNIT_ASSERT_EQUAL(ph1_index    , reader->ph1.Geti("index")    );
    CPPUNIT_ASSERT_EQUAL(ph2_index    , reader->ph2.Geti("index")    );
    CPPUNIT_ASSERT_EQUAL(ph1_isbarrel , reader->ph1.GetO("isbarrel") );
    CPPUNIT_ASSERT_EQUAL(ph2_isbarrel , reader->ph2.GetO("isbarrel") );
  } /// Loop over entries.
} /// testBufferModification


//------------------------------------------------------------------------------
void
TreeReaderTest::testBufferModificationWithRefs(void)
{
  for (Int_t i=0; i < tree->GetEntries() - 1; i++) {  
    /// Load different entries for expected and actual trees.
    tree->GetEntry(i);
    reader->GetEntry(i + 1);

    /// Make sure that we have a significant number of different values
    /// for expected and actual variables.
    CPPUNIT_ASSERT_INEQUAL(ph1_e    , reader->ph1.e    );
    CPPUNIT_ASSERT_INEQUAL(ph2_e    , reader->ph2.e    );
    CPPUNIT_ASSERT_INEQUAL(ph1_pt   , reader->ph1.pt   );
    CPPUNIT_ASSERT_INEQUAL(ph2_pt   , reader->ph2.pt   );
    CPPUNIT_ASSERT_INEQUAL(ph1_idmva, reader->ph1.idmva);
    CPPUNIT_ASSERT_INEQUAL(ph2_idmva, reader->ph2.idmva);
      
    /// Skipping variables that will be the same for many entries:
    /// ph{1,2}_hasphoton, ph{1,2}_index, ph{1,2}_isbarrel

    /// Set the actual values in the PhotonTree buffers to the expected ones.
    reader->ph1.e          = ph1_e        ;
    reader->ph2.e          = ph2_e        ;
    reader->ph1.pt         = ph1_pt       ;
    reader->ph2.pt         = ph2_pt       ;
    reader->ph1.idmva      = ph1_idmva    ;
    reader->ph2.idmva      = ph2_idmva    ;
    reader->ph1.hasphoton  = ph1_hasphoton;
    reader->ph2.hasphoton  = ph2_hasphoton;
    reader->ph1.index      = ph1_index    ;
    reader->ph2.index      = ph2_index    ;
    reader->ph1.isbarrel   = ph1_isbarrel ;
    reader->ph2.isbarrel   = ph2_isbarrel ;

    /// Test that the actual and expected values agree.
    CPPUNIT_ASSERT_EQUAL(ph1_e        , reader->ph1.e        );
    CPPUNIT_ASSERT_EQUAL(ph2_e        , reader->ph2.e        );
    CPPUNIT_ASSERT_EQUAL(ph1_pt       , reader->ph1.pt       );
    CPPUNIT_ASSERT_EQUAL(ph2_pt       , reader->ph2.pt       );
    CPPUNIT_ASSERT_EQUAL(ph1_idmva    , reader->ph1.idmva    );
    CPPUNIT_ASSERT_EQUAL(ph2_idmva    , reader->ph2.idmva    );
    CPPUNIT_ASSERT_EQUAL(ph1_hasphoton, reader->ph1.hasphoton);
    CPPUNIT_ASSERT_EQUAL(ph2_hasphoton, reader->ph2.hasphoton);
    CPPUNIT_ASSERT_EQUAL(ph1_index    , reader->ph1.index    );
    CPPUNIT_ASSERT_EQUAL(ph2_index    , reader->ph2.index    );
    CPPUNIT_ASSERT_EQUAL(ph1_isbarrel , reader->ph1.isbarrel );
    CPPUNIT_ASSERT_EQUAL(ph2_isbarrel , reader->ph2.isbarrel );
  } /// Loop over entries.
} /// testBufferModificationWithRefs


//------------------------------------------------------------------------------
void
TreeReaderTest::testConfigCtor(void)
{
  TTree *tclone = static_cast<TTree*>(tree->Clone());
  TreeReader::Configuration config(tclone);
  TreeReader reader2(config);
  for (Int_t i=0; i < tree->GetEntries(); i++) {
    reader->GetEntry(i);
    reader2.GetEntry(i);
    CPPUNIT_ASSERT_EQUAL(reader->ph1.e        , reader2.ph1.e        );
    CPPUNIT_ASSERT_EQUAL(reader->ph2.e        , reader2.ph2.e        );
    CPPUNIT_ASSERT_EQUAL(reader->ph1.pt       , reader2.ph1.pt       );
    CPPUNIT_ASSERT_EQUAL(reader->ph2.pt       , reader2.ph2.pt       );
    CPPUNIT_ASSERT_EQUAL(reader->ph1.idmva    , reader2.ph1.idmva    );
    CPPUNIT_ASSERT_EQUAL(reader->ph2.idmva    , reader2.ph2.idmva    );
    CPPUNIT_ASSERT_EQUAL(reader->ph1.hasphoton, reader2.ph1.hasphoton);
    CPPUNIT_ASSERT_EQUAL(reader->ph2.hasphoton, reader2.ph2.hasphoton);
    CPPUNIT_ASSERT_EQUAL(reader->ph1.index    , reader2.ph1.index    );
    CPPUNIT_ASSERT_EQUAL(reader->ph2.index    , reader2.ph2.index    );
    CPPUNIT_ASSERT_EQUAL(reader->ph1.isbarrel , reader2.ph1.isbarrel );
    CPPUNIT_ASSERT_EQUAL(reader->ph2.isbarrel , reader2.ph2.isbarrel );
  } /// Loop over entries
} /// testConfigCtor
