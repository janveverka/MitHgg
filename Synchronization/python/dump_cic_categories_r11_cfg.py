# -*- coding: utf-8 -*-
import os
import FWCore.ParameterSet.Config as cms
from os.path import join

_cmssw_base = os.environ['CMSSW_BASE']

#_______________________________________________________________________________
process = cms.Process('DUMP')

#_______________________________________________________________________________
process.inputs = cms.PSet(
    fileNames = cms.vstring() + [
        join('/home/mingyang/cms/hist/hgg-2013Final7TeV/merged', 
             'hgg-2013Final7TeV_r11%s-pho-j21-v1_noskim.root' % r) 
        for r in 'ab'
        ],
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
