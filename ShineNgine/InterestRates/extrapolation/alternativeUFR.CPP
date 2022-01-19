
#include <test-suite/utilities.hpp>
#include "alternativeUFR.hpp"
#include "SmithWilson.h"
#include <ql/quantlib.hpp>
#include <ql/indexes/ibor/krwcd.hpp>
#include <ql/time/date.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <string>

using namespace QuantLib;
//using namespace boost::unit_test_framework;




struct Result
{
	Date date;
	Rate zeros;
	Rate forwards1Y;
};

namespace Alternative_forward_term_structure
{
	struct Datum
	{
		Integer n;
		TimeUnit units;
		Rate rate;
	};

	struct LLFRWeight
	{
		Time ttm;
		Real weight;
	};

	/* Interest Rate Data and Index*/
	Datum swapData[] =
	{
		//{3, Months, 0.0128}
		{1, Years, 0.015692857143}
		,{2, Years, 0.01755}
		,{3, Years, 0.017967857143}
		,{4, Years, 0.01815}
		,{5, Years, 0.0182}
		,{7, Years, 0.018175}
		,{10, Years,0.018275}
		,{12, Years,0.018192857143}
		,{15, Years,0.017125}
		,{20, Years,0.015896428571}
	};

	/* Interest Rate Data and Index*/
	Datum swapDataCCP[] =
	{
		 {1, Days, 0.010599634892}
		,{3, Months, 0.012800000000}
		,{6, Months, 0.013925000000}
		,{9, Months, 0.014775000000}
		,{1, Years,0.015692857143}
		,{18, Months,0.016917857143}
		,{2, Years,0.01755000000000}
		,{3, Years,0.01796785714300}
		,{4, Years,0.01815000000000}
		,{5, Years,0.01820000000000}
		,{6, Years,0.01816071428600}
		,{7, Years,0.01817500000000}
		,{8, Years,0.01820714285700}
		,{9, Years,0.01824285714300}
		,{10, Years,0.01827500000000}
		,{12, Years,0.01819285714300}
		,{15, Years,0.01712500000000}
		,{20, Years,0.01589642857100}
	};

	struct CommonVars
	{
		Date today,settlement;
		Calendar calendar;
		Natural settlementDays;
		Currency ccy;
		BusinessDayConvention businessConvention;
		DayCounter dayCount;
		Frequency fixedFrequency;
		Period floatingTenor;
		Real tolerance;

		ext::shared_ptr<IborIndex> index;
		ext::shared_ptr<YieldTermStructure> ftkCurve;
		RelinkableHandle<YieldTermStructure> ftkCurveHandle;
		RelinkableHandle<YieldTermStructure> ftkSpreadedCurveHandle;


		ext::shared_ptr<Quote> ufrRate;
		Period fsp;
		Real alpha; // for Beta;
		std::vector<Rate> rates;
		SavedSettings backup;; // for cleanup
		

		/* Input Utilities */
		CommonVars()
		{
			settlementDays = 1;
			businessConvention = ModifiedFollowing;
			dayCount = Actual365Fixed();
			calendar = SouthKorea();
			ccy = KRWCurrency();
			fixedFrequency = Annual;
			floatingTenor = 12 * Months;
			alpha = 0.1;
			tolerance = 1e-15;

			//Date todaysDate(2, Sep, 2021);
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

			/* Interest Rate Data and Index*/
			/*atum swapData[] =
			{
				{3, Months, 0.0128}
				,{1, Years, 0.015692857143}
				,{2, Years, 0.01755}
				,{3, Years, 0.017967857143}
				,{4, Years, 0.01815}
				,{5, Years, 0.0182}
				,{7, Years, 0.018175}
				,{10, Years,0.018275}
				,{12, Years,0.018192857143}
				,{15, Years,0.017125}
				,{20, Years,0.015896428571}
			};
			*/

			InterestRate ufr(0.05069310344377, dayCount, Continuous, Annual);
			ufrRate = ext::shared_ptr<Quote>(new SimpleQuote(ufr.equivalentRate(Continuous, Annual, 1.0)));
			ext::shared_ptr<IborIndex> index(new KrwCD3M);

			today = calendar.adjust(Date(28, December, 2021));
			Settings::instance().evaluationDate() = today;

			settlement = calendar.advance(today, settlementDays, Days);
			//index->addFixing(settlement, swapData[0].rate, true);

			// Approaching member variable in the swapData Structure Array

			/* Time Data */
			fsp = Period(20, Years);

			/* Helpers */
			Size nInstruments = LENGTH(swapData);
			std::vector<ext::shared_ptr<RateHelper>> instruments(nInstruments);
			for (Size i = 0; i < nInstruments; i++)
			{
				instruments[i] = ext::shared_ptr<RateHelper>(new SwapRateHelper(
					swapData[i].rate, Period(swapData[i].n, swapData[i].units), calendar,
					fixedFrequency, businessConvention, dayCount, index));
			}


			ext::shared_ptr<SimpleQuote> LP = ext::make_shared<SimpleQuote>(0.00456);
			Handle<Quote> LP_spread(LP);

			ext::shared_ptr<YieldTermStructure> ftkCurve(new PiecewiseYieldCurve<Discount, LogLinear>(settlement, instruments, dayCount));
			ftkCurve->enableExtrapolation();
			ftkCurveHandle.linkTo(ftkCurve);
			ext::shared_ptr<YieldTermStructure> spreaded(new ZeroSpreadedTermStructure(Handle<YieldTermStructure>(ftkCurve), LP_spread, Continuous));
			spreaded->enableExtrapolation();
			ftkSpreadedCurveHandle.linkTo(spreaded);// for Obeserver and Observable

		}
	};

