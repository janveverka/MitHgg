#!/bin/env python
# -*- coding: utf-8 -*-

'''
Update MIT configuration macro with given scalings and smearings from Shervin.

Jan Veverka, MIT, jan.veverka@cern.ch, 13 October 2013.

USAGE: update-scales-and-smearings [my_cfg.py]
'''

import imp
import os
import pprint
import re
import sys

_default_config = os.path.join(os.environ['CMSSW_BASE'], 
                               'src/MitHgg/Tools/python',
                               'update_scales_and_smearings_default_cfg.py')

#_______________________________________________________________________________
def main():
    '''
    Main entry point to execution.
    '''
    print_welcome_message()
    cfg = parse_arguments()
    parse_and_update_corrections(cfg)
    print_termination_message()

#_______________________________________________________________________________
def print_welcome_message():
    print "Welcome to scales and smearings update!"

#_______________________________________________________________________________
def parse_arguments():
    if len(sys.argv) == 1:
        config_path = _default_config
    elif len(sys.argv) == 2:
        config_path = sys.argv[1]
    else:
        print_usage()
        raise RuntimeError, 'Illegal arguments: ' + str(sys.argv)
    cfg = imp.load_source(__name__ + '.cfg', config_path)
    return cfg

#_______________________________________________________________________________
def parse_and_update_corrections(cfg):
    smearing_for_category = parse_smearings(cfg)
    smearings = format_smearings(smearing_for_category, cfg)
    print 'Updating smearings to:'
    pprint.pprint(smearings)
    update_smearings(smearings, cfg)

    scales_for_run_range = parse_scales(cfg)
    scales = format_scales(scales_for_run_range, cfg)
    print 'Updating scales to:'
    pprint.pprint(scales)

#_______________________________________________________________________________
def print_termination_message():
    print "Exiting the ECALELF correction convertor."

#_______________________________________________________________________________
def print_usage():
    print 'USAGE: update-scales-and-smearings cfg.py'

#_______________________________________________________________________________
def parse_smearings(cfg):
    source_smearings = read_and_check_corrections(
        file_name = cfg.smearings_source_file_name,
        expected_number_of_lines = 8
        )
    smearing_for_category = {}
    for line in source_smearings:
        ## Example line:
        ## EBlowEtaBad          0.96  0.03
        category, smearing, error = line.split()
        smearing_for_category[category] = smearing
    return smearing_for_category

#_______________________________________________________________________________
def format_smearings(smearing_for_category, cfg):
    '''
    Returns the arguments of PhotonPairSelector::SetMCSmearFactors as a string
    given cat->dsigma dictionary smearing_for_category, which maps the ECALELF
    category name (cat) and ECALELF smearing as a string in % (dsigma).
    Example of a smearing_for_category dictionary:
    {'EBhighEtaBad': '1.85',
     'EBhighEtaGold': '1.01',
     'EBlowEtaBad': '0.96',
     'EBlowEtaGold': '0.68',
     'EEhighEtaBad': '1.83',
     'EEhighEtaGold': '2.01',
     'EElowEtaBad': '1.85',
     'EElowEtaGold': '1.58'}
    Example of a formatted output:
    '0.0068, 0.0068, 0.0096, 0.0101, 0.0185, 0.0158, 0.0185, 0.0201, 0.0183'
    '''
    mit_smearings = []
    ecalelf_from_mit_category = cfg.ecalelf_from_mit_smearing_category
    for mit_category in cfg.mit_smearing_categories:
        ecalelf_category = ecalelf_from_mit_category[mit_category]
        ecalelf_smearing = smearing_for_category[ecalelf_category]
        ## Transform the ECALELF value convention to the MIT one,
        ## for instance from values in % to actual values, i.e.
        ## from 0.96 to 0.0096.
        mit_smearing = cfg.mit_from_ecalelf_smearing(ecalelf_smearing)
        mit_smearings.append(mit_smearing)
    return ', '.join(mit_smearings)

#_______________________________________________________________________________
def update_smearings(smearings, cfg):
    source_macro = read_file(cfg.source_macro_file_name)
    pattern = re.compile(r'((%s)\s*)\(.*\)' % '|'.join(cfg.smearing_setters))
    updated_macro = pattern.sub(r'\1(%s)' % smearings, source_macro)
    write_macro_to_file(updated_macro, cfg.destination_macro_file_name)

