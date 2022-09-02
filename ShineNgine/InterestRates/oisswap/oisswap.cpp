
#include <ql/qldefines.hpp>
#if !defined(BOOST_ALL_NO_LIB) && defined(BOOST_MSVC)
#  include <ql/auto_link.hpp>
#endif
#include <ql/termstructures/yield/piecewiseyieldcurve.hpp>
#include <ql/termstructures/yieldtermstructure.hpp>
#include <ql/termstructures/yield/ratehelpers.hpp>
#include <ql/termstructures/yield/oisratehelper.hpp>
#include <ql/pricingengines/swap/discountingswapengine.hpp>
#include <ql/indexes/ibor/tona.hpp>
#include <ql/time/imm.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/time/calendars/japan.hpp>
#include <ql/time/daycounters/actual360.hpp>
#include <ql/time/daycounters/thirty360.hpp>
#include <ql/time/daycounters/actualactual.hpp>
#include <ql/math/interpolations/cubicinterpolation.hpp>
#include <ql/math/interpolations/loginterpolation.hpp>

#include <iostream>
#include <iomanip>
#include <map>

using namespace QuantLib;

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {

    ThreadKey sessionId() { return {}; }

}
#endif


#include <iostream>
#include <iomanip>

using namespace QuantLib;

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {

    ThreadKey sessionId() { return {}; }

}
#endif

// input data //





