# -*- coding: utf-8 -*-

import os

_base_path = '/home/veverka/cms/cmssw/031/CMSSW_5_3_10_patch1/src/MitHgg/macros'

scales_source_file_name = os.path.join(
    _base_path,
    '21Jun2012_7TeV-step2-invMass_SC_regrCorrSemiPar7TeVtrainV8_pho-'
        'loose-Et_25-noPF-HggRunEtaR9.dat'
    )

smearings_source_file_name = os.path.join(
    _base_path,
    '21Jun2012_7TeV-outProfile-scaleStep2smearing-'
        'Et_25-noPF-FitResult.config'
    )

source_macro_file_name = os.path.join(_base_path, 'runHgg2013Final_8TeV_test.C')

destination_macro_file_name = os.path.join(_base_path,
                                           'runHgg2013Final_8TeV_test_new.C')

## The order, matters here! See PhotonPairSelector::SetMCSmearFactors(...) in
## UserCode/MitPhysics/Mods/interface/PhotonPairSelector.h
mit_smearing_categories = '''
    _EBlowEta_hR9central
    _EBlowEta_hR9gap
    _EBlowEta_lR9
    _EBhighEta_hR9
    _EBhighEta_lR9
    _EElowEta_hR9
    _EElowEta_lR9
    _EEhighEta_hR9
    _EEhighEta_lR9
    '''.split()

## The order, matters here! See PhotonPairSelector::SetMCSmearFactors(...) in
## UserCode/MitPhysics/Mods/interface/PhotonPairSelector.h
mit_scale_categories = '''
    corr_EBlowEta_hR9central
    corr_EBlowEta_hR9gap
    corr_EBlowEta_lR9
    corr_EBhighEta_hR9
    corr_EBhighEta_lR9
    corr_EElowEta_hR9
    corr_EElowEta_lR9
    corr_EEhighEta_hR9
    corr_EEhighEta_lR9
    '''.split()

## This is the meat defining the mapping of the smearing
## numbers between ECALELF and MIT
mit_to_ecalelf_smearing_category_map = {
    '_EBlowEta_hR9central' : 'EBlowEtaGold' ,
    '_EBlowEta_hR9gap'     : 'EBlowEtaGold' ,
    '_EBlowEta_lR9'        : 'EBlowEtaBad'  ,
    '_EBhighEta_hR9'       : 'EBhighEtaGold',
    '_EBhighEta_lR9'       : 'EBhighEtaBad' ,
    '_EElowEta_hR9'        : 'EElowEtaGold' ,
    '_EElowEta_lR9'        : 'EElowEtaBad'  ,
    '_EEhighEta_hR9'       : 'EEhighEtaGold',
    '_EEhighEta_lR9'       : 'EEhighEtaBad' ,
    }

## ECALELF smearings are in %, MIT expects them in unity.
ecalelf_to_mit_smearing_value_transform = lambda x: str(0.01 * float(x))

## Names of the PhotonPairSelector methods that set the smearings.
smearing_setters = '''
    SetMCSmearFactors2012HCP
    SetMCSmearFactors2012HCPMVA
    '''.split()

## Names of the PhotonPairSelector methods that set the scales.
scale_setters = '''
    AddEnCorrPerRun2012HCP
    '''.split()
