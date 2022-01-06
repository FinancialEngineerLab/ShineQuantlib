#pragma once

#include <ql/quantlib.hpp>
#include <ql/indexes/ibor/krwcd.hpp>

#ifdef BOOST_MSVC
#include <ql/auto_link.hpp>
#endif

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "utils.hpp"

using namespace QuantLib;
using namespace std;
using namespace ext;


#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib
{
	ThreadKey sessionId() { return (); }
}
#endif



struct CurveData
{
	ext::shared_ptr<SimpleQuote> krw01dCl;
	ext::shared_ptr<SimpleQuote> krw03mCD;
	ext::shared_ptr<SimpleQuote> krw06mSW;
	ext::shared_ptr<SimpleQuote> krw09mSW;
	ext::shared_ptr<SimpleQuote> krw01ySW;
	ext::shared_ptr<SimpleQuote> krw18mSW;
	ext::shared_ptr<SimpleQuote> krw02ySW;
	ext::shared_ptr<SimpleQuote> krw03ySW;
	ext::shared_ptr<SimpleQuote> krw04ySW;
	ext::shared_ptr<SimpleQuote> krw05ySW;
	ext::shared_ptr<SimpleQuote> krw06ySW;
	ext::shared_ptr<SimpleQuote> krw07ySW;
	ext::shared_ptr<SimpleQuote> krw08ySW;
	ext::shared_ptr<SimpleQuote> krw09ySW;
	ext::shared_ptr<SimpleQuote> krw10ySW;
	ext::shared_ptr<SimpleQuote> krw12ySW;
	ext::shared_ptr<SimpleQuote> krw15ySW;
	ext::shared_ptr<SimpleQuote> krw20ySW;

	CurveData() :
		krw01dCl(new SimpleQuote()),
		krw03mCD(new SimpleQuote()),
		krw06mSW(new SimpleQuote()),
		krw09mSW(new SimpleQuote()),
		krw01ySW(new SimpleQuote()),
		krw18mSW(new SimpleQuote()),
		krw02ySW(new SimpleQuote()),
		krw03ySW(new SimpleQuote()),
		krw04ySW(new SimpleQuote()),
		krw05ySW(new SimpleQuote()),
		krw06ySW(new SimpleQuote()),
		krw07ySW(new SimpleQuote()),
		krw08ySW(new SimpleQuote()),
		krw09ySW(new SimpleQuote()),
		krw10ySW(new SimpleQuote()),
		krw12ySW(new SimpleQuote()),
		krw15ySW(new SimpleQuote()),
		krw20ySW(new SimpleQuote())
	{
	}

	ext::shared_ptr<SimpleQuote>& operator[] (int index) { return *(&krw01dCl + index); }

};

class IRSPricer
{
public:

	void sampleMktData(CurveData &cd, std::string fileIRPath)
	{
		std::vector<double> irsData = dataReader.readRateCSV(fileIRPath);
		(*cd.krw01dCl) = irsData[0];
		(*cd.krw03mCD) = irsData[1];
		(*cd.krw06mSW) = irsData[2];
		(*cd.krw09mSW) = irsData[3];
		(*cd.krw01ySW) = irsData[4];
		(*cd.krw18mSW) = irsData[5];
		(*cd.krw02ySW) = irsData[6];
		(*cd.krw03ySW) = irsData[7];
		(*cd.krw04ySW) = irsData[8];
		(*cd.krw05ySW) = irsData[9];
		(*cd.krw06ySW) = irsData[10];
		(*cd.krw07ySW) = irsData[11];
		(*cd.krw08ySW) = irsData[12];
		(*cd.krw09ySW) = irsData[13];
		(*cd.krw10ySW) = irsData[14];
		(*cd.krw12ySW) = irsData[15];
		(*cd.krw15ySW) = irsData[16];
		(*cd.krw20ySW) = irsData[17];
	};

	void fixingMaker(std::string fileFixingPath)
	{
		fixingData = dataReader.readfixingCSV(fileFixingPath);
	}

