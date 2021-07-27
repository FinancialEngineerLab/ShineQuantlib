#include <ql/quantlib.hpp>

#include <iostream>



#include "hybridblackscholeshullwhiteprocess.hpp"



#include "smithwilson.hpp"



using namespace std;

using namespace QuantLib;

using namespace boost;





QuantLib::Date TenBusinessDayBeforeEOM(QuantLib::Date d, Calendar cal)

{

Date eom = Date::endOfMonth(d);

eom = cal.adjust(eom, ModifiedFollowing);



Date tbd = cal.advance(eom, -1 * Days, Preceding, true);

for (int i = 0; i < 9; i++)

{

tbd = cal.advance(tbd, -1 * Days, Preceding, true);

}

return tbd;

}





int main()

{

std::cout.precision(15);



Date todaysDate = Date(16, Dec, 2020);

Calendar calendar = SouthKorea();

DayCounter daycounter = Actual365Fixed();



calendar.addHoliday(Date(7, May, 2018));

calendar.addHoliday(Date(13, June, 2018));

calendar.addHoliday(Date(26, September, 2018));

calendar.addHoliday(Date(15, Apr, 2020));



Date settlementDate = calendar.advance(todaysDate, 1 * Days);

Settings::instance().evaluationDate() = todaysDate;





// input data : 2020-12-16 KRXCCP intraday data //

//Real d03mRate = isTargetIR ? RateVector_Target[0] : RateVector_Base[0];

Rate cd03MRate = 0.0058;

Rate sw06MRate = 0.0072;

Rate sw01YRate = 0.0075;

Rate sw02YRate = 0.0081;

Rate sw03YRate = 0.0088;

Rate sw04YRate = 0.0093;

Rate sw05YRate = 0.0096;

Rate sw06YRate = 0.0098;

Rate sw07YRate = 0.0100;

Rate sw10YRate = 0.0106;

Rate sw12YRate = 0.0110;

Rate sw15YRate = 0.0103;

Rate sw20YRate = 0.0095;



// Quotes Adjustment //

Handle<Quote> cd03MQuote(ext::make_shared <SimpleQuote>(cd03MRate));

Handle<Quote> sw06MQuote(ext::make_shared <SimpleQuote>(sw06MRate));

Handle<Quote> sw01YQuote(ext::make_shared <SimpleQuote>(sw01YRate));

Handle<Quote> sw02YQuote(ext::make_shared <SimpleQuote>(sw02YRate));

Handle<Quote> sw03YQuote(ext::make_shared <SimpleQuote>(sw03YRate));

Handle<Quote> sw04YQuote(ext::make_shared <SimpleQuote>(sw04YRate));

Handle<Quote> sw05YQuote(ext::make_shared <SimpleQuote>(sw05YRate));

Handle<Quote> sw06YQuote(ext::make_shared <SimpleQuote>(sw06YRate));

Handle<Quote> sw07YQuote(ext::make_shared <SimpleQuote>(sw07YRate));

Handle<Quote> sw10YQuote(ext::make_shared <SimpleQuote>(sw10YRate));

Handle<Quote> sw12YQuote(ext::make_shared <SimpleQuote>(sw12YRate));

Handle<Quote> sw15YQuote(ext::make_shared <SimpleQuote>(sw15YRate));

Handle<Quote> sw20YQuote(ext::make_shared <SimpleQuote>(sw20YRate));



// swap instrument //

Frequency swFixedLegFrequency = Quarterly;

BusinessDayConvention swFixedLegConvention = ModifiedFollowing;

DayCounter swFixedLegDayCounter = Actual365Fixed();



ext::shared_ptr<IborIndex> swFloatingLegIndex(new KrwCD3M);

swFloatingLegIndex->addFixing(settlementDate, cd03MQuote->value(), true);



ext::shared_ptr<RateHelper> d03M(new DepositRateHelper(Handle<Quote>(cd03MQuote), 3 * Months, 0, calendar, ModifiedFollowing, false, daycounter));

ext::shared_ptr<RateHelper> d06M(new SwapRateHelper(Handle<Quote>(sw06MQuote), 6 * Months, calendar, swFixedLegFrequency,swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));

ext::shared_ptr<RateHelper> d01Y(new SwapRateHelper(Handle<Quote>(sw01YQuote), 1 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));

ext::shared_ptr<RateHelper> d02Y(new SwapRateHelper(Handle<Quote>(sw02YQuote), 2 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));

ext::shared_ptr<RateHelper> d03Y(new SwapRateHelper(Handle<Quote>(sw03YQuote), 3 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));

ext::shared_ptr<RateHelper> d04Y(new SwapRateHelper(Handle<Quote>(sw04YQuote), 4 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));

ext::shared_ptr<RateHelper> d05Y(new SwapRateHelper(Handle<Quote>(sw05YQuote), 5 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));

ext::shared_ptr<RateHelper> d06Y(new SwapRateHelper(Handle<Quote>(sw06YQuote), 6 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));

ext::shared_ptr<RateHelper> d07Y(new SwapRateHelper(Handle<Quote>(sw07YQuote), 7 * Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));

ext::shared_ptr<RateHelper> d10Y(new SwapRateHelper(Handle<Quote>(sw10YQuote), 10 * Years, calendar, swFixedLegFrequency,swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));

ext::shared_ptr<RateHelper> d12Y(new SwapRateHelper(Handle<Quote>(sw12YQuote), 12 * Years, calendar, swFixedLegFrequency,swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));

ext::shared_ptr<RateHelper> d15Y(new SwapRateHelper(Handle<Quote>(sw15YQuote), 15 * Years, calendar, swFixedLegFrequency,swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));

ext::shared_ptr<RateHelper> d20Y(new SwapRateHelper(Handle<Quote>(sw20YQuote), 20 * Years, calendar,swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));



// ratehelper //

std::vector<ext::shared_ptr<RateHelper>> rateHelperVector;

rateHelperVector.push_back(d03M);

rateHelperVector.push_back(d06M);

rateHelperVector.push_back(d01Y);

rateHelperVector.push_back(d02Y);

rateHelperVector.push_back(d03Y);

rateHelperVector.push_back(d04Y);

rateHelperVector.push_back(d05Y);

rateHelperVector.push_back(d06Y);

rateHelperVector.push_back(d07Y);

rateHelperVector.push_back(d10Y);

rateHelperVector.push_back(d12Y);

rateHelperVector.push_back(d15Y);

rateHelperVector.push_back(d20Y);



// Term Structure //

ext::shared_ptr<YieldTermStructure> initialTS(new PiecewiseYieldCurve<Discount, LogLinear>(settlementDate, rateHelperVector, daycounter));

initialTS->enableExtrapolation();



std::vector<Date> sw_dates;

std::vector<Real> sw_dfs;

sw_dates.push_back(settlementDate);

sw_dfs.push_back(1.0);





// Smith Wilson for LEAPs //

SmithWilson sw;

Matrix P(rateHelperVector.size(), 1);

Matrix terms(rateHelperVector.size(), 1);

Matrix terms_ext(rateHelperVector.size() + 100, 1);



for (Size i = 0; i < rateHelperVector.size(); i++)

{

sw_dates.push_back(rateHelperVector[i]->maturityDate());

terms[i][0] = 

daycounter.yearFraction(settlementDate, rateHelperVector[i]->maturityDate());

P[i][0] = initialTS->discount(terms[i][0]);

terms_ext[i][0] = terms[i][0];

}



for (Size i = 0; i < 100; i++)

{

sw_dates.push_back(calendar.advance(settlementDate,

(21 + i) * Years, Unadjusted, false));



terms_ext[rateHelperVector.size() + i][0] = 

daycounter.yearFraction(settlementDate,

calendar.advance(settlementDate,

(21 + i) * Years, Unadjusted, false));

}



Matrix sw_df = sw.DiscountFactor(P, terms, terms_ext, 0.1418783, 0.0506931143155182);



for (Size i = 0; i < terms_ext.rows(); i++)

sw_dfs.push_back(sw_df[i][0]);



// Smith Wilson End //

//*********************************************************************************************//

// Path Generator //



const Size nPaths = 1000;



ext::shared_ptr<YieldTermStructure> riskFreeTS(

new InterpolatedDiscountCurve<LogLinear>(sw_dates, sw_dfs, daycounter));

riskFreeTS->enableExtrapolation();



Real spot = 367.30;

ext::shared_ptr<Quote> s0(new SimpleQuote(spot));

Real volatility = 0.209049; // 1.5year volatility 

ext::shared_ptr<BlackVolTermStructure> blackVol(new BlackConstantVol(0, calendar, volatility, daycounter));

blackVol->enableExtrapolation();





// forward

std::vector<Real> sw_fwds;

for(Size i=0;i<sw_dates.size();i++)

{ 

Real t1;

if (i == 0)

t1 = daycounter.yearFraction(sw_dates[0], sw_dates[i]);

else

t1 = daycounter.yearFraction(sw_dates[0], sw_dates[i - 1]);

Real t2 = daycounter.yearFraction(sw_dates[0], sw_dates[i]);

sw_fwds.push_back(riskFreeTS->forwardRate(t1, t2, Continuous, NoFrequency, true) * (1 - 0.2384));

}

ext::shared_ptr<ForwardRateStructure> dividendTS(new ForwardCurve(sw_dates, sw_fwds, daycounter));



// Joint Models - Hybrid //

ext::shared_ptr<BlackScholesProcess> blackScholesProcess(new BlackScholesProcess(Handle<Quote>(s0), Handle<YieldTermStructure>(riskFreeTS), Handle <BlackVolTermStructure>(blackVol)));

ext::shared_ptr<HullWhiteProcess> hwProcess(new HullWhiteProcess(Handle <YieldTermStructure>(riskFreeTS), 0.05, 0.04));

ext::shared_ptr<HybridBlackScholesHullWhiteProcess> jointProcess(new HybridBlackScholesHullWhiteProcess(blackScholesProcess, hwProcess, 0.0, HybridBlackScholesHullWhiteProcess::Discretization::BSMHullWhite));

ext::shared_ptr<HullWhite> hwModel(new HullWhite(Handle<YieldTermStructure>(riskFreeTS), 0.05, 0.04));



Date DefermentDate = Date(10, Dec, 2024);

Schedule projectionDates(todaysDate, DefermentDate, 1 * Months, calendar, Unadjusted, Unadjusted, DateGeneration::Backward, true);

Schedule premiumDates(todaysDate, DefermentDate, 1 * Months, calendar, Unadjusted, Unadjusted, DateGeneration::Backward, true);



std::vector<Date> optionMaturities;

Schedule optionSchedule(calendar.advance(todaysDate, -18 * Months, Unadjusted, true), calendar.advance(DefermentDate, 18 * Months, Unadjusted, true), 1 * Months, calendar, Unadjusted, Unadjusted, DateGeneration::Backward, true);



for (Size i = 0; i < optionSchedule.size(); i++)

{

Date d = TenBusinessDayBeforeEOM(optionSchedule.date(i), calendar);

if (d >= todaysDate)

{

optionMaturities.push_back(d);

}

}

std::vector<Date> discretizationDates = projectionDates.dates();

for (Size i = 0; i < optionMaturities.size(); i++)

{

Date d = optionMaturities[i];

if (std::find(discretizationDates.begin(), discretizationDates.end(), d) == discretizationDates.end())

{

discretizationDates.push_back(d);

}

}

std::sort(discretizationDates.begin(), discretizationDates.end());



std::vector<Time> maturities;

for (Size i = 0; i < discretizationDates.size(); i++)

maturities.push_back(daycounter.yearFraction(todaysDate, discretizationDates[i]));



// Path Generattion Execution //

Date issueDate = Date(16, Dec, 2020);

TimeGrid grid(maturities.begin(), maturities.end());

BigNatural seed = issueDate.dayOfMonth();


typedef RandomSequenceGenerator<CLGaussianRng<MersenneTwisterUniformRng>> GSG;

typedef Sample <MultiPath> sample_type;


MersenneTwisterUniformRng generator(seed);

CLGaussianRng<MersenneTwisterUniformRng> gaussianGenerator(generator);


// random nuber geneator//

const Size factors = jointProcess->factors();

GSG rsg(jointProcess->factors()* (grid.size() - 1), gaussianGenerator);



MultiPathGenerator<GSG> pathGenerator(jointProcess, grid, rsg, false);


std::map<Date, std::vector<Real>> state0;//stock

std::map<Date, std::vector<Real>> state1;// short rate


for (unsigned int i = 0; i != nPaths; i++)

{

const bool antithetic = (i % 2) == 0 ? false : true;

const sample_type path = antithetic ? pathGenerator.antithetic() : pathGenerator.next();


for (unsigned int j = 0; i != maturities.size(); j++)

{

Date thisDate = discretizationDates[j];

Array states(2);

for (Size k = 0; k < 2; ++k)

{

states[k] = path.value[k][j];



std::cout << "Path Vector :: " << states[k] << std::endl;

}

state0[thisDate].push_back(states[0]);

state1[thisDate].push_back(states[1]);

}


}



}
