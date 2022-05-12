#include "cappedfloorednotes.hpp"
#include <algorithm>




// ARGUMENTS CLASS Start //
cappedFlooredNotes::cappedFlooredNotes(Real notional, Real initialFixing,
	const std::vector<Date>& fixingDates, const std::vector<Date>& paymentDates,
	Real cap, Real floor) : notional_(notional), initialFixing_(initialFixing),
	fixingDates_(fixingDates), paymentDates_(paymentDates), cap_(cap), floor_(floor)
{

}

bool cappedFlooredNotes::isExpired() const
{
	Date valuationDate = Settings::instance().evaluationDate();

	if (valuationDate > fixingDates_.back())
	{
		return true;
	}
	return false;
}



void cappedFlooredNotes::setupArguments(PricingEngine::arguments* args) const
{
	cappedFlooredNotes::arguments* args_ = dynamic_cast<cappedFlooredNotes::arguments*>(args);

	QL_REQUIRE(args_ != nullptr, "arguments casting error");

	// member vars //
	args_->notional = notional_;
	args_->initialFixing = initialFixing_;
	args_->fixingDates = fixingDates_;
	args_->paymentDates = paymentDates_;
	args_->cap = cap_;
	args_->floor = floor_;

}

void cappedFlooredNotes::arguments::validate() const
{
	QL_REQUIRE(notional > 0.0, "");
	QL_REQUIRE(initialFixing > 0.0, "");
	QL_REQUIRE(cap > floor, "");

	for (int i = 0; i != paymentDates.size(); ++i)
	{
		if (std::find(fixingDates.begin(), fixingDates.end(),
			paymentDates[i]) == fixingDates.end())
		{
			QL_REQUIRE(false, "");
		}
	}
}

// ARGUMENTS CLASS END //

// Path Pricer Class Start //

cappedFlooredNotesPathPricer::cappedFlooredNotesPathPricer(Real notional,
	Real initialFixing, const std::vector<Date>& fixingDates, const std::vector<Date>& paymentDates,
	Real cap, Real floor, const Handle<YieldTermStructure>& curve) :
	notional_(notional), initialFixing_(initialFixing), fixingDates_(fixingDates),
	paymentDates_(paymentDates), cap_(cap), floor_(floor), curve_(curve)
{
}

// IMPORTANT ! : Pricing Algorithm !! //

Real cappedFlooredNotesPathPricer::operator() (const Path& path) const
{
	Real coupon = 0.0;
	Real cummCoupon = 0.0;
	Real aggregatePathPayoff = 0.0;
	int paymentDateCounter = 0;

	// Fixing Dates Loop ! //
	for (int i = 1; i != fixingDates_.size(); ++i)
	{
		coupon = std::min(path.at(i) / path.at(i - 1) - 1, cap_); // Term Payoff Coupon!
		cummCoupon += coupon; // cumm coupon calc


		if (fixingDates_[i] == paymentDates_[paymentDateCounter])
		{
			aggregatePathPayoff += std::max(cummCoupon, floor_) * notional_ * curve_->discount(fixingDates_[i]); // from Term ! 
			cummCoupon = 0.0;
			paymentDateCounter++;
		}
	}
	return aggregatePathPayoff;
}