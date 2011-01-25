// $Id: $

#ifndef MITHGG_MODS_LINKDEF_H
#define MITHGG_MODS_LINKDEF_H
#include "MitHgg/Mods/interface/HggAnalysis.h"
#include "MitHgg/Mods/interface/ZeeAnalysis.h"
#include "MitHgg/Mods/interface/ZmmAnalysis.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::HggAnalysis+;
#pragma link C++ class mithep::ZeeAnalysis+;
#pragma link C++ class mithep::ZmmAnalysis+;
#endif
