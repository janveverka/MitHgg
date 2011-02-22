// $Id: HGGSimMod.cc,v 1.6 2010/03/12 13:51:26 bendavid Exp $

#include "MitHgg/Mods/interface/HGGSimMod.h"
#include <TH1D.h>
#include <TH2F.h>
#include <TNtuple.h>
#include "Math/GenVector/Boost.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataTree/interface/ElectronCol.h"
#include "MitAna/DataTree/interface/MCParticleCol.h"
#include "MitAna/DataTree/interface/MuonCol.h"
#include "MitAna/DataTree/interface/TrackCol.h"
#include "MitAna/DataTree/interface/PFMetCol.h"
#include "MitAna/DataTree/interface/CaloMetCol.h"
#include "MitAna/DataTree/interface/VertexCol.h"
#include "MitAna/DataTree/interface/PFCandidateCol.h"
#include "MitAna/DataTree/interface/DecayParticleCol.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitPhysics/Utils/interface/ElectronTools.h"

using namespace mithep;

ClassImp(mithep::HGGSimMod)

//--------------------------------------------------------------------------------------------------
HGGSimMod::HGGSimMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fMCPartName(Names::gkMCPartBrn),
  fTrackName(Names::gkTrackBrn),
  fMuonName(Names::gkMuonBrn),
  fElectronName(Names::gkElectronBrn),
  fPFMetName("PFMet"),
  fCaloMetName("CorMuonMet"),
  fPFCandidatesName("PFCandidates"),
  fPrimaryVertexName("PrimaryVertexes"),
  fConversionName("MvfConversions"),
  fMuonsFromBranch(kTRUE),
  fElectronsFromBranch(kTRUE),
  fMatchWZ(kFALSE),
  fMatchSim(kFALSE),
  fParticles(0),
  fTracks(0),
  fMuons(0),
  fElectrons(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void HGGSimMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void HGGSimMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and
  // fill the histograms.
  

  LoadEventObject(fMCPartName,fParticles);
  for (UInt_t i=0; i<fParticles->GetEntries(); ++i) {

    const MCParticle *p = fParticles->At(i);

//     if (p->PdgId()==22) {
//       const MCParticle *mother = p->Mother();
//       if (mother && mother->AbsPdgId()!=11 && mother->AbsPdgId()!=22 && mother->AbsPdgId()!=111 && mother->AbsPdgId()!=221) {
//         printf("Photon: index %i, mother pdgid = %i\n",i,mother->PdgId());
//       }
//     }
    //if (p->AbsPdgId()==24) printf("W status = %i\n",p->Status());
    
    //if (!(p->Status()==2 && p->AbsPdgId()==24)) continue;

    //if (!(p->AbsPdgId()==25)) continue;
    if (!(p->AbsPdgId()==25 || p->AbsPdgId()<=6 || p->AbsPdgId()==21)) continue;


//     printf("found higgs, status = %i, ndaughters = %i\n",p->Status(),p->NDaughters());
//     for (UInt_t j = 0; j<p->NDaughters(); ++j) {
//       printf("Daughter %i, pdgid = %i\n", j, p->Daughter(j)->PdgId());
//     }

    if (p->NDaughters()<2) continue;

    const MCParticle *g1 = p->Daughter(0);
    const MCParticle *g2 = p->Daughter(1);

    if (g1->PdgId()!=22 || g2->PdgId()!=22) continue;

    if (g2->Pt() > g1->Pt()) {
      g1 = p->Daughter(1);
      g2 = p->Daughter(0);
    }

//     printf("g1: status = %i, isgenerated = %i, is simulated = %i, ndaughters = %i\n",g1->Status(),g1->IsGenerated(),g1->IsSimulated(),g1->NDaughters());
//      for (UInt_t j = 0; j<g1->NDaughters(); ++j) {
//       printf("g1 Daughter %i, pdgid = %i, status = %i, isgenerated = %i, issimulated = %i, ndaughters = %i\n", j, g1->Daughter(j)->PdgId(),g1->Daughter(j)->Status(),g1->Daughter(j)->IsGenerated(),g1->Daughter(j)->IsSimulated(),g1->Daughter(j)->NDaughters());
//     }   

    Bool_t isconverted1 = kFALSE;
    Bool_t isconverted2 = kFALSE;

    UInt_t nelectrons1 = 0;
    UInt_t nelectrons2 = 0;
    Double_t ept11 = -99.;
    Double_t ept21 = -99.;
    Double_t ept12 = -99.;
    Double_t ept22 = -99.;

    Double_t cosdalpha1 = -99.;
    Double_t cosdalpha2 = -99.;
    Double_t cosdalphaboost1 = -99.;
    Double_t cosdalphaboost2 = -99.;

    ThreeVector norm1;
    ThreeVector norm2;
    ThreeVector normboost1;
    ThreeVector normboost2;

    const MCParticle *simg1 = g1;
    const MCParticle *simg2 = g2;

    while (simg1 && !simg1->IsSimulated()) simg1 = simg1->FindDaughter(22);
    while (simg2 && !simg2->IsSimulated()) simg2 = simg2->FindDaughter(22);

    if (!simg1 || !simg2) continue;

    for (UInt_t j=0; j<simg1->NDaughters(); ++j) {
      if (simg1->Daughter(j)->AbsPdgId()==11) ++nelectrons1;
    }

    for (UInt_t j=0; j<simg2->NDaughters(); ++j) {
      if (simg2->Daughter(j)->AbsPdgId()==11) ++nelectrons2;
    }

    FourVectorM hvector = g1->Mom() + g2->Mom();

    if (nelectrons1==2) {
      isconverted1 = kTRUE;
      const MCParticle *electron = simg1->FindDaughter(11,kTRUE);
      const MCParticle *positron = simg1->FindDaughter(-11,kTRUE);

      if (electron->Pt()>positron->Pt()) {
        ept11 = electron->Pt();
        ept21 = positron->Pt();
      }
      else {
        ept11 = positron->Pt();
        ept21 = electron->Pt();
      }

      ThreeVector momelectron(electron->Px(),electron->Py(),electron->Pz());
      ThreeVector mompositron(positron->Px(),positron->Py(),positron->Pz());

      cosdalpha1 = momelectron.Unit().Dot(mompositron.Unit());
      norm1 = momelectron.Cross(mompositron).Unit();

      ROOT::Math::Boost hboost(hvector.BoostToCM().X(),hvector.BoostToCM().Y(),hvector.BoostToCM().Z());

      FourVector electronboost = hboost(FourVector(electron->Mom()));
      FourVector positronboost = hboost(FourVector(positron->Mom()));

      ThreeVector momelectronboost(electronboost.Px(),electronboost.Py(),electronboost.Pz());
      ThreeVector mompositronboost(positronboost.Px(),positronboost.Py(),positronboost.Pz());

      cosdalphaboost1 = momelectronboost.Unit().Dot(mompositronboost.Unit());
      normboost1 = momelectronboost.Cross(mompositronboost).Unit();

    }

    if (nelectrons2==2) {
      isconverted2 = kTRUE;
      const MCParticle *electron = simg2->FindDaughter(11,kTRUE);
      const MCParticle *positron = simg2->FindDaughter(-11,kTRUE);

      if (electron->Pt()>positron->Pt()) {
        ept12 = electron->Pt();
        ept22 = positron->Pt();
      }
      else {
        ept12 = positron->Pt();
        ept22 = electron->Pt();
      }

      ThreeVector momelectron(electron->Px(),electron->Py(),electron->Pz());
      ThreeVector mompositron(positron->Px(),positron->Py(),positron->Pz());

      cosdalpha2 = momelectron.Unit().Dot(mompositron.Unit());
      norm2 = momelectron.Cross(mompositron).Unit();

      ROOT::Math::Boost hboost(hvector.BoostToCM().X(),hvector.BoostToCM().Y(),hvector.BoostToCM().Z());

      FourVector electronboost = hboost(FourVector(electron->Mom()));
      FourVector positronboost = hboost(FourVector(positron->Mom()));

      ThreeVector momelectronboost(electronboost.Px(),electronboost.Py(),electronboost.Pz());
      ThreeVector mompositronboost(positronboost.Px(),positronboost.Py(),positronboost.Pz());

      cosdalphaboost2 = momelectronboost.Unit().Dot(mompositronboost.Unit());
      normboost2 = momelectronboost.Cross(mompositronboost).Unit();

    }

    Double_t cosdbeta = norm1.Dot(norm2);
    Double_t cosdbetaboost = normboost1.Dot(normboost2);

    
    Float_t fillGen[] = { p->Pt(),
                          p->Eta(),
                          p->Phi(),
                          p->Mass(),
                          g1->Pt(),
                          g1->Eta(),
                          g1->Phi(),
                          g2->Pt(),
                          g2->Eta(),
                          g2->Phi(),
                          isconverted1,
                          simg1->DecayVertex().X(),
                          simg1->DecayVertex().Y(),
                          simg1->DecayVertex().Z(),
                          simg1->DecayVertex().Rho(),
                          simg1->DecayVertex().Phi(),
                          ept11,
                          ept21,
                          isconverted2,
                          simg2->DecayVertex().X(),
                          simg2->DecayVertex().Y(),
                          simg2->DecayVertex().Z(),
                          simg2->DecayVertex().Rho(),
                          simg2->DecayVertex().Phi(),
                          ept12,
                          ept22,
                          cosdalpha1,
                          cosdalpha2,
                          cosdbeta,
                          cosdalphaboost1,
                          cosdalphaboost2,
                          cosdbetaboost,
                          (g1->Mom() + g2->Mom()).Pt()
                          };
                          
    hGenNtuple->Fill(fillGen);
    break;
  }
  

}