	/****************************************************************************/
	std::vector<ext::shared_ptr<RateHelper>> buildCurve(Date input, CurveData &cd)
	{
		ccpInstruments.clear();
		//Settings::instance().evaluationDate() = input;
		//input = Settings::instance().evaluationDate();

		//Date todaysDate(2, Sep, 2021);
		Calendar calendar = publicCalendar;
		calendar.addHoliday(Date(4, Oct, 2021));
		calendar.addHoliday(Date(11, Oct, 2021));
		calendar.addHoliday(Date(9, Mar, 2022));
		calendar.addHoliday(Date(1, Jun, 2022));
		calendar.addHoliday(Date(10, Oct, 2022));
		calendar.addHoliday(Date(3, Mar, 2025));
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
		calendar.addHoliday(Date(10, Apr, 2024));
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
		calendar.addHoliday(Date(4, Oct, 2038));
		calendar.addHoliday(Date(11, Oct, 2038));
		calendar.addHoliday(Date(10, Oct, 2039));
		calendar.addHoliday(Date(11, Apr, 2040));
		calendar.addHoliday(Date(3, Mar, 2042));

		DayCounter dayCounter = Actual365Fixed();
		Integer fixingDays = 1;
		Date settlementDate = calendar.advance(input, fixingDays, Days);

		std::vector<pair<Period, Real> > inputVec;
		inputVec.push_back(make_pair(Period(1 * Days), cd.krw01dCl.get()->value()));
		inputVec.push_back(make_pair(Period(3 * Months), cd.krw03mCD.get()->value()));
		inputVec.push_back(make_pair(Period(6 * Months), cd.krw06mSW.get()->value()));
		inputVec.push_back(make_pair(Period(9 * Months), cd.krw09mSW.get()->value()));
		inputVec.push_back(make_pair(Period(12 * Months), cd.krw01ySW.get()->value()));
		inputVec.push_back(make_pair(Period(18 * Months), cd.krw18mSW.get()->value()));
		inputVec.push_back(make_pair(Period(24 * Months), cd.krw02ySW.get()->value()));
		inputVec.push_back(make_pair(Period(36 * Months), cd.krw03ySW.get()->value()));
		inputVec.push_back(make_pair(Period(48 * Months), cd.krw04ySW.get()->value()));
		inputVec.push_back(make_pair(Period(60 * Months), cd.krw05ySW.get()->value()));
		inputVec.push_back(make_pair(Period(72 * Months), cd.krw06ySW.get()->value()));
		inputVec.push_back(make_pair(Period(84 * Months), cd.krw07ySW.get()->value()));
		inputVec.push_back(make_pair(Period(96 * Months), cd.krw08ySW.get()->value()));
		inputVec.push_back(make_pair(Period(108 * Months), cd.krw09ySW.get()->value()));
		inputVec.push_back(make_pair(Period(120 * Months), cd.krw10ySW.get()->value()));
		inputVec.push_back(make_pair(Period(144 * Months), cd.krw12ySW.get()->value()));
		inputVec.push_back(make_pair(Period(180 * Months), cd.krw15ySW.get()->value()));
		inputVec.push_back(make_pair(Period(240 * Months), cd.krw20ySW.get()->value()));

		// Helpers & Instruments //
		//cout << fixingData[today] << endl;
		Frequency swFixedLegFrequency = Quarterly;
		BusinessDayConvention swFixedLegConvention = ModifiedFollowing;
		DayCounter swFixedLegDayCounter = Actual365Fixed();

		ext::shared_ptr<IborIndex> krwCD(new KrwCD3M());
		//krwCD->addFixing(input,fixingData[input], true);
		ext::shared_ptr<RateHelper> dp01d(new DepositRateHelper(Handle<Quote>(cd.krw01dCl),
			Period(1 * Days), 1, calendar, ModifiedFollowing, true, Actual365Fixed()));
		ccpInstruments.push_back(dp01d);

		for (Size i = 0; i <= 80; i++)
		{
			for (Size j = 1; j <= inputVec.size() - 1; j++)
			{
				if (Period((i + 1) * 3 * Months) == inputVec[j].first)
				{
					ext::shared_ptr<Quote> tempPar(new SimpleQuote(inputVec[j].second));
					ext::shared_ptr<RateHelper> tempHelper(new SwapRateHelper(Handle<Quote>(tempPar), inputVec[j].first,
						publicCalendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));
					ccpInstruments.push_back(tempHelper);
				}

				if ((j < 16) && ((Period((i + 1) * 3 * Months) < inputVec[j + 1].first) && (Period((i + 1) * 3 * Months) > inputVec[j].first)))
				{
					Date tempDt1 = publicCalendar.advance(settlementDate, inputVec[j + 1].first);
					Date tempDt0 = publicCalendar.advance(settlementDate, inputVec[j].first);
					Date tempDtn = publicCalendar.advance(settlementDate, (i + 1) * 3, Months);
					Rate intpl = inputVec[j].second + (inputVec[j + 1].second - inputVec[j].second) *
						(tempDtn.serialNumber() - tempDt0.serialNumber()) / (tempDt1.serialNumber() - tempDt0.serialNumber());

					ext::shared_ptr<Quote> tempPar(new SimpleQuote(intpl));
					ext::shared_ptr<RateHelper> tempHelper(new SwapRateHelper(Handle<Quote>(tempPar), (i + 1) * 3 * Months,
						publicCalendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));
					ccpInstruments.push_back(tempHelper);

				}
				// 15 ~ 20
				if ((j >= 16) && (j < 17) && (Period((i + 1) * 3 * Months) < inputVec[17].first) && (Period((i + 1) * 3 * Months) > inputVec[16].first))
				{
					Date tempDt1 = publicCalendar.advance(settlementDate, inputVec[17].first);
					Date tempDt0 = publicCalendar.advance(settlementDate, inputVec[16].first);
					Date tempDtn = publicCalendar.advance(settlementDate, (i + 1) * 3, Months);

					Rate intpl = inputVec[16].second + (inputVec[17].second - inputVec[16].second) *
						(tempDtn.serialNumber() - tempDt0.serialNumber()) / (tempDt1.serialNumber() - tempDt0.serialNumber());

					ext::shared_ptr<Quote> tempPar(new SimpleQuote(intpl));
					ext::shared_ptr<RateHelper> tempHelper(new SwapRateHelper(Handle<Quote>(tempPar), (i + 1) * 3 * Months,
						publicCalendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));
					ccpInstruments.push_back(tempHelper);
				}
			}
		}
		return ccpInstruments;
		//double tolerance = 1.0e-15;
		//ext::shared_ptr<YieldTermStructure> ccpTermStructure(new PiecewiseYieldCurve<Discount, LogLinear>(settlementDate, ccpInstruments, Actual365Fixed(), tolerance));
		//ccpTermStructure->enableExtrapolation(true);
		//return ccpTermStructure;
	}


