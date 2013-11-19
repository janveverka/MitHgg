#ifndef MitHgg_Tools_PathMacros_h
#define MitHgg_Tools_PathMacros_h

#define STRINGIFY(s) #s
#define EXPAND_AND_STRINGIFY(s) STRINGIFY(s)
#define CMSSW_BASE_STRING EXPAND_AND_STRINGIFY(CMSSW_BASE)
#define PREPEND_CMSSW_BASE_AND_STRINGIFY(p) \
  CMSSW_BASE_STRING "/" EXPAND_AND_STRINGIFY(p)

#endif // MitHgg_Tools_PathMacros_h