		struct CCPVars
		{
			Date today, settlement;
			Calendar calendar;
			Natural settlementDays;
			Currency ccy;
			BusinessDayConvention businessConvention;
			DayCounter dayCount;
			Frequency fixedFrequency;
			Period floatingTenor;
			Real tolerance;

			ext::shared_ptr<IborIndex> index;

			ext::shared_ptr<YieldTermStructure> ccpCurve;
			RelinkableHandle<YieldTermStructure> ccpCurveHandle;
			RelinkableHandle<YieldTermStructure> ccpSpreadedCurveHandle;

			ext::shared_ptr<Quote> ufrRate;
			Period fsp;
			Real alpha; // for Beta;
			std::vector<Rate> rates;
			std::vector<Rate> CCPrates;
			SavedSettings backup;; // for cleanup


			/* Input Utilities */
			CCPVars()
			{
				settlementDays = 1;
				businessConvention = ModifiedFollowing;
				dayCount = Actual365Fixed();
				calendar = SouthKorea();
				ccy = KRWCurrency();
				fixedFrequency = Quarterly;
				floatingTenor = 3 * Months;
				alpha = 0.1;
				tolerance = 1e-15;

				//Date todaysDate(2, Sep, 2021);
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

				InterestRate ufr(0.05069310344377, dayCount, Continuous, Annual);
				ufrRate = ext::shared_ptr<Quote>(new SimpleQuote(ufr.equivalentRate(Continuous, Annual, 1.0)));
				ext::shared_ptr<IborIndex> index(new KrwCD3M);

				today = calendar.adjust(Date(28, December, 2021));
				Settings::instance().evaluationDate() = today;

				settlement = calendar.advance(today, settlementDays, Days);
				index->addFixing(settlement, swapDataCCP[1].rate, true);

				// Approaching member variable in the swapData Structure Array

				/* Time Data */
				fsp = Period(20, Years);

				//Size nCcpInstruments =81;
				std::vector<ext::shared_ptr<RateHelper>> ccpInst;
				std::vector<std::pair<Period, Rate> > inputVec;
				inputVec.push_back(std::make_pair(Period(1 * Days), swapDataCCP[0].rate));
				inputVec.push_back(std::make_pair(Period(3 * Months), swapDataCCP[1].rate));
				inputVec.push_back(std::make_pair(Period(6 * Months), swapDataCCP[2].rate));
				inputVec.push_back(std::make_pair(Period(9 * Months), swapDataCCP[3].rate));
				inputVec.push_back(std::make_pair(Period(12 * Months), swapDataCCP[4].rate));
				inputVec.push_back(std::make_pair(Period(18 * Months), swapDataCCP[5].rate));
				inputVec.push_back(std::make_pair(Period(24 * Months), swapDataCCP[6].rate));
				inputVec.push_back(std::make_pair(Period(36 * Months), swapDataCCP[7].rate));
				inputVec.push_back(std::make_pair(Period(48 * Months), swapDataCCP[8].rate));
				inputVec.push_back(std::make_pair(Period(60 * Months), swapDataCCP[9].rate));
				inputVec.push_back(std::make_pair(Period(72 * Months), swapDataCCP[10].rate));
				inputVec.push_back(std::make_pair(Period(84 * Months), swapDataCCP[11].rate));
				inputVec.push_back(std::make_pair(Period(96 * Months), swapDataCCP[12].rate));
				inputVec.push_back(std::make_pair(Period(108 * Months), swapDataCCP[13].rate));
				inputVec.push_back(std::make_pair(Period(120 * Months), swapDataCCP[14].rate));
				inputVec.push_back(std::make_pair(Period(144 * Months), swapDataCCP[15].rate));
				inputVec.push_back(std::make_pair(Period(180 * Months), swapDataCCP[16].rate));
				inputVec.push_back(std::make_pair(Period(240 * Months), swapDataCCP[17].rate));

				// Helpers & Instruments //
				//cout << fixingData[today] << endl;
				Frequency swFixedLegFrequency = Quarterly;
				BusinessDayConvention swFixedLegConvention = ModifiedFollowing;
				DayCounter swFixedLegDayCounter = Actual365Fixed();

				ext::shared_ptr<IborIndex> krwCD(new KrwCD3M());
				krwCD->addFixing(settlement, swapDataCCP[1].rate, true);
				ext::shared_ptr<SimpleQuote> krw01dCl(new SimpleQuote(swapDataCCP[0].rate));
				ext::shared_ptr<RateHelper> dp01d(new DepositRateHelper(Handle<Quote>(krw01dCl),
					Period(1 * Days), 1, calendar, ModifiedFollowing, true, Actual365Fixed()));
				ccpInst.push_back(dp01d);


				/*
				std::vector<ext::shared_ptr<RateHelper>> instruments(nInstruments);
				for (Size i = 0; i < nInstruments; i++)
				{
					instruments[i] = ext::shared_ptr<RateHelper>(new SwapRateHelper(
						swapData[i].rate, Period(swapData[i].n, swapData[i].units), calendar,
						fixedFrequency, businessConvention, dayCount, index));
				}
				*/

				for (Size i = 0; i <= 79; i++)
				{
					for (Size j = 1; j <= inputVec.size() - 1; j++)
					{
						if (Period((i + 1) * 3 * Months) == inputVec[j].first)
						{
							ext::shared_ptr<Quote> tempPar(new SimpleQuote(inputVec[j].second));
							ext::shared_ptr<RateHelper> tempHelper(new SwapRateHelper(Handle<Quote>(tempPar), inputVec[j].first,
								calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));
							ccpInst.push_back(tempHelper);
						}

						if ((j < 16) && ((Period((i + 1) * 3 * Months) < inputVec[j + 1].first) && (Period((i + 1) * 3 * Months) > inputVec[j].first)))
						{
							Date tempDt1 = calendar.advance(settlement, inputVec[j + 1].first);
							Date tempDt0 = calendar.advance(settlement, inputVec[j].first);
							Date tempDtn = calendar.advance(settlement, (i + 1) * 3, Months);
							Rate intpl = inputVec[j].second + (inputVec[j + 1].second - inputVec[j].second) *
								(tempDtn.serialNumber() - tempDt0.serialNumber()) / (tempDt1.serialNumber() - tempDt0.serialNumber());

							ext::shared_ptr<Quote> tempPar(new SimpleQuote(intpl));
							ext::shared_ptr<RateHelper> tempHelper(new SwapRateHelper(Handle<Quote>(tempPar), (i + 1) * 3 * Months,
								calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));
							ccpInst.push_back(tempHelper);

						}
						// 15 ~ 20
						if ((j >= 16) && (j < 17) && (Period((i + 1) * 3 * Months) < inputVec[17].first) && (Period((i + 1) * 3 * Months) > inputVec[16].first))
						{
							Date tempDt1 = calendar.advance(settlement, inputVec[17].first);
							Date tempDt0 = calendar.advance(settlement, inputVec[16].first);
							Date tempDtn = calendar.advance(settlement, (i + 1) * 3, Months);

							Rate intpl = inputVec[16].second + (inputVec[17].second - inputVec[16].second) *
								(tempDtn.serialNumber() - tempDt0.serialNumber()) / (tempDt1.serialNumber() - tempDt0.serialNumber());

							ext::shared_ptr<Quote> tempPar(new SimpleQuote(intpl));
							ext::shared_ptr<RateHelper> tempHelper(new SwapRateHelper(Handle<Quote>(tempPar), (i + 1) * 3 * Months,
								calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));
							ccpInst.push_back(tempHelper);
						}
					}
				}

				ext::shared_ptr<SimpleQuote> LP = ext::make_shared<SimpleQuote>(0.00456);
				Handle<Quote> LP_spread(LP);
				ext::shared_ptr<YieldTermStructure> ccpCurve(new PiecewiseYieldCurve<Discount, LogLinear>(settlement, ccpInst, dayCount, tolerance));
				ccpCurve->enableExtrapolation();
				ccpCurveHandle.linkTo(ccpCurve);
				ext::shared_ptr<YieldTermStructure> ccpspreaded(new ZeroSpreadedTermStructure(Handle<YieldTermStructure>(ccpCurve), LP_spread));
				ccpspreaded->enableExtrapolation();
				ccpSpreadedCurveHandle.linkTo(ccpspreaded);// for Obeserver and Observable
			}
	};