int main(int, char* []) {

    try {

        std::cout << std::endl;

        /*************************
         ***  GLOBAL SETTINGS  ***
         *************************/

        Calendar calendar = Japan();
        Date todaysDate(30, August, 2022);
        Settings::instance().evaluationDate() = todaysDate;
        todaysDate = Settings::instance().evaluationDate();

        Integer fixingDays = 2;
        Date settlementDate = calendar.advance(todaysDate, fixingDays, Days);
        // must be a business day
        settlementDate = calendar.adjust(settlementDate);
        DayCounter dc = Actual365Fixed();

        std::cout << "Today: " << todaysDate.weekday()
            << ", " << todaysDate << std::endl;

        std::cout << "Settlement date: " << settlementDate.weekday()
            << ", " << settlementDate << std::endl;

        /*********************
        **  TONA CURVE    **
        *********************/
        /*
        std::vector<std::pair<Period, Real> > tonaOIS;
        tonaOIS.push_back(std::make_pair(1 * Months,  (-0.000275)));
        tonaOIS.push_back(std::make_pair(3 * Months,  (-0.000244)));
        tonaOIS.push_back(std::make_pair(6 * Months,  (-0.000147)));
        tonaOIS.push_back(std::make_pair(9 * Months,  (-0.000003)));
        tonaOIS.push_back(std::make_pair(12 * Months, (0.00015  )));
        tonaOIS.push_back(std::make_pair(18 * Months, (0.000375 )));
        tonaOIS.push_back(std::make_pair(24 * Months, (0.000587 )));
        tonaOIS.push_back(std::make_pair(36 * Months, (0.000931 )));
        tonaOIS.push_back(std::make_pair(48 * Months, (0.001238 )));
        tonaOIS.push_back(std::make_pair(60 * Months, (0.001581 )));
        tonaOIS.push_back(std::make_pair(72 * Months, (0.002025 )));
        */

        DayCounter termStructureDayCounter = Actual365Fixed();

        std::vector<ext::shared_ptr<RateHelper> > tonaInstruments_index;
        std::vector<ext::shared_ptr<RateHelper> > tonaInstruments_JSCC;
        std::vector<ext::shared_ptr<RateHelper> > tonaInstruments_LCH;
        ext::shared_ptr<Tona> tona(new Tona);

        // deposits
        // short-term OIS
        /*
        std::map<Period, ext::shared_ptr<Quote>> oisQuotes_index = {
            {1 * Months,  ext::make_shared<SimpleQuote>(-0.000275)},
            {2 * Months,  ext::make_shared<SimpleQuote>(-0.000269)},
            {3 * Months,  ext::make_shared<SimpleQuote>(-0.000244)},
            {4 * Months,  ext::make_shared<SimpleQuote>(-0.000222)},
            {5 * Months,  ext::make_shared<SimpleQuote>(-0.000184)},
            {6 * Months,  ext::make_shared<SimpleQuote>(-0.000147)},
            {7 * Months,  ext::make_shared<SimpleQuote>(-0.000103)},
            {8 * Months,  ext::make_shared<SimpleQuote>(-0.000059)},
            {9 * Months,  ext::make_shared<SimpleQuote>(-0.000003)},
            {10 * Months,  ext::make_shared<SimpleQuote>(0.000047)},
            {11 * Months,  ext::make_shared<SimpleQuote>(0.000103)},
            {12 * Months,  ext::make_shared<SimpleQuote>(0.00015 )},
            {15 * Months,  ext::make_shared<SimpleQuote>(0.000262)},
            {18 * Months,  ext::make_shared<SimpleQuote>(0.000375)},
            {21 * Months,  ext::make_shared<SimpleQuote>(0.000483)},
            {24 * Months,  ext::make_shared<SimpleQuote>(0.000587)},
            {36 * Months,  ext::make_shared<SimpleQuote>(0.000931)},
            {48 * Months,  ext::make_shared<SimpleQuote>(0.001238)},
            {60 * Months,  ext::make_shared<SimpleQuote>(0.001581)}
        };
        */

        
        std::map<Period, ext::shared_ptr<Quote>> oisQuotes_index = {
         
           {3 * Months,  ext::make_shared<SimpleQuote>(-0.000244)},
           {6 * Months,  ext::make_shared<SimpleQuote>(-0.000147)},
           {12 * Months,  ext::make_shared<SimpleQuote>(0.00015)},
           {24 * Months,  ext::make_shared<SimpleQuote>(0.000587)},
           {36 * Months,  ext::make_shared<SimpleQuote>(0.000931)},
           {60 * Months,  ext::make_shared<SimpleQuote>(0.001581)},
           {120 * Months,  ext::make_shared<SimpleQuote>(0.003788)}
        };
        

        /*
        std::map<Period, ext::shared_ptr<Quote>> oisQuotes_index = {
            {3 * Months,  ext::make_shared<SimpleQuote>(-0.0002479)},
            {6 * Months,  ext::make_shared<SimpleQuote>(-0.0001485)},
            {12 * Months, ext::make_shared<SimpleQuote>(0.0001500)},
            {24 * Months, ext::make_shared<SimpleQuote>(0.0005999)},
            {36 * Months, ext::make_shared<SimpleQuote>(0.0009376)},
            {60 * Months, ext::make_shared<SimpleQuote>(0.0015917)},
            {120 * Months, ext::make_shared<SimpleQuote>(0.0038486)}
        };
        */

        std::map<Period, ext::shared_ptr<Quote>> oisQuotes_JSCC = {
            {3 * Months,  ext::make_shared<SimpleQuote>(-0.0002479)},
            {6 * Months,  ext::make_shared<SimpleQuote>(-0.0001485)},
            {12 * Months, ext::make_shared<SimpleQuote>(0.0001500)},
            {24 * Months, ext::make_shared<SimpleQuote>(0.0005999)},
            {36 * Months, ext::make_shared<SimpleQuote>(0.0009376)},
            {60 * Months, ext::make_shared<SimpleQuote>(0.0015917)},
            {120 * Months, ext::make_shared<SimpleQuote>(0.0038486)}
        };

        std::map<Period, ext::shared_ptr<Quote>> oisQuotes_LCH = {
            {3 * Months,  ext::make_shared<SimpleQuote>(-0.0002625)},
            {6 * Months,  ext::make_shared<SimpleQuote>(-0.0001422)},
            {12 * Months, ext::make_shared<SimpleQuote>(0.0002000)},
            {24 * Months, ext::make_shared<SimpleQuote>(0.0007625)},
            {36 * Months, ext::make_shared<SimpleQuote>(0.0012000)},
            {60 * Months, ext::make_shared<SimpleQuote>(0.0019688)},
            {120 * Months, ext::make_shared<SimpleQuote>(0.0042063)}
         };



        for (const auto& q : oisQuotes_index) {
            auto tenor = q.first;
            auto quote = q.second;
            auto helper = ext::make_shared<OISRateHelper>(
                2, tenor, Handle<Quote>(quote), tona,
                Handle<YieldTermStructure>(),false,2,ModifiedFollowing);
            tonaInstruments_index.push_back(helper);
        } 
        for (const auto& q : oisQuotes_JSCC) {
            auto tenor = q.first;
            auto quote = q.second;
            auto helper = ext::make_shared<OISRateHelper>(
                2, tenor, Handle<Quote>(quote), tona,
                Handle<YieldTermStructure>(), false, 2, ModifiedFollowing);
            tonaInstruments_JSCC.push_back(helper);
        }

        for (const auto& q : oisQuotes_LCH) {
            auto tenor = q.first;
            auto quote = q.second;
            auto helper = ext::make_shared<OISRateHelper>(
                2, tenor, Handle<Quote>(quote), tona, 
                Handle<YieldTermStructure>(), false, 2, ModifiedFollowing);
            tonaInstruments_LCH.push_back(helper);
        }
        /*
        ext::shared_ptr<YieldTermStructure> eoniaTermStructure(
            new PiecewiseYieldCurve<Discount, LogLinear>(
                todaysDate, tonaInstruments,
                termStructureDayCounter));
        */

        ext::shared_ptr<YieldTermStructure> tonaTermStructure_index(
            new PiecewiseYieldCurve<Discount, LogLinear>(
                todaysDate, tonaInstruments_index,
                termStructureDayCounter));

        ext::shared_ptr<YieldTermStructure> tonaTermStructure_JSCC(
            new PiecewiseYieldCurve<Discount, LogLinear>(
                todaysDate, tonaInstruments_JSCC,
                termStructureDayCounter));

        ext::shared_ptr<YieldTermStructure> tonaTermStructure_LCH(
            new PiecewiseYieldCurve<Discount, LogLinear>(
                todaysDate, tonaInstruments_LCH,
                termStructureDayCounter));


        tonaTermStructure_index->enableExtrapolation();
        tonaTermStructure_JSCC->enableExtrapolation();
        tonaTermStructure_LCH->enableExtrapolation();
     

        RelinkableHandle<YieldTermStructure> forecastTermStructure_index;
        forecastTermStructure_index.linkTo(tonaTermStructure_index);

        RelinkableHandle<YieldTermStructure> discountingTermStructure_JSCC;
        discountingTermStructure_JSCC.linkTo(tonaTermStructure_JSCC);

        RelinkableHandle<YieldTermStructure> discountingTermStructure_LCH;
        discountingTermStructure_LCH.linkTo(tonaTermStructure_LCH);


        std::cout << discountingTermStructure_JSCC->discount(calendar.advance(settlementDate, Period(1 * Years))) << std::endl;
        std::cout << discountingTermStructure_JSCC->discount(calendar.advance(settlementDate, Period(2 * Years))) << std::endl;
        std::cout << discountingTermStructure_JSCC->discount(calendar.advance(settlementDate, Period(3 * Years))) << std::endl;
        std::cout << discountingTermStructure_JSCC->discount(calendar.advance(settlementDate, Period(4 * Years))) << std::endl;
        std::cout << discountingTermStructure_JSCC->discount(calendar.advance(settlementDate, Period(5 * Years))) << std::endl;
        std::cout << discountingTermStructure_LCH-> discount(calendar.advance(settlementDate, Period(1 * Years))) << std::endl;
        std::cout << discountingTermStructure_LCH-> discount(calendar.advance(settlementDate, Period(2 * Years))) << std::endl;
        std::cout << discountingTermStructure_LCH-> discount(calendar.advance(settlementDate, Period(3 * Years))) << std::endl;
        std::cout << discountingTermStructure_LCH-> discount(calendar.advance(settlementDate, Period(4 * Years))) << std::endl;
        std::cout << discountingTermStructure_LCH-> discount(calendar.advance(settlementDate, Period(5 * Years))) << std::endl;

        
        /*********************
        * SWAPS TO BE PRICED *
        **********************/

        // constant nominal 1,000,000 Euro
        Real nominal = 10000000000; // 100억엔
        // fixed leg
        Frequency fixedLegFrequency = Annual;
        BusinessDayConvention fixedLegConvention = ModifiedFollowing;
        BusinessDayConvention floatingLegConvention = ModifiedFollowing;
        DayCounter fixedLegDayCounter = Actual365Fixed();
        Rate fixedRate_JSCC = 0.0016688; // 
        Rate fixedRate_LCH = 0.0019688; // Real fixedRate_LCH = 0.0003; // 3bp

        DayCounter floatingLegDayCounter = Actual365Fixed();
        Frequency floatingLegFrequency = Annual;
        
        ext::shared_ptr<OvernightIndex> tonaIndex_JSCC(
            new Tona(forecastTermStructure_index));
        ext::shared_ptr<OvernightIndex> tonaIndex_LCH(
            new Tona(forecastTermStructure_index));
        Spread spread = 0.0;

        Integer lengthInYears = 5;
        Swap::Type swapType_LCH = Swap::Receiver;
        Swap::Type swapType_JSCC = Swap::Payer;

        Date maturity = calendar.advance(settlementDate,lengthInYears * Years);
        std::cout << maturity << std::endl;
        

        Schedule fixedSchedule(settlementDate, maturity,
            Period(fixedLegFrequency),
            calendar, fixedLegConvention,
            fixedLegConvention,
            DateGeneration::Forward, false);
        Schedule floatSchedule(settlementDate, maturity,
            Period(floatingLegFrequency),
            calendar, floatingLegConvention,
            floatingLegConvention,
            DateGeneration::Forward, false);

        OvernightIndexedSwap JSCC_CCP(OvernightIndexedSwap::Payer, nominal,
            fixedSchedule, fixedRate_JSCC, fixedLegDayCounter, tonaIndex_JSCC,
             spread, 2,  fixedLegConvention, calendar, false);
        OvernightIndexedSwap LCH_CCP(OvernightIndexedSwap::Receiver, nominal,
            fixedSchedule, fixedRate_LCH, fixedLegDayCounter, tonaIndex_LCH,
            spread, 2, fixedLegConvention, calendar, false);

        //VanillaSwap JSCC_CCP(Swap::Payer, nominal,
        //    fixedSchedule, fixedRate_JSCC, fixedLegDayCounter,
        //    floatSchedule, tonaIndex_JSCC, spread,
        //    floatingLegDayCounter);
       // VanillaSwap LCH_CCP(Swap::Receiver, nominal,
        //    fixedSchedule, fixedRate_LCH, fixedLegDayCounter,
         //   floatSchedule, tonaIndex_LCH, spread,
          //  floatingLegDayCounter);

        Real NPV;
        Rate fairRate;
        Spread fairSpread;

        ext::shared_ptr<PricingEngine> swapEngine_JSCC(
            new DiscountingSwapEngine(discountingTermStructure_JSCC));

        ext::shared_ptr<PricingEngine> swapEngine_LCH(
            new DiscountingSwapEngine(discountingTermStructure_LCH));

        JSCC_CCP.setPricingEngine(swapEngine_JSCC);
        LCH_CCP.setPricingEngine(swapEngine_LCH);


        Real JSCC_NPV = JSCC_CCP.NPV();
        Real JSCC_fairSpread = JSCC_CCP.fairSpread();
        Real JSCC_fairRate = JSCC_CCP.fairRate();
        std::cout << " JSCC Payer ! " << std::endl;
        std::cout << std::fixed << std::setprecision(10) << JSCC_NPV << std::endl;
        std::cout << io::rate(JSCC_fairSpread) << std::endl;
        std::cout << io::rate(JSCC_fairRate) << std::endl;


        std::cout << " LCH Receiver ! " << std::endl;
        Real LCH_NPV = LCH_CCP.NPV();
        Real LCH_fairSpread = LCH_CCP.fairSpread();
        Real LCH_fairRate = LCH_CCP.fairRate();
        std::cout << std::fixed << std::setprecision(10) << LCH_NPV << std::endl;
        std::cout << io::rate(LCH_fairSpread) << std::endl;
        std::cout << io::rate(LCH_fairRate) << std::endl;


        std::cout << std::endl;
        std::cout << JSCC_CCP.fixedLeg()[0]->amount() << std::endl;
        std::cout << JSCC_CCP.fixedLeg()[1]->amount() << std::endl;
        std::cout << JSCC_CCP.fixedLeg()[2]->amount() << std::endl;
        std::cout << JSCC_CCP.fixedLeg()[3]->amount() << std::endl;
        std::cout << JSCC_CCP.fixedLeg()[4]->amount() << std::endl;
        std::cout << LCH_CCP.fixedLeg()[0]->amount() << std::endl;
        std::cout << LCH_CCP.fixedLeg()[1]->amount() << std::endl;
        std::cout << LCH_CCP.fixedLeg()[2]->amount() << std::endl;
        std::cout << LCH_CCP.fixedLeg()[3]->amount() << std::endl;
        std::cout << LCH_CCP.fixedLeg()[4]->amount() << std::endl;

        std::cout << LCH_CCP.fixedLegBPS() << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;

        for (auto iter = oisQuotes_JSCC.begin(); iter != oisQuotes_JSCC.end(); iter++)
        {
            auto bumped= oisQuotes_JSCC[iter->first];
            Real bumped_base = bumped->value();
            Real bumped_up = bumped->value() + 0.0001;
            ext::shared_ptr<SimpleQuote> bumping_up =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_up->setValue(bumped_up);

            Real NPV_up = JSCC_CCP.NPV();

            Real bumped_dn = bumped->value() -0.0002;
            ext::shared_ptr<SimpleQuote> bumping_dn =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_dn->setValue(bumped_dn);

            Real NPV_dn = JSCC_CCP.NPV();

            std::cout << bumped_base << std::endl;
            std::cout << bumped_up << std::endl;
            std::cout << bumped_dn << std::endl;

            std::cout << "UP Swap Fair NPV  : " << NPV_up << std::endl;
            std::cout << "Down Swap Fair NPV  : " << NPV_dn << std::endl;
            std::cout << "Rho in " << iter->first << " Tenor is " << (NPV_up - NPV_dn) / 0.0002 << std::endl;

            ext::shared_ptr<SimpleQuote> bumping_back =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_back->setValue(bumped_dn+0.0001);

            //std::cout << bumped->value() << std::endl;
            std::cout << std::endl;

        }


        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << " GREEKS !!!! " << std::endl;

        std::cout << std::endl;

        std::cout << " JSCC ! " << std::endl;
        for (auto iter = oisQuotes_JSCC.begin(); iter != oisQuotes_JSCC.end(); iter++)
        {
            auto bumped = oisQuotes_JSCC[iter->first];
            Real bumped_base = bumped->value();
            Real bumped_up = bumped->value() + 0.0001;
            ext::shared_ptr<SimpleQuote> bumping_up =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_up->setValue(bumped_up);

            Real NPV_up = JSCC_CCP.NPV();

            Real bumped_dn = bumped->value() - 0.0002;
            ext::shared_ptr<SimpleQuote> bumping_dn =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_dn->setValue(bumped_dn);

            Real NPV_dn = JSCC_CCP.NPV();


           // std::cout << bumped_base << std::endl;
            //std::cout << bumped_up << std::endl;
            //std::cout << bumped_dn << std::endl;

            //std::cout << "UP Swap Fair NPV  : " << NPV_up << std::endl;
            //std::cout << "Down Swap Fair NPV  : " << NPV_dn << std::endl;
            std::cout << "Rho in " << iter->first << " Tenor is " << (NPV_up - NPV_dn) / 0.0002 /10000<< std::endl;

            ext::shared_ptr<SimpleQuote> bumping_back =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_back->setValue(bumped_dn + 0.0001);

            //std::cout << bumped->value() << std::endl;
           // std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << " LCH ! " << std::endl;
        for (auto iter = oisQuotes_LCH.begin(); iter != oisQuotes_LCH.end(); iter++)
        {
            auto bumped = oisQuotes_LCH[iter->first];
            Real bumped_base = bumped->value();
            Real bumped_up = bumped->value() + 0.0001;
            ext::shared_ptr<SimpleQuote> bumping_up =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_up->setValue(bumped_up);

            Real NPV_up = LCH_CCP.NPV();

            Real bumped_dn = bumped->value() - 0.0002;
            ext::shared_ptr<SimpleQuote> bumping_dn =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_dn->setValue(bumped_dn);

            Real NPV_dn = LCH_CCP.NPV();


            // std::cout << bumped_base << std::endl;
             //std::cout << bumped_up << std::endl;
             //std::cout << bumped_dn << std::endl;

             //std::cout << "UP Swap Fair NPV  : " << NPV_up << std::endl;
             //std::cout << "Down Swap Fair NPV  : " << NPV_dn << std::endl;
            std::cout << "Rho in " << iter->first << " Tenor is " << (NPV_up - NPV_dn) / 0.0002 / 10000 << std::endl;

            ext::shared_ptr<SimpleQuote> bumping_back =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_back->setValue(bumped_dn + 0.0001);

            //std::cout << bumped->value() << std::endl;
            // std::cout << std::endl;
        }
        std::cout << " Float Rate! ! " << std::endl;
        for (auto iter = oisQuotes_index.begin(); iter != oisQuotes_index.end(); iter++)
        {
            auto bumped = oisQuotes_index[iter->first];
            Real bumped_base = bumped->value();
            Real bumped_up = bumped->value() + 0.0001;
            ext::shared_ptr<SimpleQuote> bumping_up =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_up->setValue(bumped_up);

            Real NPV_up = JSCC_CCP.NPV();

            Real bumped_dn = bumped->value() - 0.0002;
            ext::shared_ptr<SimpleQuote> bumping_dn =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_dn->setValue(bumped_dn);

            Real NPV_dn = JSCC_CCP.NPV();


            // std::cout << bumped_base << std::endl;
             //std::cout << bumped_up << std::endl;
             //std::cout << bumped_dn << std::endl;

             //std::cout << "UP Swap Fair NPV  : " << NPV_up << std::endl;
             //std::cout << "Down Swap Fair NPV  : " << NPV_dn << std::endl;
            std::cout << "Rho in " << iter->first << " Tenor is " << (NPV_up - NPV_dn) / 0.0002 / 10000 << std::endl;

            ext::shared_ptr<SimpleQuote> bumping_back =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_back->setValue(bumped_dn + 0.0001);

            //std::cout << bumped->value() << std::endl;
            // std::cout << std::endl;
        }

      
        std::cout << " Float Rate! ! " << std::endl;
        for (auto iter = oisQuotes_index.begin(); iter != oisQuotes_index.end(); iter++)
        {
            auto bumped = oisQuotes_index[iter->first];
            Real bumped_base = bumped->value();
            Real bumped_up = bumped->value() + 0.0001;
            ext::shared_ptr<SimpleQuote> bumping_up =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_up->setValue(bumped_up);

            Real NPV_up = LCH_CCP.NPV();

            Real bumped_dn = bumped->value() - 0.0002;
            ext::shared_ptr<SimpleQuote> bumping_dn =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_dn->setValue(bumped_dn);

            Real NPV_dn = LCH_CCP.NPV();


            // std::cout << bumped_base << std::endl;
             //std::cout << bumped_up << std::endl;
             //std::cout << bumped_dn << std::endl;

             //std::cout << "UP Swap Fair NPV  : " << NPV_up << std::endl;
             //std::cout << "Down Swap Fair NPV  : " << NPV_dn << std::endl;
            std::cout << "Rho in " << iter->first << " Tenor is " << (NPV_up - NPV_dn) / 0.0002 / 10000 << std::endl;

            ext::shared_ptr<SimpleQuote> bumping_back =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_back->setValue(bumped_dn + 0.0001);

            //std::cout << bumped->value() << std::endl;
            // std::cout << std::endl;
        }


        std::cout << " Cross Gamma  Rate  JSCC !! " << std::endl;

        for (auto iter = oisQuotes_index.begin(), iter2 = oisQuotes_JSCC.begin();
            iter != oisQuotes_index.end() && iter2 != oisQuotes_JSCC.end(); iter++, iter2++)
        {
            auto bumped_index = oisQuotes_index[iter->first];
            auto bumped_JSCC = oisQuotes_JSCC[iter2->first];
            Real bumped_base_index = bumped_index->value();
            Real bumped_base_JSCC = bumped_JSCC->value();

            Real bumped_up_index = bumped_index->value() + 0.0001;
            Real bumped_up_JSCC = bumped_JSCC->value() + 0.0001;

            ext::shared_ptr<SimpleQuote> bumping_up_index =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped_index);
            bumping_up_index->setValue(bumped_up_index);

            ext::shared_ptr<SimpleQuote> bumping_up_JSCC =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped_JSCC);
            bumping_up_JSCC->setValue(bumped_up_JSCC);

            Real NPV_up = JSCC_CCP.NPV();
            Real bumped_dn_index = bumped_index->value() - 0.0002;
            Real bumped_dn_JSCC = bumped_JSCC->value() - 0.0002;

            ext::shared_ptr<SimpleQuote> bumping_dn_index =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped_index);
            bumping_dn_index->setValue(bumped_dn_index);

            ext::shared_ptr<SimpleQuote> bumping_dn_JSCC =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped_JSCC);
            bumping_dn_JSCC->setValue(bumped_dn_JSCC);

            Real NPV_dn = JSCC_CCP.NPV();

            // std::cout << bumped_base << std::endl;
             //std::cout << bumped_up << std::endl;
             //std::cout << bumped_dn << std::endl;

             //std::cout << "UP Swap Fair NPV  : " << NPV_up << std::endl;
             //std::cout << "Down Swap Fair NPV  : " << NPV_dn << std::endl;
            std::cout << "Rho in " << iter->first <<", " << iter2->first 
                << " Tenor is " << 1*(NPV_up - NPV_dn) / 0.0001 / 10000 << std::endl;

            ext::shared_ptr<SimpleQuote> bumping_back_index =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped_index);
            bumping_back_index->setValue(bumped_dn_index + 0.0001);


            ext::shared_ptr<SimpleQuote> bumping_back_JSCC =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped_JSCC);
            bumping_back_JSCC->setValue(bumped_dn_JSCC + 0.0001);


            //std::cout << bumped->value() << std::endl;
            // std::cout << std::endl;
        }


        std::cout << " Cross Gamma  Rate, LSH !! " << std::endl;

        for (auto iter = oisQuotes_index.begin(), iter2 = oisQuotes_LCH.begin();
            iter != oisQuotes_index.end() && iter2 != oisQuotes_LCH.end(); iter++, iter2++)
        {
            auto bumped_index = oisQuotes_index[iter->first];
            auto bumped_LCH = oisQuotes_LCH[iter2->first];
            Real bumped_base_index = bumped_index->value();
            Real bumped_base_LCH = bumped_LCH->value();

            Real bumped_up_index = bumped_index->value() + 0.0001;
            Real bumped_up_LCH = bumped_LCH->value() + 0.0001;

            ext::shared_ptr<SimpleQuote> bumping_up_index =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped_index);
            bumping_up_index->setValue(bumped_up_index);

            ext::shared_ptr<SimpleQuote> bumping_up_LCH =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped_LCH);
            bumping_up_LCH->setValue(bumped_up_LCH);

            Real NPV_up = LCH_CCP.NPV();
            Real bumped_dn_index = bumped_index->value() - 0.0002;
            Real bumped_dn_LCH = bumped_LCH->value() - 0.0002;

            ext::shared_ptr<SimpleQuote> bumping_dn_index =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped_index);
            bumping_dn_index->setValue(bumped_dn_index);

            ext::shared_ptr<SimpleQuote> bumping_dn_LCH =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped_LCH);
            bumping_dn_LCH->setValue(bumped_dn_LCH);

            Real NPV_dn = LCH_CCP.NPV();

            // std::cout << bumped_base << std::endl;
             //std::cout << bumped_up << std::endl;
             //std::cout << bumped_dn << std::endl;

             //std::cout << "UP Swap Fair NPV  : " << NPV_up << std::endl;
             //std::cout << "Down Swap Fair NPV  : " << NPV_dn << std::endl;
            std::cout << "Rho in " << iter->first << ", " << iter2->first
                << " Tenor is " << 1 * (NPV_up - NPV_dn) / 0.0001 / 10000 << std::endl;

            ext::shared_ptr<SimpleQuote> bumping_back_index =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped_index);
            bumping_back_index->setValue(bumped_dn_index + 0.0001);


            ext::shared_ptr<SimpleQuote> bumping_back_LCH =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped_LCH);
            bumping_back_LCH->setValue(bumped_dn_LCH + 0.0001);


            //std::cout << bumped->value() << std::endl;
            // std::cout << std::endl;
        }

        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "FIXED LEG!! !!!! " << std::endl;

        std::cout << std::endl;

        std::cout << " JSCC ! " << std::endl;
        for (auto iter = oisQuotes_JSCC.begin(); iter != oisQuotes_JSCC.end(); iter++)
        {
            auto bumped = oisQuotes_JSCC[iter->first];
            Real bumped_base = bumped->value();
            Real bumped_up = bumped->value() + 0.0001;
            ext::shared_ptr<SimpleQuote> bumping_up =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_up->setValue(bumped_up);

            Real NPV_up = LCH_CCP.fixedLegNPV() + JSCC_CCP.fixedLegNPV();
       //     std::cout << NPV_up << std::endl;

            Real bumped_dn = bumped->value() - 0.0002;
            ext::shared_ptr<SimpleQuote> bumping_dn =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_dn->setValue(bumped_dn);
             
            Real NPV_dn = LCH_CCP.fixedLegNPV() + JSCC_CCP.fixedLegNPV();
       //     std::cout << NPV_dn << std::endl;


            // std::cout << bumped_base << std::endl;
             //std::cout << bumped_up << std::endl;
             //std::cout << bumped_dn << std::endl;

             //std::cout << "UP Swap Fair NPV  : " << NPV_up << std::endl;
             //std::cout << "Down Swap Fair NPV  : " << NPV_dn << std::endl;
            std::cout << "Rho in " << iter->first << " Tenor is " << (NPV_up - NPV_dn) / 0.0002 /10000<< std::endl;

            ext::shared_ptr<SimpleQuote> bumping_back =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_back->setValue(bumped_dn + 0.0001);

            //std::cout << bumped->value() << std::endl;
           // std::cout << std::endl;
        }

        std::cout << " LCH ! " << std::endl;
        for (auto iter = oisQuotes_LCH.begin(); iter != oisQuotes_LCH.end(); iter++)
        {
            auto bumped = oisQuotes_LCH[iter->first];
            Real bumped_base = bumped->value();
            Real bumped_up = bumped->value() + 0.0001;
            ext::shared_ptr<SimpleQuote> bumping_up =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_up->setValue(bumped_up);

            Real NPV_up = LCH_CCP.fixedLegNPV() +JSCC_CCP.fixedLegNPV();
           // std::cout << NPV_up << std::endl;
            Real bumped_dn = bumped->value() - 0.0002;
            ext::shared_ptr<SimpleQuote> bumping_dn =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_dn->setValue(bumped_dn);

            Real NPV_dn = LCH_CCP.fixedLegNPV() + JSCC_CCP.fixedLegNPV();
         //   std::cout << NPV_dn << std::endl;

            // std::cout << bumped_base << std::endl;
             //std::cout << bumped_up << std::endl;
             //std::cout << bumped_dn << std::endl;

             //std::cout << "UP Swap Fair NPV  : " << NPV_up << std::endl;
             //std::cout << "Down Swap Fair NPV  : " << NPV_dn << std::endl;
            std::cout << "Rho in " << iter->first << " Tenor is " << (NPV_up - NPV_dn) / 0.0002 / 10000 << std::endl;

            ext::shared_ptr<SimpleQuote> bumping_back =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_back->setValue(bumped_dn + 0.0001);

            //std::cout << bumped->value() << std::endl;
            // std::cout << std::endl;
        }
        std::cout << std::endl;


        std::cout << "Float  LEG!! !!!! " << std::endl;

        std::cout << std::endl;

        std::cout << " JSCC ! " << std::endl;
        for (auto iter = oisQuotes_JSCC.begin(); iter != oisQuotes_JSCC.end(); iter++)
        {
            auto bumped = oisQuotes_JSCC[iter->first];
            Real bumped_base = bumped->value();
            Real bumped_up = bumped->value() + 0.0001;
            ext::shared_ptr<SimpleQuote> bumping_up =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_up->setValue(bumped_up);

            Real NPV_up = JSCC_CCP.overnightLegNPV();

            Real bumped_dn = bumped->value() - 0.0002;
            ext::shared_ptr<SimpleQuote> bumping_dn =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_dn->setValue(bumped_dn);

            Real NPV_dn = JSCC_CCP.overnightLegNPV();


            // std::cout << bumped_base << std::endl;
             //std::cout << bumped_up << std::endl;
             //std::cout << bumped_dn << std::endl;

             //std::cout << "UP Swap Fair NPV  : " << NPV_up << std::endl;
             //std::cout << "Down Swap Fair NPV  : " << NPV_dn << std::endl;
            std::cout << "Rho in " << iter->first << " Tenor is " << (NPV_up - NPV_dn) / 0.0002 / 10000 << std::endl;

            ext::shared_ptr<SimpleQuote> bumping_back =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_back->setValue(bumped_dn + 0.0001);

            //std::cout << bumped->value() << std::endl;
           // std::cout << std::endl;
        }

        std::cout << " LCH ! " << std::endl;
        for (auto iter = oisQuotes_LCH.begin(); iter != oisQuotes_LCH.end(); iter++)
        {
            auto bumped = oisQuotes_LCH[iter->first];
            Real bumped_base = bumped->value();
            Real bumped_up = bumped->value() + 0.0001;
            ext::shared_ptr<SimpleQuote> bumping_up =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_up->setValue(bumped_up);

            Real NPV_up = LCH_CCP.overnightLegNPV();

            Real bumped_dn = bumped->value() - 0.0002;
            ext::shared_ptr<SimpleQuote> bumping_dn =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_dn->setValue(bumped_dn);

            Real NPV_dn = LCH_CCP.overnightLegNPV();


            // std::cout << bumped_base << std::endl;
             //std::cout << bumped_up << std::endl;
             //std::cout << bumped_dn << std::endl;

             //std::cout << "UP Swap Fair NPV  : " << NPV_up << std::endl;
             //std::cout << "Down Swap Fair NPV  : " << NPV_dn << std::endl;
            std::cout << "Rho in " << iter->first << " Tenor is " << (NPV_up - NPV_dn) / 0.0002 / 10000 << std::endl;

            ext::shared_ptr<SimpleQuote> bumping_back =
                ext::dynamic_pointer_cast<SimpleQuote>(bumped);
            bumping_back->setValue(bumped_dn + 0.0001);

            //std::cout << bumped->value() << std::endl;
            // std::cout << std::endl;
        }
        std::cout << std::endl;

        // GREEKS ! //
        /*
        auto s5yRate = shortOisQuotes[5 * Years];
        ext::shared_ptr<SimpleQuote> fiveYearsRate =
            ext::dynamic_pointer_cast<SimpleQuote>(s5yRate);
        fiveYearsRate->setValue(s5yRate->value() + 0.0001);

        // now get the updated results

        Real NPV_bumped = spot5YearSwap.NPV();
        Real fairSpread_bumped = spot5YearSwap.fairSpread();
        Real fairRate_bumped = spot5YearSwap.fairRate();

        std::cout << std::fixed << std::setprecision(2) << NPV_bumped << std::endl;
        std::cout << io::rate(fairSpread_bumped) << std::endl;
        std::cout << io::rate(fairRate_bumped) << std::endl;
        */
        return 0;

    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "unknown error" << std::endl;
        return 1;
    }
}
