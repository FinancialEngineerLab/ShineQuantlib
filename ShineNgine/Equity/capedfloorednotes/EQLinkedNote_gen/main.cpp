#include "cappedfloorednotes.hpp"




int main()
{
	try
	{
		Calendar calendar = SouthKorea();
		DayCounter dayCounter = Actual365Fixed();

		Date transactionDate(12, May, 2022);
		Natural settlementDays = 1;
		Date settlementDate = calendar.advance(transactionDate,
			Period(settlementDays, Days));
		Settings::instance().evaluationDate() = settlementDate;

		Real notional = 100;
		Real cap = 0.015;
		Real floor = 0.0;

		// Schedule for Fixing, Observation Dates
		std::vector<Date> fixingDates
		{
			calendar.advance(transactionDate,Period(1, Months)),
			calendar.advance(transactionDate,Period(2, Months)),
			calendar.advance(transactionDate,Period(3, Months)),
			calendar.advance(transactionDate,Period(4, Months)),
			calendar.advance(transactionDate,Period(5, Months)),
			calendar.advance(transactionDate,Period(6, Months)),
			calendar.advance(transactionDate,Period(7, Months)),
			calendar.advance(transactionDate,Period(8, Months)),
			calendar.advance(transactionDate,Period(9, Months)),
			calendar.advance(transactionDate,Period(10, Months)),
			calendar.advance(transactionDate,Period(11, Months)),
			calendar.advance(transactionDate,Period(12, Months)),
			calendar.advance(transactionDate,Period(13, Months)),
			calendar.advance(transactionDate,Period(14, Months)),
			calendar.advance(transactionDate,Period(15, Months)),
			calendar.advance(transactionDate,Period(16, Months)),
			calendar.advance(transactionDate,Period(17, Months)),
			calendar.advance(transactionDate,Period(18, Months)),
			calendar.advance(transactionDate,Period(19, Months)),
			calendar.advance(transactionDate,Period(20, Months)),
			calendar.advance(transactionDate,Period(21, Months)),
			calendar.advance(transactionDate,Period(22, Months)),
			calendar.advance(transactionDate,Period(23, Months)),
			calendar.advance(transactionDate,Period(24, Months)),
			calendar.advance(transactionDate,Period(25, Months)),
			calendar.advance(transactionDate,Period(26, Months)),
			calendar.advance(transactionDate,Period(27, Months)),
			calendar.advance(transactionDate,Period(28, Months)),
			calendar.advance(transactionDate,Period(29, Months)),
			calendar.advance(transactionDate,Period(30, Months)),
			calendar.advance(transactionDate,Period(31, Months)),
			calendar.advance(transactionDate,Period(32, Months)),
			calendar.advance(transactionDate,Period(33, Months)),
			calendar.advance(transactionDate,Period(34, Months)),
			calendar.advance(transactionDate,Period(35, Months)),
			calendar.advance(transactionDate,Period(36, Months))
		};

		std::vector<Date> paymentDates
		{

			calendar.advance(transactionDate,Period(1, Years)),
			calendar.advance(transactionDate,Period(2, Years)),
			calendar.advance(transactionDate,Period(3, Years))
		};

		// Maker //

		Real initialFixing = 2500.00;
		Real initialUp = initialFixing * 1.01;
		Real initialDn = initialFixing * 0.99;

		Real volatility = 0.20;
		Real volUp = 0.20 + 0.0001;
		Real volDn = 0.20 - 0.0001;
		Real RFR = 0.015;


		auto note = boost::make_shared<cappedFlooredNotes>(notional, initialFixing,
			fixingDates, paymentDates, cap, floor);


		// Inputs //
		auto RFRQuote = boost::make_shared<SimpleQuote>(RFR);
		Handle<Quote> RFRHandle(RFRQuote);
		auto RFRTS = boost::make_shared<FlatForward>(settlementDate, RFRHandle, dayCounter);
		Handle<YieldTermStructure> riskFreeRateTermStructureHandle(RFRTS);

		// Process //
		Handle<Quote> initialFixingHandle(boost::shared_ptr<Quote>(new SimpleQuote(initialFixing)));
		auto volatilityQuote = boost::make_shared<SimpleQuote>(volatility);
		Handle<Quote> volatilityHandle(volatilityQuote);
		Handle<BlackVolTermStructure> volatilityTSHandle(boost::shared_ptr<BlackVolTermStructure>
			(new BlackConstantVol(settlementDays, calendar, volatilityHandle, dayCounter)));
		auto process = boost::make_shared<BlackScholesProcess>(initialFixingHandle,
			riskFreeRateTermStructureHandle, volatilityTSHandle);

		bool useAntitheticVariates = false;
		Size requiredSamples = 1000;
		Real requiredTolerance = Null<Real>();
		Size maxSamples = 1000;
		BigNatural seed = 1;

		// Run ! //
		auto engine = boost::make_shared< cappedFlooredNotesMCPricer<PseudoRandom, Statistics>>
			(process, riskFreeRateTermStructureHandle, useAntitheticVariates, requiredSamples,
				requiredTolerance, maxSamples, seed);

		note->setPricingEngine(engine);
		std::cout << std::setprecision(4) << std::endl;
		std::cout << note->NPV() << std::endl;



		// For greeks //

		auto noteUp = boost::make_shared<cappedFlooredNotes>(notional, initialUp,
			fixingDates, paymentDates, cap, floor);

		// Process //
		Handle<Quote> initialFixingHandleUp(boost::shared_ptr<Quote>(new SimpleQuote(initialUp)));
		auto processUp = boost::make_shared<BlackScholesProcess>(initialFixingHandleUp,
			riskFreeRateTermStructureHandle, volatilityTSHandle);

		// Run ! //
		auto engineUp = boost::make_shared< cappedFlooredNotesMCPricer<PseudoRandom, Statistics>>
			(processUp, riskFreeRateTermStructureHandle, useAntitheticVariates, requiredSamples,
				requiredTolerance, maxSamples, seed);

		noteUp->setPricingEngine(engineUp);
		Real priceUp = noteUp->NPV();


		auto noteDn = boost::make_shared<cappedFlooredNotes>(notional, initialDn,
			fixingDates, paymentDates, cap, floor);

		// Process //
		Handle<Quote> initialFixingHandleDn(boost::shared_ptr<Quote>(new SimpleQuote(initialDn)));
		auto processdn = boost::make_shared<BlackScholesProcess>(initialFixingHandleDn,
			riskFreeRateTermStructureHandle, volatilityTSHandle);

		// Run ! //
		auto engineDn = boost::make_shared< cappedFlooredNotesMCPricer<PseudoRandom, Statistics>>
			(processdn, riskFreeRateTermStructureHandle, useAntitheticVariates, requiredSamples,
				requiredTolerance, maxSamples, seed);

		noteDn->setPricingEngine(engineDn);
		Real priceDn = noteDn->NPV();
		
		Real delta = (priceUp - priceDn) / 0.02 * initialFixing * 250000;
		std::cout << " Delta Exposure = " << delta << std::endl;

	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}