/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2007, 2008 Klaus Spanderen

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file hybridblackscholeshullwhiteprocess.hpp
	\brief hybrid equity (blackscholes model)
		   with stochastic interest rates (hull white model)
*/

#include <ql/termstructures/yield/flatforward.hpp>
#include "hybridblackscholeshullwhiteprocess.hpp"
#include <iostream>
namespace QuantLib {

	HybridBlackScholesHullWhiteProcess::HybridBlackScholesHullWhiteProcess(
		const ext::shared_ptr<BlackScholesProcess> & blackScholesProcess,
		const ext::shared_ptr<HullWhiteProcess> & hullWhiteProcess,
		Real corrEquityShortRate,
		HybridBlackScholesHullWhiteProcess::Discretization discretization)
		: blackScholesProcess_(blackScholesProcess),
		hullWhiteProcess_(hullWhiteProcess),
		hullWhiteModel_(new HullWhite(blackScholesProcess->riskFreeRate(),
			hullWhiteProcess->a(),
			hullWhiteProcess->sigma())),
		corrEquityShortRate_(corrEquityShortRate),
		discretization_(discretization),
		//maxRho_(std::sqrt(1 - blackScholesProcess->rho()*blackScholesProcess->rho())
		//	- std::sqrt(QL_EPSILON) /* reserve for rounding errors */),

		//T_(hullWhiteProcess->getForwardMeasureTime()),
		T_(0.0),
		endDiscount_(blackScholesProcess->riskFreeRate()->discount(T_)) {

		QL_REQUIRE(corrEquityShortRate*corrEquityShortRate <= 1.0,
			//+ blackScholesProcess->rho()*blackScholesProcess->rho() <= 1.0,
			"correlation matrix is not positive definite");

		QL_REQUIRE(hullWhiteProcess->sigma() > 0.0,
			"positive vol of Hull White process is required");
	}
	
	Size HybridBlackScholesHullWhiteProcess::size() const {
		return 2;
	}

	Disposable<Array> HybridBlackScholesHullWhiteProcess::initialValues() const {
		Array retVal(2);
		retVal[0] = blackScholesProcess_->x0();
		retVal[1] = hullWhiteProcess_->x0();

		return retVal;
	}

	Disposable<Array>
		HybridBlackScholesHullWhiteProcess::drift(Time t, const Array& x) const {
		Array retVal(2);

		retVal[0] = blackScholesProcess_->drift(t, x[0]);
		retVal[1] = hullWhiteProcess_->drift(t, x[1]);

		return retVal;
	}

	Disposable<Array>
		HybridBlackScholesHullWhiteProcess::apply(const Array& x0, const Array& dx) const {
		Array retVal(2);
		
		retVal[0] = blackScholesProcess_->apply(x0[0], dx[1]);
		retVal[1] = hullWhiteProcess_->apply(x0[1], dx[1]);

		return retVal;
	}

	Disposable<Matrix>
		HybridBlackScholesHullWhiteProcess::diffusion(Time t, const Array& x) const {
		Matrix retVal(2, 2);

		retVal[0][0] = blackScholesProcess_->diffusion(t, x[0]);
		retVal[0][1] = 0.0;
		
		const Real sigma = hullWhiteProcess_->sigma();
		retVal[1][0] = corrEquityShortRate_ * sigma;
		retVal[1][1] = std::sqrt(sigma*sigma - retVal[1][0] * retVal[1][0]);

		return retVal;
	}

