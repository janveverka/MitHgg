#!/bin/env python
# -*- coding: utf-8 -*-
import os
import sys
sys.argv.append('-b')
import ROOT

ROOT.gROOT.ProcessLine(".L dumpCats.C+")

debug           = False
dump_data       = False
use_mingmings   = True
smear_mc_energy = False
smear_mass_err  = True

mc_to_process = '''
    s12-h120gg-vh-v7n
    '''.split()

data_to_process = '''
    r12a-pho-j22-v1
    r12b-dph-j22-v1
    r12c-dph-j22-v1
    r12d-dph-j22-v1
    '''.split()[:]

if dump_data:
    to_process = data_to_process
else:
    to_process = mc_to_process

if use_mingmings:
    base_dir = '/home/mingyang/cms/hist/hgg-2013Final8TeV/merged/'
else:
    base_dir = '/home/veverka/cms/hist/hgg-2013Final8TeV/merged/'

if smear_mc_energy:
    dirname = 'PhotonTreeWriterPresel'
else:
    dirname = 'PhotonTreeWriterPreselNoSmear'

for dataset in to_process:
    base_filename = 'hgg-2013Final8TeV_%s_noskim.root' % dataset
    filename = os.path.join(base_dir, base_filename)
    print 'Processing', filename
    ROOT.dumpCats(debug, filename, dirname, smear_mass_err)
