// MitHgg/TreeWriter/PhotonTree.h - PhotonTree class
// Holds a pointer to the PhotonTree and sets branch addresses to the
// PhotonTreeWriterDiphotonEvent members
// Author: J. Veverka
//-----------------------------------------------------------------------------
#ifndef MITHGG_TREEWRITER_PHOTONTREE_H
#define MITHGG_TREEWRITER_PHOTONTREE_H

#include <map>
#include <string>
#include "TObject.h"
#include "TTree.h"
#include "TDataMember.h"
#include "TString.h"

// Provides mithep::PhotonTreeWriterDiphotonEvent
#include "MitPhysics/Mods/interface/PhotonTreeWriter.h"

namespace mithep
{
  namespace hgg
  {
    class PhotonTree : public TObject, public PhotonTreeWriterDiphotonEvent
    {
      public:
        PhotonTree (TTree* iTree);
        virtual ~PhotonTree();
        
        Int_t GetEntry(Long64_t entry = 0, Int_t getall = 0) {
          return fTree->GetEntry(entry, getall);
        }
        TTree * GetTree() {return fTree;}

        Char_t   & ph1B(const char *name){return *(Char_t   *) GetPtr(0, name);}
        Char_t   & ph2B(const char *name){return *(Char_t   *) GetPtr(1, name);}
        UChar_t  & ph1b(const char *name){return *(UChar_t  *) GetPtr(0, name);}
        UChar_t  & ph2b(const char *name){return *(UChar_t  *) GetPtr(1, name);}
        Short_t  & ph1S(const char *name){return *(Short_t  *) GetPtr(0, name);}
        Short_t  & ph2S(const char *name){return *(Short_t  *) GetPtr(1, name);}
        UShort_t & ph1s(const char *name){return *(UShort_t *) GetPtr(0, name);}
        UShort_t & ph2s(const char *name){return *(UShort_t *) GetPtr(1, name);}
        Int_t    & ph1I(const char *name){return *(Int_t    *) GetPtr(0, name);}
        Int_t    & ph2I(const char *name){return *(Int_t    *) GetPtr(1, name);}
        UInt_t   & ph1i(const char *name){return *(UInt_t   *) GetPtr(0, name);}
        UInt_t   & ph2i(const char *name){return *(UInt_t   *) GetPtr(1, name);}
        Float_t  & ph1F(const char *name){return *(Float_t  *) GetPtr(0, name);}
        Float_t  & ph2F(const char *name){return *(Float_t  *) GetPtr(1, name);}
        Double_t & ph1D(const char *name){return *(Double_t *) GetPtr(0, name);}
        Double_t & ph2D(const char *name){return *(Double_t *) GetPtr(1, name);}
        Long64_t & ph1L(const char *name){return *(Long64_t *) GetPtr(0, name);}
        Long64_t & ph2L(const char *name){return *(Long64_t *) GetPtr(1, name);}
        ULong64_t& ph1l(const char *name){return *(ULong64_t*) GetPtr(0, name);}
        ULong64_t& ph2l(const char *name){return *(ULong64_t*) GetPtr(1, name);}
        Bool_t   & ph1O(const char *name){return *(Bool_t   *) GetPtr(0, name);}
        Bool_t   & ph2O(const char *name){return *(Bool_t   *) GetPtr(1, name);}
      private:
        void Init();
        void InitBranchAddresses();
        void InitNameToOffsetMap();
        void SetBranchAddress(const TDataMember &member, Char_t *addr,
                              const char *prefix = "");
        bool HasValidType(const TDataMember &member);
        TString GetTypeString(const TDataMember &tdm);

        Char_t* GetPtr(UInt_t photonIndex, const char * memberName);

        TTree *fTree; //!
        std::map<std::string, Long_t> offsetForName; //!

        ClassDef(PhotonTree,1)

    }; // PhotonTree
  } // hgg
} // mithep

#endif // MITHGG_TREEWRITER_TRANSIENTDIPHOTON_H