	/****************************************************************************/
	std::vector<ext::shared_ptr<RateHelper>> buildCurve(Date input,Date past, CurveData &cd)
	{
		ccpInstruments.clear();
		Settings::instance().evaluationDate() = input;
		input = Settings::instance().evaluationDate();

		//Date todaysDate(2, Sep, 2021);
		Calendar calendar = publicCalendar;
		calendar.addHoliday(Date(4, Oct, 2021));
		calendar.addHoliday(Date(11, Oct, 2021));
		calendar.addHoliday(Date(9, Mar, 2022));
		calendar.addHoliday(Date(1, Jun, 2022));
		calendar.addHoliday(Date(10, Oct, 2022));
		calendar.addHoliday(Date(3, Mar, 2025));
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
		calendar.addHoliday(Date(10, Apr, 2024));
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
		calendar.addHoliday(Date(4, Oct, 2038));
		calendar.addHoliday(Date(11, Oct, 2038));
		calendar.addHoliday(Date(10, Oct, 2039));
		calendar.addHoliday(Date(11, Apr, 2040));
		calendar.addHoliday(Date(3, Mar, 2042));

		DayCounter dayCounter = Actual365Fixed();
		Integer fixingDays = 1;
		Date settlementDate = calendar.advance(input, fixingDays, Days);

		std::vector<pair<Period, Real> > inputVec;
		inputVec.push_back(make_pair(Period(1 * Days), cd.krw01dCl.get()->value()));
		inputVec.push_back(make_pair(Period(3 * Months), cd.krw03mCD.get()->value()));
		inputVec.push_back(make_pair(Period(6 * Months), cd.krw06mSW.get()->value()));
		inputVec.push_back(make_pair(Period(9 * Months), cd.krw09mSW.get()->value()));
		inputVec.push_back(make_pair(Period(12 * Months), cd.krw01ySW.get()->value()));
		inputVec.push_back(make_pair(Period(18 * Months), cd.krw18mSW.get()->value()));
		inputVec.push_back(make_pair(Period(24 * Months), cd.krw02ySW.get()->value()));
		inputVec.push_back(make_pair(Period(36 * Months), cd.krw03ySW.get()->value()));
		inputVec.push_back(make_pair(Period(48 * Months), cd.krw04ySW.get()->value()));
		inputVec.push_back(make_pair(Period(60 * Months), cd.krw05ySW.get()->value()));
		inputVec.push_back(make_pair(Period(72 * Months), cd.krw06ySW.get()->value()));
		inputVec.push_back(make_pair(Period(84 * Months), cd.krw07ySW.get()->value()));
		inputVec.push_back(make_pair(Period(96 * Months), cd.krw08ySW.get()->value()));
		inputVec.push_back(make_pair(Period(108 * Months), cd.krw09ySW.get()->value()));
		inputVec.push_back(make_pair(Period(120 * Months), cd.krw10ySW.get()->value()));
		inputVec.push_back(make_pair(Period(144 * Months), cd.krw12ySW.get()->value()));
		inputVec.push_back(make_pair(Period(180 * Months), cd.krw15ySW.get()->value()));
		inputVec.push_back(make_pair(Period(240 * Months), cd.krw20ySW.get()->value()));

		// Helpers & Instruments //
		//cout << fixingData[today] << endl;
		Frequency swFixedLegFrequency = Quarterly;
		BusinessDayConvention swFixedLegConvention = ModifiedFollowing;
		DayCounter swFixedLegDayCounter = Actual365Fixed();

		ext::shared_ptr<IborIndex> krwCD(new KrwCD3M());
		krwCD->addFixing(input,fixingData[input], true);
		ext::shared_ptr<RateHelper> dp01d(new DepositRateHelper(Handle<Quote>(cd.krw01dCl),
			Period(1 * Days), 1, calendar, ModifiedFollowing, true, Actual365Fixed()));

		//ext::shared_ptr<RateHelper> dp01d(new SwapRateHelper(Handle<Quote>(cd.krw01dCl),
		//	Period(1 * Days), publicCalendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));

		for (Size i = 0; i <= 80; i++)
		{
			for (Size j = 1; j <= inputVec.size() - 1; j++)
			{
				if (Period((i + 1) * 3 * Months) == inputVec[j].first)
				{
					ext::shared_ptr<Quote> tempPar(new SimpleQuote(inputVec[j].second));
					ext::shared_ptr<RateHelper> tempHelper(new SwapRateHelper(Handle<Quote>(tempPar), inputVec[j].first,
						publicCalendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));
					ccpInstruments.push_back(tempHelper);
				}

				if ((j < 16) && ((Period((i + 1) * 3 * Months) < inputVec[j + 1].first) && (Period((i + 1) * 3 * Months) > inputVec[j].first)))
				{
					Date tempDt1 = publicCalendar.advance(settlementDate, inputVec[j + 1].first);
					Date tempDt0 = publicCalendar.advance(settlementDate, inputVec[j].first);
					Date tempDtn = publicCalendar.advance(settlementDate, (i + 1) * 3, Months);
					Rate intpl = inputVec[j].second + (inputVec[j + 1].second - inputVec[j].second) *
						(tempDtn.serialNumber() - tempDt0.serialNumber()) / (tempDt1.serialNumber() - tempDt0.serialNumber());

					ext::shared_ptr<Quote> tempPar(new SimpleQuote(intpl));
					ext::shared_ptr<RateHelper> tempHelper(new SwapRateHelper(Handle<Quote>(tempPar), (i + 1) * 3 * Months,
						publicCalendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));
					ccpInstruments.push_back(tempHelper);

				}
				// 15 ~ 20
				if ((j >= 16) && (j < 17) && (Period((i + 1) * 3 * Months) < inputVec[17].first) && (Period((i + 1) * 3 * Months) > inputVec[16].first))
				{
					Date tempDt1 = publicCalendar.advance(settlementDate, inputVec[17].first);
					Date tempDt0 = publicCalendar.advance(settlementDate, inputVec[16].first);
					Date tempDtn = publicCalendar.advance(settlementDate, (i + 1) * 3, Months);

					Rate intpl = inputVec[16].second + (inputVec[17].second - inputVec[16].second) *
						(tempDtn.serialNumber() - tempDt0.serialNumber()) / (tempDt1.serialNumber() - tempDt0.serialNumber());

					ext::shared_ptr<Quote> tempPar(new SimpleQuote(intpl));
					ext::shared_ptr<RateHelper> tempHelper(new SwapRateHelper(Handle<Quote>(tempPar), (i + 1) * 3 * Months,
						publicCalendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));
					ccpInstruments.push_back(tempHelper);
				}
			}
		}
		krwCD->clearFixings();
		return ccpInstruments;
		//double tolerance = 1.0e-15;
		//ext::shared_ptr<YieldTermStructure> ccpTermStructure(new PiecewiseYieldCurve<Discount, LogLinear>(settlementDate, ccpInstruments, Actual365Fixed(), tolerance));
		//ccpTermStructure->enableExtrapolation(true);
		//return ccpTermStructure;
	}


