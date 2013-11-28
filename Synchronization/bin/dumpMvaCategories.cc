#include <boost/shared_ptr.hpp>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWLite/Tools/interface/ChainSource.h"
#include "MitHgg/Synchronization/interface/MvaCategoryDumper.h"
#include "MitHgg/Tools/interface/PathMacros.h"
#include "MitHgg/Tools/interface/PSet.h"

#define HGG_DEFAULT_MVA_CATEGORY_DUMPER_CONFIG src/MitHgg/Synchronization/\
python/dump_mva_categories_default_cfg.py

#define HGG_DEFAULT_MVA_CATEGORY_DUMPER_CONFIG_PATH \
  PREPEND_CMSSW_BASE_AND_STRINGIFY(HGG_DEFAULT_MVA_CATEGORY_DUMPER_CONFIG)


using fwlite::tools::ChainSource;
using mithep::hgg::PSet;
using mithep::hgg::PSetPtr;
using mithep::hgg::MvaCategoryDumper;

//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  const char *cfg_path = HGG_DEFAULT_MVA_CATEGORY_DUMPER_CONFIG_PATH;
  if (argc >= 2) cfg_path = argv[1];

  // Get the configuration:
  PythonProcessDesc builder(cfg_path, argc, argv);
  PSetPtr cfg(builder.processDesc()->getProcessPSet());
  // Get the input chain:
  ChainSource source(cfg->getParameter<PSet>("inputs"));
  // Create the dumper:
  MvaCategoryDumper dumper(&source.chain(), cfg->getParameter<PSet>("dump"));
  // Run!
  dumper.ProduceDump();
} // main
