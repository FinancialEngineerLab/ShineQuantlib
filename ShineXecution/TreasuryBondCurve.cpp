
//
// main.cpp
//
// Created on 2021/06/10.

//

// KTB Curve Trading //

#include <ql/quantlib.hpp>


#ifdef BOOST_MSVC

# include <ql/auto_link.hpp>

#endif


#include <iostream>

#include <iomanip>


#define LENGTH(a) (sizeof(a)/sizeof(a[0]))


using namespace std;

using namespace QuantLib;


#if defined(QL_ENABLE_SESSIONS)

namespace QuantLib

{

ThreadKey sessionId() { return {}; }

}

#endif


// par-rate approximation //

Rate parRate(const YieldTermStructure& yts,

const std::vector<Date>& dates,

const DayCounter& resultDayCounter)

{

QL_REQUIRE(dates.size() >= 2, "at least two dates are required");

Real sum = 0.0;

Time dt;

for (Size i = 1; i < dates.size(); ++i)

{

dt = resultDayCounter.yearFraction(dates[i - 1], dates[i]);

QL_REQUIRE(dt >= 0.0, "unsorted dates");

sum += yts.discount(dates[i])*dt;

}

Real result = yts.discount(dates.front()) - yts.discount(dates.back());

return result / sum;

}


void printOutput(const std::string& tag, const ext::shared_ptr<FittedBondDiscountCurve>& curve)

{

cout << tag << endl;

cout << "reference Date: " << curve->referenceDate() << endl;

cout << "number of iterations : " << curve->fitResults().numberOfIterations() << endl << endl;

}


int main(int, char*[])

