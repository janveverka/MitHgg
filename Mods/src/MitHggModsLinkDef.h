// $Id: MitHggModsLinkDef.h,v 1.9 2012/03/31 23:27:25 paus Exp $

#ifndef MITHGG_MODS_LINKDEF_H
#define MITHGG_MODS_LINKDEF_H
#include "MitHgg/Mods/interface/HggAnalysis.h"
#include "MitHgg/Mods/interface/HggAnalysisMod.h"
#include "MitHgg/Mods/interface/ZeeAnalysis.h"
#include "MitHgg/Mods/interface/ZmmAnalysis.h"
#include "MitHgg/Mods/interface/HGGMod.h"
#include "MitHgg/Mods/interface/HGGModSimple.h"
#include "MitHgg/Mods/interface/HGGSimMod.h"
#include "MitHgg/Mods/interface/MVASystematicsMod.h"
#include "MitHgg/Mods/interface/PhotonEffMod.h"
#include "MitHgg/Mods/interface/PhotonMCDataSFMod.h"
#include "MitHgg/Mods/interface/PhotonIsoAreaMod.h"
#include "MitHgg/Mods/interface/PhSCEffMod.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::HggAnalysis+;
#pragma link C++ class mithep::HggAnalysisMod+;
#pragma link C++ class mithep::ZeeAnalysis+;
#pragma link C++ class mithep::ZmmAnalysis+;
#pragma link C++ class mithep::HGGMod+;
#pragma link C++ class mithep::HGGModSimple+;
#pragma link C++ class mithep::HGGSimMod+;
#pragma link C++ class mithep::MVASystematicsMod+;
#pragma link C++ class mithep::PhotonEffMod+;
#pragma link C++ class mithep::PhotonMCDataSFMod+;
#pragma link C++ class mithep::PhotonIsoAreaMod+;
#pragma link C++ class mithep::PhSCEffMod+;

#endif
