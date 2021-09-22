#include "vanillaswap.hpp"

namespace QuantLib {

    std::vector<Real> interest_rate_swap(Date evaluationDate,
        VanillaSwap::Type type,
        Real nominal,
        Schedule fixedSchedule,
        Rate fixedRate,
        DayCounter fixedDayCount,
        Schedule floatSchedule,
        boost::shared_ptr<IborIndex> iborIndex,
        Spread spread,
        DayCounter floatingDayCount,
        boost::shared_ptr<YieldTermStructure> termStructure) {

            Date todaysDate = evaluationDate;
            Settings::instance().evaluationDate() = todaysDate;

            /*Make Engine*/
            boost::shared_ptr<PricingEngine> engine =
                boost::shared_ptr<PricingEngine>(new DiscountingSwapEngine(
                Handle<YieldTermStructure>(termStructure)));

            /*Pricing*/
            VanillaSwap testProduct(type, nominal, fixedSchedule, fixedRate, fixedDayCount,
                floatSchedule, iborIndex, spread, floatingDayCount);

            testProduct.setPricingEngine(engine);

            std::vector<Real> rst;
            rst.push_back(testProduct.NPV());
            return rst;
    }


    std::vector<Real> cross_currency_swap(Date evaluationDate,
        VanillaSwap::Type type,
        Real nominal_krw,
        Real nominal_frn,
        Schedule fixedSchedule,
        Rate fixedRate,
        DayCounter fixedDayCount,
        Schedule floatSchedule,
        boost::shared_ptr<IborIndex> iborIndex,
        Spread spread,
        DayCounter floatingDayCount,
        Real spotFX,
        boost::shared_ptr<YieldTermStructure> krw_termStructure,
        boost::shared_ptr<YieldTermStructure> frn_termStructure) {

            Date todaysDate = evaluationDate;
            Settings::instance().evaluationDate() = todaysDate;

            /*Make Engine*/
            boost::shared_ptr<PricingEngine> engine =
                boost::shared_ptr<PricingEngine>(new DiscountingSwapEngine(
                Handle<YieldTermStructure>(krw_termStructure)));

            /*Pricing*/
            VanillaSwap testProduct(type, nominal_krw, fixedSchedule, fixedRate, fixedDayCount,
                floatSchedule, iborIndex, spread, floatingDayCount);

            testProduct.setPricingEngine(engine);

            std::vector<Real> rst;
            rst.push_back(testProduct.NPV());
            return rst;
    }


}