	Real Calculate(Date inputtoday,Date tradeDate,Rate fixedRate, double tenor,
		VanillaSwap::Type position, Real notional, Rate spread)
	{
		Settings::instance().resetEvaluationDate();
		Settings::instance().evaluationDate() = inputtoday;
		inputtoday = Settings::instance().evaluationDate();

		Calendar calendar = publicCalendar;
		Integer fixingDays = 1;
		Date settlementDate = calendar.advance(tradeDate, fixingDays, Days);

		ext::shared_ptr<IborIndex> krwCD(new KrwCD3M());
		Date maturity = calendar.advance(settlementDate, tenor * 12, Months);

		std::vector<ext::shared_ptr<RateHelper>> instrument= buildCurve(tradeDate,tradeDate, cd);
		double tolerance = 1.0e-15;
		ext::shared_ptr<YieldTermStructure> ccpTermStructure(new PiecewiseYieldCurve<Discount, Linear>
			(settlementDate, instrument, Actual365Fixed(), tolerance));
		ccpTermStructure->enableExtrapolation(true);

		boost::shared_ptr<YieldTermStructure> curve = ccpTermStructure;
		RelinkableHandle<YieldTermStructure> discountingDiscountCurve;
		discountingDiscountCurve.linkTo(curve);
		cout << "reference date" << discountingDiscountCurve->referenceDate() << std::endl;
		cout << discountingDiscountCurve->discount(Date(15, Oct, 2021), true) << std::endl;
		cout << discountingDiscountCurve->discount(Date(18, Oct, 2021), true) << std::endl;

		DayCounter dayCounter = Actual365Fixed();
		Frequency fixedLegFrequency = Quarterly;
		BusinessDayConvention dayConvention = ModifiedFollowing;
		cout << maturity << endl;
		Schedule schedule(settlementDate, maturity, Period(fixedLegFrequency),
			calendar, dayConvention, dayConvention, DateGeneration::Forward, true);


		ext::shared_ptr<IborIndex> krwCDsw(new KrwCD3M(discountingDiscountCurve));
		krwCDsw->clearFixings();
		//krwCDsw->addFixing(krwCDsw->fixingDate(settlementDate), fixingData[tradeDate], true);
		std::vector<Date> pastFixingDates;
		std::vector<Rate> pastFixingRates;
		for (Size i = 0; i < 365; ++i)
		{
			if (publicCalendar.isBusinessDay(inputtoday - i * Days))
			{
				pastFixingDates.push_back(Date(Date(inputtoday - i * Days).serialNumber()));
				pastFixingRates.push_back(fixingData[tradeDate]);
			}
		}
		TimeSeries<Real> pastFixings(pastFixingDates.begin(), pastFixingDates.end(), pastFixingRates.begin());
		krwCDsw->addFixings(pastFixings);

		cout << settlementDate << endl;
		cout << fixingData[tradeDate] << endl;

		Real NPV;
		Rate fairRate;

		VanillaSwap Swap(position, notional, schedule, fixedRate,
			dayCounter, schedule, krwCDsw, spread, dayCounter);

		ext::shared_ptr<PricingEngine> swapEngine(new DiscountingSwapEngine(discountingDiscountCurve));
		Swap.setPricingEngine(swapEngine);
		NPV = Swap.NPV();

		return NPV;
		
	}

