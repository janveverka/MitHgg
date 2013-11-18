#ifndef MITHGG_TREEWRITER_PHOTONTREEREADEROBJECTBASE_H
#define MITHGG_TREEWRITER_PHOTONTREEREADEROBJECTBASE_H

#include <assert.h>
#include <iostream>             // std::cerr and std::endl
#include <typeinfo>             // operator typeid
#include "TObject.h"
#include "TString.h"
#include "TClass.h"
#include "TList.h"
#include "TDataMember.h"
#include "TTree.h"

namespace mithep
{
  namespace hgg
  {
    //-------------------------------------------------------------------------
    template <class WriterObject>
    class PhotonTreeReaderObjectBase : public TObject, public WriterObject
    {
    public:
      typedef WriterObject WriterObjectType;
      PhotonTreeReaderObjectBase(TTree* iTree,
                                 const char *iPrefix = "");
      virtual ~PhotonTreeReaderObjectBase(){}
    protected:
      virtual void Init();
      static bool ConsiderDataMember(const TDataMember &member);
      void SetBranchAddress(const TDataMember &member, Char_t *address);
      static TString GetBranchNameSuffix(const TDataMember &member);
      TTree  *fTree;
      TString fPrefix;
      std::map<std::string, Long_t> fOffsetForName;
      
      ClassDef(PhotonTreeReaderObjectBase, 0);
    }; // PhotonTreeReaderObjectBase
  } // hgg
} // mithep

templateClassImp(::mithep::hgg::PhotonTreeReaderObjectBase)

namespace mithep
{
  namespace hgg
  {
    //-------------------------------------------------------------------------
    template <class WriterObject>
    PhotonTreeReaderObjectBase<WriterObject>::PhotonTreeReaderObjectBase(
      TTree* iTree, const char *iPrefix
    ) :
      TObject(),
      WriterObject(),
      fTree(iTree),
      fPrefix(iPrefix)
    {
      Init();
    } /// Ctor
    
    
    //-------------------------------------------------------------------------
    template <class WriterObject>
    void
    PhotonTreeReaderObjectBase<WriterObject>::Init()
    {
      assert(sizeof(Char_t) == 1);
      TClass *myClass = TClass::GetClass(typeid(WriterObject));
      TList  *myList  = myClass->GetListOfDataMembers();
      // Loop over all data members of the WriterObject
      Char_t *address = (Char_t*) static_cast<WriterObject*>(this);
      for (int i=0; i<myList->GetEntries(); ++i) {
        const TDataMember *member(
          static_cast<const TDataMember*>(myList->At(i))
        );
        if (!ConsiderDataMember(*member)) continue;
        fOffsetForName[member->GetName()] = member->GetOffset();
        SetBranchAddress(*member, address);
      } // End of loop over all datamembers of the WriterObject
    } /// Init


    //-------------------------------------------------------------------------
    template <class WriterObject>
    bool
    PhotonTreeReaderObjectBase<WriterObject>::ConsiderDataMember(
      const TDataMember &member
    )
    {
      bool consider = false;
      if (member.IsBasic() &&
          member.IsPersistent() &&
          (TString(member.GetTypeName()).BeginsWith("Char_t"   ) ||
           TString(member.GetTypeName()).BeginsWith("UChar_t"  ) ||
           TString(member.GetTypeName()).BeginsWith("Short_t"  ) ||
           TString(member.GetTypeName()).BeginsWith("UShort_t" ) ||
           TString(member.GetTypeName()).BeginsWith("Int_t"    ) ||
           TString(member.GetTypeName()).BeginsWith("UInt_t"   ) ||
           TString(member.GetTypeName()).BeginsWith("Float_t"  ) ||
           TString(member.GetTypeName()).BeginsWith("Double_t" ) ||
           TString(member.GetTypeName()).BeginsWith("Long64_t" ) ||
           TString(member.GetTypeName()).BeginsWith("ULong64_t") ||
           TString(member.GetTypeName()).BeginsWith("Bool_t"   ))) {
        consider = true;
      }
      return consider;
    } /// ConsiderDataMember

    //--------------------------------------------------------------------------
    template <class WriterObject>
    void
    PhotonTreeReaderObjectBase<WriterObject>::SetBranchAddress(
      const TDataMember &member, Char_t *address
    )
    {
      TString suffix = GetBranchNameSuffix(member);
      TString branchName = TString::Format("%s%s%s", fPrefix.Data(),
                                           member.GetName(), suffix.Data());
      const char *name = branchName.Data();
      if (fTree->GetBranch(name)) {
        fTree->SetBranchAddress(name, address + member.GetOffset());
      } else {
        std::cerr << "WARNING: PhotonTreeReaderObjectBase<"
                  << typeid(WriterObject).name() << ">::SetBranchAddress(): "
                  << "Didn't find branch `" << name << "'" << std::endl;
      }
    } /// SetBranchAddress


    //--------------------------------------------------------------------------
    template <class WriterObject>
    TString
    PhotonTreeReaderObjectBase<WriterObject>::GetBranchNameSuffix(
      const TDataMember &member
    )
    {
      TString suffix;
      switch (member.GetArrayDim()) {
        case 0:
          suffix = "";
          break;
        case 1:
          suffix = TString::Format("[%i]", member.GetMaxIndex(0));
          break;
        default:
          /// This should never happen.
          std::cerr << "Illegal array dimension: "
                    << member.GetArrayDim() << "!" << std::endl;
          assert(0);
      } // switch
      return suffix;
    } // GetBranchNameSuffix
  } // hgg
} // mithep

#endif // MITHGG_TREEWRITER_PHOTONTREEREADEROBJECTBASE_H