//--------------------------------------------------------------------------------------------------
void HGGSimMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqEventObject(fMCPartName,   fParticles, kTRUE);
  ReqEventObject(fTrackName,    fTracks, kTRUE);
  ReqEventObject(fMuonName,     fMuons, fMuonsFromBranch);
  ReqEventObject(fElectronName, fElectrons, fElectronsFromBranch);
  ReqEventObject(fElectronLooseName, fElectronsLoose, kFALSE);  
  ReqEventObject("Electrons", fRecoElectrons, kTRUE);  
  ReqEventObject(fPFMetName,    fPFMet, kTRUE);
  ReqEventObject(fCaloMetName,    fCaloMet, kTRUE);
  ReqEventObject(fPFCandidatesName, fPFCandidates, kTRUE);
  ReqEventObject(fPrimaryVertexName, fPrimaryVertex, kTRUE);
  ReqEventObject(fConversionName, fConversions, kTRUE);
  
  
  hGenNtuple = new TNtuple("hGenNtuple","hGenNtuple","hpt:heta:hphi:hmass:pt1:eta1:phi1:pt2:eta2:phi2:isconverted1:vx1:vy1:vz1:vrho1:vphi1:ept11:ept21:isconverted2:vx2:vy2:vz2:vrho2:vphi2:ept12:ept22:cosdalpha1:cosdalpha2:cosdbeta:cosdalphaboost1:cosdalphaboost2:cosdbetaboost:dipt");
  AddOutput(hGenNtuple);
}

