MitHgg/TreeWriter
=================

To-Do List
----------
  * Rename package to MitHgg/PhotonTree (done 19.11. 03:55)
  * Virtually base DiphotonMvaReader on TreeReader (done 19.11. 15:10)
  * Virtually base DijetMvaReader on TreeReader (done 19.11 17:10)
  * Base CombinedMvaReader on DiphotonMvaReader and DijetMvaReader (done 19.11.
    20:40)
  * Base CategoryReader on CombinedMvaReader
  * Produce dumps of data and MC
  * Make a dumper that is an exacutable based on the CategoryReader
  * Add missing variables
  * Fix cos theta star
  * Add diphoton MVA cuts to exclusive cats
  * Produce dumps
  * Compare dumps
  * Find interesting events with large discrepancies and/or
    representing large miscategorization groups
  * Factor out members and methods common to MVA readers to a separate
    class and ecapsulate those there while keeping the unique bits
    (mva weights value, etc.)
  * Delete TMVA readers in MVA reader dtors
  * Remove virtual inheritance
  * combinedMVA -> combiMVA
  * Define GetEntry once and inherit it elsewhere, make Update virtual and let
  the magic happen there, e.g. call all the needed base class updates.