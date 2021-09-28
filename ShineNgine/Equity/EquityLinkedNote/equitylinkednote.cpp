#include "equitylinkednote.hpp"
#include <algorithm>

EquityLinkedNote::EquityLinkedNote(Real notional, Real initialFixing,
	const std::vector<Date>& fixingDates,
	const std::vector<Date>& paymentDates,
	Real cap, Real floor)
	: notional_(notional), initialFixing_(initialFixing), fixingDates_(fixingDates),
	paymentDates_(paymentDates), cap_(cap), floor_(floor)
{
}

bool EquityLinkedNote::isExpired() const
{
	Date valuationDate = Settings::instance().evaluationDate();
	if (valuationDate > fixingDates_.back())
	{
		return true;
	}
	return false;
}

void EquityLinkedNote::setupArguments(PricingEngine::arguments* args) const
{
	EquityLinkedNote::arguments* args_ = dynamic_cast<EquityLinkedNote::arguments*>(args);
	QL_REQUIRE(args_ != nullptr, "arguments casting error");
	args_->notional = notional_;
	args_->initialFixing = initialFixing_;
	args_->fixingDates = fixingDates_;
	args_->paymentDates = paymentDates_;
	args_->cap = cap_;
	args_->floor = floor_;
}

void EquityLinkedNote::arguments::validate() const
{
	QL_REQUIRE(notional > 0.0, "Notional must be greater than zero");
	QL_REQUIRE(initialFixing > 0.0, "Initial Fixing Must be greater than zero");
	QL_REQUIRE(cap > floor, "Cap must be greater than floor");

	for (int i = 0; i != paymentDates.size(); ++i)
	{
		if (std::find(fixingDates.begin(), fixingDates.end(),
			paymentDates[i]) == fixingDates.end())
		{
			QL_REQUIRE(false, "Payment date has to be included within given fixing dates");
		}
	}
}

EquityLinkedNotePathPricer::EquityLinkedNotePathPricer(Real notional, Real initialFixing,
	const std::vector<Date>& fixingDates, const std::vector<Date>& paymentDates,
	Real cap, Real floor, const Handle<YieldTermStructure>& curve) :
	notional_(notional), initialFixing_(initialFixing), fixingDates_(fixingDates),
	paymentDates_(paymentDates), cap_(cap), floor_(floor), curve_(curve)
{
}

Real EquityLinkedNotePathPricer::operator() (const Path& path) const
{
	Real coupon = 0.0;
	Real cumulativeCoupon = 0.0;
	Real aggregatePathPayoff = 0.0;
	int paymentDateCounter = 0;

	for (int i = 1; i != fixingDates_.size(); ++i)
	{
		coupon = std::min(path.at(i) / path.at(i - 1) - 1, cap_);
		cumulativeCoupon += coupon;

		if (fixingDates_[i] == paymentDates_[paymentDateCounter])
		{
			aggregatePathPayoff += std::max(cumulativeCoupon, floor_) * notional_ * curve_->
				discount(fixingDates_[i]);
			cumulativeCoupon = 0.0;
			paymentDateCounter++;
		}
	}
	return aggregatePathPayoff;
}
	


