// $Id: MitHggModsLinkDef.h,v 1.5 2011/04/13 12:14:27 bendavid Exp $

#ifndef MITHGG_MODS_LINKDEF_H
#define MITHGG_MODS_LINKDEF_H
#include "MitHgg/Mods/interface/HggAnalysis.h"
#include "MitHgg/Mods/interface/ZeeAnalysis.h"
#include "MitHgg/Mods/interface/ZmmAnalysis.h"
#include "MitHgg/Mods/interface/HGGMod.h"
#include "MitHgg/Mods/interface/HGGModSimple.h"
#include "MitHgg/Mods/interface/HGGSimMod.h"
#include "MitHgg/Mods/interface/PhotonEffMod.h"
#include "MitHgg/Mods/interface/PhSCEffMod.h"
#include "MitHgg/Mods/interface/PhotonIsoAreaMod.h"
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
#pragma link C++ class mithep::HGGModSimple+;
#pragma link C++ class mithep::HGGSimMod+;
#pragma link C++ class mithep::PhotonEffMod+;
#pragma link C++ class mithep::PhSCEffMod+;
#pragma link C++ class mithep::PhotonIsoAreaMod+;

#endif
