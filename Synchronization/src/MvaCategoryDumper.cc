#include <iostream>  // std::cout
#include <string>
#include "MitHgg/Synchronization/interface/MvaCategoryDumper.h"
#include "MitHgg/PhotonTree/interface/TestTreeFactory.h"

using ::mithep::hgg::MvaCategoryDumper;
using ::mithep::hgg::PSet;

//------------------------------------------------------------------------------
MvaCategoryDumper::MvaCategoryDumper(TTree *tree, const PSet &iConfig) :
  MvaCategoryReader(
    tree,
    iConfig.getParameter<std::string>("diphotonMvaWeights").c_str()
  )
{} /// Ctor


//------------------------------------------------------------------------------
MvaCategoryDumper::~MvaCategoryDumper() {} /// Dtor


//------------------------------------------------------------------------------
void
MvaCategoryDumper::produceDump()
{
  for (unsigned iEntry=0; iEntry < fTree->GetEntries(); iEntry++) {
    GetEntry(iEntry);
    dumpVar("run"  , run );
    dumpVar("lumi" , lumi);
    dumpVar("event", evt );
    std::cout << std::endl;
  } /// Loop over entries
} /// run


//------------------------------------------------------------------------------
template <class T>
void
MvaCategoryDumper::dumpVar(const char *name, T value, const char *suffix)
{
  std::cout << name << ":" << value << suffix;
} /// dumpVar