//--------------------------------------------------------------------------------------------------
void HGGSimMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void HGGSimMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}

const MCParticle *HGGSimMod::MatchWZ(const Electron *e) const
{
  for (UInt_t i=0; i<fParticles->GetEntries(); ++i) {
    const MCParticle *p = fParticles->At(i);
    if ( p->AbsPdgId()==11 && p->IsGenerated() && MathUtils::DeltaPhi(*e,*p) < 0.3 && p->DistinctMother() && (p->DistinctMother()->AbsPdgId()==24 || p->DistinctMother()->AbsPdgId()==23) )
      return p;
  }
  
  return 0;
  
}

const MCParticle *HGGSimMod::SimConv(const Electron *e) const
{

  const MCParticle *p = e->GsfTrk()->MCPart();
  if (!p) return 0;

  if (p->AbsPdgId() != 11) return 0;

  const MCParticle *m = p->DistinctMother();

  if (!m) return 0;

  if (!m->PdgId()==22) return 0;

  return m;

  
}

const MCParticle *HGGSimMod::SimDalitz(const Electron *e) const
{

  const MCParticle *p = e->GsfTrk()->MCPart();
  if (!p) return 0;

  if (p->AbsPdgId() != 11) return 0;

  const MCParticle *m = p->DistinctMother();

  if (!m) return 0;

  if (!m->PdgId()==111) return 0;

  return m;

  
}

const MCParticle *HGGSimMod::SimChargedPiKP(const Electron *e) const
{

  const MCParticle *p = e->GsfTrk()->MCPart();
  if (!p) return 0;

  if ( ! (p->AbsPdgId() == 211 || p->AbsPdgId()==321 || p->AbsPdgId()==2212) ) return 0;

  return p;

  
}

const MCParticle *HGGSimMod::SimHeavyFlavour(const Electron *e) const
{

  const MCParticle *p = e->GsfTrk()->MCPart();
  if (!p) return 0;

  if (p->AbsPdgId() != 11) return 0;

  const MCParticle *m = p->DistinctMother();

  if (!m) return 0;

  if (m->PdgId()==111 || m->PdgId()==22 || m->AbsPdgId()==24 || m->AbsPdgId()==23) return 0;

  return m;

  
}

