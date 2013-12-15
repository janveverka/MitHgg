UserCode_MitHgg_Synchronization
===============================

The dumper and the comparison scripts are at [1] and [2] and some notes are at [3].  You will need to checkout also [4] and [5] to compile the dumper [1]:

    cd $CMSSW_BASE/src
    cvs co -d MitHgg/PhotonTree UserCode/MitHgg/PhotonTree
    cvs co -d MitHgg/Synchronization UserCode/MitHgg/Syncronization
    cvs co -d FWLite/Tools UserCode/JanVeverka/FWLite/Tools
    scram b -j10

To produce the dump 'test.dat' for data, do:

    cd $CMSSW_BASE/src/MitHgg/Synchronization/python
    hgg-dump-mva-categories dump_mva_categories_7TeV_data_cfg.py > test.dat &

To produce the plots and a merged tree, do:

    cd $CMSSW_BASE/src
    SOURCE=UserCode_HiggsAnalysis_HggSynchronization.git
    DESTINATION=HiggsAnalysis/HggSynchronization
    git clone https://github.com/janveverka/$SOURCE $DESTINATION
    cd $DESTINATION/scripts
    ## Download dumps mit_cats*.txt and globe_massfact*.dat from 
    ## https://twiki.cern.ch/twiki/bin/view/CMS/HggSyncLegacy
    ## Make plots
    ./sync_cats_2013.py mit_cats*.txt globe_massfact*.dat
    ## Make the merged tree
    ./merge_dumps.py mit_cats*.txt globe_massfact*.dat

If you want to start hacking the dumper/categoriztion code, it can be useful to look at the unit tests in MitHgg/PhotonTree/test.  You can run them like this

    cd $CMSSW_BASE/src/MitHgg/PhotonTree
    scram b runtests

[1] http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/MitHgg/Synchronization/bin/dumpMvaCategories.cc
[2] https://github.com/janveverka/UserCode_HiggsAnalysis_HggSynchronization
[3] http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/MitHgg/PhotonTree/README.txt
[4] http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/MitHgg/PhotonTree
[5] http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/JanVeverka/FWLite/Tools/