{

try

{

const Size numberOfBonds = 9;

Real cleanPrice[numberOfBonds];


for (double& i : cleanPrice)

{

i = 100.0;

}


std::vector < ext::shared_ptr<SimpleQuote>> quote;

for (double i : cleanPrice)

{

ext::shared_ptr<SimpleQuote> cp(new SimpleQuote(i));

quote.push_back(cp);

}


RelinkableHandle<Quote> quoteHandle[numberOfBonds];

for (Size i = 0; i < numberOfBonds; i++)

{

quoteHandle[i].linkTo(quote[i]);

}

Integer lengths[] = { 1,2,3,4,5,7,10,15,20 };

Real coupons[] = { 0.0078,0.01030, 0.01197, 0.01504, 0.01700, 0.01951, 0.02155, 0.02249, 0.02257 };

Frequency frequency = Annual;

DayCounter dc = SimpleDayCounter();

BusinessDayConvention accrualConvention = ModifiedFollowing;

BusinessDayConvention convention = ModifiedFollowing;

Real redemption = 100.0;


Calendar calendar = SouthKorea();

Date today = calendar.adjust(Date::todaysDate());

Date origToday = today;

Settings::instance().evaluationDate() = today;

Natural bondSettlementDays = 0;

Natural curveSettlementDays = 0;


Date bondSettlementDate = calendar.advance(today, bondSettlementDays * Days);


cout << endl;

cout << "Today's date : " << today << endl;

cout << "Settlement dats : " << bondSettlementDate << endl;

cout << " Calcualating fit for bonds " << endl << endl;


std::vector<ext::shared_ptr<BondHelper>> instrumentsA;

std::vector<ext::shared_ptr<RateHelper>> instrumentsB;


for (Size j = 0; j < LENGTH(lengths); j++)

{

Date maturity = calendar.advance(bondSettlementDate, lengths[j] * Years);

Schedule schedule(bondSettlementDate, maturity, Period(frequency), calendar, accrualConvention, accrualConvention, DateGeneration::Backward, false);

ext::shared_ptr<BondHelper> helperA(new FixedRateBondHelper(quoteHandle[j], bondSettlementDays, 100.0, schedule, std::vector<Rate>(1, coupons[j]), dc, convention, redemption));

ext::shared_ptr<RateHelper> helperB(new FixedRateBondHelper(quoteHandle[j], bondSettlementDays, 100.0, schedule, std::vector<Rate>(1, coupons[j]), dc, convention, redemption));


instrumentsA.push_back(helperA);

instrumentsB.push_back(helperB);

}


bool constrainAtZero = true;

Real tolerance = 1.0e-10;

Size max =5000;


ext::shared_ptr<YieldTermStructure> ts0(new PiecewiseYieldCurve<Discount, LogLinear>(curveSettlementDays, calendar, instrumentsB, dc));

ext::shared_ptr<SimpleQuote> LP = ext::make_shared<SimpleQuote>(0.00456);

ext::shared_ptr<ZeroSpreadedTermStructure> ts000(new ZeroSpreadedTermStructure(Handle<YieldTermStructure>(ts0), Handle<Quote>(LP), Compounded, Annual, dc));

ts000->enableExtrapolation();


cout << "*************************************** Simple Splines *******************************************" << endl;


ExponentialSplinesFitting exponentialSplines(constrainAtZero);

ext::shared_ptr<FittedBondDiscountCurve> ts1(new FittedBondDiscountCurve(curveSettlementDays, calendar, instrumentsA, dc, exponentialSplines, tolerance, max));

printOutput("(a) Exponential Splines", ts1);

ext::shared_ptr<ZeroSpreadedTermStructure> ts01(new ZeroSpreadedTermStructure(Handle<YieldTermStructure>(ts1), Handle<Quote>(LP), Compounded, Annual, dc));



SimplePolynomialFitting simplePolynomial(3, constrainAtZero);

ext::shared_ptr<FittedBondDiscountCurve> ts2(new FittedBondDiscountCurve(curveSettlementDays, calendar, instrumentsA, dc, simplePolynomial, tolerance, max));

ext::shared_ptr<ZeroSpreadedTermStructure> ts02(new ZeroSpreadedTermStructure(Handle<YieldTermStructure>(ts2), Handle<Quote>(LP), Compounded, Annual, dc));

printOutput("(b) Simple Polynomial", ts2);


NelsonSiegelFitting nelsonSiegel;

ext::shared_ptr<FittedBondDiscountCurve> ts3(new FittedBondDiscountCurve(curveSettlementDays, calendar, instrumentsA, dc, nelsonSiegel, tolerance, max));

ext::shared_ptr<ZeroSpreadedTermStructure> ts03(new ZeroSpreadedTermStructure(Handle<YieldTermStructure>(ts3), Handle<Quote>(LP), Compounded, Annual, dc));

printOutput("(c) Nelson - Siegel", ts3);


cout << "*************************************** Cubic Splines *******************************************" << endl;


Time knots[] = { -30.0, -20.0, 0.0, 5.0, 10.0, 15.0, 20.0, 25.0, 30.0, 40.0, 50.0 };

std::vector<Time> knotVector;

for (double& knot : knots)

{

knotVector.push_back(knot);

}

CubicBSplinesFitting cubicBSplines(knotVector, constrainAtZero);

ext::shared_ptr<FittedBondDiscountCurve> ts4(new FittedBondDiscountCurve(curveSettlementDays, calendar, instrumentsA, dc, cubicBSplines, tolerance, max));

ext::shared_ptr<ZeroSpreadedTermStructure> ts04(new ZeroSpreadedTermStructure(Handle<YieldTermStructure>(ts4), Handle<Quote>(LP), Compounded, Annual, dc));

printOutput("(d) Cubic B Splines", ts4);


SvenssonFitting svensson;

ext::shared_ptr<FittedBondDiscountCurve> ts5(new FittedBondDiscountCurve(curveSettlementDays, calendar, instrumentsA, dc, svensson, tolerance, max));

ext::shared_ptr<ZeroSpreadedTermStructure> ts05(new ZeroSpreadedTermStructure(Handle<YieldTermStructure>(ts5), Handle<Quote>(LP), Compounded, Annual, dc));

printOutput("(e) Svensson ", ts5);


Handle<YieldTermStructure> discountCurve(ext::make_shared<FlatForward>(curveSettlementDays, calendar, 0.01, dc));

SpreadFittingMethod nelsonSiegelSpread(ext::make_shared<NelsonSiegelFitting>(), discountCurve);

ext::shared_ptr<FittedBondDiscountCurve> ts6(new FittedBondDiscountCurve(curveSettlementDays, calendar, instrumentsA, dc, nelsonSiegelSpread, tolerance, max));

ext::shared_ptr<ZeroSpreadedTermStructure> ts06(new ZeroSpreadedTermStructure(Handle<YieldTermStructure>(ts6), Handle<Quote>(LP), Compounded, Annual, dc));

printOutput("(f) Nelson-Siegel Spreaded ", ts6);


// Fixed Kappa and 7 Coefficients //

ExponentialSplinesFitting exponentialSplinesFixed(constrainAtZero, 7, 0.02);

ext::shared_ptr<FittedBondDiscountCurve> ts7(new FittedBondDiscountCurve(curveSettlementDays, calendar, instrumentsA, dc, exponentialSplinesFixed, tolerance, max));

ext::shared_ptr<ZeroSpreadedTermStructure> ts07(new ZeroSpreadedTermStructure(Handle<YieldTermStructure>(ts7), Handle<Quote>(LP), Compounded, Annual, dc));

printOutput("(g) Exponential Splines, fixed Kappa ", ts7);



cout << "Output par rates for each curve. In this case. "

<< endl

<< "par rates should equal coupons for these par bonds."

<< endl

<< endl;


cout<< setw(6)<<" Tenor" << " | "

<< setw(6) << " Coupon" << " | "

<< setw(6) << " Bstrp" << " | "

<< setw(6) << " (a) " << " | "

<< setw(6) << " (b) " << " | "

<< setw(6) << " (c) " << " | "

<< setw(6) << " (d) " << " | "

<< setw(6) << " (e) " << " | "

<< setw(6) << " (f) " << " | "

<< setw(6) << " (g) " << endl;


for (Size i = 0; i < instrumentsA.size(); i++)

{

std::vector<ext::shared_ptr<CashFlow>> cfs = instrumentsA[i]->bond()->cashflows();

Size cfSize = instrumentsA[i]->bond()->cashflows().size();

std::vector<Date> keyDates;

keyDates.push_back(bondSettlementDate);


for (Size j = 0; j < cfSize - 1; j++)

{

if (!cfs[j]->hasOccurred(bondSettlementDate, false))

{

Date myDate = cfs[j]->date();

keyDates.push_back(myDate);

}

}


Real tenor = dc.yearFraction(today, cfs[cfSize - 1]->date());


cout << setw(6) << fixed << setprecision(3) << tenor << "|"

<< setw(6) << fixed << setprecision(3)

<< 100.0 * coupons[i] << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts0, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts1, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts2, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts3, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts4, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts5, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts6, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts7, keyDates, dc) << endl;

}



cout << "Output +LP Spread for each curve. In this case. "

<< endl

<< endl;


cout << setw(6) << " Tenor" << " | "

<< setw(6) << " Coupon" << " | "

<< setw(6) << " Bstrp" << " | "

<< setw(6) << " (a) " << " | "

<< setw(6) << " (b) " << " | "

<< setw(6) << " (c) " << " | "

<< setw(6) << " (d) " << " | "

<< setw(6) << " (e) " << " | "

<< setw(6) << " (f) " << " | "

<< setw(6) << " (g) " << endl;


for (Size i = 0; i < instrumentsA.size(); i++)

{

std::vector<ext::shared_ptr<CashFlow>> cfs = instrumentsA[i]->bond()->cashflows();

Size cfSize = instrumentsA[i]->bond()->cashflows().size();

std::vector<Date> keyDates;

keyDates.push_back(bondSettlementDate);


for (Size j = 0; j < cfSize - 1; j++)

{

if (!cfs[j]->hasOccurred(bondSettlementDate, false))

{

Date myDate = cfs[j]->date();

keyDates.push_back(myDate);

}

}


Real tenor = dc.yearFraction(today, cfs[cfSize - 1]->date());


cout << setw(6) << fixed << setprecision(3) << tenor << "|"

<< setw(6) << fixed << setprecision(3)

<< 100.0 * coupons[i] << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts000, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts01, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts02, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts03, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts04, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts05, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts06, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts07, keyDates, dc) << endl;

}


