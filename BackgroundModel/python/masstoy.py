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
import os
import ROOT
import PyCintex
import FWLite.Tools.roofit as roo
import FWLite.Tools.canvases as canvases
from FWLite.Tools.roochi2calculator import RooChi2Calculator

ROOT.gSystem.Load('libHiggsAnalysisCombinedLimit.so')
#ROOT.gSystem.Load('libMitHggBackgroundModel.so')
#PyCintex.Cintex.Enable()

cat_name = 'bdt4'
fbase = os.path.join(os.environ['CMSSW_BASE'],
                     'src/MitHgg/BackgroundModel/data')
fname = os.path.join(fbase, 'bkgdatawithfit.root')
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
    #___________________________________________________________________________
    def __init__(self, workspace, cat_name, model_name, model_label):
        '''
        Trivial ctor / initialization
        '''
        self.ws = workspace
        self.cat_name = cat_name
        self.model_name = model_name
        self.model_label = model_label
        self._init()

    #___________________________________________________________________________
    def _init(self):
        '''
        More complicated initialization.
        '''
        data_name = 'data_hgg_8TeV_2013final_' + self.cat_name
        model_name = self.model_name + '_' self.cat_name
        mass_name = 'CMS_hgg_mass'
        self.data = ws.data(data_name)
        self.model = ws.pdf(model_name)
        self.mass = ws.var(mass_name)
        self.observables = ROOT.RooArgSet(mass)
        self.parameters = model.getParameters(observables)
        self.name = cat_name + '_' + model_label
        self.title = cat_name + ' ' + model_label
        self.mplots = PlotFactory(mass)

    #___________________________________________________________________________
    def run(self):
        self.plot_mass()
        self.plot_residuals()
        self.plot_pulls()

    #___________________________________________________________________________
    def plot_mass(self):
        plot = self.mplots.next(roo.Title(self.name + '_mass'))
        self.data.plotOn(plot)
        self.model.plotOn(plot)

    #___________________________________________________________________________
    def plot_pulls(self):
        pass

    #___________________________________________________________________________
    def plot_residuals(self):
        pass
## End of GoodnessOfFitTest


#===============================================================================
class PlotFactory:
    '''
    Collects RooFit plots related to the same variable and automates
    some of the common steps.
    '''
    #___________________________________________________________________________
    def __init__(self, xvar):
        self.xvar = xvar
        self.plots = []
    #___________________________________________________________________________
    def next(self, *args):
        plot = self.xvar.frame(*args)
        self.plots.append(plot)
        return plot
    #___________________________________________________________________________
    def draw(self):
        for plot in self.plots:
            canvas = canvases.next(plot.GetName())
            canvas.SetTitle(plot.GetName())
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

colors = 'Red Green Blue Black'.split()
mranges = [(100, 120), (120, 140), (140, 160), (160, 180)]
for color, mrange in zip(colors, mranges):
    hresid.Fit('pol1', '+', '', *mrange)
    fit = hresid.GetFunction('pol1')
    fit.SetName('pol1_' + color.lower())
    fit.SetLineColor(getattr(ROOT, 'k' + color))

plot = mplots.next(roo.Name(name + '_residuals'),
                   roo.Title(title + ' Fit Residuals'))
plot.addPlotable(hresid, 'P')

#plot = mplots.next(roo.Name(name + '_pulls'), roo.Title(title + '_pulls'))
#plot.addPlotable(hpull, 'P')

mplots.draw()
