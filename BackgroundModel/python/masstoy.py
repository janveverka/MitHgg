# -*- coding: utf-8 -*-
'''
Developing the Matryoshka PDF nesting.  Assume X ~ f and Y ~ g where U is from
[0,1].  The Matryoshka density h, defined as f nested in g is:

    h(x) = g(F(x)) * f(x)

This is easy to remember in terms of the CDF's:

    H(x) = G(F(x))

The resulting CDF is the original CDF folded with the correction CDF, hence
the matryoshka name.

Plan: use pow for f and chebyshev for g.
'''

## Let's fit the inclusive cat2 with a power law now and then transform the
## with the probability integral of the fit.
import ROOT
import FWLite.Tools.roofit as roo
import PyCintex
import FWLite.Tools.canvases as canvases

PyCintex.Cintex.Enable()
ROOT.gSystem.Load('libHiggsAnalysisCombinedLimit.so')
ROOT.gSystem.Load('libMitHggBackgroundModel.so')

cat_name = 'bdt4'
fname = ('/afs/cern.ch/work/v/veverka/cms/2013Final/FINAL/8TeV/unblinding/'
         'bambu_mva_bernfast_envelop/databkg/bkgdatawithfit.root')
wsname = 'multipdf'
data_name = 'data_hgg_8TeV_2013final_' + cat_name
mass_name = 'CMS_hgg_mass'
model_name = 'bkgpdf_pow_0_hgg_8TeV_2013final_' + cat_name

myfile = ROOT.TFile.Open(fname)
ws = myfile.Get(wsname)
data = ws.data(data_name)
model = ws.pdf(model_name)
mass = ws.var(mass_name)

plot = mass.frame(roo.Title(cat_name))
data.plotOn(plot)
model.plotOn(plot)
canvases.next(cat_name)
plot.Draw()
canvases.update()
