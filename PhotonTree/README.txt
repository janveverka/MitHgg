MitHgg/TreeWriter
=================

To-Do List
----------
  * Make a dumper that is an exacutable based on the CategoryReader (28.11. 04:00)
  * Add diphoton MVA cuts to exclusive cats, see table 41 of the AN 2013/253 v3
    (the Hgg AN)
  * Fix MET corrections
  https://github.com/h2gglobe/h2gglobe/blob/
  237f80d3f62f16e563d248f78507101374442a87/
  GeneralFunctions.cc#L1273
  Section 17.5 of
  https://twiki.cern.ch/twiki/pub/CMS/HggAnalyisNote13253/AN-13-253_temp.pdf
  * Produce dumps (preliminarily done 22.11.)
  * Compare dumps (preliminarily done 22.11.)
  * Find interesting events with large discrepancies and/or
    representing large miscategorization groups
  * Factor out members and methods common to MVA readers to a separate
    class and ecapsulate those there while keeping the unique bits
    (mva weights value, etc.)
  * Add more thorough tests of all 3 MVA's to CombinedMvaReaderTest
  * Check vertex selection in exclusive categories
  * Update ElectronIDMod for VH-lep-tag soft electrons to store more than one
    electron
  * Check if the MuonIDMod for VH-lep-tag stores more than one muon.
  * Check the extra delta R (gsf track, photon) cut for the VH cats.
  * Check the VH had selection, especially the photon pt and ID cuts.
  * Check the VH MET selection, especially the photon pt and ID cuts.
  * Run nightly trees, write a log for the last version, make dumps.
  * Remove jet veto from the thight VH-lep tag dilepton selection.
  * Use common jet selector throughout.
  * Write MvaCategoryReaderTest.
  * Factor out ctor params of the MvaCategoryReader and its bases into
  separate class holding the configuration data.
  * Move MitHgg/Synchronization/scripts on CVS to
    HiggsAnalysis/HggSynchroniztion/scripts on git.

To-Done List
------------
  * Added a second lepton to the ElectronIDMod for kHggLeptonTagId2012HCP
    (28.11. 19:00)
  * Removed the photon pt and idmva cuts from the tree writer (28.11. 18:30)
  * Updated the VHHadTag selection - merging b-tag cats into one (26.11. 18:40)
  * Updated the VHHadTag convention in the PhotonTreeWriter (26.11. 18:40)
  * Double-checked b-jet ID (medium trhoughout, 26.11. 18:40)
  * Added jet-lepton Delta R cut for ttH (26.11. 17:55)
  * Check jet-lepton Delta R cut in VH lep (done 26.11. 17:50).
  * Updated the photon ID MVA cuts for the tth tag to agree with the Hgg AN
    (26.11. 13:40)
  * Updated the photon pt cuts for the tth tag to agree with the Hgg AN
    (26.11. 11:40)
  * Updated the tthTag convention in the PhotonTreeWriter (done 26.11 11:00)
  * Defined GetEntry once and inherited it elsewhere, made Update virtual and
    let the magic happen there, e.g. call all the needed base class updates.
    (25.11 23:30)
  * Delete TMVA readers in MVA reader dtors (done 25.11. 15:30)
  * combinedMVA -> combiMVA (done 25.11. 15:30)
  * Added diphoMVA check to DiphotonAndDijetMvaReaderTest. (done 24.11. 23:40)
  * Renamed DijetMvaReader to DiphotonAndDijetMvaReader (done 24.11. 23:40)
  * Removed virtual inheritance (done 23.11.)
  * Understood why there is a difference in the MC diphoMVA between
    Globe Nov 5 and MIT Nov 20 (done 21.11. - mass error smearing)
  * Based CategoryReader on CombinedMvaReader (done 20.11. 4:10)
  * Added missing variables (done 20.11.)
  * Fixed cos theta star, see (done 20.11. 4:30)
    https://github.com/h2gglobe/h2gglobe/blob/\
    ae4356ac0d18e6f77da6e0420ab6f5168e353315/\
    PhotonAnalysis/src/PhotonAnalysis.cc#L4369-L4377
  * Renamed package to MitHgg/PhotonTree (done 19.11. 03:55)
  * Virtually based DiphotonMvaReader on TreeReader (done 19.11. 15:10)
  * Virtually based DijetMvaReader on TreeReader (done 19.11 17:10)
  * Based CombinedMvaReader on DiphotonMvaReader and DijetMvaReader (done 19.11.
    20:40)

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
      - ?
