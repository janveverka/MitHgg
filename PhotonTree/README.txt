MitHgg/TreeWriter
=================

To-Do List
----------
  * Rename package to MitHgg/PhotonTree (done 19.11. 03:55)
  * Virtually base DiphotonMvaReader on TreeReader (done 19.11. 15:10)
  * Virtually base DijetMvaReader on TreeReader (done 19.11 17:10)
  * Base CombinedMvaReader on DiphotonMvaReader and DijetMvaReader (done 19.11.
    20:40)
  * Base CategoryReader on CombinedMvaReader (done 20.11. 4:10)
  * Produce dumps of data and MC (submitted 20.11. 6:15)
  * Make a dumper that is an exacutable based on the CategoryReader
  * Add missing variables (done 20.11.)
  * Fix cos theta star, see (done 20.11. 4:30)
    https://github.com/h2gglobe/h2gglobe/blob/\
    ae4356ac0d18e6f77da6e0420ab6f5168e353315/\
    PhotonAnalysis/src/PhotonAnalysis.cc#L4369-L4377
  * Add diphoton MVA cuts to exclusive cats
  * Double-check b-jet ID (medium vs tight)
  * Produce dumps (preliminarily done 22.11.)
  * Compare dumps (preliminarily done 22.11.)
  * Find interesting events with large discrepancies and/or
    representing large miscategorization groups
  * Factor out members and methods common to MVA readers to a separate
    class and ecapsulate those there while keeping the unique bits
    (mva weights value, etc.)
  * Delete TMVA readers in MVA reader dtors
  * Remove virtual inheritance (done 23.11.)
  * combinedMVA -> combiMVA
  * Define GetEntry once and inherit it elsewhere, make Update virtual and let
    the magic happen there, e.g. call all the needed base class updates.
  * Understand why there is a difference in the MC diphoMVA between
    Globe Nov 5 and MIT Nov 20 (done 21.11. - mass error smearing)
  * Add diphoMVA check to DiphotonAndDijetMvaReaderTest. (done 24.11. 23:40)
  * Rename DijetMvaReader to DiphotonAndDijetMvaReader (done 24.11. 23:40)
  * Check jet-lepton Delta R cut in VH lep
  * Fix MET corrections