cout << endl << endl << endl;

cout << "***** +5bp Par Bumping *****" << endl

<< endl

<< endl;


for (Size k = 0; k < LENGTH(lengths) - 1; k++)

{

Real P = instrumentsA[k]->quote()->value();

const Bond& b = *instrumentsA[k]->bond();

Rate ytm = BondFunctions::yield(b, P, dc, Compounded, frequency, today);

Time dur = BondFunctions::duration(b, ytm, dc, Compounded, frequency, Duration::Modified, today);

const Real bpsChange = -5.;

Real deltaP = -dur * P * (bpsChange / 10000.);

quote[k + 1]->setValue(P + deltaP);

}



cout << setw(6) << "tenor" << " | "

<< setw(6) << "coupon" << " | "

<< setw(6) << "bstrap" << " | "

<< setw(6) << "(a)" << " | "

<< setw(6) << "(b)" << " | "

<< setw(6) << "(c)" << " | "

<< setw(6) << "(d)" << " | "

<< setw(6) << "(e)" << " | "

<< setw(6) << "(f)" << " | "

<< setw(6) << "(g)" << endl;


for (Size i = 0; i < instrumentsA.size(); i++)

{

std::vector<ext::shared_ptr<CashFlow>> cfs =

instrumentsA[i]->bond()->cashflows();

Size cfSize = instrumentsA[i]->bond()->cashflows().size();

std::vector<Date> keyDates;

keyDates.push_back(bondSettlementDate);


for (Size j = 0; j < cfSize - 1; j++)

{

if (!cfs[j]->hasOccurred(bondSettlementDate, false))

{

Date myDate = cfs[j]->date();

keyDates.push_back(myDate);

}

}

Real tenor = dc.yearFraction(today, cfs[cfSize - 1]->date());


cout << setw(6) << fixed << setprecision(3) << tenor << "|"

<< setw(6) << fixed << setprecision(3)

<< 100.0 * coupons[i] << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts0, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts1, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts2, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts3, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts4, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts5, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts6, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts7, keyDates, dc) << endl;



}