#_______________________________________________________________________________
def parse_scales(cfg):
    source_scales = read_and_check_corrections(
        file_name = cfg.scales_source_file_name,
        expected_number_of_lines = 64
        )
    scales_for_run_range = {}
    for line in source_scales:
        ## Example line:
        ## EB-absEta_0_1-bad    runNumber       160431  165547  0.9961  0.0002
        category, unused_text, first_run, last_run, scale, error = line.split()
        run_range = (first_run, last_run)
        if run_range in scales_for_run_range:
            scales_for_run_range[run_range][category] = scale
        else:
            scales_for_run_range[run_range] = {category: scale}
    return scales_for_run_range

#_______________________________________________________________________________
def format_scales(scales_for_run_range, cfg):
    '''
    Return a list of strings each being a set of argumets of the a scale setter
    such as the PhotonPairSelector::AddEnCorrPerRun2012HCP(..) given
    runs->(cat->corr) dictionary of dictionaries scales_for_run_range, which
    maps a run runge tuple runs = (first, last) to a dictionary, which maps
    the ECALELF category name (cat) to ECALELF scale corrections.
    Example of a scales_for_run_range dictionary:
    {('160431', '165547'): {'EB-absEta_0_1-bad': '0.9961',
                            'EB-absEta_0_1-gold': '0.9929',
                            'EB-absEta_1_1.4442-bad': '0.9981',
                            'EB-absEta_1_1.4442-gold': '0.9883',
                            'EE-absEta_1.566_2-bad': '0.9961',
                            'EE-absEta_1.566_2-gold': '0.9944',
                            'EE-absEta_2_2.5-bad': '0.9987',
                            'EE-absEta_2_2.5-gold': '0.9941'},
     ('165548', '167042'): {'EB-absEta_0_1-bad': '0.9971',
                            'EB-absEta_0_1-gold': '0.9939',
                            ...}
     ('167043', '172400'): {'EB-absEta_0_1-bad': '0.9974',
                            ...},
     ...}
     Example of the ouput list:
     ['160431, 165547, 0.9929, 0.9929, 0.9961, 0.9883, 0.9981, 0.9944, 0.9883, 0.9941, 0.9987',
      '165548, 167042, 0.9939, 0.9939, 0.9971, 0.9876, 0.9973, 0.9956, 0.9876, 0.9937, 0.9982',
      '167043, 172400, 0.9942, 0.9942, 0.9974, 0.9883, 0.9981, 0.9956, 0.9883, 0.9948, 0.9993',
      ...]
    '''
    mit_scales = []
    run_ranges = scales_for_run_range.keys()
    ## Sort the run ranges in an ascending order.
    run_ranges.sort(key = lambda x: x[0])
    ecalelf_from_mit_category = cfg.ecalelf_from_mit_scale_category
    for run_range in run_ranges:
        scales_for_category = scales_for_run_range[run_range]
        mit_scales_per_run_range = []
        for mit_category in cfg.mit_scale_categories:
            ecalelf_category = ecalelf_from_mit_category[mit_category]
            ecalelf_scale = scales_for_category[ecalelf_category]
            mit_scale = cfg.mit_from_ecalelf_scale(ecalelf_scale)
            mit_scales_per_run_range.append(mit_scale)
        args = list(run_range) + mit_scales_per_run_range
        mit_scales.append(', '.join(args))
    return mit_scales

#_______________________________________________________________________________
def read_and_check_corrections(file_name, expected_number_of_lines):
    source_corrections = read_file(file_name).split('\n')
    prune_empty_lines(source_corrections)
    if (len(source_corrections) != expected_number_of_lines):
        raise RuntimeError, ("Illegal format of `%s'!" % file_name)
    return source_corrections

#_______________________________________________________________________________
def read_file(file_name):
    with open(file_name) as source:
        text = source.read()
    return text

#_______________________________________________________________________________
def write_macro_to_file(macro, file_name):
    with open(file_name, 'w') as destination:
        destination.write(macro)
        destination.close()

#_______________________________________________________________________________
def prune_empty_lines(lines):
    empty_lines = []
    for line_number, line in enumerate(lines):
        if len(line.split()) == 0:
            empty_lines.append(line_number)
    empty_lines.sort(reverse=True)
    for line_number in empty_lines:
        del lines[line_number]

#_______________________________________________________________________________
if __name__ == '__main__':
    main()
    import user
