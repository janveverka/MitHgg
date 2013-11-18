/**
 * \file MitHgg/TreeWriter/test/PhotonTreeReaderTest.cc
 * \class PhotonTreeReaderTest
 * \brief Unit test of the PhotonTreeReader class
 * \author Jan Veverka, MIT, jan.veverka@cern.ch
 * \date 12 November 2013
 */

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "MitHgg/TreeWriter/interface/PhotonTreeReader.h"

#define CPPUNIT_ASSERT_INEQUAL(a, b) \
  CPPUNIT_ASSERT_ASSERTION_FAIL(CPPUNIT_ASSERT_EQUAL(a, b))

using ::mithep::hgg::PhotonTreeReader;

//------------------------------------------------------------------------------
/**
 * PhotonTree test fixture
 */
class PhotonTreeReaderTest : public ::CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(PhotonTreeReaderTest);
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
  TTree      *tree;
  PhotonTreeReader *reader;
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
}; // PhotonTreeReaderTest

//------------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(PhotonTreeReaderTest);

//------------------------------------------------------------------------------
void
PhotonTreeReaderTest::setUp(void)
{
  const char *fname = "hgg-2013Final8TeV_s12-h120gg-vh-v7n_head10.root";
  TString path = TString::Format("%s/src/MitHgg/TreeWriter/data/%s",
                                 gSystem->Getenv("CMSSW_BASE"), fname);
  TFile * file = TFile::Open(path.Data());
  tree = static_cast<TTree*>(file->Get("hPhotonTree"));
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
  reader = new PhotonTreeReader((TTree*) tree->Clone());
} // PhotonTreeReaderTest::setUp(void)


//------------------------------------------------------------------------------
void
PhotonTreeReaderTest::tearDown(void)
{
  delete reader;
} // PhotonTreeReaderTest::tearDown(void)


//------------------------------------------------------------------------------
void
PhotonTreeReaderTest::testTreeReading(void)
{
  for (Int_t i=0; i<tree->GetEntries(); i++) {
    CPPUNIT_ASSERT_EQUAL(tree->GetEntry(i), reader->GetEntry(i)       );
    CPPUNIT_ASSERT_EQUAL(evtcat           , reader->evtcat            );
    CPPUNIT_ASSERT_EQUAL(evt              , reader->evt               );
    CPPUNIT_ASSERT_EQUAL(mass             , reader->mass              );
    CPPUNIT_ASSERT_EQUAL(cosdphiMetgg     , reader->cosdphiMetgg      );
    CPPUNIT_ASSERT_EQUAL(ph1_e            , reader->ph1F("e")         );
    CPPUNIT_ASSERT_EQUAL(ph2_e            , reader->ph2F("e")         );
    CPPUNIT_ASSERT_EQUAL(ph1_pt           , reader->ph1F("pt")        );
    CPPUNIT_ASSERT_EQUAL(ph2_pt           , reader->ph2F("pt")        );
    CPPUNIT_ASSERT_EQUAL(ph1_idmva        , reader->ph1F("idmva")     );
    CPPUNIT_ASSERT_EQUAL(ph2_idmva        , reader->ph2F("idmva")     );
    CPPUNIT_ASSERT_EQUAL(ph1_hasphoton    , reader->ph1b("hasphoton") );
    CPPUNIT_ASSERT_EQUAL(ph2_hasphoton    , reader->ph2b("hasphoton") );
    CPPUNIT_ASSERT_EQUAL(ph1_index        , reader->ph1i("index")     );
    CPPUNIT_ASSERT_EQUAL(ph2_index        , reader->ph2i("index")     );
    CPPUNIT_ASSERT_EQUAL(ph1_isbarrel     , reader->ph1O("isbarrel")  );
    CPPUNIT_ASSERT_EQUAL(ph2_isbarrel     , reader->ph2O("isbarrel")  );
  };
} //HelloTest::testGetEntry(void)