cout << endl << endl << endl;

cout << "***** -5bp Par Bumping *****" << endl

<< endl

<< endl;


for (Size k = 0; k < LENGTH(lengths) - 1; k++)

{

Real P = instrumentsA[k]->quote()->value();

const Bond& b = *instrumentsA[k]->bond();

Rate ytm = BondFunctions::yield(b, P, dc, Compounded, frequency, today);

Time dur = BondFunctions::duration(b, ytm, dc, Compounded, frequency, Duration::Modified, today);

const Real bpsChange = 5.;

Real deltaP = -dur * P * (bpsChange / 10000.);

quote[k + 1]->setValue(P + deltaP);

}



cout << setw(6) << "tenor" << " | "

<< setw(6) << "coupon" << " | "

<< setw(6) << "bstrap" << " | "

<< setw(6) << "(a)" << " | "

<< setw(6) << "(b)" << " | "

<< setw(6) << "(c)" << " | "

<< setw(6) << "(d)" << " | "

<< setw(6) << "(e)" << " | "

<< setw(6) << "(f)" << " | "

<< setw(6) << "(g)" << endl;



for (Size i = 0; i < instrumentsA.size(); i++)

{

std::vector<ext::shared_ptr<CashFlow>> cfs =

instrumentsA[i]->bond()->cashflows();

Size cfSize = instrumentsA[i]->bond()->cashflows().size();

std::vector<Date> keyDates;

keyDates.push_back(bondSettlementDate);


for (Size j = 0; j < cfSize - 1; j++)

{

if (!cfs[j]->hasOccurred(bondSettlementDate, false))

{

Date myDate = cfs[j]->date();

keyDates.push_back(myDate);

}

}

Real tenor = dc.yearFraction(today, cfs[cfSize - 1]->date());


cout << setw(6) << fixed << setprecision(3) << tenor << "|"

<< setw(6) << fixed << setprecision(3)

<< 100.0 * coupons[i] << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts0, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts1, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts2, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts3, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts4, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts5, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts6, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts7, keyDates, dc) << endl;


}





