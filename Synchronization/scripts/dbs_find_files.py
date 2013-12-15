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
summary = []
for line in map(str.strip, event_data.split('\n')):
    dataset, run, lumi = line.split()
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

