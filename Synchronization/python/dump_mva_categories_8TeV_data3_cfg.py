# -*- coding: utf-8 -*-
'''
Configuration for the mva category dump for the 8TeV data trees
produced by Mingming on December 4.
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
    /home/mingyang/cms/hist/hgg-2013Final8TeV/filefi/032/r12a-pho-j22-v1
    /home/mingyang/cms/hist/hgg-2013Final8TeV/filefi/029/r12b-dph-j22-v1
    /home/mingyang/cms/hist/hgg-2013Final8TeV/filefi/029/r12c-dph-j22-v1
    /home/mingyang/cms/hist/hgg-2013Final8TeV/filefi/029/r12d-dph-j22-v1
    '''.split()

_unmerged_files = []
for _directory in _input_bases:
    _files_in_dir = glob.glob(join(_directory, '*.root'))
    _files_in_dir.sort()
    _unmerged_files += _files_in_dir
    _actual = len(_files_in_dir)
    ## Extract the expected number of files from the last filename, example:
    ## 'hgg-2013Final8TeV_r12d-dph-j22-v1_noskim_0183.root'.split('_')
    ## ['hgg-2013Final8TeV', 'r12d-dph-j22-v1', 'noskim', '0183.root'][-1]
    ## '0183.root'[:4]
    ## '0183'.lstrip('0')
    ## int('183')
    ## 183 + 1
    ## 184
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
        ] + _unmerged_files,
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
    diphotonMvaWeights = cms.string(
        join(_cmssw_base, 'src/MitPhysics/data/',
             'HggBambu_SMDipho_Oct01_redqcdweightallsigevenbkg_BDTG.' +
                 'weights.xml')
        ),
    dijetMvaWeights    = cms.string(
        join(_cmssw_base, 'src/MitHgg/PhotonTree/data',
             'TMVA_dijet_sherpa_scalewt50_2evenb_powheg200_maxdPhi_oct9_' +
                 'Gradient.weights.xml')
        ),
    combinedMvaWeights = cms.string(
        join(_cmssw_base, 'src/MitHgg/PhotonTree/data',
             'TMVA_vbf_dijet_dipho_evenbkg_scaledwt50_maxdPhi_Gradient.' +
                 'weights.xml')
        ),
    diphotonMvaUseSmearedMassError = cms.bool(True),
    dijetMvaMaxDPhi = cms.double(2.916),
    ## 8TeV categories by Josh from 21 November 2013
    diphotonBdtBoundaries = cms.vdouble(0.76, 0.36, 0.00, -0.42, -0.78),
    dijetBdtBoundaries    = cms.vdouble(),
    combinedBdtBoundaries = cms.vdouble(0.94, 0.82, 0.14),
    ) ## process.dump


#_______________________________________________________________________________
if __name__ == '__main__':
    import user