/**
 * \file MitHgg/PhotonTree/test/PhotonReaderTest.cc
 * \class PhotonReaderTest
 * \brief Unit test of the PhotonReader class
 * \author Jan Veverka, MIT, jan.veverka@cern.ch
 * \date 18 November 2013
 */

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include "MitHgg/PhotonTree/interface/PhotonReader.h"
#include "MitHgg/PhotonTree/interface/TestTreeFactory.h"

#define CPPUNIT_ASSERT_INEQUAL(a, b) \
  CPPUNIT_ASSERT_ASSERTION_FAIL(CPPUNIT_ASSERT_EQUAL(a, b))

using ::mithep::hgg::PhotonReader;

//------------------------------------------------------------------------------
/**
 * PhotonTree test fixture
 */
class PhotonReaderTest : public ::CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(PhotonReaderTest);
  CPPUNIT_TEST(testReadingWithRefs);
  CPPUNIT_TEST(testReadingWithGetters);
  CPPUNIT_TEST(testBufferModificationWithRefs);
  CPPUNIT_TEST(testBufferModificationWithGetters);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp(void);
  void tearDown(void);

protected:
  void testReadingWithRefs(void);
  void testReadingWithGetters(void);
  void testBufferModificationWithRefs(void);
  void testBufferModificationWithGetters(void);

private:
  TTree        *tree;
  TTree        *tclone;
  PhotonReader *ph1, *ph2;
  UChar_t      ph1_hasphoton, ph2_hasphoton;
  UInt_t       ph1_index    , ph2_index    ;
  Float_t      ph1_e        , ph2_e        ;
  Float_t      ph1_pt       , ph2_pt       ;
  Float_t      ph1_idmva    , ph2_idmva    ;
  Bool_t       ph1_isbarrel , ph2_isbarrel ;
  Float_t      ph1_ebcs[16] , ph2_ebcs[16] ;
}; // PhotonReaderTest


//------------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(PhotonReaderTest);

//------------------------------------------------------------------------------
void
PhotonReaderTest::setUp(void)
{
  tree = mithep::hgg::TestTreeFactory::Create();
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
  tclone = static_cast<TTree*>(tree->Clone());
  ph1 = new PhotonReader(tclone, "ph1.");
  ph2 = new PhotonReader(tclone, "ph2.");
} // setUp


//------------------------------------------------------------------------------
void
PhotonReaderTest::tearDown(void)
{
  delete ph1;
  delete ph2;
} // tearDown


//------------------------------------------------------------------------------
void
PhotonReaderTest::testReadingWithRefs(void)
{
  for (Int_t i=0; i<tree->GetEntries(); i++) {
    CPPUNIT_ASSERT_EQUAL(tree->GetEntry(i), tclone->GetEntry(i));
    CPPUNIT_ASSERT_EQUAL(ph1_e            , ph1->e             );
    CPPUNIT_ASSERT_EQUAL(ph2_e            , ph2->e             );
    CPPUNIT_ASSERT_EQUAL(ph1_pt           , ph1->pt            );
    CPPUNIT_ASSERT_EQUAL(ph2_pt           , ph2->pt            );
    CPPUNIT_ASSERT_EQUAL(ph1_idmva        , ph1->idmva         );
    CPPUNIT_ASSERT_EQUAL(ph2_idmva        , ph2->idmva         );
    CPPUNIT_ASSERT_EQUAL(ph1_hasphoton    , ph1->hasphoton     );
    CPPUNIT_ASSERT_EQUAL(ph2_hasphoton    , ph2->hasphoton     );
    CPPUNIT_ASSERT_EQUAL(ph1_index        , ph1->index         );
    CPPUNIT_ASSERT_EQUAL(ph2_index        , ph2->index         );
    CPPUNIT_ASSERT_EQUAL(ph1_isbarrel     , ph1->isbarrel      );
    CPPUNIT_ASSERT_EQUAL(ph2_isbarrel     , ph2->isbarrel      );
  };
} // testReadingWithRefs


