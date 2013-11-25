MitHgg/TreeWriter
=================

To-Do List
----------
  * Define GetEntry once and inherit it elsewhere, make Update virtual and let
    the magic happen there, e.g. call all the needed base class updates.
  * Make a dumper that is an exacutable based on the CategoryReader
  * Add diphoton MVA cuts to exclusive cats
  * Double-check b-jet ID (medium vs tight)
  * Check jet-lepton Delta R cut in VH lep.
  * Add jet-lepton Delta R cut for ttH
  * Fix MET corrections
  * Produce dumps (preliminarily done 22.11.)
  * Compare dumps (preliminarily done 22.11.)
  * Find interesting events with large discrepancies and/or
    representing large miscategorization groups
  * Factor out members and methods common to MVA readers to a separate
    class and ecapsulate those there while keeping the unique bits
    (mva weights value, etc.)
  * Add more thorough tests of all 3 MVA's to CombinedMvaReaderTest
  * Update the tthTag convention in the PhotonTreeWriter
  * Update the VHHadTag convention in the PhotonTreeWriter

To-Done List
------------
  * Rename package to MitHgg/PhotonTree (done 19.11. 03:55)
  * Virtually base DiphotonMvaReader on TreeReader (done 19.11. 15:10)
  * Virtually base DijetMvaReader on TreeReader (done 19.11 17:10)
  * Base CombinedMvaReader on DiphotonMvaReader and DijetMvaReader (done 19.11.
    20:40)
  * Base CategoryReader on CombinedMvaReader (done 20.11. 4:10)
  * Add missing variables (done 20.11.)
  * Fix cos theta star, see (done 20.11. 4:30)
    https://github.com/h2gglobe/h2gglobe/blob/\
    ae4356ac0d18e6f77da6e0420ab6f5168e353315/\
    PhotonAnalysis/src/PhotonAnalysis.cc#L4369-L4377
  * Delete TMVA readers in MVA reader dtors (done 25.11. 15:30)
  * Remove virtual inheritance (done 23.11.)
  * combinedMVA -> combiMVA (done 25.11. 15:30)
  * Understand why there is a difference in the MC diphoMVA between
    Globe Nov 5 and MIT Nov 20 (done 21.11. - mass error smearing)
  * Add diphoMVA check to DiphotonAndDijetMvaReaderTest. (done 24.11. 23:40)
  * Rename DijetMvaReader to DiphotonAndDijetMvaReader (done 24.11. 23:40)

Failed Jobs
-----------
   /home/veverka/cms/condor/run.sh runHgg2013Final_8TeV.C /home/cmsprod/catalog/t2mit filefi/029 r12c-dph-j22-v1 noskim 0050 hgg-2013Final8TeV /home/veverka/cms/hist
   /home/veverka/cms/condor/run.sh runHgg2013Final_8TeV.C /home/cmsprod/catalog/t2mit filefi/029 r12c-dph-j22-v1 noskim 0074 hgg-2013Final8TeV /home/veverka/cms/hist
   /home/veverka/cms/condor/run.sh runHgg2013Final_8TeV.C /home/cmsprod/catalog/t2mit filefi/029 r12c-dph-j22-v1 noskim 0084 hgg-2013Final8TeV /home/veverka/cms/hist
   /home/veverka/cms/condor/run.sh runHgg2013Final_8TeV.C /home/cmsprod/catalog/t2mit filefi/029 r12c-dph-j22-v1 noskim 0104 hgg-2013Final8TeV /home/veverka/cms/hist
   /home/veverka/cms/condor/run.sh runHgg2013Final_8TeV.C /home/cmsprod/catalog/t2mit filefi/029 r12c-dph-j22-v1 noskim 0134 hgg-2013Final8TeV /home/veverka/cms/hist

   /home/veverka/cms/condor/run.sh runHgg2013Final_8TeV.C /home/cmsprod/catalog/t2mit filefi/029 r12d-dph-j22-v1 noskim 0013 hgg-2013Final8TeV /home/veverka/cms/hist
   /home/veverka/cms/condor/run.sh runHgg2013Final_8TeV.C /home/cmsprod/catalog/t2mit filefi/029 r12d-dph-j22-v1 noskim 0042 hgg-2013Final8TeV /home/veverka/cms/hist
   /home/veverka/cms/condor/run.sh runHgg2013Final_8TeV.C /home/cmsprod/catalog/t2mit filefi/029 r12d-dph-j22-v1 noskim 0043 hgg-2013Final8TeV /home/veverka/cms/hist
   /home/veverka/cms/condor/run.sh runHgg2013Final_8TeV.C /home/cmsprod/catalog/t2mit filefi/029 r12d-dph-j22-v1 noskim 0066 hgg-2013Final8TeV /home/veverka/cms/hist