	ext::shared_ptr<Quote> calculateLLFR(const Handle<YieldTermStructure>& ts, const Period& fsp)
	{
		DayCounter dc = ts->dayCounter();
		Time cutOff = ts->timeFromReference(ts->referenceDate()+ 15 * Years);
		Time long1 = ts->timeFromReference(ts->referenceDate() + 20 * Years);
		Time long2 = ts->timeFromReference(ts->referenceDate() + 25 * Years);
		Time long3 = ts->timeFromReference(ts->referenceDate() + 30 * Years);
		Time long4 = ts->timeFromReference(ts->referenceDate() + 35 * Years);
		Time long5 = ts->timeFromReference(ts->referenceDate() + 40 * Years);

		// referenceDate: discountfactor = 1

		LLFRWeight llfrWeights[] = {
			{ long1, 1.0}, {long2, 0.0}, {long3, 0.0}
			//{35.0, 0.0}, {40.0, 0.0},{45.0, 0.0}, {50.0, 0.0}
		};
		Size nWeights = LENGTH(llfrWeights);
		Rate llfr = 0.0;
		for (Size j = 0; j< nWeights; j++)
		{
			LLFRWeight w = llfrWeights[j];
			llfr += w.weight * ts->forwardRate(cutOff, w.ttm, Continuous, NoFrequency, true);
			cutOff = ts->timeFromReference(ts->referenceDate() + fsp - 5 * Years * j);
		}
		return ext::shared_ptr<Quote>(new SimpleQuote(llfr));
	}




	Rate calculateExtrapolationForward(Time t, Time fsp, Rate llfr, Rate ufr, Real alpha)
	{
		Time deltaT = t - fsp;
		Real beta = (1.0 - std::exp(-alpha * deltaT)) / (alpha*deltaT);
		return ufr + (llfr - ufr)*beta;
	}

}