//------------------------------------------------------------------------------
void
PhotonTreeReaderTest::testBufferModification(void)
{
  /// Load different entries for expected and actual trees.
  tree->GetEntry(1);
  reader->GetEntry(0);

  /// Make sure that we have a significant number of different values
  /// for expected and actual variables.
  CPPUNIT_ASSERT_INEQUAL(evtcat           , reader->evtcat            );
  CPPUNIT_ASSERT_INEQUAL(evt              , reader->evt               );
  CPPUNIT_ASSERT_INEQUAL(mass             , reader->mass              );
  CPPUNIT_ASSERT_INEQUAL(cosdphiMetgg     , reader->cosdphiMetgg      );
  CPPUNIT_ASSERT_INEQUAL(ph1_e            , reader->ph1F("e")         );
  CPPUNIT_ASSERT_INEQUAL(ph2_e            , reader->ph2F("e")         );
  CPPUNIT_ASSERT_INEQUAL(ph1_pt           , reader->ph1F("pt")        );
  CPPUNIT_ASSERT_INEQUAL(ph2_pt           , reader->ph2F("pt")        );
  CPPUNIT_ASSERT_INEQUAL(ph1_idmva        , reader->ph1F("idmva")     );
  CPPUNIT_ASSERT_INEQUAL(ph2_idmva        , reader->ph2F("idmva")     );
    
  /// Skipping variables that will be the same for many entries:
  /// ph{1,2}_hasphoton, ph{1,2}_index, ph{1,2}_isbarrel

  /// Set the actual values in the PhotonTree buffers to the expected ones.
  reader->evtcat             = evtcat       ;
  reader->evt                = evt          ;
  reader->mass               = mass         ;
  reader->cosdphiMetgg       = cosdphiMetgg ;
  reader->ph1F("e")          = ph1_e        ;
  reader->ph2F("e")          = ph2_e        ;
  reader->ph1F("pt")         = ph1_pt       ;
  reader->ph2F("pt")         = ph2_pt       ;
  reader->ph1F("idmva")      = ph1_idmva    ;
  reader->ph2F("idmva")      = ph2_idmva    ;
  reader->ph1b("hasphoton")  = ph1_hasphoton;
  reader->ph2b("hasphoton")  = ph2_hasphoton;
  reader->ph1i("index")      = ph1_index    ;
  reader->ph2i("index")      = ph2_index    ;
  reader->ph1O("isbarrel")   = ph1_isbarrel ;
  reader->ph2O("isbarrel")   = ph2_isbarrel ;

  /// Test that the actual and expected values agree.
  CPPUNIT_ASSERT_EQUAL(evtcat           , reader->evtcat            );
  CPPUNIT_ASSERT_EQUAL(evt              , reader->evt               );
  CPPUNIT_ASSERT_EQUAL(mass             , reader->mass              );
  CPPUNIT_ASSERT_EQUAL(cosdphiMetgg     , reader->cosdphiMetgg      );
  CPPUNIT_ASSERT_EQUAL(ph1_e            , reader->ph1F("e")         );
  CPPUNIT_ASSERT_EQUAL(ph2_e            , reader->ph2F("e")         );
  CPPUNIT_ASSERT_EQUAL(ph1_pt           , reader->ph1F("pt")        );
  CPPUNIT_ASSERT_EQUAL(ph2_pt           , reader->ph2F("pt")        );
  CPPUNIT_ASSERT_EQUAL(ph1_idmva        , reader->ph1F("idmva")     );
  CPPUNIT_ASSERT_EQUAL(ph2_idmva        , reader->ph2F("idmva")     );
  CPPUNIT_ASSERT_EQUAL(ph1_hasphoton    , reader->ph1b("hasphoton") );
  CPPUNIT_ASSERT_EQUAL(ph2_hasphoton    , reader->ph2b("hasphoton") );
  CPPUNIT_ASSERT_EQUAL(ph1_index        , reader->ph1i("index")     );
  CPPUNIT_ASSERT_EQUAL(ph2_index        , reader->ph2i("index")     );
  CPPUNIT_ASSERT_EQUAL(ph1_isbarrel     , reader->ph1O("isbarrel")  );
  CPPUNIT_ASSERT_EQUAL(ph2_isbarrel     , reader->ph2O("isbarrel")  );
}
