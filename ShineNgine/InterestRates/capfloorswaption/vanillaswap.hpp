#pragma once
#ifndef vanillaswap_hpp
#define vanillaswap_hpp

#include <ql/quantlib.hpp>

namespace QuantLib
{

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
                                         boost::shared_ptr<YieldTermStructure> termStructure);

    std::vector<Real> cross_currency_swap(Date evaluationDate,
                                          VanillaSwap::Type type,
                                          Real nominal_krw,
    
                                          Schedule fixedSchedule,
                                          Rate fixedRate,
                                          DayCounter fixedDayCount,
                                          Schedule floatSchedule,
        
                                          Spread spread,
                                          DayCounter floatingDayCount,
                                          Real spotFX,
                                          boost::shared_ptr<YieldTermStructure> krw_termStructure,
                                          boost::shared_ptr<YieldTermStructure> frn_termStructure);
}

#endif /* vanillaswap_hpp */
