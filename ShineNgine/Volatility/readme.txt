

1. Local Volatility Model
Pricing with a Smile: https://www.math.nyu.edu/~benartzi/pricingw.pdf
The Volatility Smile and Its Implied Tree: http://www.cmat.edu.uy/~mordecki/hk/derman-kani.pdf
###
pros and cons:
The local volatility models will be able to match the value of the smile as of today,
but because the smile flattens for long maturities,
the model gives an almost constant smile for these maturities,
leading to a flattening of the forward smile (i.e. smile in the future), which is unrealistic.
It is not good for complex products.
#####
2. Collocating Local Volatility Model
https://hpcquantlib.files.wordpress.com/2016/12/clv.pdf
#####
3. Heston Stochastic Local Volatility Model
Volatility Smile Ch 20.
https://www.quantlib.org/slides/qlws14/spanderen.pdf
http://hpc-quantlib.de/src/slv.pdf
https://hpcquantlib.wordpress.com/2015/09/20/calibration-of-heston-stochastic-local-volatility-models/
https://hpcquantlib.wordpress.com/2016/01/10/monte-carlo-calibration-of-the-heston-stochastic-local-volatiltiy-model/
#####
3. SABR
 3-1. No Arbitrage SABR
 https://quantlib.wordpress.com/tag/sabr/
 https://hpcquantlib.wordpress.com/2019/01/11/finite-difference-solver-for-the-sabr-model/
 3-2. ZABR
 https://quantlib.wordpress.com/tag/zabr/
 3-3. Calibration
 https://letianquant.files.wordpress.com/2016/01/12-sabr-model.pdf
 https://github.com/FinancialEngineerLab/fineSABRModel
4. Andreasen Huge Interpolation
https://hpcquantlib.wordpress.com/2019/01/11/finite-difference-solver-for-the-sabr-model/