	/* Overload, focus ! */
	Real Calculate(Date inputToday, std::string BookId)
	{

		std::string strYear = BookId.substr(0, 2);
		std::string strMonth = BookId.substr(2, 2);
		std::string strDay = BookId.substr(4, 2);
		std::string strTenor = BookId.substr(9, 2);
		std::string strCoupon = BookId.substr(12, 6);
		std::string strType = BookId.substr(18, 1);
		Integer day = stoi(strDay);
		Month month = Month(stoi(strMonth));
		Integer year = stoi(strYear) + 2000;
		Date tradeDate = Date(day, month, year);
		Integer fixingDays = 1;
		Date settlementDate = publicCalendar.advance(tradeDate, fixingDays, Days);
		double tenor = stoi(strTenor) * 12.0;
		Rate coupon = stod(strCoupon) / 1000000.0;

		VanillaSwap::Type positionId = VanillaSwap::Receiver;
		if (strType == "R")
		{
			VanillaSwap::Type positionId = VanillaSwap::Receiver;
		}
		else if (strType == "P")
		{
			VanillaSwap::Type positionId = VanillaSwap::Payer;
		}

		//ext::shared_ptr<IborIndex> krwCD(new KrwCD3M());
		Date maturity = publicCalendar.advance(settlementDate, tenor, Months);
		Date curveSettlementDate = publicCalendar.advance(inputToday, fixingDays, Days);
		//Date curvePreviousDate = publicCalendar.advance(inputToday, -1, Days);

		//std::vector<ext::shared_ptr<RateHelper>> instrument = buildCurve(inputToday, cd);
		std::vector<ext::shared_ptr<RateHelper>> instrument = buildCurve(inputToday, inputToday, cd);
		double tolerance = 1.0e-15;
		ext::shared_ptr<YieldTermStructure> ccpTermStructure(
			new PiecewiseYieldCurve<Discount, LogLinear>(curveSettlementDate, instrument,
				Actual365Fixed(), tolerance));//
		//ext::shared_ptr<YieldTermStructure> ccpTermStructure(
		//	new PiecewiseYieldCurve<Discount, LogLinear>(0, publicCalendar, instrument,
		//		Actual365Fixed(), tolerance));//
		ccpTermStructure->enableExtrapolation(true);
		boost::shared_ptr<YieldTermStructure> curve_str = ccpTermStructure;
		
		RelinkableHandle<YieldTermStructure> discountingTermStuctureID;
		discountingTermStuctureID.linkTo(curve_str);
		RelinkableHandle<YieldTermStructure> forwardingTermStuctureID;
		forwardingTermStuctureID.linkTo(curve_str);

		// Fixed Leg //
		DayCounter dayCounter = Actual365Fixed();
		Frequency fixedLegFrequency = Quarterly;
		BusinessDayConvention dayConvention = ModifiedFollowing;
		Schedule schedule(settlementDate, maturity, Period(fixedLegFrequency), publicCalendar,
			dayConvention, dayConvention, DateGeneration::Forward, true);

		ext::shared_ptr<IborIndex> krwCDsw(new KrwCD3M(discountingTermStuctureID));
		//krwCDsw->clearFixings();
		krwCDsw->addFixing(tradeDate, fixingData[tradeDate], true);
		//krwCDsw->addFixing(publicCalendar.advance(tradeDate, 3 , Months), fixingData[tradeDate], true);
		//krwCDsw->addFixing(inputToday, fixingData[tradeDate], true);
		/*
		std::vector<Date> pastFixingDates;
		std::vector<Rate> pastFixingRates;
		for (Size i = 0; i < 7300; i++)
		{
			Date tempDate = publicCalendar.advance(tradeDate, i*3, Months);
			if (tempDate < inputToday)
			{
				pastFixingDates.push_back(Date(tempDate.serialNumber()));
				pastFixingRates.push_back(fixingData[tempDate]);
			}
			else
			{
				break;
			}
		}
		TimeSeries<Real> pastFixings(pastFixingDates.begin(), pastFixingDates.end(), pastFixingRates.begin());
		krwCDsw->addFixings(pastFixings);
		*/
		//cout << inputToday - tradeDate << endl;
		/*
		for (Size i = 0; i < 80;i++)
		{
			if ((publicCalendar.advance(tradeDate,i*3,Months) < inputToday)&
				(inputToday < publicCalendar.advance(tradeDate, (i+1)*3, Months)))
			{
				krwCDsw->clearFixings();
				krwCDsw->addFixing(publicCalendar.advance(tradeDate, 3*i, Months),
					fixingData[publicCalendar.advance(tradeDate, 3*i, Months)], true);
			}
			else
			{
				continue;
			}
		}
		*/
		
		Real NPV;
		Rate fairRate;
		Real spread = 0.0;
		Settings::instance().evaluationDate() = inputToday;
		inputToday = Settings::instance().evaluationDate();
		VanillaSwap Swap(positionId, 47000000000, schedule, coupon,
			dayCounter, schedule, krwCDsw, spread, dayCounter);

		ext::shared_ptr<PricingEngine> swapEngine(new DiscountingSwapEngine(discountingTermStuctureID));
		//ext::shared_ptr<PricingEngine> swapEngine(new DiscountingSwapEngine(discountingTermStuctureID));
		Swap.setPricingEngine(swapEngine);
		//cout << discountingTermStuctureID->discount(Date(12, Jan, 2022), true) << endl;
		//cout << discountingTermStuctureID->discount(Date(11, Jan, 2022), true) << endl;
		//Swap.registerWith(Settings::instance().evaluationDate()= inputToday);

		cout << Settings::instance().evaluationDate() << endl;
		cout << Swap.valuationDate() << endl;
		cout << Swap.startDate() << endl;
		cout << Swap.maturityDate() << endl;

		NPV = Swap.NPV();
		return NPV;
	}

