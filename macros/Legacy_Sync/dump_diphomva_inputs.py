# -*- coding: utf-8 -*-
import os
import pprint
import ROOT

output_file_name = 'dump.txt'
temp_file_name = 'dump_tmp.txt'

run_lumi_event_tuples = [
    # (200519, 138, 69289),
    # (194533, 150, 75715),
    # (206859, 178, 89503),
    # (200519, 21, 10412),
    (194533, 26, 12737),
    # (200519, 139, 70088),
    # (194533, 77, 38564),  
    ]

variables = '''
    masserrsmeared/mass        
    masserrsmearedwrongvtx/mass
    vtxprob                    
    ph1.pt/mass                
    ph2.pt/mass                
    ph1.eta                    
    ph2.eta                    
    TMath::Cos(ph1.phi-ph2.phi)
    ph1.idmva 
    ph2.idmva                  
    '''.split()


input_file_name = ('/home/mingyang/cms/hist/hgg-2013Final8TeV/merged/'
    'hgg-2013Final8TeV_s12-h120gg-vh-v7n_noskim.root')

#_______________________________________________________________________________
def main():
    global tree
    input_file = ROOT.TFile.Open(input_file_name)
    directory = input_file.FindObjectAny('PhotonTreeWriterPreselNoSmear')
    tree = directory.Get('hPhotonTree')
    tree.GetPlayer().SetScanRedirect(True)
    tree.GetPlayer().SetScanFileName(temp_file_name)
    with open(output_file_name, 'w') as output_file:
        output_file.write('=== MIT Diphoton MVA Inputs\n\n ===')
        output_file.close()
    for event in run_lumi_event_tuples[:]:
        dump_event(event)
    #os.remove(temp_file_name)

#_______________________________________________________________________________
def dump_event(run_lumi_event_tuple):
    run, lumi, event = run_lumi_event_tuple
    options = 'precision=15 colsize=40' # passes %40.15g to fprintf by default
    selection = 'run == %d && evt == %d' % (run, event)
    print 'Dumping', selection
    expression = ':'.join(variables)
    tree.Scan(expression, selection, options)
    columns = []
    with open(temp_file_name, 'r') as scan_output:
        for line in scan_output:
            if '***' in line or not line.strip():
                continue
            columns.append([t.strip() for 
                            t in line.strip('*').split(' * ')][1:])
    #pprint.pprint(columns)
    name_width, value_width = [max([len(t) for t in c]) for c in columns]
    lines = []
    for name, value in zip(*columns):
        lines.append(name.strip().strip(' *').ljust(name_width) + '   ' + 
                     value.strip().strip(' *').rjust(value_width))
    with open(output_file_name, 'a') as dump_output:
        dump_output.write('>> run %d, lumi %d, event %d\n' % 
                          (run, lumi, event))
        for line in lines:
            dump_output.write(line + '\n')
        dump_output.write('\n')

#_______________________________________________________________________________
if __name__ == '__main__':
    main()
    import user