cout << endl << endl << endl;

cout << "it is in today + 11 Months for Validation ! " << endl

<< "Par rates <> Coupons(YTM)" << endl

<< "But, the piecewise par curve can be used as benchmark for validating par rates" << endl

<< endl;



today = calendar.advance(origToday, 11, Months, convention);

Settings::instance().evaluationDate() = today;

bondSettlementDate = calendar.advance(today, bondSettlementDays*Days);


printOutput("(a) exponential splines", ts1);


printOutput("(b) simple polynomial", ts2);


printOutput("(c) Nelson-Siegel", ts3);


printOutput("(d) cubic B-splines", ts4);


printOutput("(e) Svensson", ts5);


printOutput("(f) Nelson-Siegel spreaded", ts6);


printOutput("(g) exponential spline, fixed kappa", ts7);


cout << endl

<< endl;



cout << setw(6) << "tenor" << " | "

<< setw(6) << "coupon" << " | "

<< setw(6) << "bstrap" << " | "

<< setw(6) << "(a)" << " | "

<< setw(6) << "(b)" << " | "

<< setw(6) << "(c)" << " | "

<< setw(6) << "(d)" << " | "

<< setw(6) << "(e)" << " | "

<< setw(6) << "(f)" << " | "

<< setw(6) << "(g)" << endl;


for (Size i = 0; i < instrumentsA.size(); i++) {


std::vector<ext::shared_ptr<CashFlow> > cfs =

instrumentsA[i]->bond()->cashflows();


Size cfSize = instrumentsA[i]->bond()->cashflows().size();

std::vector<Date> keyDates;

keyDates.push_back(bondSettlementDate);


for (Size j = 0; j < cfSize - 1; j++) {

if (!cfs[j]->hasOccurred(bondSettlementDate, false)) {

Date myDate = cfs[j]->date();

keyDates.push_back(myDate);

}

}


Real tenor = dc.yearFraction(today, cfs[cfSize - 1]->date());


cout << setw(6) << fixed << setprecision(3) << tenor << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.*coupons[i] << " | "

// piecewise bootstrap

<< setw(6) << fixed << setprecision(3)

<< 100.*parRate(*ts0, keyDates, dc) << " | "

// exponential splines

<< setw(6) << fixed << setprecision(3)

<< 100.*parRate(*ts1, keyDates, dc) << " | "

// simple polynomial

<< setw(6) << fixed << setprecision(3)

<< 100.*parRate(*ts2, keyDates, dc) << " | "

// Nelson-Siegel

<< setw(6) << fixed << setprecision(3)

<< 100.*parRate(*ts3, keyDates, dc) << " | "

// cubic bsplines

<< setw(6) << fixed << setprecision(3)

<< 100.*parRate(*ts4, keyDates, dc) << " | "

// Svensson

<< setw(6) << fixed << setprecision(3)

<< 100.*parRate(*ts5, keyDates, dc) << " | "

// Nelson-Siegel Spreaded

<< setw(6) << fixed << setprecision(3)

<< 100.*parRate(*ts6, keyDates, dc) << " | "

// exponential, fixed kappa

<< setw(6) << fixed << setprecision(3)

<< 100. * parRate(*ts7, keyDates, dc) << endl;

}


