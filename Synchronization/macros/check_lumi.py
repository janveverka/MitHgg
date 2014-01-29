#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys
## Set batch mode for root
sys.argv.append('-b')

import ROOT
filename = '/home/mingyang/cms/hist/hgg-2013Final8TeV_CIC/merged/hgg-2013Final8TeV_r12a-pho-j22-v1_noskim.root'
run, lumi = 193336, 33

source = ROOT.TFile.Open(filename)
graph = source.FindObjectAny('ProcessedRunsLumis')

found = False
for i in range(graph.GetN()):
    irun = int(graph.GetX()[i])
    ilumi = int(graph.GetY()[i])
    if (irun, ilumi) == (run, lumi):
        print 'Matching graph point:', i
        found = True
        break

if found:
    print '%d:%d' % (run, lumi), 'was processed OK'
else:
    print '%d:%d' % (run, lumi), 'was NOT processed!'