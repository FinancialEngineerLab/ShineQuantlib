#pragma once
#include <ql/quantlib.hpp>
#include <algorithm>

namespace ModelCalibratorNamespace
{
	using namespace QuantLib;
	template <typename MODEL, typename OPTIMIZER = LevenbergMarquardt>

	class ModelCalibrator
	{
	public:
		ModelCalibrator(const EndCriteria& endCriteria = EndCriteria(1000, 500, 0.0000001, 0.0000001, 0.0000001)) : endCriteria(endCriteria) { }
		
		void AddCalibrationHelper(boost::shared_ptr<BlackCalibrationHelper>& helper) // const? ,& helper?
		{
			helpers.push_back(helper);
		}

		void Calibrate(ext::shared_ptr<MODEL>& model,
			const boost::shared_ptr<PricingEngine>& pricingEngine,
			const Handle<YieldTermStructure>& curve,
			const std::vector<bool> fixedParameters = std::vector<bool>())
		{
			std::for_each(helpers.begin(), helpers.end(), [&pricingEngine](boost::shared_ptr<BlackCalibrationHelper>& helper)
			{
				helper->setPricingEngine(pricingEngine);
			});

			OPTIMIZER solver;

			// all involved params //
			if (fixedParameters.empty())
			{
				model->calibrate(helpers, solver, this->endCriteria);
			}
			// all involved non-fixed parms //
			else
			{
				model->calibrate(helpers, solver, this->endCriteria, NoConstraint(), std::vector<Real>(), fixedParameters);
			}
		}
	private:
		EndCriteria endCriteria;
		std::vector<boost::shared_ptr<BlackCalibrationHelper>> helpers;
	};
}