cout << endl << endl << endl;

cout << "***** +LP +5bp Par Bumping *****" << endl

<< endl

<< endl;



for (Size k = 0; k < LENGTH(lengths) - 1; k++)

{

Real P = instrumentsA[k]->quote()->value();

const Bond& b = *instrumentsA[k]->bond();

Rate ytm = BondFunctions::yield(b, P, dc, Compounded, frequency, today);

Time dur = BondFunctions::duration(b, ytm, dc, Compounded, frequency, Duration::Modified, today);

const Real bpsChange = -5.;

Real deltaP = -dur * P * (bpsChange / 10000.);

quote[k + 1]->setValue(P + deltaP);

}





cout << setw(6) << "tenor" << " | "

<< setw(6) << "coupon" << " | "

<< setw(6) << "bstrap" << " | "

<< setw(6) << "(a)" << " | "

<< setw(6) << "(b)" << " | "

<< setw(6) << "(c)" << " | "

<< setw(6) << "(d)" << " | "

<< setw(6) << "(e)" << " | "

<< setw(6) << "(f)" << " | "

<< setw(6) << "(g)" << endl;



for (Size i = 0; i < instrumentsA.size(); i++)

{

std::vector<ext::shared_ptr<CashFlow>> cfs =

instrumentsA[i]->bond()->cashflows();

Size cfSize = instrumentsA[i]->bond()->cashflows().size();

std::vector<Date> keyDates;

keyDates.push_back(bondSettlementDate);



for (Size j = 0; j < cfSize - 1; j++)

{

if (!cfs[j]->hasOccurred(bondSettlementDate, false))

{

Date myDate = cfs[j]->date();

keyDates.push_back(myDate);

}

}

Real tenor = dc.yearFraction(today, cfs[cfSize - 1]->date());



cout << setw(6) << fixed << setprecision(3) << tenor << "|"

<< setw(6) << fixed << setprecision(3)

<< 100.0 * coupons[i] << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts000, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts01, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts02, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts03, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts04, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts05, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts06, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts07, keyDates, dc) << endl;





}



cout << endl << endl << endl;

cout << "***** +LP -5bp Par Bumping *****" << endl

<< endl

<< endl;



for (Size k = 0; k < LENGTH(lengths) - 1; k++)

{

Real P = instrumentsA[k]->quote()->value();

const Bond& b = *instrumentsA[k]->bond();

Rate ytm = BondFunctions::yield(b, P, dc, Compounded, frequency, today);

Time dur = BondFunctions::duration(b, ytm, dc, Compounded, frequency, Duration::Modified, today);

const Real bpsChange = 5.;

Real deltaP = -dur * P * (bpsChange / 10000.);

quote[k + 1]->setValue(P + deltaP);

}





cout << setw(6) << "tenor" << " | "

<< setw(6) << "coupon" << " | "

<< setw(6) << "bstrap" << " | "

<< setw(6) << "(a)" << " | "

<< setw(6) << "(b)" << " | "

<< setw(6) << "(c)" << " | "

<< setw(6) << "(d)" << " | "

<< setw(6) << "(e)" << " | "

<< setw(6) << "(f)" << " | "

<< setw(6) << "(g)" << endl;





for (Size i = 0; i < instrumentsA.size(); i++)

{

std::vector<ext::shared_ptr<CashFlow>> cfs =

instrumentsA[i]->bond()->cashflows();

Size cfSize = instrumentsA[i]->bond()->cashflows().size();

std::vector<Date> keyDates;

keyDates.push_back(bondSettlementDate);



for (Size j = 0; j < cfSize - 1; j++)

{

if (!cfs[j]->hasOccurred(bondSettlementDate, false))

{

Date myDate = cfs[j]->date();

keyDates.push_back(myDate);

}

}

Real tenor = dc.yearFraction(today, cfs[cfSize - 1]->date());



cout << setw(6) << fixed << setprecision(3) << tenor << "|"

<< setw(6) << fixed << setprecision(3)

<< 100.0 * coupons[i] << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts000, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts01, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts02, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts03, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts04, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts05, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts06, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts07, keyDates, dc) << endl;



}