void AlternativeUltimateForwardTermStructure::interpolatedCurve()
{
	using namespace Alternative_forward_term_structure;
	CommonVars vars;
	Period tenors_intp[239] = { };
	for (Size i = 1; i<= 240; ++i)
	{
		tenors_intp[i - 1] = i * Months;
	}
	Size nTenors_intp = LENGTH(tenors_intp);
	std::cout << " **** Interpolated Curve before FSP **** " << std::endl;
	for (Size i = 1; i <= nTenors_intp; ++i)
	{
		Date mat0 = vars.ftkSpreadedCurveHandle->referenceDate() + tenors_intp[i - 1];
		Date mat1 = vars.ftkSpreadedCurveHandle->referenceDate() + tenors_intp[i];
		Time t0 = vars.ftkSpreadedCurveHandle->timeFromReference(mat0);
		Time ti = vars.ftkSpreadedCurveHandle->timeFromReference(mat1);
		Rate zero = vars.ftkSpreadedCurveHandle->zeroRate(ti, Continuous, NoFrequency, true).rate();
		Rate forward = vars.ftkSpreadedCurveHandle->forwardRate(t0, ti, Continuous, NoFrequency, true).rate();

		std::cout << "Tenor : " << tenors_intp[i] << "ZeroSpot + LP " << zero << ", forward = " << forward << std::endl;
	}
}

void AlternativeUltimateForwardTermStructure::CCPinterpolatedCurve()
{
		using namespace Alternative_forward_term_structure;
		CCPVars vars;
		Period tenors_intp[239] = { };
		for (Size i = 1; i <= 240; ++i)
		{
			tenors_intp[i - 1] = i * Months;
			std::cout << tenors_intp[i - 1] << std::endl;
		}
		Size nTenors_intp = LENGTH(tenors_intp);

		std::cout << " **** Interpolated Curve before FSP with CCP Curve **** " << std::endl;
		for (Size i = 1; i <= nTenors_intp; ++i)
		{
			Date mat0 = vars.ccpCurveHandle->referenceDate() + tenors_intp[i - 1];
			Date mat1 = vars.ccpCurveHandle->referenceDate() + tenors_intp[i];
			Time t0 = vars.ccpCurveHandle->timeFromReference(mat0);
			Time ti = vars.ccpCurveHandle->timeFromReference(mat1);
			Rate zero = vars.ccpCurveHandle->zeroRate(ti, Continuous, NoFrequency, true).rate();
			Rate forward = vars.ccpCurveHandle->forwardRate(t0, ti, Continuous, NoFrequency, true).rate();

			std::cout << "Tenor : " << tenors_intp[i] << "ZeroSpot + LP " << zero << ", forward = " << forward << std::endl;

		}
	}



void AlternativeUltimateForwardTermStructure::ExtrapolatedForward()
{
	using namespace Alternative_forward_term_structure;
	CommonVars vars;
	Period tenors[101];
	Natural h[101];
	Rate forward = 0.0;

	ext::shared_ptr<Quote> llfr = calculateLLFR(vars.ftkSpreadedCurveHandle, vars.fsp);

	ext::shared_ptr<YieldTermStructure> ufrTs(
		new UltimateForwardTermStructure(vars.ftkSpreadedCurveHandle, Handle<Quote>(llfr),
			Handle<Quote>(vars.ufrRate), vars.fsp, vars.alpha));
	Time cutOff = ufrTs->timeFromReference(ufrTs->referenceDate() + vars.fsp);

	for (int i = 20; i <= 120; ++i)
	{
		tenors[i-20] = (i) * Years;
		h[i-20] = i-20;
	}


	Size nTenors = LENGTH(tenors);

	for (Size i = 1; i <= nTenors; i++)
	{
		Date mat0 = vars.settlement + tenors[i - 1];
		Date mati = vars.settlement + tenors[i];
		Time t0 = ufrTs->timeFromReference(mat0);
		Time ti = ufrTs->timeFromReference(mati);
		Rate discount_T0;
		Rate discount_Ti;
		const Rate zero20 = ufrTs->zeroRate(cutOff, Continuous, NoFrequency, true).rate();
		Rate expected_T0 = calculateExtrapolationForward(t0, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);
		Rate expected_Ti = calculateExtrapolationForward(ti, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);
		Rate zero_T0 = std::log(exp(((20 * zero20 + h[i - 1] * expected_T0)) / (20 + h[i - 1])));
		Rate zero_Ti = std::log(exp(((20 * zero20 + h[i] * expected_Ti)) / (20 + h[i])));
		Rate forward;
		if (i == 1)
		{
			expected_T0 = 0.0;
			forward = expected_Ti;
			zero_T0 = zero20;
		}
		else
		{
			forward = -(zero_T0 * (20+h[i - 1])) + (zero_Ti*(20+h[i]));
		}
		std::cout << std::setprecision(5) << "zero at FSP: " << zero20 << ", Tenor: " << tenors[i] << ", date : " << ti << " Maturity : " << mati << ", h= " << h[i]
			<< ", 20+h = " << 20 + h[i] << " , zero_T0 = " << zero_T0 << " , zero_Ti = " << zero_Ti << ", Forward(20, 20+h-1)= " << expected_T0 << ", Forward(20, 20+h)= " << expected_Ti << ", Forward(i, i+1)= " << forward << std::endl;
	}
	
}