//------------------------------------------------------------------------------
void
PhotonReaderTest::testReadingWithGetters(void)
{
  for (Int_t i=0; i<tree->GetEntries(); i++) {
    CPPUNIT_ASSERT_EQUAL(tree->GetEntry(i), tclone->GetEntry(i)   );
    CPPUNIT_ASSERT_EQUAL(ph1_e            , ph1->GetF("e")        );
    CPPUNIT_ASSERT_EQUAL(ph2_e            , ph2->GetF("e")        );
    CPPUNIT_ASSERT_EQUAL(ph1_pt           , ph1->GetF("pt")       );
    CPPUNIT_ASSERT_EQUAL(ph2_pt           , ph2->GetF("pt")       );
    CPPUNIT_ASSERT_EQUAL(ph1_idmva        , ph1->GetF("idmva")    );
    CPPUNIT_ASSERT_EQUAL(ph2_idmva        , ph2->GetF("idmva")    );
    CPPUNIT_ASSERT_EQUAL(ph1_hasphoton    , ph1->Getb("hasphoton"));
    CPPUNIT_ASSERT_EQUAL(ph2_hasphoton    , ph2->Getb("hasphoton"));
    CPPUNIT_ASSERT_EQUAL(ph1_index        , ph1->Geti("index")    );
    CPPUNIT_ASSERT_EQUAL(ph2_index        , ph2->Geti("index")    );
    CPPUNIT_ASSERT_EQUAL(ph1_isbarrel     , ph1->GetO("isbarrel") );
    CPPUNIT_ASSERT_EQUAL(ph2_isbarrel     , ph2->GetO("isbarrel") );
  };
} // testReadingWithGetters


//------------------------------------------------------------------------------
void
PhotonReaderTest::testBufferModificationWithRefs(void)
{
  for (Int_t i=0; i < tree->GetEntries() - 1; i++) {
  
    /// Load different entries for expected and actual trees.
    tree->GetEntry(i);
    tclone->GetEntry(i + 1);

    /// Make sure that we have a significant number of different values
    /// for expected and actual variables.
    CPPUNIT_ASSERT_INEQUAL(ph1_e    , ph1->e    );
    CPPUNIT_ASSERT_INEQUAL(ph2_e    , ph2->e    );
    CPPUNIT_ASSERT_INEQUAL(ph1_pt   , ph1->pt   );
    CPPUNIT_ASSERT_INEQUAL(ph2_pt   , ph2->pt   );
    CPPUNIT_ASSERT_INEQUAL(ph1_idmva, ph1->idmva);
    CPPUNIT_ASSERT_INEQUAL(ph2_idmva, ph2->idmva);
      
    /// Set the actual values in the PhotonTree buffers to the expected ones.
    ph1->e         = ph1_e        ;
    ph2->e         = ph2_e        ;
    ph1->pt        = ph1_pt       ;
    ph2->pt        = ph2_pt       ;
    ph1->idmva     = ph1_idmva    ;
    ph2->idmva     = ph2_idmva    ;
    ph1->hasphoton = ph1_hasphoton;
    ph2->hasphoton = ph2_hasphoton;
    ph1->index     = ph1_index    ;
    ph2->index     = ph2_index    ;
    ph1->isbarrel  = ph1_isbarrel ;
    ph2->isbarrel  = ph2_isbarrel ;

    /// Test that the actual and expected values agree.
    CPPUNIT_ASSERT_EQUAL(ph1_e        , ph1->e        );
    CPPUNIT_ASSERT_EQUAL(ph2_e        , ph2->e        );
    CPPUNIT_ASSERT_EQUAL(ph1_pt       , ph1->pt       );
    CPPUNIT_ASSERT_EQUAL(ph2_pt       , ph2->pt       );
    CPPUNIT_ASSERT_EQUAL(ph1_idmva    , ph1->idmva    );
    CPPUNIT_ASSERT_EQUAL(ph2_idmva    , ph2->idmva    );
    CPPUNIT_ASSERT_EQUAL(ph1_hasphoton, ph1->hasphoton);
    CPPUNIT_ASSERT_EQUAL(ph2_hasphoton, ph2->hasphoton);
    CPPUNIT_ASSERT_EQUAL(ph1_index    , ph1->index    );
    CPPUNIT_ASSERT_EQUAL(ph2_index    , ph2->index    );
    CPPUNIT_ASSERT_EQUAL(ph1_isbarrel , ph1->isbarrel );
    CPPUNIT_ASSERT_EQUAL(ph2_isbarrel , ph2->isbarrel );
  } /// Loop over entries.
} /// testBufferModificationWithRefs


