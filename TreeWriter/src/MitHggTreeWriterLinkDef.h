#ifndef MITHGG_TREEWRITER_LINKDEF_H
#define MITHGG_TREEWRITER_LINKDEF_H

// #include "MitHgg/TreeWriter/interface/DiphotonMvaReader.h"
#include "MitHgg/TreeWriter/interface/PhotonTreeReader.h"
#include "MitHgg/TreeWriter/interface/PhotonTreeReaderDiphotonEvent.h"
#include "MitHgg/TreeWriter/interface/PhotonTreeReaderPhoton.h"
/// Provides mithep::PhotonTreeWriterDiphotonEvent and 
///          mithep::PhotonTreeWriterPhoton
#include "MitPhysics/Mods/interface/PhotonTreeWriter.h"

#endif /// MITHGG_TREEWRITER_LINKDEF_H

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;
#pragma link C++ namespace mithep::hgg;

// #pragma link C++ class mithep::hgg::DiphotonMvaReader+;
#pragma link C++ class mithep::hgg::PhotonTreeReaderDiphotonEvent;
#pragma link C++ class mithep::hgg::PhotonTreeReaderPhotonBase;
#pragma link C++ class mithep::hgg::PhotonTreeReaderPhoton;
#pragma link C++ class mithep::hgg::PhotonTreeReader;

#endif // __CINT__
