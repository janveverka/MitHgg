#ifndef MitHgg_PhotonTree_DiphotonEventReader_h
#define MitHgg_PhotonTree_DiphotonEventReader_h
#include "MitHgg/PhotonTree/interface/ObjectReader.h"
/// Provides mithep::PhotonTreeWriterDiphotonEvent
#include "MitPhysics/Mods/interface/PhotonTreeWriter.h"
namespace mithep
{
  namespace hgg 
  {
    typedef ObjectReader< 
              ::mithep::PhotonTreeWriterDiphotonEvent
              > DiphotonEventReader;
  } 
} 
#endif /// MitHgg_PhotonTree_DiphotonEventReader_h
