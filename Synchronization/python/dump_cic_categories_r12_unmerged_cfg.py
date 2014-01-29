# -*- coding: utf-8 -*-
import os
import FWCore.ParameterSet.Config as cms
from os.path import join
from MitHgg.Synchronization.unmerged_files import get_unmerged_files

_cmssw_base = os.environ['CMSSW_BASE']
_input_base = '/home/mingyang/cms/hist/hgg-2013Final8TeV/t2mit/filefi'

#_______________________________________________________________________________
process = cms.Process('DUMP')

#_______________________________________________________________________________
process.inputs = cms.PSet(
    fileNames = cms.vstring() + get_unmerged_files(
        [join(_input_base, directory) for directory in '''
            032/r12a-pho-j22-v1 
            029/r12b-dph-j22-v1 
            029/r12c-dph-j22-v1 
            029/r12d-dph-j22-v1
            '''.split()]
         ),
    treeName  = cms.string(
        '/RunLumiSelectionMod/MCProcessSelectionMod/' +
        'HLTModP/GoodPVFilterMod/PhotonMvaMod/JetPub/JetCorrectionMod/' +
        'SeparatePileUpMod/ElectronIDMod/ElectronIDMod/MuonIDMod/MuonIDMod/' +
        'PhotonPairSelectorCiC/PhotonTreeWriterCiC/' +
        'hPhotonTree'
        ),
    ) ## process.inputs

#_______________________________________________________________________________
process.dump = cms.PSet(
    maxEntriesToProcess = cms.untracked.int32(-1),
    beamEnergy = cms.string('8TeV')    
    ) ## process.dump
