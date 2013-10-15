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
    print 'Updating smearings to:', smearings
    update_smearings(smearings, cfg)
    scales_for_run_range = parse_scales(cfg)
    print '>> scales_for_run_range:'
    pprint.pprint(scales_for_run_range)
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
    mit_smearings = []
    category_map = cfg.mit_to_ecalelf_smearing_category_map
    for mit_category in cfg.mit_smearing_categories:
        ecalelf_category = category_map[mit_category]
        ecalelf_smearing = smearing_for_category[ecalelf_category]
        ## Transform, for instance from values in % to actual values, i.e.
        ## from 0.96 to 0.0096.
        mit_smearing = cfg.ecalelf_to_mit_smearing_value_transform(
            ecalelf_smearing
            )
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
    pass
    
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
