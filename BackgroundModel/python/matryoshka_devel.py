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
import ROOT
import FWLite.Tools.roofit as roo
import FWLite.Tools.canvases as canvases

plots = []

# Let's first try the probabilty transform for a binned dataset.
w = ROOT.RooWorkspace('w')


gauss = w.factory('Gaussian::gauss(xvar[-5,5],mean[0,-5,5],sigma[1,0.1,10])')
xvar = w.var('xvar')
observables = ROOT.RooArgSet(xvar)
data = gauss.generateBinned(observables, 1000)

scan_bins = 1000
scan_order = 2
inverse_precision = 1e-4
scan_pars = roo.ScanParameters(scan_bins, scan_order)
gauss_cdf = gauss.createCdf(observables, scan_pars)
gauss_cdf_inverse = ROOT.RooNumInverse(
	'gauss_cdf_inverse',
	'Inverse of ' + gauss_cdf.GetTitle(),
	xvar, gauss_cdf, inverse_precision
	)

plot = xvar.frame()
plots.append(plot)
data.plotOn(plot)
gauss.plotOn(plot)
gauss.fitTo(data)
gauss.plotOn(plot, roo.LineColor(ROOT.kRed))

canvases.next('fit')
plot.Draw()

plot = xvar.frame()
plots.append(plot)
gauss.plotOn(plot)
gauss_cdf.plotOn(plot, roo.LineColor(ROOT.kRed))
#gauss_cdf_inverse.plotOn(plot, roo.Range(0,1), roo.LineStyle(ROOT.kDashed))
canvases.next('cdf')
plot.Draw()

yvar = w.factory('yvar[0.5, 0, 1]')

canvases.update()


