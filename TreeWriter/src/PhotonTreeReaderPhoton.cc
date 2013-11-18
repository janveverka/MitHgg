#include "MitHgg/TreeWriter/interface/PhotonTreeReaderPhoton.h"

using ::mithep::hgg::PhotonTreeReaderPhoton;
using ::mithep::hgg::PhotonTreeReaderPhotonBase;

ClassImp(PhotonTreeReaderPhoton)
ClassImp(PhotonTreeReaderPhotonBase)

//------------------------------------------------------------------------------
PhotonTreeReaderPhoton::PhotonTreeReaderPhoton(TTree *iTree,
                                               const char *prefix) :
  PhotonTreeReaderPhotonBase(iTree, prefix),
  e        (GetF("e"        )),
  pt       (GetF("pt"       )),
  eta      (GetF("eta"      )),
  phi      (GetF("phi"      )),
  idmva    (GetF("idmva"    )),
  hasphoton(Getb("hasphoton")),
  index    (Geti("index"    )),
  isbarrel (GetO("isbarrel" ))
  {}


//------------------------------------------------------------------------------
Char_t *
PhotonTreeReaderPhoton::GetPtr(const char *memberName)
{
  Char_t *address = (Char_t*) static_cast<WriterObjectType*>(this);
  Long_t offset = fOffsetForName[memberName];
  return address + offset;
} /// GetPtr
