#pragma once
#include "PiecewiseCurveBuilder.hpp"
namespace PiecewiseCurveBuilderNamespace
{
	// Overall Builder //
	template<typename T, typename I> PiecewiseCurveBuilder<T, I>::PiecewiseCurveBuilder() {}

	// Building RateHelpers //
	template<typename T, typename I> void PiecewiseCurveBuilder<T, I>::AddDeposit(const pQuote& quote, const pIndex& index)
	{
		pHelper rateHelper(new DepositRateHelper(Handle<Quote>(quote), index));
		rateHelpers.push_back(rateHelper); // rateHelpers :vector<pHelper>
	}
	template<typename T, typename I> void PiecewiseCurveBuilder<T, I>::AddSwap(const pQuote& quote, const Period& periodLength,
		const Calendar& fixedCalendar, Frequency fixedFrequency, BusinessDayConvention fixedConvention, const DayCounter& fixedDayCount,
		const pIndex& floatIndex)
	{
		pHelper rateHelper(new SwapRateHelper(Handle<Quote>(quote), periodLength, fixedCalendar, fixedFrequency, fixedConvention, fixedDayCount, floatIndex));
		rateHelpers.push_back(rateHelper);
	}

	// Get TermStructure //
	template<typename T, typename I> RelinkableHandle<YieldTermStructure>
		PiecewiseCurveBuilder<T, I>::GetCurveHandle(const Date& settlementDate, const DayCounter& dayCounter)
	{
		// T = traits !, I = interpolation //
		ext::shared_ptr<YieldTermStructure> yieldTermStructure(new PiecewiseYieldCurve<T, I>(settlementDate, rateHelpers, dayCounter));
		return RelinkableHandle<YieldTermStructure>(yieldTermStructure);
	}
}

