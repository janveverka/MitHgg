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
        # join(_cmssw_base, 'src/MitHgg/macros',
        #     'hgg_r12c-dph-j22-v1_noskim_0000.root')
        '/home/mingyang/cms/hist/hgg-2013Final8TeV_CIC/merged/hgg-2013Final8TeV_r12a-pho-j22-v1_noskim.root'
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
    beamEnergy = cms.string('8TeV'),
    eventsToSkip = cms.string(
        join(_cmssw_base, 'src/MitHgg/Tools/data',
             'HCALLaser2012AllDatasets_and_ECALLaserFiltered.txt.gz')
        )
    ) ## process.dump
