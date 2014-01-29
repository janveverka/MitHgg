#ifndef MitHgg_Tools_PSet_h
#define MitHgg_Tools_PSet_h

#include <boost/shared_ptr.hpp>
#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace mithep
{
  namespace hgg
  {
    typedef edm::ParameterSet       PSet;
    typedef boost::shared_ptr<PSet> PSetPtr;
  } /// hgg
} /// mithep

#endif /// MitHgg_Tools_PSet_h
