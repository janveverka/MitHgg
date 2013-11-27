# -*- coding: utf-8 -*-
import os
import FWCore.ParameterSet.Config as cms
from os.path import join

_cmssw_base = os.environ['CMSSW_BASE']
_test_file_path =  join(_cmssw_base, 'src/MitHgg/PhotonTree/data',
                        'hgg-2013Final8TeV_s12-h120gg-vh-v7n_head10.root')

process = cms.Process('DUMP')

process.inputs = cms.PSet(
    fileNames = cms.vstring(_test_file_path),
    treeName  = cms.string('hPhotonTree'),
    ) ## process.inputs


process.dump = cms.PSet(
    combinedMvaWeights = cms.string(
        join(_cmssw_base, 'src/MitHgg/PhotonTree/data',
             'TMVA_vbf_dijet_dipho_evenbkg_scaledwt50_maxdPhi_Gradient.' +
                 'weights.xml')
        ),
    diphotonMvaWeights = cms.string(
        join(_cmssw_base, 'src/MitPhysics/data/',
             'HggBambu_SMDipho_Oct01_redqcdweightallsigevenbkg_BDTG.' +
                 'weights.xml')
        ),
    ) ## process.dump