void AlternativeUltimateForwardTermStructure::CCPExtrapolatedForward()
{
	using namespace Alternative_forward_term_structure;
	CCPVars vars;
	Period tenors[101];
	Natural h[101];
	Rate forward = 0.0;

	ext::shared_ptr<Quote> llfr = calculateLLFR(vars.ccpSpreadedCurveHandle, vars.fsp);

	ext::shared_ptr<YieldTermStructure> ufrTs(
		new UltimateForwardTermStructure(vars.ccpSpreadedCurveHandle, Handle<Quote>(llfr),
			Handle<Quote>(vars.ufrRate), vars.fsp, vars.alpha));
	Time cutOff = ufrTs->timeFromReference(ufrTs->referenceDate() + vars.fsp);

	for (int i = 20; i <= 120; ++i)
	{
		tenors[i - 20] = (i)* Years;
		h[i - 20] = i - 20;
	}


	Size nTenors = LENGTH(tenors);

	std::cout << "********* CCP **********" << std::endl;
	for (Size i = 1; i <= nTenors; i++)
	{
		Date mat0 = vars.settlement + tenors[i - 1];
		Date mati = vars.settlement + tenors[i];
		Time t0 = ufrTs->timeFromReference(mat0);
		Time ti = ufrTs->timeFromReference(mati);
		Rate discount_T0;
		Rate discount_Ti;
		const Rate zero20 = ufrTs->zeroRate(cutOff, Continuous, NoFrequency, true).rate();
		Rate expected_T0 = calculateExtrapolationForward(t0, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);
		Rate expected_Ti = calculateExtrapolationForward(ti, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);
		Rate zero_T0 = std::log(exp(((20 * zero20 + h[i - 1] * expected_T0)) / (20 + h[i - 1])));
		Rate zero_Ti = std::log(exp(((20 * zero20 + h[i] * expected_Ti)) / (20 + h[i])));
		Rate forward;
		if (i == 1)
		{
			expected_T0 = 0.0;
			forward = expected_Ti;
			zero_T0 = zero20;
		}
		else
		{
			forward = -(zero_T0 * (20 + h[i - 1])) + (zero_Ti*(20 + h[i]));
		}
		std::cout << std::setprecision(5) << "zero at FSP: " << zero20 << ", Tenor: " << tenors[i] << ", date : " << ti << " Maturity : " << mati << ", h= " << h[i]
			<< ", 20+h = " << 20 + h[i] << " , zero_T0 = " << zero_T0 << " , zero_Ti = " << zero_Ti << ", Forward(20, 20+h-1)= " << expected_T0 << ", Forward(20, 20+h)= " << expected_Ti << ", Forward(i, i+1)= " << forward << std::endl;
	}

}



std::map<Date, std::pair<Rate, Rate>> AlternativeUltimateForwardTermStructure::setExport()
{
	std::map<Date, std::pair<Rate, Rate>> datum;
	using namespace Alternative_forward_term_structure;
	CommonVars vars;
	Period tenors_intp[121] = { 1 * Years };
	for (Size i = 1; i <= 121; ++i)
	{
		tenors_intp[i - 1] = (i-1) * Years;
	}
	Size nTenors_intp = LENGTH(tenors_intp);

	for (Size i = 1; i <= 20; ++i)
	{
		Date mat0 = vars.ftkSpreadedCurveHandle->referenceDate() + tenors_intp[i - 1];
		Date mat1 = vars.ftkSpreadedCurveHandle->referenceDate() + tenors_intp[i];
		Time t0 = vars.ftkSpreadedCurveHandle->timeFromReference(mat0);
		Time ti = vars.ftkSpreadedCurveHandle->timeFromReference(mat1);
		Rate zero = vars.ftkSpreadedCurveHandle->zeroRate(ti, Continuous, NoFrequency, true).rate();
		Rate forward = vars.ftkSpreadedCurveHandle->forwardRate(t0, ti, Continuous, NoFrequency, true).rate();
		std::pair<Rate, Rate> temp = std::make_pair(zero, forward);
		//td::cout << "Tenor : " << tenors_intp[i] << "ZeroSpot + LP " << zero << ", forward = " << forward << std::endl;
		datum.insert({ mat1,temp });
	}

	Rate forward = 0.0;

	ext::shared_ptr<Quote> llfr = calculateLLFR(vars.ftkSpreadedCurveHandle, vars.fsp);

	ext::shared_ptr<YieldTermStructure> ufrTs(
		new UltimateForwardTermStructure(vars.ftkSpreadedCurveHandle, Handle<Quote>(llfr),
			Handle<Quote>(vars.ufrRate), vars.fsp, vars.alpha));
	Time cutOff = ufrTs->timeFromReference(ufrTs->referenceDate() + vars.fsp);

	Size nTenors = LENGTH(tenors_intp);

	Natural h[121];
	for (Size i = 0; i <= nTenors; ++i)
	{
		if (i < 20)
		{
			h[i] = 0;
		}
		else
		{
			h[i] = i - 20;
		}
	}
	for (Size i = 21; i <= nTenors; i++)
	{
		Date mat0 = vars.settlement + tenors_intp[i - 1];
		Date mati = vars.settlement + tenors_intp[i];
		Time t0 = ufrTs->timeFromReference(mat0);
		Time ti = ufrTs->timeFromReference(mati);
		Rate discount_T0;
		Rate discount_Ti;
		const Rate zero20 = ufrTs->zeroRate(cutOff, Continuous, NoFrequency, true).rate();
		Rate expected_T0 = calculateExtrapolationForward(t0, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);
		Rate expected_Ti = calculateExtrapolationForward(ti, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);
		Rate zero_T0 = std::log(exp(((20 * zero20 + h[i - 1] * expected_T0)) / (20 + h[i - 1])));
		Rate zero_Ti = std::log(exp(((20 * zero20 + h[i] * expected_Ti)) / (20 + h[i])));
		Rate forward;
		if (i == 21)
		{
			expected_T0 = 0.0;
			forward = expected_Ti;
			zero_T0 = zero20;
		}
		else
		{
			forward = -(zero_T0 * (20+h[i - 1])) + (zero_Ti*(20+ h[i]));
		}

		std::pair<Rate, Rate> temp = std::make_pair(zero_Ti, forward);
		datum.insert({ mati,temp });
	}
	return datum;
}


