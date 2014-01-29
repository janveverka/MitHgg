// // $Id: ZeeEfficiency.cc,v 1.1 2011/01/24 14:57:09 paus Exp $
// 
// #include <TMath.h>
// #include <TH1D.h>
// #include "MitCommon/MathTools/interface/MathUtils.h"
// #include "MitAna/DataUtil/interface/Debug.h"
// #include "MitAna/DataTree/interface/Names.h"
// #include "MitAna/DataTree/interface/ElectronCol.h"
// 
// #include "MitHgg/Mods/interface/ZeeEfficiency.h"
// 
// using namespace mithep;
// 
// ClassImp(mithep::ZeeEfficiency)
// 
// //--------------------------------------------------------------------------------------------------
// ZeeEfficiency::ZeeEfficiency(const char *name, const char *title) :
//   BaseMod             (name,title),
//   // initialize bambu objects
//   fTrigObjsName       (Names::gkHltObjBrn),
//   fElecName           (Names::gkElectronBrn),
//   fElecsFromBranch    (kTRUE),
//   fMcEventInfoName    (Names::gkMCEvtInfoBrn),
//   fElecs              (0),
//   fMcEventInfo        (0),
//   fIsData             (kTRUE),
//   fSkimming           (kFALSE),
//   // scale removal
//   fOverlapCut         (-1.0),
//   // cuts for selection
//   fElecEtMin          (20.0),
//   //fNoPixelSeedCut     (kTRUE),
//   fElecSigEeBarMax    ( 0.0105),
//   fElecSigEeEcpMax    ( 0.0300),
//   fElecIsoMax         ( 0.30),
//   // cuts for selection
//   f2ElecMassMin       (140.0),
//   //fElecR9Min          (0.9),
//   // initialize the histograms
//   hAllNElecs          (0),
//   hAllElecPt          (0),
//   hAllElecEta         (0),
//   //hAllElecPixelSeed   (0),
//   hAllElecSigEeBar    (0),
//   hAllElecSigEeEcp    (0),
//   hAllElecEcHcTrIso   (0),
//   hTrigDeltaEta       (0),
//   hTrigDeltaPhi       (0),
//   hTrigDeltaEt        (0),
//   h2SeleElecMass      (0),
//   hNElecs             (0),
//   hElecEtMx           (0),
//   hN2Elecs            (0),
//   hElecEt1            (0),
//   hElecEt2            (0),
//   hElecEta1           (0),
//   hElecEta2           (0),
//   hElecPhi1           (0),
//   hElecPhi2           (0),
//   //hElecR91            (0),
//   //hElecR92            (0),
//   hElecDelR           (0),
//   h2ElecEtSum         (0),
//   h2ElecPt            (0),
//   h2ElecEta           (0),
//   h2ElecMass          (0),
//   //hHmElecR91          (0),
//   //hHmElecR92          (0),
//   //h2R9ElecMass        (0),
//   hNTrigElecs         (0),
//   h2TrigElecMass      (0)
// {
//   // Constructor.
// }
// 
// //--------------------------------------------------------------------------------------------------
// void ZeeEfficiency::Begin()
// {
//   // Run startup code on the client machine. For this module, we dont do anything here.
// }
// 
// //--------------------------------------------------------------------------------------------------
// void ZeeEfficiency::Process()
// {
//   // Process entries of the tree. For this module, we just load the branches and fill the
//   // histograms.
// 
//   // count the events we have processed
//   IncNEventsProcessed();
// 
//   // // access the elecs
//   // LoadEventObject(fElecName,fElecs);
//   // 
//   // // loop through them and make some pictures and select the ones relevant to our analysis
//   // double lastEt = 99999.0;
//   // ResetSelectedElecs();
//   // for (UInt_t i=0; i<fElecs->GetEntries(); ++i) {
//   //   const Electron *p = fElecs->At(i);
//   // }
//   // // loop through our selected elecs and make histograms only if one was matched to trigger
//   // UInt_t n2Elecs = 0;
//   // for (UInt_t i=0; i<fSelectedElecs.size(); i++) {
//   //   const Electron *p1 = fElecs->At(fSelectedElecs[i]);
//   //   const FourVectorM m1 = p1->Mom();
//   //   for (UInt_t j=i+1; j<fSelectedElecs.size(); ++j) {
//   //     const Electron *p2 = fElecs->At(fSelectedElecs[j]);
//   //     double mass = (m1+m2).M();
//   // 
//   //     hSele2ElecMass ->Fill(mass);
//   //     hFail2ElecMass ->Fill(mass);
//   //   }
//   // }
//   // // work only with triggered elecs
//   // for (UInt_t i=0; i<fTriggeredElecs.size(); i++) {
//   //   const Electron *p1 = fElecs->At(fTriggeredElecs[i]);
//   // 
//   //   // loop through all pairs
//   //   const FourVectorM m1 = p1->Mom();
//   //   for (UInt_t j=i+1; j<fTriggeredElecs.size(); ++j) {
//   //     const Electron *p2 = fElecs->At(fTriggeredElecs[j]);
//   //     const FourVectorM m2 = p2->Mom();
//   //     h2TrigElecMass ->Fill((m1+m2).M());
//   //   }
//   // }
// 
//   return;
// }
// 
// //--------------------------------------------------------------------------------------------------
// void ZeeEfficiency::SlaveBegin()
// {
//   // Run startup code on the computer (slave) doing the actual analysis. Here,
//   // we typically initialize histograms and other analysis objects and request
//   // branches or objects created by earlier modules.
// 
//   // request the following Event Objects (often branches, but could be locally created ones)
//   ReqEventObject(fElecName,fElecs,fElecsFromBranch);
// 
//   // for MC only to adjust potential overlaps from generation
//   if (fIsData) {
//     printf(" --> this is data. Drop the McEventInfo.\n\n");
// 
//   // book all histograms
//   AddTH1(hSele2ElecMass,   "hSele2ElecMass",";Dielectron Mass [GeV/c^{2}];#",       400, 0.0,400.0);
//   AddTH1(hFail2ElecMass,   "hFail2ElecMass",";Dielectron Mass [GeV/c^{2}];#",       400, 0.0,400.0);
// }
// 
// //--------------------------------------------------------------------------------------------------
// void ZeeEfficiency::SlaveTerminate()
// {
//   // Run finishing code on the computer (slave) that did the analysis. For this
//   // module, we dont do anything here.
// }
// 
// //--------------------------------------------------------------------------------------------------
// void ZeeEfficiency::Terminate()
// {
//   // Run finishing code on the client computer. For this module, we dont do
//   // anything here.
// }
// 
// //--------------------------------------------------------------------------------------------------
// void ZeeEfficiency::ResetSelectedElecs()
// {
//   // Reset the selected and trigger matched elecs
// 
//   fSelectedElecs .clear();
//   fTriggeredElecs.clear();
// 
//   return;
// }
// 
// //--------------------------------------------------------------------------------------------------
// void ZeeEfficiency::MatchElecsToTrigger()
// {
//   // Reset the selected and trigger matched elecs
// 
//   // access all trigger objects
//   const TriggerObjectCol *tos = GetHLTObjects(fTrigObjsName);
//   if (! tos)             // this can only happen if HLTMod::SetAbortIfNotAccepted(kFALSE) was called
//     return;
//   UInt_t nEnts = tos->GetEntries();
// 
//   // loop through our selected elecs
//   for (UInt_t i=0; i<fSelectedElecs.size(); i++) {
//     const Electron *p = fElecs->At(fSelectedElecs[i]);
//     UInt_t matched = 0;
//     // loop through all trigger objects and try to find a match
//     for (UInt_t j=0; j<nEnts; ++j) {
//       const TriggerObject *to = tos->At(j);
//       if (fabs(p->Eta()-to->Eta())          < 0.10 &&
//           fabs(p->Phi()-to->Phi())          < 0.05 &&
//           fabs((p->Pt() -to->Pt())/p->Pt()) < 0.05   ) {
//         matched++;
//         hTrigDeltaEta->Fill(p->Eta()-to->Eta());
//         hTrigDeltaPhi->Fill(p->Phi()-to->Phi());
//         hTrigDeltaEt ->Fill((p->Pt()-to->Pt())/p->Pt());
//       }
//     }
//     // add to our trigger collection if it was matched
//     if (matched > 0)
//       fTriggeredElecs.push_back(fSelectedElecs[i]);
//     // print warning if desired
//     if (matched > 1) {
//       MDB(kGeneral,2) {
//         printf(" Ambiguos match found: %d\n",matched);
//         printf(" Elec:  %8.3f %8.3f %8.3f\n",p->Eta(), p->Phi(), p->Pt());
//         for (UInt_t j=0; j<nEnts; ++j) {
//           const TriggerObject *to = tos->At(j);
//           printf(" TrigObj: %8.3f %8.3f %8.3f\n",to->Eta(),to->Phi(),to->Pt());
//         }
//       }
//     }
//   }
// 
//   return;
// }
// 
// //--------------------------------------------------------------------------------------------------
// Bool_t ZeeEfficiency::ElecTriggered(UInt_t iElec)
// {
//   // Determine whether the requested elec index is in the triggered elec indices
// 
//   // per default the elec did not trigger
//   Bool_t triggered = kFALSE;
// 
//   // loop through triggered elec indices
//   for (UInt_t i=0; i<fTriggeredElecs.size(); i++) {
//     if (fTriggeredElecs[i] == iElec) {
//       // index was found -> elec triggered
//       triggered = kTRUE;
//       break;
//     }
//   }
// 
//   return triggered;
// }
