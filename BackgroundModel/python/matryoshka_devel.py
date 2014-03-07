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
Let's first try the probabilty transform for a binned dataset.
'''

