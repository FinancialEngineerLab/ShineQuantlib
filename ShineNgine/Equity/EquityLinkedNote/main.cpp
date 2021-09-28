#include "equitylinkednote.hpp"

int main()
{
	try
	{
		Calendar calendar = TARGET();
		DayCounter dayCounter = Actual365Fixed();
		Date transactionDate(30, October, 2017);
		Natural settlementDays = 2;
		Date settlementDate = calendar.advance(transactionDate, Period(settlementDays, Days));
		Settings::instance().evaluationDate() = settlementDate;

		Real notional = 100000000.0;
		Real initialFixing = 3662.18;
		Real cap = 0.015;
		Real floor = 0.0;

		// Fixing Schedule //
		std::vector<Date> fixingDates
		{
			Date(30, November, 2017),
			Date(30, December, 2017),
			Date(30, January, 2018),
			Date(28, February, 2018),
			Date(30, March, 2018),
			Date(30, April, 2018),
			Date(30, May, 2018),
			Date(30, June, 2018),
			Date(30, July, 2018),
			Date(30, August, 2018),
			Date(30, September, 2018),
			Date(30, October, 2018),
			Date(30, November, 2018),
			Date(30, December, 2018),
			Date(30, January, 2019),
			Date(28, February, 2019),
			Date(30, March, 2019),
			Date(30, April, 2019),
			Date(30, May, 2019),
			Date(30, June, 2019),
			Date(30, July, 2019),
			Date(30, August, 2019),
			Date(30, September, 2019),
			Date(30, October, 2019),
			Date(30, November, 2019),
			Date(30, December, 2019),
			Date(30, January, 2020),
			Date(28, February, 2020),
			Date(30, March, 2020),
			Date(30, April, 2020),
			Date(30, May, 2020),
			Date(30, June, 2020),
			Date(30, July, 2020),
			Date(30, August, 2020),
			Date(30, September, 2020),
			Date(30, October, 2020)
		};

		std::vector<Date> paymentDates
		{
			Date(30, October, 2018),
			Date(30, October, 2019),
			Date(30, October, 2020)
		};

		auto note = boost::make_shared<EquityLinkedNote>(notional, initialFixing, fixingDates,
			paymentDates, cap, floor);

		Real riskFreeRate = 0.01;
		auto riskFreeRateQuote = boost::make_shared<SimpleQuote>(riskFreeRate);
		Handle<Quote> riskFreeRateHandle(riskFreeRateQuote);
		auto riskFreeRateTermStructure = boost::make_shared<FlatForward>(settlementDate,
			riskFreeRateHandle, dayCounter);
		Handle<YieldTermStructure> riskFreeRateTermStructureHandle(riskFreeRateTermStructure);

		// stochastic process building //
		Handle<Quote> initialFixingHandle(boost::shared_ptr<Quote>(new SimpleQuote(initialFixing)));
		Real volatility = 0.20;
		auto volatilityQuote = boost::make_shared<SimpleQuote>(volatility);
		Handle<Quote> volatilityHandle(volatilityQuote);
		Handle<BlackVolTermStructure> volatilityTermStructureHandle(boost::shared_ptr<BlackVolTermStructure>
			(new BlackConstantVol(settlementDays, calendar, volatilityHandle, dayCounter)));
		auto process = boost::make_shared<BlackScholesProcess>(initialFixingHandle, riskFreeRateTermStructureHandle, volatilityTermStructureHandle);

		// simulation related attributes ! //
		bool useAntitheticVariates = true;
		Size requiredSamples = 2000;
		Real requiredTolerance = Null<Real>();
		Size maxSamples = 2000;
		BigNatural seed = 1;

		auto engine = boost::make_shared<EquityLiknedNoteMCPricer<PseudoRandom, Statistics>>
			(process, riskFreeRateTermStructureHandle, useAntitheticVariates, requiredSamples,
				requiredTolerance, maxSamples, seed);
		note->setPricingEngine(engine);
		std::cout << note->NPV() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}