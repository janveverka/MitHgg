#include "TClass.h"
#include "TList.h"

#include <iostream>
#include "FWCore/Utilities/interface/Exception.h"
#include "MitHgg/TreeWriter/interface/PhotonTree.h"

using mithep::hgg::PhotonTree;

typedef ::cms::Exception Bad;

/// Make this a ROOT class
ClassImp(PhotonTree)

//------------------------------------------------------------------------------
PhotonTree::PhotonTree(TTree* iTree) :
  fTree(iTree)
{
  Init();
}


//------------------------------------------------------------------------------
PhotonTree::~PhotonTree()
{}


//------------------------------------------------------------------------------
void PhotonTree::Init()
{
  assert(sizeof(Char_t) == 1);
  InitBranchAddresses();
  InitNameToOffsetMap();
}


//------------------------------------------------------------------------------
void PhotonTree::InitBranchAddresses()
{
  TClass *eclass = TClass::GetClass("mithep::PhotonTreeWriterDiphotonEvent");
  TClass *pclass = TClass::GetClass("mithep::PhotonTreeWriterPhoton<16>");
  TList  *elist  = eclass->GetListOfDataMembers();
  TList  *plist  = pclass->GetListOfDataMembers();

  // Loop over all data members of the diphoton
  Char_t *addr = (Char_t*) static_cast<PhotonTreeWriterDiphotonEvent*>(this);
  for (int i=0; i<elist->GetEntries(); ++i) {
    const TDataMember *member = static_cast<const TDataMember*>(elist->At(i));
    SetBranchAddress(*member, addr);
  } // End of loop over all datamembers of the diphoton

  // Loop over all data members of the photons
  for (int iph=0; iph<2; ++iph) {
    Char_t *addr = (Char_t*) &photons[iph];
    TString prefix = TString::Format("ph%d.", iph + 1);
    for (int i=0; i<plist->GetEntries(); ++i) {
      const TDataMember *member = static_cast<const TDataMember*>(plist->At(i));
      SetBranchAddress(*member, addr, prefix.Data());
    }
  }

}


//------------------------------------------------------------------------------
void PhotonTree::InitNameToOffsetMap()
{
  TClass *pclass = TClass::GetClass("mithep::PhotonTreeWriterPhoton<16>");
  TList  *plist  = pclass->GetListOfDataMembers();
  for (int i=0; i<plist->GetEntries(); ++i) {
    const TDataMember *member = static_cast<const TDataMember*>(plist->At(i));
    if (not (member->IsBasic() &&
              member->IsPersistent() &&
              HasValidType(*member))) {
      continue;
    }
    offsetForName[member->GetName()] = member->GetOffset();
  }

}
//------------------------------------------------------------------------------
void PhotonTree::SetBranchAddress(const TDataMember &member, Char_t *addr,
                                  const char *prefix)
{
  if (not (member.IsBasic() &&
           member.IsPersistent() &&
           HasValidType(member))) {
    return;
  }

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
    throw Bad("MemberType") << "Dimension-" << member.GetArrayDim()
                            << " array not supported!";
  } // switch
  
  TString name = TString::Format("%s%s%s", prefix, member.GetName(),
                                 suffix.Data());
  if (fTree->GetBranch(name)) {
    fTree->SetBranchAddress(name.Data(), addr + member.GetOffset());
  } else {
    std::cerr << "WARNING: PhotonTree::Init(): Didn't find branch `"
              << name << "'" << std::endl;
  }
}


//------------------------------------------------------------------------------
bool PhotonTree::HasValidType(const TDataMember &member)
{
  bool valid = false;
  if      (TString(member.GetTypeName()).BeginsWith("Char_t"   )) valid = true;
  else if (TString(member.GetTypeName()).BeginsWith("UChar_t"  )) valid = true;
  else if (TString(member.GetTypeName()).BeginsWith("Short_t"  )) valid = true;
  else if (TString(member.GetTypeName()).BeginsWith("UShort_t" )) valid = true;
  else if (TString(member.GetTypeName()).BeginsWith("Int_t"    )) valid = true;
  else if (TString(member.GetTypeName()).BeginsWith("UInt_t"   )) valid = true;
  else if (TString(member.GetTypeName()).BeginsWith("Float_t"  )) valid = true;
  else if (TString(member.GetTypeName()).BeginsWith("Double_t" )) valid = true;
  else if (TString(member.GetTypeName()).BeginsWith("Long64_t" )) valid = true;
  else if (TString(member.GetTypeName()).BeginsWith("ULong64_t")) valid = true;
  else if (TString(member.GetTypeName()).BeginsWith("Bool_t"   )) valid = true;
  return valid;
}

//------------------------------------------------------------------------------
/**
 * This is a hack around the fact that the datamembers of the photons are
 * private. We will access them anyways!
 */
Char_t* PhotonTree::GetPtr(UInt_t photonIndex, const char * memberName)
{
  Char_t *address = (Char_t*) &photons[photonIndex];
  Long_t offset = offsetForName[memberName];
  return address + offset;
}
