#include <ql/quantlib.hpp>

#include <iostream>

#include <iomanip>

 

#include <boost/timer.hpp>

 

using namespace QuantLib;

using namespace std;

 

#if defined(QL_ENABLE_SESSIONS)

{

        namespace QuantLib

        {

               Integer sessionId() { return 0; }

        }

#endif

 

int main(int, char*[])

{

        try

        {

               boost::timer timer;

               std::cout << std::endl;

 

               Calendar calendar = TARGET();

               Date settlementDate(6, Aug, 2021);

               settlementDate = calendar.adjust(settlementDate);

               Integer fixingDays = 1;

               Natural settlementDays = 1;

               Date todaysDate = calendar..advance(settlementDate, -fixingDays, Days);

               Settings::instance().evaluationDate() = todaysDate;

 

               // Treasury Bond Setting //

               Real faceAmount = 100;

               Real redemption = 100;

               Date issueDate(17, May, 2021);

               Date maturity(5, May, 2031);

               Real couponRate = 0.01625; // 1 5/8

               Real yield = 0.0165;

 

               RelinkableHandle<YieldTermStructure> discountingTermStructure;

               boost::shared_ptr<YieldTermStructure> flatTermStructure(new FlatForward(settlementDate, yield, ActualActual(ActualActual::Bond), Compounded, Semiannual));

               discountingTermStructure.linkTo(flatTermStructure);

 

               boost::shared_ptr<PricingEngine> bondEngine(new DiscountingBondEngine(discountingTermStructure));

 

               Schedule fixedBondSchedule(issueDate, maturity, Period(Semiannual), UnitedStates(UnitedStates::GovernmentBond), Unadjusted, Unadjusted, DateGeneration::Rule::Backward, false);

               FixedRateBond fixedRateBond(settlementDays, faceAmount, fixedBondSchedule, std::vector<Rate>(1, couponRate), ActualActual(ActualActual::Bond), Unadjusted, redemption, issueDate);

 

               fixedRateBond.setPricingEngine(bondEngine);

 

               std::cout << "****** Inputs ******" << std::endl;

               //std::setprecision(20);

               std::cout << " Principal = " << faceAmount << std::endl;

               std::cout << "Issue Date = " << issueDate << std::endl;

               std::cout << " Maturity = " << maturity << std::endl;

               std::cout << " Coupon  = " << io::percent(couponRate) << std::endl;

               std::cout << " Yield To Maturity = " << io::percent(yield) << std::endl;

               std::cout << "****** Result ******" << std::endl;

               std::cout << " Fair Value = " << fixedRateBond.NPV() << std::endl;

               std::cout << " Clean Price = " << fixedRateBond.cleanPrice() << std::endl;

               std::cout << " Dirty Price = " << fixedRateBond.dirtyPrice() << std::endl;

               std::cout << " Accured Coupon = " << fixedRateBond.accruedAmount() << std::endl;

               system("pause");

               return 0;

        }

        catch (std::exception& e)

        {

               std::cerr << e.what() << std::endl;

               return 1;

        }

        catch (...)

        {

               std::cerr << "Unknown Error" << std::endl;

               return 1;

 

        }

}

 
