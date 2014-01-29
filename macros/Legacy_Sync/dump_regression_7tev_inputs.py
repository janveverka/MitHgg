# -*- coding: utf-8 -*-
import os
import pprint
import ROOT

input_file_name = '/home/mingyang/cms/hist/hgg-2013Final7TeV/merged/hgg-2013Final7TeV_s11-h120gg-vh-lv3_noskim.root'

output_file_name = 'dump.txt'
temp_file_name = 'dump_tmp.txt'

events = '''1:115:80866
            1:98:68816
            1:153:107612
            1:83:57903
            1:74:51759
            1:104:73203
            1:87:60558
            1:65:45167
            1:110:76774
            1:75:52151
            1:146:102383
            1:58:40666
            1:57:39625
            1:84:59047
            1:75:52446'''.split()


run_lumi_event_tuples = [tuple(map(int, e.split(':'))) for e in events[:10]] 

control_variables = '''
    ph.e
    ph.eerr
    masserr
    masserrsmeared
    '''.split()

barrel_variables = '''
    ph.scrawe
    ph.sceta
    ph.scphi
    ph.r9
    ph.scetawidth
    ph.scphiwidth
    ph.scnclusters
    ph.hoveretower
    rho
    nVtx
    ph.etaseed-ph.sceta
    atan2(sin(ph.phiseed-ph.scphi),cos(ph.phiseed-ph.scphi))
    ph.eseed/ph.scrawe
    ph.e3x3seed/ph.e5x5seed
    ph.sigietaietaseed
    ph.sigiphiphiseed
    ph.covietaiphiseed
    ph.emaxseed/ph.e5x5seed
    ph.e2ndseed/ph.e5x5seed
    ph.etopseed/ph.e5x5seed
    ph.ebottomseed/ph.e5x5seed
    ph.eleftseed/ph.e5x5seed
    ph.erightseed/ph.e5x5seed
    ph.e2x5maxseed/ph.e5x5seed
    ph.e2x5topseed/ph.e5x5seed
    ph.e2x5bottomseed/ph.e5x5seed
    ph.e2x5leftseed/ph.e5x5seed
    ph.e2x5rightseed/ph.e5x5seed
    ph.e5x5seed/ph.eseed  
    ph.ietaseed
    ph.iphiseed
    (ph.ietaseed-1*abs(ph.ietaseed)/ph.ietaseed)%5
    (ph.iphiseed-1)%2       
    (abs(ph.ietaseed)<=25)*((ph.ietaseed-1*abs(ph.ietaseed)/ph.ietaseed)%25)+(abs(ph.ietaseed)>25)*((ph.ietaseed-26*abs(ph.ietaseed)/ph.ietaseed)%20)
    (ph.iphiseed-1)%20 
    ph.etacryseed
    ph.phicryseed
    '''.split() + control_variables

endcap_variables = '''
    ph.scrawe
    ph.sceta
    ph.r9
    ph.scetawidth
    ph.scphiwidth
    ph.scnclusters
    ph.hoveretower
    rho
    nVtx
    ph.etaseed-ph.sceta
    atan2(sin(ph.phiseed-ph.scphi),cos(ph.phiseed-ph.scphi))
    ph.eseed/ph.scrawe
    ph.e3x3seed/ph.e5x5seed
    ph.sigietaietaseed
    ph.sigiphiphiseed
    ph.covietaiphiseed
    ph.emaxseed/ph.e5x5seed
    ph.e2ndseed/ph.e5x5seed
    ph.etopseed/ph.e5x5seed
    ph.ebottomseed/ph.e5x5seed
    ph.eleftseed/ph.e5x5seed
    ph.erightseed/ph.e5x5seed
    ph.e2x5maxseed/ph.e5x5seed
    ph.e2x5topseed/ph.e5x5seed
    ph.e2x5bottomseed/ph.e5x5seed
    ph.e2x5leftseed/ph.e5x5seed
    ph.e2x5rightseed/ph.e5x5seed
    ph.scpse/ph.scrawe
    '''.split() + control_variables
    
#_______________________________________________________________________________
def main():
    global tree
    input_file = ROOT.TFile.Open(input_file_name)
    directory = input_file.FindObjectAny('PhotonTreeWriterPreselNoSmear')
    tree = directory.Get('hPhotonTree')
    tree.GetPlayer().SetScanRedirect(True)
    tree.GetPlayer().SetScanFileName(temp_file_name)
    with open(output_file_name, 'w') as output_file:
        output_file.write('=== MIT Regression Inputs\n\n ===')
        output_file.close()
    for event in run_lumi_event_tuples[:]:
        dump_event(event)
    #os.remove(temp_file_name)

#_______________________________________________________________________________
def dump_event(run_lumi_event_tuple):
    run, lumi, event = run_lumi_event_tuple
    options = 'precision=15 colsize=80' # passes %80.15g to fprintf by default
    selection = 'run == %d && evt == %d' % (run, event)
    print 'Dumping', selection
    for ph in ['ph1.', 'ph2.']:
        if tree.Draw('run', selection + ' & %sisbarrel' % ph):
            expression = ':'.join(barrel_variables)
        else:
            expression = ':'.join(endcap_variables)
        tree.Scan(expression.replace('ph.', ph), selection, options)
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
                         value.strip().strip(' *').ljust(value_width))
        with open(output_file_name, 'a') as dump_output:
            dump_output.write('>> run %d, lumi %d, event %d, photon %s\n' % 
                              (run, lumi, event, ph))
            for line in lines:
                dump_output.write(line + '\n')
            dump_output.write('\n')

#_______________________________________________________________________________
if __name__ == '__main__':
    main()
    import user