std::map<Date, std::pair<Rate, Rate>> AlternativeUltimateForwardTermStructure::setCCPExport()
{
	std::map<Date, std::pair<Rate, Rate>> datum;
	using namespace Alternative_forward_term_structure;
	CCPVars vars;
	Period tenors_intp[121] = { 1 * Years };
	for (Size i = 1; i <= 121; ++i)
	{
		tenors_intp[i - 1] = (i - 1) * Years;
	}
	Size nTenors_intp = LENGTH(tenors_intp);

	for (Size i = 1; i <= 20; ++i)
	{
		Date mat0 = vars.ccpSpreadedCurveHandle->referenceDate() + tenors_intp[i - 1];
		Date mat1 = vars.ccpSpreadedCurveHandle->referenceDate() + tenors_intp[i];
		Time t0 = vars.ccpSpreadedCurveHandle->timeFromReference(mat0);
		Time ti = vars.ccpSpreadedCurveHandle->timeFromReference(mat1);
		Rate zero = vars.ccpSpreadedCurveHandle->zeroRate(ti, Continuous, NoFrequency, true).rate();
		Rate forward = vars.ccpSpreadedCurveHandle->forwardRate(t0, ti, Continuous, NoFrequency, true).rate();
		std::pair<Rate, Rate> temp = std::make_pair(zero, forward);
		//td::cout << "Tenor : " << tenors_intp[i] << "ZeroSpot + LP " << zero << ", forward = " << forward << std::endl;
		datum.insert({ mat1,temp });
	}

	Rate forward = 0.0;

	ext::shared_ptr<Quote> llfr = calculateLLFR(vars.ccpSpreadedCurveHandle, vars.fsp);

	ext::shared_ptr<YieldTermStructure> ufrTs(
		new UltimateForwardTermStructure(vars.ccpSpreadedCurveHandle, Handle<Quote>(llfr),
			Handle<Quote>(vars.ufrRate), vars.fsp, vars.alpha));
	Time cutOff = ufrTs->timeFromReference(ufrTs->referenceDate() + vars.fsp);

	Size nTenors = LENGTH(tenors_intp);

	Natural h[121];
	for (Size i = 0; i <= nTenors; ++i)
	{
		if (i < 20)
		{
			h[i] = 0;
		}
		else
		{
			h[i] = i - 20;
		}
	}
	for (Size i = 21; i <= nTenors; i++)
	{
		Date mat0 = vars.settlement + tenors_intp[i - 1];
		Date mati = vars.settlement + tenors_intp[i];
		Time t0 = ufrTs->timeFromReference(mat0);
		Time ti = ufrTs->timeFromReference(mati);
		Rate discount_T0;
		Rate discount_Ti;
		const Rate zero20 = ufrTs->zeroRate(cutOff, Continuous, NoFrequency, true).rate();
		Rate expected_T0 = calculateExtrapolationForward(t0, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);
		Rate expected_Ti = calculateExtrapolationForward(ti, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);
		Rate zero_T0 = std::log(exp(((20 * zero20 + h[i - 1] * expected_T0)) / (20 + h[i - 1])));
		Rate zero_Ti = std::log(exp(((20 * zero20 + h[i] * expected_Ti)) / (20 + h[i])));
		Rate forward;
		if (i == 21)
		{
			expected_T0 = 0.0;
			forward = expected_Ti;
			zero_T0 = zero20;
		}
		else
		{
			forward = -(zero_T0 * (20 + h[i - 1])) + (zero_Ti*(20 + h[i]));
		}

		std::pair<Rate, Rate> temp = std::make_pair(zero_Ti, forward);
		datum.insert({ mati,temp });
	}
	return datum;
}



