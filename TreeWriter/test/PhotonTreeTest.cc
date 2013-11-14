/**
 * \file MitHgg/TreeWriter/test/PhotonTreeTest.cc
 * \class PhotonTreeTest
 * \brief Unit test of the PhotonTree class
 * \author Jan Veverka, MIT, jan.veverka@cern.ch
 * \date 12 November 2013
 */

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "MitHgg/TreeWriter/interface/PhotonTree.h"

using ::mithep::hgg::PhotonTree;

//------------------------------------------------------------------------------
/**
 * PhotonTree test fixture
 */
class PhotonTreeTest : public ::CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(PhotonTreeTest);
  CPPUNIT_TEST(testEntries);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp(void);
  void tearDown(void);

protected:
  void testEntries(void);

private:
  TTree      *tree;
  PhotonTree *photonTree;
  UChar_t    evtcat;
  UInt_t     evt;
  Float_t    mass;
  Double_t   cosdphiMetgg;
  UChar_t    ph1_hasphoton, ph2_hasphoton;
  UInt_t     ph1_index    , ph2_index    ;
  Float_t    ph1_e        , ph2_e        ;
  Bool_t     ph1_isbarrel , ph2_isbarrel ;
}; // class PhotonTreeTest

//------------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(PhotonTreeTest);

//------------------------------------------------------------------------------
void
PhotonTreeTest::setUp(void)
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
  tree->SetBranchAddress("ph1.isbarrel" , &ph1_isbarrel );
  tree->SetBranchAddress("ph2.isbarrel" , &ph2_isbarrel );
  photonTree = new PhotonTree((TTree*) tree->Clone());
} // PhotonTreeTest::setUp(void)


//------------------------------------------------------------------------------
void
PhotonTreeTest::tearDown(void)
{
  delete photonTree;
} // PhotonTreeTest::tearDown(void)


//------------------------------------------------------------------------------
void
PhotonTreeTest::testEntries(void)
{
  std::cout << std::endl;
  for (Int_t i=0; i<tree->GetEntries(); i++) {
    CPPUNIT_ASSERT_EQUAL(tree->GetEntry(i), photonTree->GetEntry(i)       );
    CPPUNIT_ASSERT_EQUAL(evtcat           , photonTree->evtcat            );
    CPPUNIT_ASSERT_EQUAL(evt              , photonTree->evt               );
    CPPUNIT_ASSERT_EQUAL(mass             , photonTree->mass              );
    CPPUNIT_ASSERT_EQUAL(cosdphiMetgg     , photonTree->cosdphiMetgg      );
    CPPUNIT_ASSERT_EQUAL(ph1_e            , photonTree->ph1F("e")         );
    CPPUNIT_ASSERT_EQUAL(ph2_e            , photonTree->ph2F("e")         );
    CPPUNIT_ASSERT_EQUAL(ph1_hasphoton    , photonTree->ph1b("hasphoton") );
    CPPUNIT_ASSERT_EQUAL(ph2_hasphoton    , photonTree->ph2b("hasphoton") );
    CPPUNIT_ASSERT_EQUAL(ph1_index        , photonTree->ph1i("index")     );
    CPPUNIT_ASSERT_EQUAL(ph2_index        , photonTree->ph2i("index")     );
    CPPUNIT_ASSERT_EQUAL(ph1_isbarrel     , photonTree->ph1O("isbarrel")  );
    CPPUNIT_ASSERT_EQUAL(ph2_isbarrel     , photonTree->ph2O("isbarrel")  );
  };
} //HelloTest::testGetEntry(void)
