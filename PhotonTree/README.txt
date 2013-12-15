MitHgg/PhotonTree Package
=========================

Excess events of 12 Dec 2013
********************************
   run *      lumi *     event *
********************************
196250 *        64 * 117746603 * sublead ele
199864 *        89 * 130028405 * lead muon
200178 *        19 *  32126525 * sublead ele
205236 *       158 * 233579287 * sublead ele
206401 *       127 * 148903812 * lead ele
206542 *       371 * 607863421 * sublead ele
207920 *       500 * 748865301 * sublead ele
********************************



Excess VH lep tight with ghost electron:
196250:64

Events missing leptons for ttH:
run:206446 lumi:784  evt:1072391444  run:D file:50A9A2B5-0583-E211-A8E2-00266CFAE050.root 
run:201202 lumi:51   evt:49882246    run:C file:9A6D0CDB-776E-E211-83D1-00261894383C.root
run:200229 lumi:506  evt:537326223   run:C file:
run:199021 lumi:1393 evt:15522682932 run:C file:

CMS Dataset                              Bambu            Runs
/Photon/Run2012A-22Jan2013-v1/AOD        r12a-pho-j22-v1  190456-193621
/DoublePhoton/Run2012B-22Jan2013-v1/AOD  r12b-dph-j22-v1  193834-196531
/DoublePhoton/Run2012C-22Jan2013-v2/AOD  r12c-dph-j22-v1  198022-203742
/DoublePhoton/Run2012D-22Jan2013-v1/AOD  r12d-dph-j22-v1  203777-208509

/mnt/hadoop/cms/store/user/paus/filefi/029/

14 missing leptonic events

#   Period  Run      Lumi    Event        File
--------------------------------------------------------------------------------------------
 1  B       194691     80      63517077   BA6F366A-116D-E211-8362-0026189438E1.root mu, 0058
 2          195397   1172    1361890417   E8047264-3B6D-E211-BAA5-0026189437F5.root e
 3          195950    188     197022547   3AD29621-1C6D-E211-9EB7-0026189438D2.root e, MIA
 4  C       198969    341     449378235   B802FEBC-2F71-E211-8D0B-00261894387D.root e
 5          199008    294     369441614   464C5B08-9770-E211-B580-002354EF3BE2.root e
 6          199021   1393    1522682932   E2F8FB06-DC70-E211-B730-003048679244.root mu
 7          199832    227     280392762   DC7DB42E-5E71-E211-98E4-00261894391F.root e
 8          200229    506     537326223   BC4184B7-B270-E211-AB38-003048679010.root e
 9          201202     51      49882246   EEF3A1EC-2571-E211-9772-003048FFCB9E.root e
10          202504     84      45496025   E40562A0-1D71-E211-8116-0026189437FA.root e
11  D       204601    218     291520084   DCF115DA-A382-E211-951D-00266CFAE518.root e
12          206331    106      93829188   D251C2DD-C884-E211-9D28-00A0D1EE968C.root e
13          206446    784    1072391444   50A9A2B5-0583-E211-A8E2-00266CFAE050.root e
14          207231    402     619292470   5C867B4C-0882-E211-9266-00A0D1EE8ECC.root e

#    Run      Lep   pt          cat**
-------------------------------------
 1   194691*  mu    34.75410     11
 2   195397   e    138.21800      9
 3   195950#  e    189.659        9
 4   198969   e     46.228698     9
 5   199008   e     20.823799     9
 6   199021*  mu    53.690799    11
 7   199832+  e     26.846399    11
 8   200229   e     56.068599    11
 9   201202+  e     38.626701    11
10   202504   e     39.029098     8
11   204601   e     21.475799     9
12   206331   e     24.265199     9
13   206446+  e     73.195396    11
14   207231   e     66.628898     8

* Recovered by relaxing Delta R(mu,g) > 1.0 to > 0.5 in TTHSelectMuon (test_3.txt)
# Recovered by Josh's ElectronIDMod fix with multiple electrons logic (test_4.txt)
+ Should be recovered by properly relaxing Delta R(e,g) > 1.0 to 0.5.
**cat definitions: VHLepTigth: 8, VHLepLoose: 9, TTHLep: 11
Fixes from Josh:
  * Loop over all leptons in VH lep, not just the hardest muon and highest-MVA
    electron
  * Remove |m(ll) - m(Z)| < 10 GeV for ttH
  * Remove |eta(ele)| > 2.5 in 

To-Do List: Features & Bugs
---------------------------
  * Dangerous events after the first logic fix:
    - Contain exclusive-tag leptons
    - Contain multiple photon pairs
    - Selected pair fails some of the selection downstream 
  * Produce and compare dumps often
  * Check vertex selection in exclusive categories
  * Check the extra delta R (gsf track, photon) cut for the VH and ttH cats.
  * Add bjet CSV
  * Loosen the pt cut for numJets and numBJets to pt > 20 GeV
  * Check diphoton vs lepton combinatorics/arbitration
  * Find interesting events with large discrepancies and/or
    representing large miscategorization groups
  * Update the plotting script
    - Make plots of all the differences
      automatically doing the absolute ones and/or the relative ones
      with both relative and absolute ranges.
    - Add both fixed and floating boundary histograms
    - exclude events with -999 in one of the frameworks, plot the 
      variable values for these instead.
    - make a 2D histogram of
      the number of events on/off in each framework.
    - list events with the largest differences
    - make plots of widht versus containment
  * Point out AN v3 inconsistent wording of the cuts for ttH and VH lep where
    MVA corresponds to diphoton in one case and photon ID in the other.

