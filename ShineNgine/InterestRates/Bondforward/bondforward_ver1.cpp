
#include <ql/quantlib.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/time/daycounters/actualactual.hpp>
#include <ql/instruments/bonds/fixedratebond.hpp>
#include <ql/pricingengines/bond/discountingbondengine.hpp>

#include <iostream>
#include <iomanip>

#include "utilities.h"
#include "bondforward.hpp"

#define LENGTH(a) (sizeof(a)/sizeof(a[0]))

using namespace std;
using namespace QuantLib;

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {
    ThreadKey sessionId() { return {}; }
}
#endif


ext::shared_ptr<Bond> buildBond(const Date &issue,
                                const Date &maturity,
                                Rate cpn)
{
    Schedule sch(issue, maturity, Period(Semiannual), TARGET(), Following, Following,
                 DateGeneration::Backward, false);

    return ext::make_shared<FixedRateBond>(2, 1.e5, sch, std::vector<Rate>(1, cpn),
                                           ActualActual(ActualActual::ISDA));
}
ext::shared_ptr<BondForward> buildBondForward(const ext::shared_ptr<Bond>& underlying,
                                              const Handle<YieldTermStructure> &handle,
                                              const Date& delivery,
                                              Position::Type type) {
    auto valueDt = handle->referenceDate();
    return ext::make_shared<BondForward>(valueDt, delivery, type, 0.0, 2,
                                         ActualActual(ActualActual::ISDA),
                                         TARGET(), Following, underlying, handle, handle);
}


// par-rate approximation
Rate parRate(const YieldTermStructure& yts,
             const std::vector<Date>& dates,
             const DayCounter& resultDayCounter) {
    QL_REQUIRE(dates.size() >= 2, "at least two dates are required");
    Real sum = 0.0;
    Time dt;
    for (Size i=1; i<dates.size(); ++i) {
        dt = resultDayCounter.yearFraction(dates[i-1], dates[i]);
        QL_REQUIRE(dt>=0.0, "unsorted dates");
        sum += yts.discount(dates[i]) * dt;
    }
    Real result = yts.discount(dates.front()) - yts.discount(dates.back());
    return result/sum;
}

void printOutput(const std::string& tag,
                 const ext::shared_ptr<FittedBondDiscountCurve>& curve) {
    cout << tag << endl;
    cout << "reference date : "
         << curve->referenceDate()
         << endl;
    cout << "number of iterations : "
         << curve->fitResults().numberOfIterations()
         << endl
         << endl;
}


