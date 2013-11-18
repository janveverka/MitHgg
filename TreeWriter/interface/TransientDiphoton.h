// MitHgg/TreeWriter/TransientDiphoton.h - TransientDiphoton class
// Holds transient diphoton final state data.
// Author: J. Veverka
//-----------------------------------------------------------------------------
#ifndef MITHGG_TREEWRITER_TRANSIENTDIPHOTON_H
#define MITHGG_TREEWRITER_TRANSIENTDIPHOTON_H

namespace mithep
{
  namespace hgg
  {
    class TransientDiphoton
    {
    public:
      TransientDiphoton(const Vertex *v, const Photon *p1, const Photon *p2) :
        fVertex(v), fLeadPhoton(p1), fSubleadPhoton(p2) {}                          
      const Vertex* Vertex() {return fVertex;}
      const Photon* LeadPhoton() {return fLeadPhoton;}
      const Photon* SubleadPhoton() {return fSubleadPhoton;}
      void SetVertex(const Vertex *v) {fVertex = v;}
      void SetPhotons(const Photon *p1, const Photon *p2) {
        fLeadPhoton = p1; 
        fSubleadPhoton = p2;
      }
    private:
      const Vertex *fVertex;
      const Photon *fLeadPhoton;
      const Photon *fSubleadPhoton;
    } // class TransientDiphoton
  } // namespace hgg
} // namespace mitheps
#endif