	/* Overload */
	Real Calculate(Date today,Rate fixedRate, double tenor, VanillaSwap::Type position,
		Real notional, Rate spread)
	{
		Settings::instance().evaluationDate() = today;
		today = Settings::instance().evaluationDate();

		Calendar calendar = SouthKorea();

		ext::shared_ptr<IborIndex> krwCD(new KrwCD3M());
		Integer fixingDays = 1;
		//Date settlementDate = calendar.advance(today, fixingDays, Days);
		Date settlementDate = calendar.advance(today, fixingDays, Days);
		//sampleMktData(cd, IRfilePath);

		std::vector<ext::shared_ptr<RateHelper>> instrument = buildCurve(today, cd);
		double tolerance = 1.0e-15;
		ext::shared_ptr<YieldTermStructure> ccpTermStructure(new PiecewiseYieldCurve<Discount, LogLinear>(settlementDate, instrument, Actual365Fixed(), tolerance));
		ccpTermStructure->enableExtrapolation(true);
		boost::shared_ptr<YieldTermStructure> curve_str = ccpTermStructure;
		RelinkableHandle<YieldTermStructure> discountingTermStuctureID;
		discountingTermStuctureID.linkTo(curve_str);


		// Fixed Leg //
		DayCounter dayCounter = Actual365Fixed();
		Frequency fixedLegFrequency = Quarterly;
		BusinessDayConvention dayConvention = ModifiedFollowing;
		//DayCounter dayCounter = Actual365Fixed();
		Date maturity = calendar.advance(settlementDate, tenor * 12, Months);
		Schedule schedule(settlementDate, maturity, Period(fixedLegFrequency), calendar, dayConvention, dayConvention, DateGeneration::Forward, true);
		ext::shared_ptr<IborIndex> krwCDsw(new KrwCD3M(discountingTermStuctureID));
		krwCDsw->clearFixings();
		krwCDsw->addFixing(settlementDate, cd.krw03mCD->value(), true);
		//krwCDsw->addFixing(settlementDate, cd.krw03mCD->value(), true);
		//cout << cd.krw03mCD->value() << endl;

		Real NPV;
		Rate fairRate;
		VanillaSwap Swap(position, notional, schedule, fixedRate, dayCounter, schedule, krwCDsw, spread, dayCounter);
		ext::shared_ptr<PricingEngine> swapEngine(new DiscountingSwapEngine(discountingTermStuctureID));

		//cout << discountingTermStuctureID->discount(Date(12, Jan, 2022), true) << endl;
		Swap.setPricingEngine(swapEngine);
		NPV = Swap.NPV();
		return NPV;
	}


	void GreeksMatrix()
	{
		Real tempSwap;
		Real tempSwapUp;
		Real tempSwapDn;
		Real tempRho;
		Rate tempFixed;
		Rate spread = 0.0;
		std::vector<vector<Real>> tempMat(18, vector<Real>(18, 0));
		Real notional = 100000000;
		VanillaSwap::Type type = VanillaSwap::Receiver;

		std::vector<double> tempTenors;
		/*tempTenors.push_back(0);
		tempTenors.push_back(3);
		tempTenors.push_back(6);
		tempTenors.push_back(9);
		tempTenors.push_back(12);
		tempTenors.push_back(18);
		tempTenors.push_back(24);
		tempTenors.push_back(36);
		tempTenors.push_back(48);
		tempTenors.push_back(60);
		tempTenors.push_back(72);
		tempTenors.push_back(84);
		tempTenors.push_back(96);
		tempTenors.push_back(108);
		tempTenors.push_back(120);
		tempTenors.push_back(144);
		tempTenors.push_back(180);
		tempTenors.push_back(240);*/
		tempTenors.push_back(0);
		tempTenors.push_back(0.25);
		tempTenors.push_back(0.5);
		tempTenors.push_back(0.75);
		tempTenors.push_back(1);
		tempTenors.push_back(1.5);
		tempTenors.push_back(2);
		tempTenors.push_back(3);
		tempTenors.push_back(4);
		tempTenors.push_back(5);
		tempTenors.push_back(6);
		tempTenors.push_back(7);
		tempTenors.push_back(8);
		tempTenors.push_back(9);
		tempTenors.push_back(10);
		tempTenors.push_back(12);
		tempTenors.push_back(15);
		tempTenors.push_back(20);


		for (Size i = 1; i < 18; i++)
		{
			tempFixed = cd[i].get()->value();
			tempSwap = Calculate(todayRho,tempFixed, tempTenors[i], type,notional,spread);
			for (Size j = 1; j <18; j++)
			{
				if (i >= j)
				{
					(*cd[j]) = cd[j]->value() + 0.0001;
					tempSwapUp = Calculate(todayRho,  tempFixed, tempTenors[i], type, notional, spread);
					(*cd[j]) = cd[j]->value() - 0.0002;
					tempSwapDn = Calculate(todayRho,  tempFixed, tempTenors[i], type, notional, spread);
					tempRho = (tempSwapDn - tempSwapUp) / 0.0002 / 100000000;
					tempMat[i-1][j-1] = tempRho;
					(*cd[j]) = cd[j]->value() + 0.0001;
				}
				//else
				//{
				//	tempMat[i].push_back(0);
				//	continue;
				//}
			}
		}
		DurationMatrix = tempMat;

	}

