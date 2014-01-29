# -*- coding: utf-8 -*-
import os
import commands

## Lepton tag deficit
event_data = '''/DoublePhoton/Run2012B-22Jan2013-v1/AOD  194691     80
                /DoublePhoton/Run2012B-22Jan2013-v1/AOD  195397   1172
                /DoublePhoton/Run2012B-22Jan2013-v1/AOD  195950    188
                /DoublePhoton/Run2012C-22Jan2013-v2/AOD  198969    341
                /DoublePhoton/Run2012C-22Jan2013-v2/AOD  199008    294
                /DoublePhoton/Run2012C-22Jan2013-v2/AOD  199021   1393
                /DoublePhoton/Run2012C-22Jan2013-v2/AOD  199832    227
                /DoublePhoton/Run2012C-22Jan2013-v2/AOD  200229    506
                /DoublePhoton/Run2012C-22Jan2013-v2/AOD  201202     51
                /DoublePhoton/Run2012C-22Jan2013-v2/AOD  202504     84
                /DoublePhoton/Run2012D-22Jan2013-v1/AOD  204601    218
                /DoublePhoton/Run2012D-22Jan2013-v1/AOD  206331    106
                /DoublePhoton/Run2012D-22Jan2013-v1/AOD  206446    784
                /DoublePhoton/Run2012D-22Jan2013-v1/AOD  207231    402'''

## Lepton tag excess
event_data = '''/DoublePhoton/Run2012B-22Jan2013-v1/AOD 195774 311
                /DoublePhoton/Run2012C-22Jan2013-v2/AOD 199864  89
                /DoublePhoton/Run2012D-22Jan2013-v1/AOD 206401 127
                /DoublePhoton/Run2012D-22Jan2013-v1/AOD 206446 784
                /DoublePhoton/Run2012D-22Jan2013-v1/AOD 206940 248
                /DoublePhoton/Run2012D-22Jan2013-v1/AOD 207269 182'''
                
## Lepton tag excess w/ ghost electron, other excess lepton events
event_data = '''/DoublePhoton/Run2012B-22Jan2013-v1/AOD 196250  64
                /DoublePhoton/Run2012C-22Jan2013-v2/AOD 199864  89
                /DoublePhoton/Run2012C-22Jan2013-v2/AOD 200178  19
                /DoublePhoton/Run2012D-22Jan2013-v1/AOD 205236 158
                /DoublePhoton/Run2012D-22Jan2013-v1/AOD 206401 127
                /DoublePhoton/Run2012D-22Jan2013-v1/AOD 206542 371
                /DoublePhoton/Run2012D-22Jan2013-v1/AOD 207920 500'''

## TTH tag deficit in 7 TeV
#event_data = '''172952 1118
                #177730 1125'''

### TTH tag deficit in 8 TeV
#event_data = '''200190  163
                #205921  484'''

## 8 TeV CiC TTH lep killed in Globe by sublead photon iso
event_data = '199021 1393'

## 8 TeV CiC last push:
## VH lep 1 killed in Globe by moun and sublead photon isolations
## 2 Globe unique VH MET events
event_data = '''202087 532
                207477 543
                195774 576
                204113 350'''
## 8 TeV CiC last push #2:
## VH had event missing in MIT
event_data = '193336 33'


#_______________________________________________________________________________
def run_period_for_run_number(run_number):
    period_range_map = {
        'Run2011A': (160404, 173692),
        'Run2011B': (175833, 180252),
        'Run2012A': (190456, 193621),
        'Run2012B': (193834, 196531),
        'Run2012C': (198022, 203742),
        'Run2012D': (203777, 208686),
        }
    
    for run_period, run_range in period_range_map.items():
        first_run, last_run = run_range
        if first_run <= run_number and run_number <= last_run:
            break
    return run_period
## run_period_for_run_number

#_______________________________________________________________________________
def dataset_for_run_period(run_period):
    return {
        'Run2011A': '/Photon/Run2011A-21Jun2013-v1/AOD'      ,
        'Run2011B': '/Photon/Run2011B-21Jun2013-v1/AOD'      ,
        'Run2012A': '/Photon/Run2012A-22Jan2013-v1/AOD'      ,
        'Run2012B': '/DoublePhoton/Run2012B-22Jan2013-v1/AOD',
        'Run2012C': '/DoublePhoton/Run2012C-22Jan2013-v2/AOD',
        'Run2012D': '/DoublePhoton/Run2012D-22Jan2013-v1/AOD',
        }[run_period]
## dataset_for_run_period

#_______________________________________________________________________________
def dataset_for_run_number(run_number):
    run_period = run_period_for_run_number(run_number)
    return dataset_for_run_period(run_period)
## dataset_for_run_number

summary = []
for line in map(str.strip, event_data.split('\n')):
    run, lumi = line.split()
    dataset = dataset_for_run_number(int(run))
    command = 'dbsql "find file where dataset=%s and run=%s and lumi=%s"' % (
        dataset, run, lumi
        )
    print command
    status, output = commands.getstatusoutput(command)
    print output
    filename = ''
    for oline in output.split('\n'):
        if '.root' in oline:
            filename = os.path.basename(oline)
            break
    summary.append([run, lumi, filename])

print 'Summary'
print '-------'
for row in summary:
    print '%s   %4s   %s' % tuple(row)

