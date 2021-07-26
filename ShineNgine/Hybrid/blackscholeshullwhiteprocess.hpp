#ifndef quantlib_hybrid_blackscholes_hull_white_process_hpp

#define quantlib_hybrid_blackscholes_hull_white_process_hpp

#include <ql/processes/blackscholesprocess.hpp>

#include <ql/processes/hullwhiteprocess.hpp>

#include <ql/processes/jointstochasticprocess.hpp>

#include <ql/models/shortrate/onefactormodels/hullwhite.hpp>

namespace QuantLib {

        class HybridBlackScholesHullWhiteProcess : public StochasticProcess {

        public:

               enum Discretization { Euler, BSMHullWhite };

              

               HybridBlackScholesHullWhiteProcess(

                       const ext::shared_ptr<BlackScholesProcess> & blackScholesProcess,

                       const ext::shared_ptr<HullWhiteProcess> & hullWhiteProcess,

                       Real corrEquityShortRate,

                       Discretization discretization = BSMHullWhite);


 

               Size size() const;

               Disposable<Array> initialValues() const;

               Disposable<Array> drift(Time t, const Array& x) const;

               Disposable<Matrix> diffusion(Time t, const Array& x) const;

               Disposable<Array> apply(const Array& x0, const Array& dx) const;


 

               Disposable<Array> evolve(Time t0, const Array& x0,

                       Time dt, const Array& dw) const;


 

               DiscountFactor numeraire(Time t, const Array& x) const;

               const ext::shared_ptr<BlackScholesProcess>& blackScholesProcess() const;

               const ext::shared_ptr<HullWhiteProcess>&

                       hullWhiteProcess() const;


 

               Real eta() const;

               Time time(const Date& date) const;

               Discretization discretization() const;

               void update();

        protected:

               const ext::shared_ptr<BlackScholesProcess> blackScholesProcess_;

               const ext::shared_ptr<HullWhiteProcess> hullWhiteProcess_;

               //model is used to calculate P(t,T)

               const ext::shared_ptr<HullWhite> hullWhiteModel_;

               const Real corrEquityShortRate_;

               const Discretization discretization_;

               const Time T_;

               DiscountFactor endDiscount_;

        };


 

}

#endif
