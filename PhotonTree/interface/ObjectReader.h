#ifndef MitHgg_PhotonTree_ObjectReader_h
#define MitHgg_PhotonTree_ObjectReader_h

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
    class ObjectReader : public TObject, public WriterObject
    {
    public:
      typedef WriterObject WriterObjectType;
      ObjectReader(TTree* iTree, const char *iPrefix = "");
      virtual ~ObjectReader(){}
    protected:
      virtual void   Init();
      static bool    ConsiderDataMember (const TDataMember &member);
      void           SetBranchAddress   (const TDataMember &member, 
                                         Char_t *address          );
      static TString GetBranchNameSuffix(const TDataMember &member);
      TTree  *fTree;
      TString fPrefix;
      std::map<std::string, Long_t> fOffsetForName;
      
      ClassDef(ObjectReader, 0);
    }; /// ObjectReader
  } /// hgg
} /// mithep

#endif /// MitHgg_PhotonTree_ObjectReader_h
