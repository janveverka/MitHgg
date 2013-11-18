#ifndef MitHgg_TreeWriter_PhotonTreeReaderPhoton_h
#define MitHgg_TreeWriter_PhotonTreeReaderPhoton_h
#include "MitHgg/TreeWriter/interface/PhotonTreeReaderObjectBase.h"
/// Provides mithep::PhotonTreeWriterPhoton<16>
#include "MitPhysics/Mods/interface/PhotonTreeWriter.h"
namespace mithep {
  namespace hgg {
    
    typedef PhotonTreeReaderObjectBase<
              ::mithep::PhotonTreeWriterPhoton<16>
              > PhotonTreeReaderPhotonBase;
    
    //--------------------------------------------------------------------------
    class PhotonTreeReaderPhoton : public PhotonTreeReaderPhotonBase {      
    public:
      PhotonTreeReaderPhoton(TTree* iTree, const char *iPrefix);
      virtual ~PhotonTreeReaderPhoton() {}
      
      /// Generic string-based accessors
      Bool_t   & GetO(const char *name){return *(Bool_t   *) GetPtr(name);}
      Char_t   & GetB(const char *name){return *(Char_t   *) GetPtr(name);}
      UChar_t  & Getb(const char *name){return *(UChar_t  *) GetPtr(name);}
      Short_t  & GetS(const char *name){return *(Short_t  *) GetPtr(name);}
      UShort_t & Gets(const char *name){return *(UShort_t *) GetPtr(name);}
      Int_t    & GetI(const char *name){return *(Int_t    *) GetPtr(name);}
      UInt_t   & Geti(const char *name){return *(UInt_t   *) GetPtr(name);}
      Long64_t & GetL(const char *name){return *(Long64_t *) GetPtr(name);}
      ULong64_t& Getl(const char *name){return *(ULong64_t*) GetPtr(name);}
      Float_t  & GetF(const char *name){return *(Float_t  *) GetPtr(name);}
      Double_t & GetD(const char *name){return *(Double_t *) GetPtr(name);}
      
      /// References to private data members of the PhotonTreeWriterPhoton.
      Float_t  &e        ;
      Float_t  &pt       ;
      Float_t  &eta      ;
      Float_t  &phi      ;
      Float_t  &idmva    ;
      UChar_t  &hasphoton;
      UInt_t   &index    ;
      Bool_t   &isbarrel ;
      
    private:
      Char_t* GetPtr(const char *memberName);
      ClassDef(PhotonTreeReaderPhoton, 0)      
    }; /// PhotonTreeReaderPhoton
    
  } /// hgg
} /// mithep
#endif /// MitHgg_TreeWriter_PhotonTreeReaderPhoton_h
