#ifndef MitHgg_PhotonTree_ObjectReader_tpp
#define MitHgg_PhotonTree_ObjectReader_tpp

#include "MitHgg/PhotonTree/interface/ObjectReader.h"

templateClassImp(::mithep::hgg::ObjectReader)

namespace mithep
{
  namespace hgg
  {
    //-------------------------------------------------------------------------
    template <class WriterObject>
    ObjectReader<WriterObject>::ObjectReader(TTree* iTree,
                                             const char *iPrefix) :
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
    ObjectReader<WriterObject>::Init()
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
    ObjectReader<WriterObject>::ConsiderDataMember(const TDataMember &member)
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
    ObjectReader<WriterObject>::SetBranchAddress(const TDataMember &member,
                                                 Char_t *address)
    {
      TString suffix = GetBranchNameSuffix(member);
      TString branchName = TString::Format("%s%s%s", fPrefix.Data(),
                                           member.GetName(), suffix.Data());
      const char *name = branchName.Data();
      if (fTree->GetBranch(name)) {
        fTree->SetBranchAddress(name, address + member.GetOffset());
      } else {
        std::cerr << "WARNING: " << typeid(*this).name() 
                  << "::SetBranchAddress(): Didn't find branch `" 
                  << name << "'" << std::endl;
      }
    } /// SetBranchAddress


    //--------------------------------------------------------------------------
    template <class WriterObject>
    TString
    ObjectReader<WriterObject>::GetBranchNameSuffix(const TDataMember &member)
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
      } /// switch
      return suffix;
    } /// GetBranchNameSuffix
  } /// hgg
} /// mithep

#endif /// MitHgg_PhotonTree_ObjectReader_tpp
