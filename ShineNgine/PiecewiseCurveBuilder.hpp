#pragma once
#include <ql/quantlib.hpp>
#include <ql/indexes/ibor/krwcd.hpp>

namespace PiecewiseCurveBuilderNamespace
{
	using namespace QuantLib;
	
	// type definition //
	using pQuote = ext::shared_ptr<Quote>;
	using pIndex = ext::shared_ptr<IborIndex>;
	using pHelper = ext::shared_ptr<RateHelper>;

	// Overall Builder Class //
	template<typename T, typename I>
	class PiecewiseCurveBuilder
	{
	public:
		PiecewiseCurveBuilder();
		void AddDeposit(const pQuote& quote, const pIndex& index);
		void AddSwap(const pQuote& quote, const Period& periodLength,
			const Calendar& fixedCalendar, Frequency fixedFrequency
			,BusinessDayConvention fixedConvention
			, const DayCounter& fixedDayCount
			, const pIndex& floatIndex);
		RelinkableHandle<YieldTermStructure>
			GetCurveHandle(
			const Date& settlementDate
			, const DayCounter& dayCounter);
	
	// Vector to rateHelpers //
	private:
		std::vector<pHelper> rateHelpers;

	};
	
}