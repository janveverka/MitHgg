# -*- coding: utf-8 -*-

import os

import MitHgg.Tools.update_scales_and_smearings_default_cfg as default_cfg

#_______________________________________________________________________________
## INPUT AND OUTPUT FILES
_cmssw_base = os.environ['CMSSW_BASE']
_corrections_base = os.path.join(_cmssw_base, 'src/MitHgg/Tools/data')
_macros_base      = os.path.join(_cmssw_base, 'src/MitPhysics/macros/examples')

scales_source_file_name = os.path.join(
    _corrections_base,
    '21Jun2012_7TeV-step2-invMass_SC_regrCorrSemiPar7TeVtrainV8_pho-'
        'loose-Et_25-noPF-HggRunEtaR9.dat'
    )

smearings_source_file_name = os.path.join(
    _corrections_base,
    '21Jun2012_7TeV-outProfile-scaleStep2smearing-'
        'Et_25-noPF-FitResult.config'
    )

source_macro_file_name = os.path.join(_macros_base, 'runHgg2013Final_7TeV.C')

destination_macro_file_name = os.path.join(_macros_base,
                                           'runHgg2013Final_7TeV_new.C')

#_______________________________________________________________________________
## Keep everything else on default
scale_setters                      = default_cfg.scale_setters                     
mit_scale_categories               = default_cfg.mit_scale_categories              
ecalelf_from_mit_scale_category    = default_cfg.ecalelf_from_mit_scale_category   
mit_from_ecalelf_scale             = default_cfg.mit_from_ecalelf_scale            
smearing_setters                   = default_cfg.smearing_setters                  
mit_smearing_categories            = default_cfg.mit_smearing_categories           
ecalelf_from_mit_smearing_category = default_cfg.ecalelf_from_mit_smearing_category
mit_from_ecalelf_smearing          = default_cfg.mit_from_ecalelf_smearing         