	std::vector<Real> GreeksKeyRate(std::string BookId)
	{
		// if bookid 211014IRS20Y018000R
		Real tempSwap;
		Real tempSwapUp;
		Real tempSwapDn;
		Real tempRho;
		Real spread = 0.0;
		std::string strYear = BookId.substr(0, 2);
		std::string strMonth = BookId.substr(2, 2);
		std::string strDay = BookId.substr(4, 2);
		std::string strTenor = BookId.substr(9, 2);
		std::string strCoupon = BookId.substr(12, 6);
		std::string strType = BookId.substr(18, 1);
		Integer day = stoi(strDay);
		Month month = Month(stoi(strMonth));
		Integer year = stoi(strYear) + 2000;
		Date settlementDate = Date(day, month , year);
		Calendar calendar = SouthKorea();
		settlementDate = calendar.advance(todayRho, 1, Days);

		Integer tenor = stod(strTenor) * 12.0;
		Date maturity = settlementDate + Period(tenor * Months);
		Rate coupon = stod(strCoupon) / 1000000.0;
		VanillaSwap::Type positionId= VanillaSwap::Receiver;
		if (strType == "P")
		{
			VanillaSwap::Type positionId = VanillaSwap::Payer;
		}
		else
		{
			VanillaSwap::Type positionId = VanillaSwap::Receiver;
		}
		Real notional = 100000000;
		setTenors();
		
		std::vector<Real> greeksById(18);
		greeksById[0] = 0;

		for (Size i = 0; i < 18; i++)
		{
			(*cd[i]) = cd[i]->value() + 0.0001;
			tempSwapUp = Calculate(todayRho, settlementDate, coupon, tenor, positionId, notional, spread);
			(*cd[i]) = cd[i]->value() - 0.0002;
			tempSwapDn = Calculate(todayRho, settlementDate, coupon, tenor, positionId, notional, spread);
			tempRho = (tempSwapDn - tempSwapUp) / 0.0002;
			greeksById[i] = tempRho;
			(*cd[i]) = cd[i]->value() + 0.0001;
		}
		return greeksById;
	}


	pair<std::string, Real> greeksTotalRho(std::string bookId)
	{
		std::vector<Real> rhoMat = GreeksKeyRate(bookId);
		Real totalSum = 0;
		for (int i = 0; i < 18; i++)
		{
			totalSum += rhoMat[i];
		}
		std::pair<std::string, Real> result = make_pair(bookId, totalSum);
		return result;
	}



	//////////////////////////////////////////////////////////////////////

	IRSPricer(Date todayDate, std::string IRPath,std::string fixingPath, std::string VolPath) :
		todayRho(todayDate), IRfilePath(IRPath), fixingFilePath(fixingPath), IRVolfilePath(VolPath)
	{
		//memset(&cd, 0, sizeof(cd));
		sampleMktData(cd, IRPath);
		fixingMaker(fixingPath);
		todayFixing = cd.krw03mCD->value();
		publicCalendarMaker();
		ext::shared_ptr<YieldTermStructure> buildreset;
		ext::shared_ptr<YieldTermStructure> buildCurve = buildreset;
		buildCurve.reset();
	}

	/*
	std::vector<double> setTenors()
	{
		Tenors.clear();
		Tenors.push_back(0);
		Tenors.push_back(3); Tenors.push_back(6);  Tenors.push_back(9);
		Tenors.push_back(12);  Tenors.push_back(18); Tenors.push_back(24);
		Tenors.push_back(36);  Tenors.push_back(48); Tenors.push_back(60);
		Tenors.push_back(72);  Tenors.push_back(84); Tenors.push_back(96);
		Tenors.push_back(108); Tenors.push_back(120); Tenors.push_back(144);
		Tenors.push_back(180); Tenors.push_back(240);
		return Tenors;
	}
	*/
	std::vector<double> setTenors()
	{
		Tenors.clear();
		Tenors.push_back(0);
		Tenors.push_back(0.25); Tenors.push_back(0.5);  Tenors.push_back(0.75);
		Tenors.push_back(1);  Tenors.push_back(1.5); Tenors.push_back(2);
		Tenors.push_back(3);  Tenors.push_back(4); Tenors.push_back(5);
		Tenors.push_back(6);  Tenors.push_back(7); Tenors.push_back(8);
		Tenors.push_back(9); Tenors.push_back(10); Tenors.push_back(12);
		Tenors.push_back(15); Tenors.push_back(20);
		return Tenors;
	}