Wish List: Readability, Agility & User-Friendliness
---------------------------------------------------
  * Describe the meaning of the variables defined in the MvaCategoryReader
  * Write dump directly into a file given by the configuration
  * Print the list of the categories and their numbers
  * Factor out members and methods common to MVA readers to a separate
    class and ecapsulate those there while keeping the unique bits
    (mva weights value, etc.)
  * Add more thorough tests of all 3 MVA's to CombinedMvaReaderTest
  * Use common jet selector throughout the PhotonTreeWriter
  * Write MvaCategoryReaderTest.
  * Factor out ctor params of the MvaCategoryReader and its bases into
    separate classes holding the configuration data with the same
    inheritance structure as the readers.
  * Move MitHgg/Synchronization/scripts on CVS to
    HiggsAnalysis/HggSynchroniztion/scripts on git.

To-Done List
------------
  * 04.12. 12:00 - Support configurable options for TMVA::Readers, defaults to
                   "Silent"
  * 04.12. 11:00 - MvaCategoryReader works for both 7 and 8 TeV
  * 04.12. 09:00 - Fixed the missing std::max around deltaPhi vars for VH MET
                   in the MvaCategoryReader
  * 03.12. 22:00 - Put together a configuration file for 7 TeV
  * 30.11. 11:12 - DumpMvaCategories: Added new preselection for the dijet-tag 
                   related vars, aligned ele* and cos(theta*) defaults with Globe, 
                   changed cos(theta*) to |cos(theta*)|
  * 30.11. 10:30 - DumpMvaCategories: Removed preselection, removed trailing 
                   tab on each line, set undifend muon variables to -999
  * 29.11. 14:00 - Updated photon pt and diphoton MVA cuts in and new 
                   conventions for the ttH and VH lep in MvaCategoryReader
  * 29.11. 13:00 - Set cos theta*  to -999 in the dump if not VH had,
                   similarly for dijetMVA and combiMVA if not VBF
  * 29.11. 12:00 - Implemented the VBF selection in the MvaCategoryReader
  * 29.11. 02:00 - Added more plots to the comparison script, produced new comparisons
    to the first Globe dump with the inclusive/dijet cats a la Josh
    http://www.hep.caltech.edu/~veverka/plots/2013/13-11-29/globe_mit/
  * 29.11. 01:00 - Updated the tree writer tag deffinitions for VH lep and 
    ttH to keep track of VH lep loose and ttH had independently of VH lep tight and
    ttH lep, respectively
  * 28.11. 19:00 - Added the second lepton to the ElectronIDMod for
  kHggLeptonTagId2012HCP
  * 28.11. 18:30 - Removed the photon pt and idmva cuts from the tree 
  writer exclusive tags
  * 28.11. 15:00 - Josh fixed the MET corrections
  * 28.11. 06:00 - Produced first data dumps with the updated categories,
    compared to Jim's cats for Globe.
    http://www.hep.caltech.edu/~veverka/plots/2013/13-11-28/globe_vs_mit/
  * 28.11. 05:00 - Made dumper config files for data and MC with the updated
    untagged and dijet categories from Josh
  * 28.11. 04:00 - Made a dumper that is an exacutable based on the MvaCategoryReader
  * 28.11. 00:45 - Josh implemented the stochastic smearing and E-dependent 
  corrections
  * 27.11.       - Implemented lepton arbitration for the sttH lep category
  * 27.11.       - Checked that the MuonIDMod for VH-lep-tag stores more than one muon.
  * 27.11.       - Removed the jet veto from the thight VH-lep tag dilepton selection.
  * 26.11. 18:40 - Updated the VHHadTag selection - merging b-tag cats into one
  * 26.11. 18:40 - Updated the VHHadTag convention in the PhotonTreeWriter
  * 26.11. 18:40 - Double-checked b-jet ID (medium throughout)
  * 26.11. 17:55 - Added jet-lepton Delta R cut for ttH
  * 26.11. 17:50 - Checked jet-lepton Delta R cut in VH lep
  * 26.11. 13:40 - Updated the photon ID MVA cuts for the tth tag to agree 
    with the Hgg AN
  * 26.11. 11:40 - Updated the photon pt cuts for the tth tag to agree 
    with the Hgg AN
  * 26.11. 11:00 - Updated the tthTag convention in the PhotonTreeWriter
  * 25.11. 23:30 - Defined GetEntry once and inherited it elsewhere, made 
    Update virtual and let the magic happen there, e.g. call the needed 
    base class Update in the correct order.
  * 25.11. 15:30 - Delete TMVA readers in all MVA reader dtors
  * 25.11. 15:30 - combinedMVA -> combiMVA
  * 24.11. 23:40 - Added diphoMVA check to DiphotonAndDijetMvaReaderTest
  * 24.11. 23:40 - Renamed DijetMvaReader to DiphotonAndDijetMvaReader
  * 23.11.       - Completely removed virtual inheritance
  * 22.11.       - Produced and compared preliminary data dumps
  * 21.11.       - Understood why there is a difference in the MC diphoMVA between
    Globe Nov 5 and MIT Nov 20 (mass error smearing)
  * 20.11. 4:10  - Based CategoryReader on CombinedMvaReader
  * 20.11.       - Added missing variables to the category dumper
  * 20.11. 4:30  - Fixed cos theta star, see 
    https://github.com/h2gglobe/h2gglobe/blob/\
    ae4356ac0d18e6f77da6e0420ab6f5168e353315/\
    PhotonAnalysis/src/PhotonAnalysis.cc#L4369-L4377
  * 19.11. 03:55 - Renamed the MitHgg/TreeWriter package to MitHgg/PhotonTree
  * 19.11. 15:10 - Virtually based DiphotonMvaReader on TreeReader
  * 19.11. 17:10 - Virtually based DijetMvaReader on TreeReader
  * 19.11. 20:40 - Based CombinedMvaReader on DiphotonMvaReader and DijetMvaReader

