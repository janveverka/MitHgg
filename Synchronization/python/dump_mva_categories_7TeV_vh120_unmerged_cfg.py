# -*- coding: utf-8 -*-
'''
Configuration for the mva category dump for the 7TeV data trees
produced by Mingming on December 3.
The list of unmerged input files is globbed by the script given
the list of source directories.
'''
import glob
import os
import sys
import FWCore.ParameterSet.Config as cms
from os.path import join

_cmssw_base = os.environ['CMSSW_BASE']
_input_bases = '''
    /home/mingyang/cms/hist/hgg-2013Final7TeV/t2mit/filefi/031/s11-h120gg-vh-lv3
    '''.split()

_unmerged_files = []
for _directory in _input_bases:
    _files_in_dir = glob.glob(join(_directory, '*.root'))
    _files_in_dir.sort()
    _unmerged_files += _files_in_dir
    _actual = len(_files_in_dir)
    _expected = int(_files_in_dir[-1].split('_')[-1][:4].lstrip('0')) + 1
    print >> sys.stderr, '%s: added %3d files' % (_directory, _actual),
    print >> sys.stderr, 'of %3d expected ...' % _expected
    
#_______________________________________________________________________________
process = cms.Process('DUMP')

#_______________________________________________________________________________
process.inputs = cms.PSet(
    fileNames = cms.vstring() + [
        #join(_input_base, 'hgg-2013Final8TeV_r12%s-j22-v1_noskim.root' % r) 
        #for r in 'a-pho b-dph c-dph d-dph'.split()
        ] + _unmerged_files[:],
    treeName  = cms.string(
        '/RunLumiSelectionMod/MCProcessSelectionMod/' +
        'HLTModP/GoodPVFilterMod/PhotonMvaMod/JetPub/JetCorrectionMod/' +
        'SeparatePileUpMod/ElectronIDMod/ElectronIDMod/MuonIDMod/MuonIDMod/' +
        'PhotonPairSelectorPresel/PhotonTreeWriterPresel/' +
        'hPhotonTree'
        ),
    ) ## process.inputs

#_______________________________________________________________________________
process.dump = cms.PSet(
    beamEnergy = cms.string("7TeV"),
    ## source: /afs/cern.ch/user/b/bendavid/cmspublic/hggmvaOct29/\
    ## HggBambu_SMDipho_Oct29_rwgtptallsigevenbkg7TeV_BDTG.weights.xml
    diphotonMvaWeights = cms.string(
        join(_cmssw_base, 'src/MitHgg/PhotonTree/data',
             'HggBambu_SMDipho_Oct29_rwgtptallsigevenbkg7TeV_BDTG.weights.xml')
        ),
    ## source: https://github.com/h2gglobe/h2gglobe/blob/master/AnalysisScripts/\
    ## aux/TMVA_dijet_7TeV_evenb_v1_ptrwght_Gradient.weights.xml
    ## Accessed: Tue Dec  3 21:33:22 CET 2013
    dijetMvaWeights    = cms.string(
        join(_cmssw_base, 'src/MitHgg/PhotonTree/data',
             'TMVA_dijet_7TeV_evenb_v1_ptrwght_Gradient.weights.xml')
        ),
    ## source: https://github.com/h2gglobe/h2gglobe/blob/master/AnalysisScripts/\
    ## aux/TMVA_vbf_dijet_dipho_2evenb_maxdPhi_7TeV_v1_Gradient.weights.xml
    ## Accessed: Tue Dec  3 21:33:22 CET 2013
    combinedMvaWeights = cms.string(
        join(_cmssw_base, 'src/MitHgg/PhotonTree/data',
             'TMVA_vbf_dijet_dipho_2evenb_maxdPhi_7TeV_v1_Gradient.weights.xml')
        ),
    diphotonMvaUseSmearedMassError = cms.bool(True),
    dijetMvaMaxDPhi = cms.double(2.9416),
    ## 7TeV diphoton categories by Josh from 21 November 2013
    diphotonBdtBoundaries = cms.vdouble(0.93, 0.85, 0.70, 0.19),
    dijetBdtBoundaries    = cms.vdouble(),
    ## Updated 7TeV combined BDT categories by Josh from 3 December 2013
    combinedBdtBoundaries = cms.vdouble(0.995, 0.917),
    ) ## process.dump


#_______________________________________________________________________________
if __name__ == '__main__':
    import user