#ifndef MitHgg_PhotonTree_MitHggPhotonTreeLinkDef_h
#define MitHgg_PhotonTree_MitHggPhotonTreeLinkDef_h

#include "MitHgg/PhotonTree/interface/PhotonReader.h"
#include "MitHgg/PhotonTree/interface/DiphotonEventReader.h"
#include "MitHgg/PhotonTree/interface/TreeReader.h"
#include "MitHgg/PhotonTree/interface/DiphotonMvaReader.h"
#include "MitHgg/PhotonTree/interface/DijetMvaReader.h"
#include "MitHgg/PhotonTree/interface/CombinedMvaReader.h"

#endif /// MitHgg_PhotonTree_MitHggPhotonTreeLinkDef_h


//------------------------------------------------------------------------------
#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;
#pragma link C++ namespace mithep::hgg;

/// Order matters!  If class B depends on class A, then A must come first.
#pragma link C++ class mithep::hgg::PhotonReaderBase;
#pragma link C++ class mithep::hgg::PhotonReader;
#pragma link C++ class mithep::hgg::DiphotonEventReader;
#pragma link C++ class mithep::hgg::TreeReader;
#pragma link C++ class mithep::hgg::DiphotonMvaReader+;
#pragma link C++ class mithep::hgg::DijetMvaReader+;
#pragma link C++ class mithep::hgg::CombinedMvaReader+;

#endif // __CINT__