	Disposable<Array>			//ø©±‚∫Œ≈Õ evole
		HybridBlackScholesHullWhiteProcess::evolve(Time t0, const Array& x0, Time dt, const Array& dw) const {
		
		const Rate r = x0[1];
		const Real a = hullWhiteProcess_->a();
		const Real sigma = hullWhiteProcess_->sigma();
		const Real rho = corrEquityShortRate_;
		
		blackScholesProcess_->localVolatility(); // trigger update
		const Volatility eta = blackScholesProcess_->stdDeviation(t0, x0[0], dt) / std::sqrt(dt);
		
		const Time s = t0;
		const Time t = t0 + dt;
		const Time T = T_;
		const Rate dy = blackScholesProcess_->dividendYield()->forwardRate(s, t, Continuous, NoFrequency);
		const Real df = std::log(blackScholesProcess_->riskFreeRate()->discount(t) / blackScholesProcess_->riskFreeRate()->discount(s));

		const Real eaT = std::exp(-a * T);
		const Real eat = std::exp(-a * t);
		const Real eas = std::exp(-a * s);
		const Real iat = 1.0 / eat;
		const Real ias = 1.0 / eas;

		const Real m1 = -(dy + 0.5*eta*eta)*dt - df;

		/*const Real m2 = -rho * sigma*eta / a * (dt - 1 / a * eaT*(iat - ias));
		
		const Real m3 = (r - hullWhiteProcess_->alpha(s))
			*hullWhiteProcess_->B(s, t);

		const Real m4 = sigma * sigma / (2 * a*a)
			*(dt + 2 / a * (eat - eas) - 1 / (2 * a)*(eat*eat - eas * eas));

		const Real m5 = -sigma * sigma / (a*a)
			*(dt - 1 / a * (1 - eat * ias) - 1 / (2 * a)*eaT*(iat - 2 * ias + eat * ias*ias));
*/
		const Real mu = m1;// +m2 + m3 + m4 + m5;

		Array retVal(2);				
		if (discretization_ == BSMHullWhite) {
			const Real v1 = eta * eta*dt
				+ sigma * sigma / (a*a)*(dt - 2 / a * (1 - eat * ias)
					+ 1 / (2 * a)*(1 - eat * eat*ias*ias))
				+ 2 * sigma*eta / a * rho*(dt - 1 / a * (1 - eat * ias));
			const Real v2 = hullWhiteProcess_->variance(t0, r, dt);

			const Real v12 = (1 - eat * ias)*(sigma*eta / a * rho + sigma * sigma / (a*a))
				- sigma * sigma / (2 * a*a)*(1 - eat * eat*ias*ias);

			QL_REQUIRE(v1 > 0.0 && v2 > 0.0, "zero or negative variance given");
			
			// terminal rho must be between -maxRho and +maxRho
			const Real rhoT = v12 / std::sqrt(v1*v2);

			QL_REQUIRE(rhoT <= 1.0 && rhoT >= -1.0
				&& 1 - rhoT * rhoT >= 0.0,
				"invalid terminal correlation");

			const Real dw_0 = dw[0];
			const Real dw_1 = rhoT * dw[0]
				+ std::sqrt(1 - rhoT * rhoT)*dw[1];

			retVal[1] = hullWhiteProcess_->evolve(t0, r, dt, dw_1);

			const Real vol = std::sqrt(v1)*dw_0;
			retVal[0] = x0[0] * std::exp(mu + vol);
		}
		else if (discretization_ == Euler) {
			const Real dw_1 = rho * dw[0];
				+ std::sqrt(1 - rho * rho)*dw[1];

			retVal[1] = hullWhiteProcess_->evolve(t0, r, dt, dw_1);

			const Real vol = eta * std::sqrt(dt)*dw[0];
			retVal[0] = x0[0] * std::exp(mu + vol);
		}
		else
			QL_FAIL("unknown discretization scheme");

		return retVal;
	}

	DiscountFactor
		HybridBlackScholesHullWhiteProcess::numeraire(Time t, const Array& x) const {
		return hullWhiteModel_->discountBond(t, T_, x[1]) / endDiscount_;
	}

	Real HybridBlackScholesHullWhiteProcess::eta() const {
		return corrEquityShortRate_;
	}

	const ext::shared_ptr<BlackScholesProcess>&
		HybridBlackScholesHullWhiteProcess::blackScholesProcess() const {
		return blackScholesProcess_;
	}

	const ext::shared_ptr<HullWhiteProcess>&
		HybridBlackScholesHullWhiteProcess::hullWhiteProcess() const {
		return hullWhiteProcess_;
	}

	HybridBlackScholesHullWhiteProcess::Discretization
		HybridBlackScholesHullWhiteProcess::discretization() const {
		return discretization_;
	}

	Time HybridBlackScholesHullWhiteProcess::time(const Date& date) const {
		return blackScholesProcess_->time(date);
	}

	void HybridBlackScholesHullWhiteProcess::update() {
		endDiscount_ = blackScholesProcess_->riskFreeRate()->discount(T_);
	}
}