cout << endl << endl << endl;
cout << " ********************************************" << endl;
cout << "it is in today + 11 Months for Validation ! " << endl

<< "Par rates <> Coupons(YTM)" << endl

<< "But, the piecewise par curve can be used as benchmark for validating par rates" << endl

<< endl;





today = calendar.advance(origToday, 11, Months, convention);

Settings::instance().evaluationDate() = today;

bondSettlementDate = calendar.advance(today, bondSettlementDays*Days);



printOutput("(a) exponential splines", ts1);



printOutput("(b) simple polynomial", ts2);



printOutput("(c) Nelson-Siegel", ts3);



printOutput("(d) cubic B-splines", ts4);



printOutput("(e) Svensson", ts5);



printOutput("(f) Nelson-Siegel spreaded", ts6);



printOutput("(g) exponential spline, fixed kappa", ts7);



cout << endl

<< endl;





cout << setw(6) << "tenor" << " | "

<< setw(6) << "coupon" << " | "

<< setw(6) << "bstrap" << " | "

<< setw(6) << "(a)" << " | "

<< setw(6) << "(b)" << " | "

<< setw(6) << "(c)" << " | "

<< setw(6) << "(d)" << " | "

<< setw(6) << "(e)" << " | "

<< setw(6) << "(f)" << " | "

<< setw(6) << "(g)" << endl;



for (Size i = 0; i < instrumentsA.size(); i++) {



std::vector<ext::shared_ptr<CashFlow> > cfs =

instrumentsA[i]->bond()->cashflows();



Size cfSize = instrumentsA[i]->bond()->cashflows().size();

std::vector<Date> keyDates;

keyDates.push_back(bondSettlementDate);



for (Size j = 0; j < cfSize - 1; j++) {

if (!cfs[j]->hasOccurred(bondSettlementDate, false)) {

Date myDate = cfs[j]->date();

keyDates.push_back(myDate);

}

}



Real tenor = dc.yearFraction(today, cfs[cfSize - 1]->date());



cout << setw(6) << fixed << setprecision(3) << tenor << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.*coupons[i] << " | "

// piecewise bootstrap

<< setw(6) << fixed << setprecision(3)

<< 100.*parRate(*ts0, keyDates, dc) << " | "

// exponential splines

<< setw(6) << fixed << setprecision(3)

<< 100.*parRate(*ts1, keyDates, dc) << " | "

// simple polynomial

<< setw(6) << fixed << setprecision(3)

<< 100.*parRate(*ts2, keyDates, dc) << " | "

// Nelson-Siegel

<< setw(6) << fixed << setprecision(3)

<< 100.*parRate(*ts3, keyDates, dc) << " | "

// cubic bsplines

<< setw(6) << fixed << setprecision(3)

<< 100.*parRate(*ts4, keyDates, dc) << " | "

// Svensson

<< setw(6) << fixed << setprecision(3)

<< 100.*parRate(*ts5, keyDates, dc) << " | "

// Nelson-Siegel Spreaded

<< setw(6) << fixed << setprecision(3)

<< 100.*parRate(*ts6, keyDates, dc) << " | "

// exponential, fixed kappa

<< setw(6) << fixed << setprecision(3)

<< 100. * parRate(*ts7, keyDates, dc) << endl;

}




cout << endl << endl << endl;

cout << "it is in today + 1 Years for Validation ! "<< endl

<< "Par rates <> Coupons(YTM)" << endl

<< "But, the piecewise par curve can be used as benchmark for validating par rates" << endl

<< endl;


instrumentsA.erase(instrumentsA.begin(), instrumentsA.begin() + 1);

instrumentsB.erase(instrumentsB.begin(), instrumentsB.begin() + 1);


