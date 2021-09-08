
#include <ql/quantlib.hpp>
#include <ql/indexes/ibor/krwcd.hpp>

#ifdef BOOST_MSVC
#include <ql/auto_link.hpp>
#endif

#include <iostream>
#include <iomanip>

using namespace QuantLib;
using namespace std;
using namespace ext;


#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib
{
	ThreadKey sessionId() { return (); }
}
#endif

int main()
{
	try
	{
		std::cout << std::endl;

		/* Inputting 1. Conventions */

		Calendar calendar = SouthKorea();

		calendar.addHoliday(Date(4, Oct, 2021));
		calendar.addHoliday(Date(11, Oct, 2021));
		calendar.addHoliday(Date(10, Oct, 2021));
		calendar.addHoliday(Date(2, Mar, 2026));
		calendar.addHoliday(Date(3, Jun, 2026));
		calendar.addHoliday(Date(17, Aug, 2026));
		calendar.addHoliday(Date(5, Oct, 2026));
		calendar.addHoliday(Date(3, Mar, 2027));
		calendar.addHoliday(Date(16, Aug, 2027));
		calendar.addHoliday(Date(4, Oct, 2027));
		calendar.addHoliday(Date(11, Oct, 2027));
		calendar.addHoliday(Date(3, Mar, 2031));
		calendar.addHoliday(Date(16, Aug, 2032));
		calendar.addHoliday(Date(4, Oct, 2032));
		calendar.addHoliday(Date(11, Oct, 2032));
		calendar.addHoliday(Date(10, Oct, 2033));
		calendar.addHoliday(Date(3, Mar, 2036));
		calendar.addHoliday(Date(7, Oct, 2036));
		calendar.addHoliday(Date(2, Mar, 2037));
		calendar.addHoliday(Date(17, Aug, 2037));
		calendar.addHoliday(Date(5, Oct, 2037));
		calendar.addHoliday(Date(16, Aug, 2038));
		calendar.addHoliday(Date(4, Oct, 2038));
		calendar.addHoliday(Date(11, Oct, 2038));
		calendar.addHoliday(Date(10, Oct, 2039));
		// Election //
		calendar.addHoliday(Date(3, Mar, 2025));
		calendar.addHoliday(Date(12, Apr, 2028));
		calendar.addHoliday(Date(12, Jun, 2030));
		calendar.addHoliday(Date(3, Mar, 2032));
		calendar.addHoliday(Date(14, Apr, 2032));
		calendar.addHoliday(Date(31, May, 2034));
		calendar.addHoliday(Date(9, Apr, 2036));
		calendar.addHoliday(Date(4, Mar, 2037));
		calendar.addHoliday(Date(2, Jun, 2038));
		calendar.addHoliday(Date(11, Apr, 2038));

		DayCounter dayCounter = Actual365Fixed();
		Date todaysDate(2, Sep, 2021);
		Settings::instance().evaluationDate() = todaysDate;
		todaysDate = Settings::instance().evaluationDate();

		Integer fixingDays = 1;
		Date settlementDate = calendar.advance(todaysDate, fixingDays, Days);
		RelinkableHandle<YieldTermStructure> discountingTermStucture;

		/* Inputting 2. Quotes, Helpers */

		ext::shared_ptr<IborIndex> krwCD(new KrwCD3M());
		ext::shared_ptr<Quote> krw01dCl(new SimpleQuote(0.007536411299));
		ext::shared_ptr<Quote> krw03mCD(new SimpleQuote(0.0092));
		ext::shared_ptr<Quote> krw06mSW(new SimpleQuote(0.0105));
		ext::shared_ptr<Quote> krw09mSW(new SimpleQuote(0.011239285714));
		ext::shared_ptr<Quote> krw01ySW(new SimpleQuote(0.0119107142860));
		ext::shared_ptr<Quote> krw18mSW(new SimpleQuote(0.012917857143));
		ext::shared_ptr<Quote> krw02ySW(new SimpleQuote(0.013675));
		ext::shared_ptr<Quote> krw03ySW(new SimpleQuote(0.014582142857));
		ext::shared_ptr<Quote> krw04ySW(new SimpleQuote(0.01525));
		ext::shared_ptr<Quote> krw05ySW(new SimpleQuote(0.015725));
		ext::shared_ptr<Quote> krw06ySW(new SimpleQuote(0.015932142857));
		ext::shared_ptr<Quote> krw07ySW(new SimpleQuote(0.016114285714));
		ext::shared_ptr<Quote> krw08ySW(new SimpleQuote(0.016228571429));
		ext::shared_ptr<Quote> krw09ySW(new SimpleQuote(0.016478571429));
		ext::shared_ptr<Quote> krw10ySW(new SimpleQuote(0.016567857143));
		ext::shared_ptr<Quote> krw12ySW(new SimpleQuote(0.016814285714));
		ext::shared_ptr<Quote> krw15ySW(new SimpleQuote(0.016364285714));
		ext::shared_ptr<Quote> krw20ySW(new SimpleQuote(0.015932142857));

		std::vector<pair<Period, Real> > inputVec;
		inputVec.push_back(make_pair(Period(1 * Days), krw01dCl.get()->value()));
		inputVec.push_back(make_pair(Period(3 * Months), krw03mCD.get()->value()));
		inputVec.push_back(make_pair(Period(6 * Months), krw06mSW.get()->value()));
		inputVec.push_back(make_pair(Period(9 * Months), krw09mSW.get()->value()));
		inputVec.push_back(make_pair(Period(12 * Months), krw01ySW.get()->value()));
		inputVec.push_back(make_pair(Period(18 * Months), krw18mSW.get()->value()));
		inputVec.push_back(make_pair(Period(24 * Months), krw02ySW.get()->value()));
		inputVec.push_back(make_pair(Period(36 * Months), krw03ySW.get()->value()));
		inputVec.push_back(make_pair(Period(48 * Months), krw04ySW.get()->value()));
		inputVec.push_back(make_pair(Period(60 * Months), krw05ySW.get()->value()));
		inputVec.push_back(make_pair(Period(72 * Months), krw06ySW.get()->value()));
		inputVec.push_back(make_pair(Period(84 * Months), krw07ySW.get()->value()));
		inputVec.push_back(make_pair(Period(96 * Months), krw08ySW.get()->value()));
		inputVec.push_back(make_pair(Period(108 * Months), krw09ySW.get()->value()));
		inputVec.push_back(make_pair(Period(120 * Months), krw10ySW.get()->value()));
		inputVec.push_back(make_pair(Period(144 * Months), krw12ySW.get()->value()));
		inputVec.push_back(make_pair(Period(180 * Months), krw15ySW.get()->value()));
		inputVec.push_back(make_pair(Period(240 * Months), krw20ySW.get()->value()));

		// Helpers & Instruments //
		krwCD->addFixing(settlementDate, krw03mCD->value(), true);
		Frequency swFixedLegFrequency = Quarterly;
		BusinessDayConvention swFixedLegConvention = ModifiedFollowing;
		DayCounter swFixedLegDayCounter = Actual365Fixed();

		ext::shared_ptr<RateHelper> dp01d(new DepositRateHelper(Handle<Quote>(krw01dCl), Period(1 * Days), 0, calendar, ModifiedFollowing, true, Actual365Fixed()));

		std::vector<ext::shared_ptr<RateHelper>> ccpInstruments;

		for (Size i = 0; i <= 80; i++)
		{
			for (Size j = 1; j <= inputVec.size() - 1; j++)
			{
				if (Period((i + 1) * 3 * Months) == inputVec[j].first)
				{
					ext::shared_ptr<Quote> tempPar(new SimpleQuote(inputVec[j].second));
					ext::shared_ptr<RateHelper> tempHelper(new SwapRateHelper(Handle<Quote>(tempPar), inputVec[j].first,
						calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));
					ccpInstruments.push_back(tempHelper);


					cout << setprecision(15) << endl;
					cout << "input " << tempPar.get()->value() << std::endl;
				}

				if ((j < 16) && ((Period((i + 1) * 3 * Months) < inputVec[j + 1].first) && (Period((i + 1) * 3 * Months) > inputVec[j].first)))
				{
					Date tempDt1 = calendar.advance(settlementDate, inputVec[j + 1].first);
					Date tempDt0 = calendar.advance(settlementDate, inputVec[j].first);
					Date tempDtn = calendar.advance(settlementDate, (i + 1) * 3, Months);
					Rate intpl = inputVec[j].second + (inputVec[j + 1].second - inputVec[j].second) *
						(tempDtn.serialNumber() - tempDt0.serialNumber()) / (tempDt1.serialNumber() - tempDt0.serialNumber());

					ext::shared_ptr<Quote> tempPar(new SimpleQuote(intpl));
					ext::shared_ptr<RateHelper> tempHelper(new SwapRateHelper(Handle<Quote>(tempPar), (i + 1) * 3 * Months,
						calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));
					ccpInstruments.push_back(tempHelper);

				}
				// 15 ~ 20
				if ((j >= 16) && (j < 17) && (Period((i + 1) * 3 * Months) < inputVec[17].first) && (Period((i + 1) * 3 * Months) > inputVec[16].first))
				{
					Date tempDt1 = calendar.advance(settlementDate, inputVec[17].first);
					Date tempDt0 = calendar.advance(settlementDate, inputVec[16].first);
					Date tempDtn = calendar.advance(settlementDate, (i + 1) * 3, Months);

					Rate intpl = inputVec[16].second + (inputVec[17].second - inputVec[16].second) *
						(tempDtn.serialNumber() - tempDt0.serialNumber()) / (tempDt1.serialNumber() - tempDt0.serialNumber());

					ext::shared_ptr<Quote> tempPar(new SimpleQuote(intpl));
					ext::shared_ptr<RateHelper> tempHelper(new SwapRateHelper(Handle<Quote>(tempPar), (i + 1) * 3 * Months,
						calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));
					ccpInstruments.push_back(tempHelper);
				}
			}
		}
		std::cout << ccpInstruments.size() << std::endl;

		double tolerance = 1.0e-15;
		//ext::shared_ptr<YieldTermStructure> ccpTermStructure(new PiecewiseYieldCurve<ZeroYield, Linear>(settlementDate, ccpInstruments, Actual365Fixed(), tolerance));
		ext::shared_ptr<YieldTermStructure> ccpTermStructure(new PiecewiseYieldCurve<Discount, LogLinear>(settlementDate, ccpInstruments, Actual365Fixed(), tolerance));
		
		ccpTermStructure->enableExtrapolation(true);
		discountingTermStucture.linkTo(ccpTermStructure);

		// Swap Information //
		Rate fixedRate = 0.01575;
		Integer lengthInYears = 5;
		VanillaSwap::Type swapType = VanillaSwap::Payer;
		Date maturity = settlementDate + Period(lengthInYears * Years);
		Real nominal = 8000000000;
		Spread spread = 0.0;

		// Fixed Leg //

		Frequency fixedLegFrequency = Quarterly;
		BusinessDayConvention dayConvention = ModifiedFollowing;
		//DayCounter dayCounter = Actual365Fixed();
		Schedule schedule(settlementDate, maturity, Period(fixedLegFrequency), calendar, dayConvention, dayConvention, DateGeneration::Forward, true);
		ext::shared_ptr<IborIndex> krwCDsw(new KrwCD3M(discountingTermStucture));
		krwCDsw->addFixing(settlementDate, krw03mCD->value(), true);

		Real NPV;
		Rate fairRate;
		VanillaSwap Swap(swapType, nominal, schedule, fixedRate, dayCounter, schedule, krwCDsw, spread, dayCounter);
		ext::shared_ptr<PricingEngine> swapEngine(new DiscountingSwapEngine(discountingTermStucture));
		Swap.setPricingEngine(swapEngine);
		NPV = Swap.NPV();

		std::cout << ccpTermStructure->discount(Date(3, Sep, 2021), true);
		std::cout << std::endl;
		std::cout << ccpTermStructure->discount(Date(3, Sep, 2021), false);
		std::cout << std::endl;

		std::cout << "Vanilla Swap Price (Korea Central Celearing Counterparty in KRX) " << std::endl;
		std::cout << "Precision : 0.E-12 " << std::endl;

		std::cout << setprecision(15) << std::endl;
		std::cout << "Settle Date " << schedule.startDate() << std::endl;
		std::cout << "Mat Date " << schedule.endDate() << std::endl;


		std::cout << Swap.fixedLegNPV() << std::endl;
		std::cout << Swap.floatingLegNPV() << std::endl;

		std::cout << Swap.fixedLegBPS() << std::endl;
		std::cout << Swap.floatingLegBPS() << std::endl;

		std::cout << "Swap Fair NPV  : " << Swap.NPV() << std::endl;
		std::cout << "Swap Fair Rate : " << Swap.fairRate() << std::endl;
		std::cout << "Swap Fair Spread  : " << Swap.fairSpread() << std::endl;

		std::cout << std::endl;
		//std::cout << "Vanilla Swap Greeks for Trading and Hedging " << std::endl;

		return 0;
	}
	catch (int e)
	{
		std::cout << e << std::endl;
		return 1;
	}



}
