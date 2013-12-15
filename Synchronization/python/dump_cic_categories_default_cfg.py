# -*- coding: utf-8 -*-
import os
import FWCore.ParameterSet.Config as cms
from os.path import join

_cmssw_base = os.environ['CMSSW_BASE']

#_______________________________________________________________________________
process = cms.Process('DUMP')

#_______________________________________________________________________________
process.inputs = cms.PSet(
    fileNames = cms.vstring(
        join('/home/mingyang/cms/hist/hgg-2013Final8TeV/merged',
             'hgg-2013Final8TeV_s12-h120gg-vh-v7n_noskim.root')
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
    maxEventsToProces = cms.untracked.int32(10),
    beamEnergy = cms.string('8TeV')    
    ) ## process.dump