	void publicCalendarMaker()
	{
		publicCalendar = SouthKorea();

		publicCalendar.addHoliday(Date(4, Oct, 2021));
		publicCalendar.addHoliday(Date(11, Oct, 2021));
		publicCalendar.addHoliday(Date(9, Mar, 2022));
		publicCalendar.addHoliday(Date(1, Jun, 2022));
		publicCalendar.addHoliday(Date(10, Oct, 2022));
		publicCalendar.addHoliday(Date(3, Mar, 2025));
		publicCalendar.addHoliday(Date(2, Mar, 2026));
		publicCalendar.addHoliday(Date(3, Jun, 2026));
		publicCalendar.addHoliday(Date(17, Aug, 2026));
		publicCalendar.addHoliday(Date(5, Oct, 2026));
		publicCalendar.addHoliday(Date(3, Mar, 2027));
		publicCalendar.addHoliday(Date(16, Aug, 2027));
		publicCalendar.addHoliday(Date(4, Oct, 2027));
		publicCalendar.addHoliday(Date(11, Oct, 2027));
		publicCalendar.addHoliday(Date(3, Mar, 2031));
		publicCalendar.addHoliday(Date(16, Aug, 2032));
		publicCalendar.addHoliday(Date(4, Oct, 2032));
		publicCalendar.addHoliday(Date(11, Oct, 2032));
		publicCalendar.addHoliday(Date(10, Oct, 2033));
		publicCalendar.addHoliday(Date(3, Mar, 2036));
		publicCalendar.addHoliday(Date(7, Oct, 2036));
		publicCalendar.addHoliday(Date(2, Mar, 2037));
		publicCalendar.addHoliday(Date(17, Aug, 2037));
		publicCalendar.addHoliday(Date(5, Oct, 2037));
		publicCalendar.addHoliday(Date(16, Aug, 2038));
		publicCalendar.addHoliday(Date(4, Oct, 2038));
		publicCalendar.addHoliday(Date(11, Oct, 2038));
		publicCalendar.addHoliday(Date(10, Oct, 2039));
		// Election //
		publicCalendar.addHoliday(Date(10, Apr, 2024));
		publicCalendar.addHoliday(Date(3, Mar, 2025));
		publicCalendar.addHoliday(Date(12, Apr, 2028));
		publicCalendar.addHoliday(Date(12, Jun, 2030));
		publicCalendar.addHoliday(Date(3, Mar, 2032));
		publicCalendar.addHoliday(Date(14, Apr, 2032));
		publicCalendar.addHoliday(Date(31, May, 2034));
		publicCalendar.addHoliday(Date(9, Apr, 2036));
		publicCalendar.addHoliday(Date(4, Mar, 2037));
		publicCalendar.addHoliday(Date(2, Jun, 2038));
		publicCalendar.addHoliday(Date(11, Apr, 2038));
		publicCalendar.addHoliday(Date(4, Oct, 2038));
		publicCalendar.addHoliday(Date(11, Oct, 2038));
		publicCalendar.addHoliday(Date(10, Oct, 2039));
		publicCalendar.addHoliday(Date(11, Apr, 2040));
		publicCalendar.addHoliday(Date(3, Mar, 2042));
		publicCalendar.addHoliday(Date(4, Oct, 2021));
		publicCalendar.addHoliday(Date(11, Oct, 2021));
		publicCalendar.addHoliday(Date(2, Mar, 2026));
		publicCalendar.addHoliday(Date(3, Jun, 2026));
		publicCalendar.addHoliday(Date(17, Aug, 2026));
		publicCalendar.addHoliday(Date(5, Oct, 2026));
		publicCalendar.addHoliday(Date(3, Mar, 2027));
		publicCalendar.addHoliday(Date(16, Aug, 2027));
		publicCalendar.addHoliday(Date(4, Oct, 2027));
		publicCalendar.addHoliday(Date(11, Oct, 2027));
		publicCalendar.addHoliday(Date(3, Mar, 2031));
		publicCalendar.addHoliday(Date(16, Aug, 2032));
		publicCalendar.addHoliday(Date(4, Oct, 2032));
		publicCalendar.addHoliday(Date(11, Oct, 2032));
		publicCalendar.addHoliday(Date(10, Oct, 2033));
		publicCalendar.addHoliday(Date(3, Mar, 2036));
		publicCalendar.addHoliday(Date(7, Oct, 2036));
		publicCalendar.addHoliday(Date(2, Mar, 2037));
		publicCalendar.addHoliday(Date(17, Aug, 2037));
		publicCalendar.addHoliday(Date(5, Oct, 2037));
		publicCalendar.addHoliday(Date(16, Aug, 2038));
		publicCalendar.addHoliday(Date(4, Oct, 2038));
		publicCalendar.addHoliday(Date(11, Oct, 2038));
		publicCalendar.addHoliday(Date(10, Oct, 2039));
		publicCalendar.addHoliday(Date(3, Mar, 2025));
		publicCalendar.addHoliday(Date(12, Apr, 2028));
		publicCalendar.addHoliday(Date(12, Jun, 2030));
		publicCalendar.addHoliday(Date(3, Mar, 2032));
		publicCalendar.addHoliday(Date(14, Apr, 2032));
		publicCalendar.addHoliday(Date(31, May, 2034));
		publicCalendar.addHoliday(Date(9, Apr, 2036));
		publicCalendar.addHoliday(Date(4, Mar, 2037));
		publicCalendar.addHoliday(Date(2, Jun, 2038));
		publicCalendar.addHoliday(Date(11, Apr, 2038));
		publicCalendar.addHoliday(Date(4, Oct, 2038));
		publicCalendar.addHoliday(Date(11, Oct, 2038));
		publicCalendar.addHoliday(Date(10, Oct, 2039));
		publicCalendar.addHoliday(Date(11, Apr, 2040));
		publicCalendar.addHoliday(Date(3, Mar, 2042));
	}

	std::string IRfilePath;
	std::string IRVolfilePath;
	std::string fixingFilePath;
	std::vector< std::vector<Real> > DurationMatrix;
	std::map<Date, double> fixingData;
	Rate todayFixing;
	//
	std::vector<double> Tenors;
	Date todayRho;
	Calendar publicCalendar;
	//Rate fixedRate;
	//Integer tenor;
	//VanillaSwap::Type type;
	//Real notional;
	//Rate spread;
	CurveData cd;
	Reader dataReader;

	std::vector<ext::shared_ptr<RateHelper>> ccpInstruments;
	
	~IRSPricer()
	{
		Tenors.clear();
		fixingData.clear();
		DurationMatrix.clear();
	}
};
