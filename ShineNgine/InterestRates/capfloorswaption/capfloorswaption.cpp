#include "capfloorswaption.hpp"

namespace QuantLib
{
    std::vector<Real> capfloor(Date evaluationDate,
                               CapFloor::Type type,
                               Real strike,
                               Real nominal,
                               Schedule schedule,
                               Natural fixingDays,
                               BusinessDayConvention convention,
                               boost::shared_ptr<IborIndex> index,
                               boost::shared_ptr<YieldTermStructure> termStructure,
                               Volatility volatility){
        Date todaysDate = evaluationDate;
        Settings::instance().evaluationDate() = todaysDate;
        std::vector<Real> nominals = std::vector<Real>(1, nominal);
        
        /* Leg */
        Leg leg = IborLeg(schedule, index).withNotionals(nominals).withPaymentDayCounter(index->dayCounter()).withPaymentAdjustment(convention).withFixingDays(fixingDays);
        
        /* Engine */
        Handle<Quote> vol(boost::shared_ptr<Quote>(new SimpleQuote(volatility)));
        boost::shared_ptr<PricingEngine> engine = boost::shared_ptr<PricingEngine>(new BlackCapFloorEngine(Handle<YieldTermStructure>(termStructure), vol));
        
        /* pricing */
        boost::shared_ptr<CapFloor> testProduct;
        switch(type)
        {
            case CapFloor::Cap:
                testProduct = boost::shared_ptr<CapFloor>(new Cap(leg, std::vector<Rate>(1, strike)));
                break;
            case CapFloor::Floor:
                testProduct = boost::shared_ptr<CapFloor>(new Floor(leg, std::vector<Rate>(1, strike)));
                break;
            default:
                QL_FAIL("unknown cap/floor type");
        }
        testProduct->setPricingEngine(engine);
        std::vector<Real> rst;
        rst.push_back(testProduct->NPV());
        return rst;
    }


        std::vector<Real> swaption(Date evaluationDate,
                VanillaSwap::Type type,
                Settlement::Type settlementType,
                Real strike,
                Real nominal,
                Date exerciseDate,
                Schedule fixedSchedule,
                DayCounter fixedDayCount,
                Schedule floatSchedule,
                DayCounter floatDayCount,
                Natural fixingDays,
                BusinessDayConvention convention,
                boost::shared_ptr<IborIndex> index,
                boost::shared_ptr<YieldTermStructure> termStructure,
                Volatility volatility)
    {
        Date todaysDate = evaluationDate;
        Settings::instance().evaluationDate() = todaysDate;
        boost::shared_ptr<VanillaSwap> swap(new VanillaSwap(type,
                                                            nominal,
                                                            fixedSchedule,
                                                            strike,
                                                            fixedDayCount,
                                                            floatSchedule, index, 0.0, floatDayCount, convention));
        Handle<Quote> vol(boost::shared_ptr<Quote>(new SimpleQuote(volatility)));
        boost::shared_ptr<PricingEngine> engine(new BlackSwaptionEngine(Handle<YieldTermStructure>(termStructure),vol));
        
        boost::shared_ptr<Swaption> testProduct(new Swaption(swap, boost::shared_ptr<Exercise>(new EuropeanExercise(exerciseDate)), settlementType));
        
        testProduct->setPricingEngine(engine);
            
       // return testProduct->NPV();
            
            std::vector<Real> rst;
            rst.push_back(testProduct->NPV());
            return rst;
    }
}
