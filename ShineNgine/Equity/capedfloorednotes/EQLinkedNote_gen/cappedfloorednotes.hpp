#pragma once

#include <ql/quantlib.hpp>

using namespace QuantLib;


// ARGUMENTS CLASS Start //
class cappedFlooredNotes : public Instrument
{
public:
	class arguments;
	class engine;

	cappedFlooredNotes(Real notional, Real initialFixing, const std::vector<Date>& fixingDates,
		const std::vector<Date>& paymentDates, Real cap, Real floor);

	bool isExpired() const;

private:
	void setupArguments(PricingEngine::arguments* args) const;

	// member vars //
	Real notional_;
	Real initialFixing_;
	std::vector<Date> fixingDates_;
	std::vector<Date> paymentDates_;
	Real cap_;
	Real floor_;
};

class cappedFlooredNotes::arguments : public PricingEngine::arguments
{
public:
	void validate() const;

	Real notional;
	Real initialFixing;
	std::vector<Date> fixingDates;
	std::vector<Date> paymentDates;
	Real cap;
	Real floor;
};

class cappedFlooredNotes::engine : public GenericEngine<cappedFlooredNotes::arguments,
	cappedFlooredNotes::results>
{
};

// ARGUMENTS CLASS END //

// Path Pricer Class Start //
class cappedFlooredNotesPathPricer : public PathPricer<Path>
{
public:
	cappedFlooredNotesPathPricer(Real notional, Real initialFixing,
		const std::vector<Date>& fixingDates,const std::vector<Date>& paymentDates,
		Real cap, Real floor, const Handle<YieldTermStructure>& curve);

	Real operator()(const Path& path) const;

private:
	Real notional_;
	Real initialFixing_;
	std::vector<Date> fixingDates_;
	std::vector<Date> paymentDates_;
	Real cap_;
	Real floor_;
	Handle<YieldTermStructure> curve_;

};

//Path Pricer Class End //

// MC Frameworkd Start //
template <typename RNG = PseudoRandom, typename S = Statistics>
class cappedFlooredNotesMCPricer :
	public cappedFlooredNotes::engine, private McSimulation<SingleVariate, RNG, S>
{
public : 
	cappedFlooredNotesMCPricer(const boost::shared_ptr<StochasticProcess>& process,
		const Handle<YieldTermStructure>& curve, bool antitheticVariate, Size requiredSamples,
		Real requiredTolerance, Size maxSamples, BigNatural seed):
		McSimulation<SingleVariate, RNG, S> (antitheticVariate, false), process_(process),
		curve_(curve),
			requiredSamples_(requiredSamples), requiredTolerance_(requiredTolerance),
		maxSamples_(maxSamples), seed_(seed)
	{
		registerWith(process_);
		registerWith(curve_);
	}

	void calculate() const
	{
		McSimulation<SingleVariate, RNG, S>::calculate(requiredTolerance_, requiredSamples_,
			maxSamples_);
		this->results_.value = this->mcModel_->sampleAccumulator().mean();

		if (RNG::allowsErrorEstimate)
		{
			this->results_.errorEstimate = this->mcModel_->sampleAccumulator().errorEstimate();
		}
		else
		{
			this->results_.errorEstimate = Null<Real>();
		}
	}

private:
	typedef McSimulation<SingleVariate, RNG, S> simulation;
	typedef typename simulation::path_pricer_type path_pricer_type;
	typedef typename simulation::path_generator_type path_generator_type;

	// implementation for Mc Simulation class virtual method //
	TimeGrid timeGrid() const
	{
		Date referenceDate = curve_->referenceDate();
		DayCounter dayCounter = curve_->dayCounter();
		std::vector<Time> fixingTimes(arguments_.fixingDates.size());
		for (Size i = 0; i != fixingTimes.size(); ++i)
		{
			fixingTimes[i] = dayCounter.yearFraction(referenceDate, arguments_.fixingDates[i]);
		}
		return TimeGrid(fixingTimes.begin(), fixingTimes.end());
	}

	boost::shared_ptr<path_generator_type> pathGenerator() const
	{
		TimeGrid grid = timeGrid();
		Size steps = (grid.size() - 1);

		typename RNG::rsg_type generator = RNG::make_sequence_generator(steps, seed_);
		return boost::make_shared<path_generator_type>(process_, grid, generator, false);
	}

	boost::shared_ptr<path_pricer_type> pathPricer() const
	{
		return boost::make_shared< cappedFlooredNotesPathPricer>(arguments_.notional,
			arguments_.initialFixing,
			arguments_.fixingDates, arguments_.paymentDates, arguments_.cap, arguments_.floor, this->curve_);
	}
	// virtual method end //
	
	// member vars //
	boost::shared_ptr<StochasticProcess> process_;
	Handle<YieldTermStructure> curve_;
	Size requiredSamples_;
	Real requiredTolerance_;
	Size maxSamples_;
	BigNatural seed_;
};



