# -*- coding: utf-8 -*-
import os
import FWCore.ParameterSet.Config as cms
from os.path import join
from MitHgg.Synchronization.unmerged_files import get_unmerged_files

_cmssw_base = os.environ['CMSSW_BASE']
_input_base = '/home/mingyang/cms/hist/hgg-2013Final7TeV/filefi/031/'

#_______________________________________________________________________________
process = cms.Process('DUMP')

#_______________________________________________________________________________
process.inputs = cms.PSet(
    fileNames = cms.vstring() + get_unmerged_files(
        [join(_input_base, 'r11%s-pho-j21-v1' % run) for run in 'ab']
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
    beamEnergy = cms.string('7TeV')    
    ) ## process.dump