int main(int, char* []) {

    try {

        const Size numberOfBonds = 14;
        Real cleanPrice[numberOfBonds];

        for (double& i : cleanPrice)
        {
            i = 100.0;
        }

        std::vector< ext::shared_ptr<SimpleQuote> > quote;
        for (double i : cleanPrice)
        {
            ext::shared_ptr<SimpleQuote> cp(new SimpleQuote(i));
            quote.push_back(cp);
        }

        RelinkableHandle<Quote> quoteHandle[numberOfBonds];
        for (Size i=0; i<numberOfBonds; i++) {
            quoteHandle[i].linkTo(quote[i]);
        }
        
        Integer lengths[] = { 3, 6, 9, 12, 24,
            36, 48, 60,84, 120,
            180, 240, 360};
        Real coupons[] = { 0.01345, 0.01434, 0.01564, 0.01680, 0.02218,
                           0.02515, 0.02697, 0.02727, 0.02842, 0.02877,
                           0.02879, 0.0287, 0.02777};

        Frequency frequency = Semiannual;
        DayCounter dc = SimpleDayCounter();
        BusinessDayConvention accrualConvention = Following;
        BusinessDayConvention convention = Following;
        Real redemption = 100.0;

        Calendar calendar = SouthKorea();
        Date today = calendar.adjust(Date(25, Mar, 2022));
      //  Date origToday = today;
        Settings::instance().evaluationDate() = today;

        // changing bondSettlementDays=3 increases calculation
        // time of exponentialsplines fitting method
        Natural bondSettlementDays = 1;
        Natural curveSettlementDays = 1;

        Date bondSettlementDate = calendar.advance(today, bondSettlementDays*Days);

        cout << endl;
        cout << "Today's date: " << today << endl;
        cout << "Bonds' settlement date: " << bondSettlementDate << endl;
        cout << "Calculating fit for 15 bonds....." << endl << endl;

        std::vector<ext::shared_ptr<BondHelper> > instrumentsA;
        std::vector<ext::shared_ptr<RateHelper> > instrumentsB;

        for (Size j=0; j<LENGTH(lengths); j++) {

            Date maturity = calendar.advance(bondSettlementDate, lengths[j]*Months);

            Schedule schedule(bondSettlementDate, maturity, Period(frequency),
                              calendar, accrualConvention, accrualConvention,
                              DateGeneration::Backward, false);

            ext::shared_ptr<BondHelper> helperA(
                     new FixedRateBondHelper(quoteHandle[j],
                                             bondSettlementDays,
                                             100.0,
                                             schedule,
                                             std::vector<Rate>(1,coupons[j]),
                                             dc,
                                             convention,
                                             redemption));

            ext::shared_ptr<RateHelper> helperB(
                     new FixedRateBondHelper(quoteHandle[j],
                                             bondSettlementDays,
                                             100.0,
                                             schedule,
                                             std::vector<Rate>(1, coupons[j]),
                                             dc,
                                             convention,
                                             redemption));
            instrumentsA.push_back(helperA);
            instrumentsB.push_back(helperB);
        }


       // bool constrainAtZero = true;
        //Real tolerance = 1.0e-10;
        //Size max = 5000;

        ext::shared_ptr<YieldTermStructure> ts0 (
              new PiecewiseYieldCurve<Discount,LogLinear>(curveSettlementDays,
                                                          calendar,
                                                          instrumentsB,
                                                          dc));
        
        
        // Term structures that will be used for pricing:
        // the one used for discounting cash flows
        RelinkableHandle<YieldTermStructure> discountingTermStructure;
        // the one used for forward rate forecasting
        discountingTermStructure.linkTo(ts0);

        // Pricing engine
        ext::shared_ptr<PricingEngine> bondEngine(
                new DiscountingBondEngine(discountingTermStructure));

        /*
        // Fixed 4.5% US Treasury Note
        Schedule fixedBondSchedule(Date(15, May, 2007),
                        Date(15,May,2017), Period(Semiannual),
                        SouthKorea(),
                                   Following, Following, DateGeneration::Backward, false);

                FixedRateBond fixedRateBond(
                                            bondSettlementDays,
                                            redemption,
                        fixedBondSchedule,
                        std::vector<Rate>(1, 0.045),
                        ActualActual(ActualActual::Bond),
                        Following,
                        100.0, Date(15, May, 2007));

                fixedRateBond.setPricingEngine(bondEngine);
        
        */
        
        cout<< endl;
        cout<< endl;
        
        cout << "Testing clean forward price replication..." << endl;
        
       // Real tolerance = 1.0e-10;

        Date issue(25, Mar, 2022);
        Date maturity = calendar.advance(issue, 30*Years);//issue.advance(30*Years);//))(15, August, 2046);
        Rate cpn = 0.025;

        auto bnd = buildBond(issue, maturity, cpn);
       // auto pricer = ext::make_shared<DiscountingBondEngine>(vars.curveHandle);
        bnd->setPricingEngine(bondEngine);

        Date delivery= calendar.advance(issue, 3*Years);
        auto bndFwd = buildBondForward(bnd, discountingTermStructure, delivery, Position::Long);

        auto fwdCleanPrice = bndFwd->cleanForwardPrice();
        auto fwdPrice = bndFwd->forwardValue();
        auto expectedFwdCleanPrice = bndFwd->forwardValue() - bnd->accruedAmount(delivery);
        auto underlyingCleanPrice = bnd->cleanPrice();
        
        std::cout << "Forward Price Cleaned = " << fwdCleanPrice << std::endl;
        std::cout << "Forward Dirty Price = " << fwdPrice <<std::endl;
        std::cout << "Expected Forward Price Cleaned = " << expectedFwdCleanPrice << std::endl;
        std::cout << "Undrlying Dirty Price= " << underlyingCleanPrice << std::endl;
     
        /*
        if (std::fabs(fwdCleanPrice - expectedFwdCleanPrice) > tolerance)
            cout << "unable to replicate clean forward price\n"
                        << std::setprecision(5) << "    calculated:    " << fwdCleanPrice << "\n"
            << "    expected:    " << expectedFwdCleanPrice << "\n" << endl;
        */
        
        std::cout << std::endl;
        std::cout << " Par Bumping Greeks " << std::endl;
    
        
        auto bnd_temp = buildBond(issue, maturity, cpn);
        for (Size i =0; i<LENGTH(lengths);i++)
        {
            coupons[i] = coupons[i]+0.0001;
            std::vector<ext::shared_ptr<RateHelper> > instrumentsC;
            Real bndFwdvalue_temp;
            Real keyrate_rho;
            
            for(Size j=0;j<LENGTH(lengths);j++)
            {
                Date maturity = calendar.advance(bondSettlementDate, lengths[j]*Months);
                

                    Schedule schedule(bondSettlementDate, maturity, Period(frequency),
                                      calendar, accrualConvention, accrualConvention,
                                      DateGeneration::Backward, false);
                
                    ext::shared_ptr<RateHelper> helperC(
                             new FixedRateBondHelper(quoteHandle[j],
                                                     bondSettlementDays,
                                                     100.0,
                                                     schedule,
                                                     std::vector<Rate>(1, coupons[j]),
                                                     dc,
                                                     convention,
                                                     redemption));
                    instrumentsC.push_back(helperC);
            }
            ext::shared_ptr<YieldTermStructure> ts_temp (
                      new PiecewiseYieldCurve<Discount,LogLinear>(curveSettlementDays,
                                                                  calendar,
                                                                  instrumentsC,
                                                                  dc));
                
                RelinkableHandle<YieldTermStructure> discountingTermStructure_temp;
                discountingTermStructure_temp.linkTo(ts_temp);
                ext::shared_ptr<PricingEngine> bondEngine_temp(
                                                          new DiscountingBondEngine(discountingTermStructure_temp));
                bnd_temp->setPricingEngine(bondEngine_temp);
                auto bndFwd_temp = buildBondForward(bnd_temp, discountingTermStructure_temp, delivery, Position::Long);
                bndFwdvalue_temp = bndFwd_temp->forwardValue();
            
            keyrate_rho = (bndFwdvalue_temp - fwdPrice)/ 0.0001;
            std::cout << i+1 << "th Key Rate Rho= " <<keyrate_rho << std::endl;
            coupons[i] = coupons[i]-0.0001;
        }


       // bool constrainAtZero = true;
        //Real tolerance = 1.0e-10;
        //Size max = 5000;

        
        std::cout << std::endl;
        std::cout << std::endl;

        
       
        std::cout << "Testing that forward value is equal to spot value if no income..." << std::endl;

            //using namespace bond_forward_test;

         //   CommonVars vars;

            //Real tolerance = 1.0e-2;

          //  Date issue(15, August, 2015);
         //   Date maturity(15, August, 2046);
         //   Rate cpn = 0.025;

            auto bnd2 = buildBond(issue, maturity, cpn);
            //auto pricer = ext::make_shared<DiscountingBondEngine>(vars.curveHandle);
            bnd2->setPricingEngine(bondEngine);

            //Date delivery(10, March, 2022);
            auto bndFwd2 = buildBondForward(bnd2, discountingTermStructure, delivery, Position::Long);

            auto bndFwdValue2 = bndFwd2->forwardValue();
            auto expectedFwdDirtyPrice2 = bndFwd2->forwardValue() - bnd2->accruedAmount(delivery);
            auto underlyingDirtyPrice2 = bnd2->dirtyPrice();

            
        std::cout << "Forward Dirty Price  = " << bndFwdValue2 << std::endl;
        std::cout << "Expected Forward Dirty Price = " << expectedFwdDirtyPrice2 << std::endl;
        std::cout << "Undrlying Dirty Price= " << underlyingDirtyPrice2 << std::endl;
     

        std::cout << std::endl;
        std::cout << std::endl;
        
        
        /*
        // write column headings
        Size widths[] = { 18, 10, 10, 10 };

        std::cout << std::setw(widths[0]) <<  "                 "
        << std::setw(widths[1]) << "ZC"
        << std::setw(widths[2]) << "Fixed"
        << std::setw(widths[3]) << "Floating"
        << std::endl;

        Size width = widths[0] + widths[1] + widths[2] + widths[3];
        std::string rule(width, '-');

        std::cout << rule << std::endl;

        std::cout << std::fixed;
        std::cout << std::setprecision(2);

        std::cout << std::setw(widths[0]) << "Net present value"
      //  << std::setw(widths[1]) << zeroCouponBond.NPV()
        << std::setw(widths[2]) << fixedRateBond.NPV()
       // << std::setw(widths[3]) << floatingRateBond.NPV()
        << std::endl;

        std::cout << std::setw(widths[0]) << "Clean price"
       // << std::setw(widths[1]) << zeroCouponBond.cleanPrice()
        << std::setw(widths[2]) << fixedRateBond.cleanPrice()
       // << std::setw(widths[3]) << floatingRateBond.cleanPrice()
        << std::endl;

        std::cout << std::setw(widths[0]) << "Dirty price"
        //<< std::setw(widths[1]) << zeroCouponBond.dirtyPrice()
        << std::setw(widths[2]) << fixedRateBond.dirtyPrice()
        //<< std::setw(widths[3]) << floatingRateBond.dirtyPrice()
        << std::endl;

        std::cout << std::setw(widths[0]) << "Accrued coupon"
      //  << std::setw(widths[1]) << zeroCouponBond.accruedAmount()
        << std::setw(widths[2]) << fixedRateBond.accruedAmount()
      //  << std::setw(widths[3]) << floatingRateBond.accruedAmount()
        << std::endl;

        std::cout << std::setw(widths[0]) << "Previous coupon"
        << std::setw(widths[1]) << "N/A" // zeroCouponBond
        << std::setw(widths[2]) << io::rate(fixedRateBond.previousCouponRate())
      //  << std::setw(widths[3]) << io::rate(floatingRateBond.previousCouponRate())
        << std::endl;

        std::cout << std::setw(widths[0]) << "Next coupon"
        << std::setw(widths[1]) << "N/A" // zeroCouponBond
        << std::setw(widths[2]) << io::rate(fixedRateBond.nextCouponRate())
       // << std::setw(widths[3]) << io::rate(floatingRateBond.nextCouponRate())
        << std::endl;

        std::cout << std::setw(widths[0]) << "Yield"
        << std::setw(widths[1])
       // << io::rate(zeroCouponBond.yield(Actual360(),Compounded,Annual))
        << std::setw(widths[2])
        << io::rate(fixedRateBond.yield(Actual360(),Compounded,Annual))
        << std::setw(widths[3])
      //  << io::rate(floatingRateBond.yield(Actual360(),Compounded,Annual))
        << std::endl;

        std::cout << std::endl;
*/

        return 0;

    } catch (std::exception& e) {
        cerr << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "unknown error" << endl;
        return 1;
    }

}

