MitHgg/PhotonTree Package
=========================

To-Do List: Features & Bugs
---------------------------
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

Tree Production
---------------
  * hgg-2013Final8TeV_0
      - From Nov 14
      - only s12-h125gg-vh-v7n
  * hgg-2013Final8TeV_1
      - From Nov 20
  * hgg-2013Final8TeV_2
      - From Nov 21
  * hgg-2013Final8TeV_3
      - From Nov 28
      - only s12-h125gg-vh-v7n
      - Has MET corrections, stochastic smearing, no photon pt/idmva
      - Misses fully combinatoric VHLepTag and tthTag

Dump Production
---------------
hgg-dump-mva-categories dump_mva_categories_8TeV_data_cfg.py > mit_cats_runABCD_dec04_v2.txt &
hgg-dump-mva-categories dump_mva_categories_8TeV_data3_cfg.py > mit_cats_runABCD_dec04_v3.txt &
hgg-dump-mva-categories dump_mva_categories_7TeV_data_unmerged_cfg.py > mit_cats_2011AB_dec04.txt &
