#ifndef MitHgg_TreeWriter_PhotonTreeReaderDiphotonEvent_h
#define MitHgg_TreeWriter_PhotonTreeReaderDiphotonEvent_h
#include "MitHgg/TreeWriter/interface/PhotonTreeReaderObjectBase.h"
// Provides mithep::PhotonTreeWriterDiphotonEvent
#include "MitPhysics/Mods/interface/PhotonTreeWriter.h"
namespace mithep {
  namespace hgg {
    typedef PhotonTreeReaderObjectBase<
              ::mithep::PhotonTreeWriterDiphotonEvent
              > PhotonTreeReaderDiphotonEvent;
  } // hgg
} // mithep
#endif // MitHgg_TreeWriter_PhotonTreeReaderDiphotonEvent_h