//------------------------------------------------------------------------------
void
PhotonReaderTest::testBufferModificationWithGetters(void)
{
  for (Int_t i=0; i < tree->GetEntries() - 1; i++) {  
    /// Load different entries for expected and actual trees.
    tree->GetEntry(i);
    tclone->GetEntry(i + 1);

    /// Make sure that we have a significant number of different values
    /// for expected and actual variables.
    CPPUNIT_ASSERT_INEQUAL(ph1_e    , ph1->GetF("e")    );
    CPPUNIT_ASSERT_INEQUAL(ph2_e    , ph2->GetF("e")    );
    CPPUNIT_ASSERT_INEQUAL(ph1_pt   , ph1->GetF("pt")   );
    CPPUNIT_ASSERT_INEQUAL(ph2_pt   , ph2->GetF("pt")   );
    CPPUNIT_ASSERT_INEQUAL(ph1_idmva, ph1->GetF("idmva"));
    CPPUNIT_ASSERT_INEQUAL(ph2_idmva, ph2->GetF("idmva"));
      
    /// Set the actual values in the PhotonTree buffers to the expected ones.
    ph1->GetF("e")         = ph1_e        ;
    ph2->GetF("e")         = ph2_e        ;
    ph1->GetF("pt")        = ph1_pt       ;
    ph2->GetF("pt")        = ph2_pt       ;
    ph1->GetF("idmva")     = ph1_idmva    ;
    ph2->GetF("idmva")     = ph2_idmva    ;
    ph1->Getb("hasphoton") = ph1_hasphoton;
    ph2->Getb("hasphoton") = ph2_hasphoton;
    ph1->Geti("index")     = ph1_index    ;
    ph2->Geti("index")     = ph2_index    ;
    ph1->GetO("isbarrel")  = ph1_isbarrel ;
    ph2->GetO("isbarrel")  = ph2_isbarrel ;

    /// Test that the actual and expected values agree.
    CPPUNIT_ASSERT_EQUAL(ph1_e        , ph1->GetF("e")        );
    CPPUNIT_ASSERT_EQUAL(ph2_e        , ph2->GetF("e")        );
    CPPUNIT_ASSERT_EQUAL(ph1_pt       , ph1->GetF("pt")       );
    CPPUNIT_ASSERT_EQUAL(ph2_pt       , ph2->GetF("pt")       );
    CPPUNIT_ASSERT_EQUAL(ph1_idmva    , ph1->GetF("idmva")    );
    CPPUNIT_ASSERT_EQUAL(ph2_idmva    , ph2->GetF("idmva")    );
    CPPUNIT_ASSERT_EQUAL(ph1_hasphoton, ph1->Getb("hasphoton"));
    CPPUNIT_ASSERT_EQUAL(ph2_hasphoton, ph2->Getb("hasphoton"));
    CPPUNIT_ASSERT_EQUAL(ph1_index    , ph1->Geti("index")    );
    CPPUNIT_ASSERT_EQUAL(ph2_index    , ph2->Geti("index")    );
    CPPUNIT_ASSERT_EQUAL(ph1_isbarrel , ph1->GetO("isbarrel") );
    CPPUNIT_ASSERT_EQUAL(ph2_isbarrel , ph2->GetO("isbarrel") );
  } /// Loop over entries.
} /// testBufferModificationWithGetters
