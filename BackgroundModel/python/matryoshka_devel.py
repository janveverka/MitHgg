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
import array
import ROOT
import FWLite.Tools.roofit as roo
import FWLite.Tools.canvases as canvases

plots = []

## Let's first try the probabilty transform for a binned dataset.
## The original variable is xvar, the transformed une is uvar (for uniform).

#===============================================================================
class MatroyshkaTest:
    #___________________________________________________________________________
    def __init__(self, nevents=1000):
        self.plots = []
        self.w = w = ROOT.RooWorkspace('w')
        self.gauss = w.factory('''Gaussian::gauss(xvar[-5,5],mean[0,-5,5],
                                                  sigma[1,0.1,10])''')
        self.xvar = w.var('xvar')
        self.xset = ROOT.RooArgSet(self.xvar)
        self.data = self.gauss.generateBinned(self.xset, nevents)
        self._init_cdf()
        self._init_inverse_cdf()
        self._init_uvar()
        self._init_titles()

    #___________________________________________________________________________
    def _init_cdf(self, scan_bins = 1000, scan_order = 2):
        scan_pars = roo.ScanParameters(scan_bins, scan_order)
        self.gauss_cdf = self.gauss.createCdf(self.xset, scan_pars)

    #___________________________________________________________________________
    def _init_inverse_cdf(self, inverse_precision = 1e-4):
        self.gauss_cdf_inverse = ROOT.RooNumInverse(
            'gauss_cdf_inverse',
            'Inverse of ' + self.gauss_cdf.GetTitle(),
            self.xvar, self.gauss_cdf, inverse_precision
            )

    #___________________________________________________________________________
    def _init_uvar(self):
        self.uvar = self.w.factory('uvar[0.5, 0, 1]')
        xbinning = self.xvar.getBinning()
        nbins = xbinning.numBins()
        uedges = []
        for iedge in range(nbins + 1):
            self.xvar.setVal(xbinning.array()[iedge])
            uedges.append(self.gauss_cdf.getVal())
        uboundaries = array.array('d', uedges)
        ubinning = ROOT.RooBinning(nbins, uboundaries)
        self.uvar.setBinning(ubinning)

    #___________________________________________________________________________
    def _init_titles(self):
        self.xvar.SetTitle('x')
        self.uvar.SetTitle('u')
    #___________________________________________________________________________
    def run(self):
        self.fit_toy()

    #___________________________________________________________________________
    def fit_toy(self):
        plot = self.xvar.frame(roo.Title('Gaussian Toy'))
        self.plots.append(plot)
        self.data.plotOn(plot, roo.Name('data'))
        self.gauss.plotOn(plot, roo.Name('model'))
        self.gauss.fitTo(self.data)
        self.gauss.plotOn(plot, roo.LineColor(ROOT.kRed), roo.Name('fit'))
        canvases.next('GaussianToy')
        plot.Draw()
        legend = ROOT.TLegend(0.7, 0.95, 0.95, 0.8)
        legend.SetFillColor(0)
        legend.SetShadowColor(0)
        legend.SetBorderSize(0)
        legend.AddEntry(plot.findObject('model'), 'Parent Model', 'L' )
        legend.AddEntry(plot.findObject('data' ), 'Data'        , 'PE')
        legend.AddEntry(plot.findObject('fit'  ), 'Fitted Model', 'L' )
        legend.Draw()
        plot.legend = legend
        ## FIXME: Add legend a la http://root.cern.ch/phpBB3/viewtopic.php?p=31694
# End of MatroyshkaTest

#===============================================================================
def main():
    global mtest
    mtest = MatroyshkaTest()
    mtest.run()
    canvases.update()
## End of main


#plot = xvar.frame()
#plots.append(plot)
#gauss.plotOn(plot)
#gauss_cdf.plotOn(plot, roo.LineColor(ROOT.kRed))
##gauss_cdf_inverse.plotOn(plot, roo.Range(0,1), roo.LineStyle(ROOT.kDashed))
#canvases.next('cdf')
#plot.Draw()

#canvases.update()


#===============================================================================
if __name__ == '__main__':
    main()
