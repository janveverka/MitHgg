#!/usr/bin/env python

import ROOT

outfile = ROOT.TFile.Open('merged_ascii_dump.root', 'RECREATE')
tree = ROOT.TTree('synch', 'merged synch tree')
branch_descriptor = ' run/I:lumi:event:cat[2]/F:diphoMVA[2]:ele1_eta[2]:ele1_pt[2]:jet1_eta[2]:jet1_phi[2]:jet1_pt[2]:jet2_eta[2]:jet2_phi[2]:jet2_pt[2]:mass[2]:met[2]:mu1_eta[2]:mu1_pt[2]:pho1_e[2]:pho1_eErr[2]:pho1_eta[2]:pho1_idMVA[2]:pho1_phi[2]:pho2_e[2]:pho2_eErr[2]:pho2_eta[2]:pho2_idMVA[2]:pho2_phi[2]:tth[2]:vhHad[2]:vhLep[2]:vhMet[2]'
tree.ReadFile('merged_ascii_dump.txt', branch_descriptor)
#tree.Write()
outfile.Write()
outfile.Close()

