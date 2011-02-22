// $Id: MitHggModsLinkDef.h,v 1.1 2011/01/25 11:58:53 paus Exp $

#ifndef MITHGG_MODS_LINKDEF_H
#define MITHGG_MODS_LINKDEF_H
#include "MitHgg/Mods/interface/HggAnalysis.h"
#include "MitHgg/Mods/interface/ZeeAnalysis.h"
#include "MitHgg/Mods/interface/ZmmAnalysis.h"
#include "MitHgg/Mods/interface/HGGMod.h"
#include "MitHgg/Mods/interface/HGGSimMod.h"
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
#pragma link C++ class mithep::HGGMod+;
#pragma link C++ class mithep::HGGSimMod+;

#endif
