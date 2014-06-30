#!/usr/bin/env python
# -*- coding: utf-8 -*-
import glob
import os
import sys
## Set batch mode for root
sys.argv.append('-b')

import ROOT

#_______________________________________________________________________________
def get_latest_root_file():
    filenames =  glob.glob('hgg_*_noskim_0000.root')
    mtimes = [os.stat(f)[-2] for f in filenames]
    return max(zip(mtimes, filenames))[1]
## get_latest_root_file

#filename = get_latest_root_file()
#filename = 'hgg-tthtest_s12-h125gg-tt-v7a_noskim_0000.root'
#filename = 'hgg_s12-h120gg-vh-v7n_noskim_0000.root'
#filename = 'hgg_r12b-dph-j22-v1_noskim_0058.root'
#filename =  'hgg_r12c-dph-j22-v1_noskim_0000.root'
#filename = '/home/mingyang/cms/hist/hgg-2013Final8TeV_2/merged/hgg-2013Final8TeV_s12-h120gg-vh-v7n_noskim.root'
#filename = '/home/veverka/cms/hist/hgg-2013Final8TeV/merged/hgg-2013Final8TeV_s12-h120gg-vh-v7n_noskim.root'
#filename = '/home/veverka/cms/hist/hgg-2013Final8TeV/filefi/030/s12-h120gg-vh-v7n/hgg-2013Final8TeV_s12-h120gg-vh-v7n_noskim_0004.root'
filename = '/home/mingyang/cms/hist/hgg-2013Final8TeV_CIC/merged/hgg-2013Final8TeV_r12a-pho-j22-v1_noskim.root'
filename = '/home/mingyang/cms/hist/hgg-2013Final8TeV_CIC/merged/hgg-2013Final8TeV_r12a-pho-j22-v1_noskim.root'

print filename
source = ROOT.TFile(filename)
tree = source.FindObjectAny('PhotonTreeWriterCiC').Get('hPhotonTree')
#tree.Scan('run:lumi:evt:mass', 'evt==26819112 && run==193336 && lumi==33', '', 20)
tree.Scan('run:lumi:evt:mass:VHHadTag', 'run==193336 && lumi==33')

#______________________________________________________________________________
if __name__ == '__main__':
    import user

