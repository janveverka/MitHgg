# -*- coding: utf-8 -*-
import os
import FWCore.ParameterSet.Config as cms
from os.path import join

_cmssw_base = os.environ['CMSSW_BASE']
_test_file_path =  join(_cmssw_base, 'src/MitHgg/PhotonTree/data',
                        'hgg-2013Final8TeV_s12-h120gg-vh-v7n_head10.root')

#_______________________________________________________________________________
process = cms.Process('DUMP')

#_______________________________________________________________________________
process.inputs = cms.PSet(
    fileNames = cms.vstring(_test_file_path),
    treeName  = cms.string('hPhotonTree'),
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