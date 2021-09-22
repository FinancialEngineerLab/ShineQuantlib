
#ifndef capfloorswaption_hpp
#define capfloorswaption_hpp

#include <ql/quantlib.hpp>

namespace QuantLib
{
    std::vector<Real> capfloor(Date evaluationDate,
                               CapFloor::Type type,
                               Real strike,
                               Real nominal,
                               Schedule schedule,
                               Natural FixingDays,
                               BusinessDayConvention convention,
                               boost::shared_ptr<IborIndex> index,
                               boost::shared_ptr<YieldTermStructure> termStructure,
                               Volatility volatility);
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
                  Volatility volatility);
}

#endif /* capfloorswaption_hpp */