today = calendar.advance(origToday, 12, Months, convention);

Settings::instance().evaluationDate() = today;

bondSettlementDate = calendar.advance(today, bondSettlementDays * Days);


ext::shared_ptr<YieldTermStructure> ts00(new PiecewiseYieldCurve<Discount, LogLinear>(curveSettlementDays, calendar, instrumentsB, dc));

ext::shared_ptr<FittedBondDiscountCurve> ts11(new FittedBondDiscountCurve(curveSettlementDays, calendar, instrumentsA, dc, exponentialSplines, tolerance, max));

printOutput("(a) Exponential Splines", ts11);

ext::shared_ptr<FittedBondDiscountCurve> ts22(new FittedBondDiscountCurve(curveSettlementDays, calendar, instrumentsA, dc, simplePolynomial, tolerance, max));

printOutput("(b) Simple Polynomial", ts22);

ext::shared_ptr<FittedBondDiscountCurve> ts33(new FittedBondDiscountCurve(curveSettlementDays, calendar, instrumentsA, dc, nelsonSiegel, tolerance, max));

printOutput("(c) Nelson Siegel", ts33);

ext::shared_ptr<FittedBondDiscountCurve> ts44(new FittedBondDiscountCurve(curveSettlementDays, calendar, instrumentsA, dc, cubicBSplines, tolerance, max));

printOutput("(d) Cubic B-Splines", ts44);

ext::shared_ptr<FittedBondDiscountCurve> ts55(new FittedBondDiscountCurve(curveSettlementDays, calendar, instrumentsA, dc, svensson, tolerance, max));

printOutput("(e) Svensson", ts55);

ext::shared_ptr<FittedBondDiscountCurve> ts66(new FittedBondDiscountCurve(curveSettlementDays, calendar, instrumentsA, dc, nelsonSiegelSpread, tolerance, max));

printOutput("(f) Nelson Siegel Spreaded", ts66);

ext::shared_ptr<FittedBondDiscountCurve> ts77(new FittedBondDiscountCurve(curveSettlementDays, calendar, instrumentsA, dc, exponentialSplinesFixed, tolerance, max));

printOutput("(g) exponential, fixed kappa", ts77);


cout << setw(6) << "Tenor " << " | "

<< setw(6) << "Coupon " << "| "

<< setw(6) << "bstrap " << "| "

<< setw(6) << "(a) " << "| "

<< setw(6) << "(b) " << "| "

<< setw(6) << "(c) " << "| "

<< setw(6) << "(d) " << "| "

<< setw(6) << "(e) " << "| "

<< setw(6) << "(f) " << "| "

<< setw(6) << "(g) " << endl;


for (Size i = 0; i < instrumentsA.size(); i++)

{

std::vector<ext::shared_ptr<CashFlow>> cfs =

instrumentsA[i]->bond()->cashflows();

Size cfSize = instrumentsA[i]->bond()->cashflows().size();

std::vector<Date> keyDates;

keyDates.push_back(bondSettlementDate);


for (Size j = 0; j < cfSize - 1; j++)

{

if (!cfs[j]->hasOccurred(bondSettlementDate, false))

{

Date myDate = cfs[j]->date();

keyDates.push_back(myDate);

}

}

Real tenor = dc.yearFraction(today, cfs[cfSize - 1]->date());


cout << setw(6) << fixed << setprecision(3) << tenor << "|"

<< setw(6) << fixed << setprecision(3)

<< 100.0 * coupons[i+1] << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts00, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts11, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts22, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts33, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts44, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts55, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts66, keyDates, dc) << " | "

<< setw(6) << fixed << setprecision(3)

<< 100.0 * parRate(*ts77, keyDates, dc) << endl;


}



return 0;

}


catch (std::exception& e)

{

cerr << e.what() << endl;

return 1;

}

catch (...)

{

cerr << "Unknown Error " << endl;

return 1;

}



}