std::map<Date, std::pair<Rate, Rate>> AlternativeUltimateForwardTermStructure::getDF()
{
	std::map<Date, std::pair<Rate, Rate>> datum;
	using namespace Alternative_forward_term_structure;
	CommonVars vars;
	Period tenors_intp[121] = { 1 * Years };
	for (Size i = 1; i <= 121; ++i)
	{
		tenors_intp[i - 1] = (i - 1) * Years;
	}
	Size nTenors_intp = LENGTH(tenors_intp);

	for (Size i = 1; i <= 20; ++i)
	{
		Date mat0 = vars.ftkSpreadedCurveHandle->referenceDate() + tenors_intp[i - 1];
		Date mat1 = vars.ftkSpreadedCurveHandle->referenceDate() + tenors_intp[i];
		Time t0 = vars.ftkSpreadedCurveHandle->timeFromReference(mat0);
		Time ti = vars.ftkSpreadedCurveHandle->timeFromReference(mat1);
		Rate zero = vars.ftkSpreadedCurveHandle->zeroRate(ti, Continuous, Annual, true).rate();
		Rate forward = vars.ftkSpreadedCurveHandle->forwardRate(t0, ti, Continuous, Annual, true).rate();
		Rate df = vars.ftkSpreadedCurveHandle->discount(ti, true);
		std::pair<Rate, Rate> temp = std::make_pair(df, forward);
		//td::cout << "Tenor : " << tenors_intp[i] << "ZeroSpot + LP " << zero << ", forward = " << forward << std::endl;
		datum.insert({ mat1,temp });
	}

	Rate forward = 0.0;

	ext::shared_ptr<Quote> llfr = calculateLLFR(vars.ftkSpreadedCurveHandle, vars.fsp);

	ext::shared_ptr<YieldTermStructure> ufrTs(
		new UltimateForwardTermStructure(vars.ftkSpreadedCurveHandle, Handle<Quote>(llfr),
			Handle<Quote>(vars.ufrRate), vars.fsp, vars.alpha));
	Time cutOff = ufrTs->timeFromReference(ufrTs->referenceDate() + vars.fsp);

	Size nTenors = LENGTH(tenors_intp);

	int h[121];
	for (Size i = 0; i <= nTenors; ++i)
	{
		if (i < 20)
		{
			h[i] = 0;
		}
		else
		{
			h[i] = i - 20;
		}
	}
	for (Size i = 21; i <= nTenors; i++)
	{
		Date mat0 = vars.settlement + tenors_intp[i - 1];
		Date mati = vars.settlement + tenors_intp[i];
		Time t0 = ufrTs->timeFromReference(mat0);
		Time ti = ufrTs->timeFromReference(mati);
		Rate discount_T0;
		//Rate discount_Ti;
		const Rate zero20 = ufrTs->zeroRate(cutOff, Continuous, NoFrequency, true).rate();
		Rate expected_T0 = calculateExtrapolationForward(t0, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);
		Rate expected_Ti = calculateExtrapolationForward(ti, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);
		Rate zero_T0 = std::log(exp(((20 * zero20 + h[i - 1] * expected_T0)) / (20 + h[i - 1])));
		Rate zero_Ti = std::log(exp(((20 * zero20 + h[i] * expected_Ti)) / (20 + h[i])));
		Rate forward;
		if (i == 21)
		{
			expected_T0 = 0.0;
			forward = expected_Ti;
			zero_T0 = zero20;
		}
		else
		{
			forward = -(zero_T0 * (20 + h[i - 1])) + (zero_Ti*(20 + h[i]));
		}

		Rate discount_Ti = exp(-(20 + h[i])*zero_Ti);

		std::pair<Rate, Rate> temp = std::make_pair(discount_Ti, forward);
		datum.insert({ mati,temp });
	}
	return datum;
}



std::map<Date, std::pair<Rate, Rate>> AlternativeUltimateForwardTermStructure::getCCPDF()
{
	std::map<Date, std::pair<Rate, Rate>> datum;
	using namespace Alternative_forward_term_structure;
	CCPVars vars;
	Period tenors_intp[121] = { 1 * Years };
	for (Size i = 1; i <= 121; ++i)
	{
		tenors_intp[i - 1] = (i - 1) * Years;
	}
	Size nTenors_intp = LENGTH(tenors_intp);

	for (Size i = 1; i <= 20; ++i)
	{
		Date mat0 = vars.ccpSpreadedCurveHandle->referenceDate() + tenors_intp[i - 1];
		Date mat1 = vars.ccpSpreadedCurveHandle->referenceDate() + tenors_intp[i];
		Time t0 = vars.ccpSpreadedCurveHandle->timeFromReference(mat0);
		Time ti = vars.ccpSpreadedCurveHandle->timeFromReference(mat1);
		Rate zero = vars.ccpSpreadedCurveHandle->zeroRate(ti, Continuous, NoFrequency, true).rate();
		Rate forward = vars.ccpSpreadedCurveHandle->forwardRate(t0, ti, Continuous, NoFrequency, true).rate();
		Rate df = vars.ccpSpreadedCurveHandle->discount(ti, true);
		std::pair<Rate, Rate> temp = std::make_pair(df, forward);
		//td::cout << "Tenor : " << tenors_intp[i] << "ZeroSpot + LP " << zero << ", forward = " << forward << std::endl;
		datum.insert({ mat1,temp });
	}

	Rate forward = 0.0;

	ext::shared_ptr<Quote> llfr = calculateLLFR(vars.ccpSpreadedCurveHandle, vars.fsp);

	ext::shared_ptr<YieldTermStructure> ufrTs(
		new UltimateForwardTermStructure(vars.ccpSpreadedCurveHandle, Handle<Quote>(llfr),
			Handle<Quote>(vars.ufrRate), vars.fsp, vars.alpha));
	Time cutOff = ufrTs->timeFromReference(ufrTs->referenceDate() + vars.fsp);

	Size nTenors = LENGTH(tenors_intp);

	int h[121];
	for (Size i = 0; i <= nTenors; ++i)
	{
		if (i < 20)
		{
			h[i] = 0;
		}
		else
		{
			h[i] = i - 20;
		}
	}
	for (Size i = 21; i <= nTenors; i++)
	{
		Date mat0 = vars.settlement + tenors_intp[i - 1];
		Date mati = vars.settlement + tenors_intp[i];
		Time t0 = ufrTs->timeFromReference(mat0);
		Time ti = ufrTs->timeFromReference(mati);
		Rate discount_T0;
		//Rate discount_Ti;
		const Rate zero20 = ufrTs->zeroRate(cutOff, Continuous, NoFrequency, true).rate();
		Rate expected_T0 = calculateExtrapolationForward(t0, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);
		Rate expected_Ti = calculateExtrapolationForward(ti, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);
		Rate zero_T0 = std::log(exp(((20 * zero20 + h[i - 1] * expected_T0)) / (20 + h[i - 1])));
		Rate zero_Ti = std::log(exp(((20 * zero20 + h[i] * expected_Ti)) / (20 + h[i])));
		Rate forward;
		if (i == 21)
		{
			expected_T0 = 0.0;
			forward = expected_Ti;
			zero_T0 = zero20;
		}
		else
		{
			forward = -(zero_T0 * (20 + h[i - 1])) + (zero_Ti*(20 + h[i]));
		}

		Rate discount_Ti = exp(-(20 + h[i])*zero_Ti);

		std::pair<Rate, Rate> temp = std::make_pair(discount_Ti, forward);
		datum.insert({ mati,temp });
	}
	return datum;
}

