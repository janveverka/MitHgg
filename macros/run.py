#!/bin/env python
# -*- coding: utf-8 -*-
# Wrapper around a run macro to ease passing the arguments
import sys
sys.argv.append('-b') ## Load ROOT in batch mode
import ROOT

runMacro   = 'runHgg2013Final_8TeV.C'
skim       = 'noskim'
catalogDir = '/home/cmsprod/catalog'
outputName = 'hgg'

#book       = 't2mit/filefi/030'
#dataset    = 's12-h120gg-vh-v7n'
#fileset    = '0000'
#nEvents    = 1000

#dataset    = 'r12a-pho-j22-v1'
#book       = 't2mit/filefi/032'
#fileset    = '0000'
#nEvents    = 1000

dataset    = 'r12c-dph-j22-v1'
book       = 't2mit/filefi/029'
fileset    = '0000'
nEvents    = -1

#dataset    = 'r11a-pho-j21-v1'
#book       = 't2mit/filefi/031'
#fileset    = '0000'
#nEvents    = -1


ROOT.gROOT.ProcessLine('.L ' + runMacro + '++' )
#ROOT.gROOT.ProcessLine('.L ' + runMacro + '+' )
run = getattr(ROOT, runMacro.split('.')[0])
run(fileset, skim, dataset, book, catalogDir, outputName, nEvents)

# events    user time
#       1    0m17.439s
#     200    0m17.110s
#   1,000    0m17.889s
#  10,000    0m19.240s
#  20,000    0m21.571s
#  50,000    0m28.642s
# 100,000    0m41.085s
# 500,000    1m37.247s
