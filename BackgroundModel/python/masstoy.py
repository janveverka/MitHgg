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
import PyCintex
import FWLite.Tools.roofit as roo
import FWLite.Tools.canvases as canvases
from FWLite.Tools.roochi2calculator import RooChi2Calculator

ROOT.gSystem.Load('libHiggsAnalysisCombinedLimit.so')
#ROOT.gSystem.Load('libMitHggBackgroundModel.so')
#PyCintex.Cintex.Enable()

cat_name = 'bdt3'
fname = ('/afs/cern.ch/work/v/veverka/cms/2013Final/FINAL/8TeV/unblinding/'
         'bambu_mva_bernfast_envelop/databkg/bkgdatawithfit.root')
wsname = 'multipdf'
data_name = 'data_hgg_8TeV_2013final_' + cat_name
mass_name = 'CMS_hgg_mass'
model_name = 'bkgpdf_pow_0_hgg_8TeV_2013final_' + cat_name
model_label = 'pow1'

myfile = ROOT.TFile.Open(fname)
ws = myfile.Get(wsname)
data = ws.data(data_name)
model = ws.pdf(model_name)
mass = ws.var(mass_name)
observables = ROOT.RooArgSet(mass)
parameters = model.getParameters(observables)
name = cat_name + '_' + model_label
title = cat_name + ' ' + model_label

#===============================================================================
class GoodnessOfFitTest:
    '''
    Tests the goodness of fit for a given category and model.
    '''
    def __init__(self, cat_name, model_name, model_label):
        self.cat_name = cat_name
        self.model_name = model_name
        self.model_label = model_label
        self.name = cat_name + '_' + model_label
        self.title = cat_name + ' ' + model_label
## End of GofTest


#===============================================================================
class PlotFactory:
    '''
    Collects RooFit plots related to the same variable and automates
    some of the common steps.
    '''
    def __init__(self, xvar):
        self.xvar = xvar
        self.plots = []
    def next(self, *args):
        plot = self.xvar.frame(*args)
        self.plots.append(plot)
        return plot
    def draw(self):
        for plot in self.plots:
            canvas = canvases.next(plot.GetName())
            canvas.SetTitle(plot.GetTitle())
            canvas.SetGrid()
            plot.Draw()
            canvas.Update()
## End of PlotFactory

mplots = PlotFactory(mass)
plot = mplots.next(roo.Title(name + '_mass'))
data.plotOn(plot)
model.plotOn(plot)

calculator = RooChi2Calculator(plot)
reduced_chi2 = calculator.chiSquare(parameters.getSize())

hresid = calculator.residHist()
hpull = calculator.pullHist()
ndof = hresid.GetN() - parameters.getSize()
chi2 = reduced_chi2 * ndof
pvalue = ROOT.TMath.Prob(chi2, ndof)

print 'Chi2 / NDOF:', chi2, '/', ndof
print 'p-value:', pvalue

hresid.Fit('pol1', '', '', 100, 120)
fit = hresid.GetFunction('pol1')
fit.SetLineColor(ROOT.kRed)

plot = mplots.next(roo.Name(name + '_residuals'),
                   roo.Title(title + ' Fit Residuals'))
plot.addPlotable(hresid, 'P')

plot = mplots.next(roo.Name(name + '_pulls'), roo.Title(title + '_pulls'))
plot.addPlotable(hpull, 'P')

mplots.draw()