void write_csv(std::string fileName, std::map < Date, std::pair<Rate, Rate>> dataset)
{
	std::ofstream myFile(fileName);
	myFile << "Date,Zero,forward1Y";
	myFile << "\n";
	for (std::map<Date, std::pair<Rate, Rate>>::iterator iter = dataset.begin(); iter != dataset.end(); iter++)
	{
		myFile << iter->first << "," << iter->second.first << "," << iter->second.second;
		myFile << "\n";
	}
	myFile.close();
}

void write_csv2(std::string fileName, std::map < Date, std::pair<Rate, Rate>> dataset)
{
	std::ofstream myFile(fileName);
	myFile << "Date,DF,forward1Y";
	myFile << "\n";
	for (std::map<Date, std::pair<Rate, Rate>>::iterator iter = dataset.begin(); iter != dataset.end(); iter++)
	{
		myFile << iter->first << "," << iter->second.first << "," << iter->second.second;
		myFile << "\n";
	}
	myFile.close();
}


void write_csvLH(std::string fileName, std::map < Date, std::pair<Rate, Rate>> dataset)
{
	std::ofstream myFile(fileName);
	myFile << "# Format: ConventionKey;Type;Tenor;Interval;StartDate;EndDate;MidRate;Spread";
	//KRWIRS2;DF;2020-12-31;;;;1.00000000;0.00000000
	//KRWIRS2; DF; 2021 - 01 - 31;;;; 0.999694159662227; 0.00000000
	myFile << "\n";
	for (std::map<Date, std::pair<Rate, Rate>>::iterator iter = dataset.begin(); iter != dataset.end(); iter++)
	{

		myFile  <<"KRWIRS;DF;" << detail::iso_date_holder(iter->first) << ";;;;" << iter->second.first << ";0.0000000" ;
		myFile << "\n";
	}
	myFile.close();
}

int main()
{

		AlternativeUltimateForwardTermStructure::interpolatedCurve();
		AlternativeUltimateForwardTermStructure::ExtrapolatedForward();
		AlternativeUltimateForwardTermStructure::CCPinterpolatedCurve();
		AlternativeUltimateForwardTermStructure::CCPExtrapolatedForward();



		std::map<Date, std::pair<Rate, Rate>>temp = AlternativeUltimateForwardTermStructure::setExport();
		std::map<Date, std::pair<Rate, Rate>>temp2 = AlternativeUltimateForwardTermStructure::getDF();
		std::map<Date, std::pair<Rate, Rate>>tempCCP = AlternativeUltimateForwardTermStructure::setCCPExport();
		std::map<Date, std::pair<Rate, Rate>>tempCCPDF = AlternativeUltimateForwardTermStructure::getCCPDF();

		
		std::cout << "Date | Zero Rate | Forward 1Y Rate " << std::endl;
		for (std::map<Date, std::pair<Rate, Rate>>::iterator iter = temp.begin(); iter != temp.end(); iter++)
		{
			std::cout << iter->first << ", " << iter->second.first << ", " << iter->second.second << std::endl;
		}
		std::cout << "Date | Discount Factor | Forward 1Y Rate " << std::endl;
		for (std::map<Date, std::pair<Rate, Rate>>::iterator iter = temp2.begin(); iter != temp2.end(); iter++)
		{
			std::cout << iter->first << ", " << iter->second.first << ", " << iter->second.second << std::endl;
		}

		write_csv("data2.csv", temp);
		write_csv2("data2.csv", temp2);

		std::cout << "******* CCP *******" << std::endl;
		std::cout << "Date | Zero Rate | Forward 1Y Rate " << std::endl;
		for (std::map<Date, std::pair<Rate, Rate>>::iterator iter = tempCCP.begin(); iter != tempCCP.end(); iter++)
		{
			std::cout << iter->first << ", " << iter->second.first << ", " << iter->second.second << std::endl;
		}
		std::cout << "Date | Discount Factor | Forward 1Y Rate " << std::endl;
		for (std::map<Date, std::pair<Rate, Rate>>::iterator iter = tempCCP.begin(); iter != tempCCP.end(); iter++)
		{
			std::cout << iter->first << ", " << iter->second.first << ", " << iter->second.second << std::endl;
		}

		write_csv2("dataCCP.csv", tempCCP);
		write_csv2("dataCCPDF.csv", tempCCPDF);
		write_csvLH("LHfile.csv", temp2);
		return 0;
	
}