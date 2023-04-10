#include "krwcd.hpp"
#include <ql/time/calendars/southkorea.hpp>
#include <ql/time/daycounters/actual365fixed.hpp>
#include <ql/currencies/asia.hpp>

namespace QuantLib {

    namespace {

        BusinessDayConvention krwcdConvention(const Period& p) {
            switch (p.units()) {
              case Days:
				  return ModifiedFollowing;
              case Weeks:
                return Following;
              case Months:
              case Years:
                return ModifiedFollowing;
              default:
                QL_FAIL("invalid time units");
            }
        }

        bool krwcdEOM(const Period& p) {
            switch (p.units()) {
              case Days:
              case Weeks:
                return false;
              case Months:
              case Years:
                return true;
              default:
                QL_FAIL("invalid time units");
            }
        }

    }

    KrwCD::KrwCD(const Period& tenor,
                     const Handle<YieldTermStructure>& h)
    : IborIndex("KrwCD", tenor,
                1, // settlement days
                KRWCurrency(), SouthKorea(),
                krwcdConvention(tenor), krwcdEOM(tenor),
                Actual365Fixed(), h) {
        QL_REQUIRE(this->tenor().units()!=Days,
                   "for daily tenors (" << this->tenor() <<
                   ") dedicated DailyTenor constructor must be used");
    }

    KrwCD365::KrwCD365(const Period& tenor,
                           const Handle<YieldTermStructure>& h)
    : IborIndex("KrwCD365", tenor,
                1, // settlement days
				KRWCurrency(), SouthKorea(),
                krwcdConvention(tenor), krwcdEOM(tenor),
                Actual365Fixed(), h) {
        QL_REQUIRE(this->tenor().units()!=Days,
                   "for daily tenors (" << this->tenor() <<
                   ") dedicated DailyTenor constructor must be used");
    }

}
