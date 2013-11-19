#!/bin/env python
'''
Dumps a C++ snippet for the PhotonReader class.
'''
import ROOT

type_string_map = {
    'Bool_t'  : 'O',
    'Chat_t'  : 'B', 'UChar_t'  : 'b',
    'Short_t' : 'S', 'UShort_t' : 's',
    'Int_t'   : 'I', 'UInt_t'   : 'i',
    'Long64_t': 'L', 'ULong64_t': 'l',
    'Float_t' : 'F', 'Double_t' : '',
    }


#_______________________________________________________________________________
def main():
    ROOT.gSystem.Load('libMitHggPhotonTree')
    ROOT.gSystem.Load('libMitPhysicsMods')
    tclass = ROOT.TClass.GetClass('mithep::PhotonTreeWriterPhoton<16>')
    rows = []
    for member in tclass.GetListOfDataMembers():
        if not consider(member):
            continue
        rows.append((member.GetTypeName(), member.GetName()))
    tmax = max([len(t) for t,n in rows])
    nmax = max([len(n) for t,n in rows])
    print "\n== MitHgg/PhotonTree/interface/PhotonReader.h =="
    for mtype, mname in rows:
        print '        %s &%s;' % (mtype.ljust(tmax), mname.ljust(nmax))
    print "\n== MitHgg/PhotonTree/src/PhotonReader.cc =="
    ccrows = ['  %s(Get%s(%s))' % (mname.ljust(nmax), 
                                   type_string_map[mtype],
                                   ('"%s"' % mname).ljust(nmax + 2))
              for mtype, mname in rows]
    print ',\n'.join(ccrows)
## main


#_______________________________________________________________________________
def consider(member):
    return (member.IsBasic() and
            member.IsPersistent() and
            member.GetArrayDim() == 0 and
            member.GetTypeName() in type_string_map.keys())
## consider


#_______________________________________________________________________________
if __name__ == '__main__':
    main()
    import user
