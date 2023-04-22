#include <ql/quantlib.hpp>

#if !defined(BOOST_ALL_NO_LIB) && defined(BOOST_MSVC)
#  include <ql/auto_link.hpp>
#endif

#include <iostream>
#include <iomanip>
using namespace QuantLib;
using namespace std;

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {
    Integer sessionId() { return 0; }
}
#endif

#define LENGTH(a) (sizeof(a)/sizeof(a[0]))



#include <Python.h>



namespace QuantLib
{
struct hwDatum
{
    Size rank;
    Real spread[3];
};

struct hwDatumDist
{
    Size rank;
    Real spread[4];
};


hwDatum hwData[] =
{
    {1, {603, 440, 293} },
    {2, {98, 139, 137} },
    {3, {12, 53, 79} },
    {4, {1, 21, 49} },
    {5, {0, 8, 31} },
    {6, {0, 3, 19} },
    {7, {0, 1, 12} },
    {8, {0, 0, 7} },
    {9, {0, 0, 3} },
    {10, {0, 0, 1} },
};

Real hwCorrelation[]= {0.0, 0.3, 0.6};

// corr 3, NM/NZ, rank inf/inf 5/inf inf/5 5/5
hwDatumDist hwDataDist[] =
{
    {1, {440, 419, 474,455} },
    {2, {98, 139, 137,1} },
    {3, {12, 53, 79,1} },
    {4, {1, 21, 49,1} },
    {5, {0, 8, 31,1} },
    {6, {0, 3, 19,1} },
    {7, {0, 1, 12,1} },
    {8, {0, 0, 7,1} },
    {9, {0, 0, 3,1} },
    {10, {0, 0, 1,1} },
};

}

int main(int, char* [])
{
    try
       {
        
        Date tradeDate(10,Mar,2023);
        Date CDS_settle =  WeekendsOnly().advance(tradeDate, 1 * Days);
        
        Settings::instance().evaluationDate() =tradeDate;
        Date curveDate = Settings::instance().evaluationDate();
        
        SavedSettings backup;
        
        std::cout << Settings::instance().evaluationDate()  << std::endl;
        std::cout << tradeDate << std::endl;
        std::cout << curveDate << std::endl;
        
        ext::shared_ptr<Estr> estser(new Estr());
        std::vector<ext::shared_ptr<RateHelper>> esterInstruments;
        std::vector<std::pair<int, double>> SWAPDATA;
        SWAPDATA.emplace_back(std::make_pair(6, 0.03309));
        SWAPDATA.emplace_back(std::make_pair(12, 0.03582));
        SWAPDATA.emplace_back(std::make_pair(18, 0.03564));
        SWAPDATA.emplace_back(std::make_pair(24, 0.03467));
        SWAPDATA.emplace_back(std::make_pair(36, 0.03224));
        
        for (int i = 0; i < SWAPDATA.size(); i++)
        {
            ext::shared_ptr<Quote> sw(new SimpleQuote(SWAPDATA[i].second));
            auto helper = ext::make_shared<OISRateHelper>(2, SWAPDATA[i].first * Months, Handle<Quote>(sw), estser);
            esterInstruments.push_back(helper);
        }
        
        
        Settings::instance().evaluationDate() =tradeDate;
        ext::shared_ptr<YieldTermStructure> esterTermStructure(new PiecewiseYieldCurve<Discount, LogLinear>(curveDate,
                                                                                                            esterInstruments,                           Actual365Fixed()));
        
           esterTermStructure->enableExtrapolation(true);
        RelinkableHandle<YieldTermStructure> discountingTermStructure;
        discountingTermStructure.linkTo(esterTermStructure);
        
        
        std::cout << discountingTermStructure->referenceDate() << std::endl;
        std::cout << Settings::instance().evaluationDate()  << std::endl;
        std::cout <<discountingTermStructure->discount(0.25, true) << std::endl;
        std::cout <<discountingTermStructure->discount(0.5, true) << std::endl;
        std::cout <<discountingTermStructure->discount(0.75, true) << std::endl;
        std::cout <<discountingTermStructure->discount(1.0, true) << std::endl;
        std::cout <<discountingTermStructure->discount(2.0, true) << std::endl;
        
        CreditDefaultSwap::PricingModel model = CreditDefaultSwap::ISDA;
           
        
           double cds6mpremium = 0.00300;
           double cds12mpremium = 0.008489;
           double cds24mpremium = 0.006002;
           double cds36mpremium = 0.005002;
           
           std::vector<double> cdspremium_vec;
           cdspremium_vec.emplace_back(cds6mpremium);
           cdspremium_vec.emplace_back(cds12mpremium);
           cdspremium_vec.emplace_back(cds24mpremium);
           cdspremium_vec.emplace_back(cds36mpremium);
           
           
        ext::shared_ptr<CdsHelper> cds6m(new SpreadCdsHelper(
                                                             cdspremium_vec[0], 6 * Months, 1, WeekendsOnly(), Quarterly, Following,
                                                             DateGeneration::CDS, Actual360(), 0.4, discountingTermStructure, true, true, Date(),
                                                             Actual360(true), true, model));
        ext::shared_ptr<CdsHelper> cds12m(new SpreadCdsHelper(
                                                              cdspremium_vec[1], 12 * Months, 1, WeekendsOnly(), Quarterly, Following,
                                                              DateGeneration::CDS,  Actual360(), 0.4, discountingTermStructure, true, true, Date(),
                                                              Actual360(true), true, model));
        ext::shared_ptr<CdsHelper> cds24m(new SpreadCdsHelper(
                                                              cdspremium_vec[2], 24 * Months, 1,WeekendsOnly(), Quarterly, Following,
                                                              DateGeneration::CDS,  Actual360(), 0.4, discountingTermStructure, true, true, Date(),
                                                              Actual360(true), true, model));
        ext::shared_ptr<CdsHelper> cds36m(new SpreadCdsHelper(
                                                              cdspremium_vec[3], 36 * Months, 1, WeekendsOnly(), Quarterly, Following,
                                                              DateGeneration::CDS,  Actual360(), 0.4, discountingTermStructure, true, true, Date(),
                                                              Actual360(true), true, model));
        
        std::vector<ext::shared_ptr<DefaultProbabilityHelper> > isdaCdsHelpers;
        
        isdaCdsHelpers.push_back(cds6m);
        isdaCdsHelpers.push_back(cds12m);
        isdaCdsHelpers.push_back(cds24m);
        isdaCdsHelpers.push_back(cds36m);
        
        Handle<DefaultProbabilityTermStructure> isdaCts =
        Handle<DefaultProbabilityTermStructure>(ext::make_shared<
                                                PiecewiseDefaultCurve<SurvivalProbability, LogLinear> >(curveDate
                                                                                                        ,isdaCdsHelpers,                            Actual365Fixed()));
        isdaCts->enableExtrapolation(true);
           
           Settings::instance().evaluationDate() =tradeDate;
        std::cout << "ISDA credit curve:" << std::endl;
        std::cout << "date;time;survivalprob" << std::endl;
        std::cout <<  isdaCts->survivalProbability(0.25,true) << std::endl;
        std::cout <<  isdaCts->survivalProbability(0.5,true) << std::endl;
        std::cout << isdaCts->survivalProbability(0.75,true) << std::endl;
        std::cout <<  isdaCts->survivalProbability(1.0,true) << std::endl;
        std::cout <<  isdaCts->survivalProbability(1.25,true) << std::endl;
        std::cout <<  isdaCts->survivalProbability(1.50, true) << std::endl;
        std::cout <<  isdaCts->survivalProbability(1.75, true) << std::endl;
        std::cout <<  isdaCts->survivalProbability(2.00, true) << std::endl;
        
           std::cout << "Hazard Rate " << std::endl;
        std::cout << isdaCts->hazardRate(0.25, true) << std::endl;
        std::cout << isdaCts->hazardRate(0.5, true) << std::endl;
        std::cout << isdaCts->hazardRate(0.75, true) << std::endl;
        std::cout << isdaCts->hazardRate(1.0, true) << std::endl;
        std::cout << isdaCts->hazardRate(1.25, true) << std::endl;
        std::cout << isdaCts->hazardRate(1.5, true) << std::endl;
        std::cout << isdaCts->hazardRate(1.75, true) << std::endl;
        std::cout << isdaCts->hazardRate(2.0, true) << std::endl;
           
        std::cout << "Default Probability " << std::endl;
        std::cout << isdaCts->defaultProbability(0.25, true) << std::endl;
        std::cout << isdaCts->defaultProbability(0.5, true) << std::endl;
        std::cout << isdaCts->defaultProbability(0.75, true) << std::endl;
        std::cout << isdaCts->defaultProbability(1.0, true) << std::endl;
        std::cout << isdaCts->defaultProbability(1.25, true) << std::endl;
        std::cout << isdaCts->defaultProbability(1.5, true) << std::endl;
        std::cout << isdaCts->defaultProbability(1.75, true) << std::endl;
        std::cout << isdaCts->defaultProbability(2.0, true) << std::endl;
           
        ext::shared_ptr<IsdaCdsEngine> isdaPricer =
        ext::make_shared<IsdaCdsEngine>(isdaCts, 0.4, discountingTermStructure);
           
        
        Schedule sched_1y(Date(20,March,2023), Date(20,December, 2023), 3*Months,
                   TARGET(), Following, Following, DateGeneration::CDS, false, Date(), Date());
           
           
        ext::shared_ptr<CreditDefaultSwap> trade_uscds_eur_1y =
               ext::shared_ptr<CreditDefaultSwap>(
                   new CreditDefaultSwap(Protection::Seller, 100000000.0, 0.0088, sched_1y,
                                         Following, Actual360(), true, true,
                                         Date(10,March,2023), ext::shared_ptr<Claim>(),
                                         Actual360(true), true));

           ext::shared_ptr<FixedRateCoupon>
           cp = ext::dynamic_pointer_cast<FixedRateCoupon>(trade_uscds_eur_1y->coupons()[0]);
           std::cout << "first period = " << cp->accrualStartDate() << " to " << cp->accrualEndDate() <<
               " accrued amount = " << cp->accruedAmount(Date(12,March,2023)) << std::endl;

           
           ext::shared_ptr<IsdaCdsEngine> engine = ext::make_shared<IsdaCdsEngine>(
                   Handle<DefaultProbabilityTermStructure>(isdaCts), 0.4, discountingTermStructure,
                   false, IsdaCdsEngine::Taylor, IsdaCdsEngine::NoBias, IsdaCdsEngine::Piecewise);

           trade_uscds_eur_1y->setPricingEngine(engine);

           std::cout << "reference trade NPV = " << trade_uscds_eur_1y->NPV() << std::endl;

           
           std::cout << "Par Spread Sensitivity " << std::endl;
           
           for (int i =0; i<isdaCdsHelpers.size();i++)
           {
               
               cdspremium_vec[i] = cdspremium_vec[i] + 0.0001;
                  
               
            ext::shared_ptr<CdsHelper> cds6m(new SpreadCdsHelper(
                                                                 cdspremium_vec[0], 6 * Months, 1, WeekendsOnly(), Quarterly, Following,
                                                                 DateGeneration::CDS, Actual360(), 0.4, discountingTermStructure, true, true, Date(),
                                                                 Actual360(true), true, model));
            ext::shared_ptr<CdsHelper> cds12m(new SpreadCdsHelper(
                                                                  cdspremium_vec[1], 12 * Months, 1, WeekendsOnly(), Quarterly, Following,
                                                                  DateGeneration::CDS,  Actual360(), 0.4, discountingTermStructure, true, true, Date(),
                                                                  Actual360(true), true, model));
            ext::shared_ptr<CdsHelper> cds24m(new SpreadCdsHelper(
                                                                  cdspremium_vec[2], 24 * Months, 1,WeekendsOnly(), Quarterly, Following,
                                                                  DateGeneration::CDS,  Actual360(), 0.4, discountingTermStructure, true, true, Date(),
                                                                  Actual360(true), true, model));
            ext::shared_ptr<CdsHelper> cds36m(new SpreadCdsHelper(
                                                                  cdspremium_vec[3], 36 * Months, 1, WeekendsOnly(), Quarterly, Following,
                                                                  DateGeneration::CDS,  Actual360(), 0.4, discountingTermStructure, true, true, Date(),
                                                                  Actual360(true), true, model));
            
               std::vector<ext::shared_ptr<DefaultProbabilityHelper> > isdaCdsHelpers_temp;
               
               isdaCdsHelpers_temp.push_back(cds6m);
               isdaCdsHelpers_temp.push_back(cds12m);
               isdaCdsHelpers_temp.push_back(cds24m);
               isdaCdsHelpers_temp.push_back(cds36m);
               
               Handle<DefaultProbabilityTermStructure> isdaCts_temp =
               Handle<DefaultProbabilityTermStructure>(ext::make_shared<
                                                       PiecewiseDefaultCurve<SurvivalProbability, LogLinear> >(curveDate
                                                                                                               ,isdaCdsHelpers_temp,                            Actual365Fixed()));
               isdaCts->enableExtrapolation(true);
               
            ext::shared_ptr<CreditDefaultSwap> trade_uscds_eur_1y_temp =
                   ext::shared_ptr<CreditDefaultSwap>(
                       new CreditDefaultSwap(Protection::Seller, 100000000.0, 0.0088, sched_1y,
                                             Following, Actual360(), true, true,
                                             Date(10,March,2023), ext::shared_ptr<Claim>(),
                                             Actual360(true), true));

               ext::shared_ptr<IsdaCdsEngine> engine_temp = ext::make_shared<IsdaCdsEngine>(
                       Handle<DefaultProbabilityTermStructure>(isdaCts_temp), 0.4, discountingTermStructure,
                       false, IsdaCdsEngine::Taylor, IsdaCdsEngine::NoBias, IsdaCdsEngine::Piecewise);

               trade_uscds_eur_1y_temp->setPricingEngine(engine_temp);

               std::cout << i <<"th tenor Par Spread Delta = " << fixed<<
                -(trade_uscds_eur_1y->NPV()- trade_uscds_eur_1y_temp->NPV())/0.0001 << std::endl;

               
               cdspremium_vec[i] = cdspremium_vec[i] -0.0001;
           }
           
           
           
           
           std::cout << "Swap Sensitivity " << std::endl;
           
           for (int i =0; i<SWAPDATA.size();i++)
           {
               
               SWAPDATA[i].second = SWAPDATA[i].second + 0.0001;
               
               std::vector<ext::shared_ptr<RateHelper>> esterInstruments_temp;
               
               for (int i = 0; i < SWAPDATA.size(); i++)
               {
                   ext::shared_ptr<Quote> sw(new SimpleQuote(SWAPDATA[i].second));
                   auto helper = ext::make_shared<OISRateHelper>(2, SWAPDATA[i].first * Months, Handle<Quote>(sw), estser);
                   esterInstruments_temp.push_back(helper);
               }
               
               
               ext::shared_ptr<YieldTermStructure> esterTermStructure_temp(new PiecewiseYieldCurve<Discount, LogLinear>(curveDate,
                                                                                                                   esterInstruments_temp,                           Actual365Fixed()));
               
               esterTermStructure_temp->enableExtrapolation(true);
               RelinkableHandle<YieldTermStructure> discountingTermStructure_temp;
               discountingTermStructure_temp.linkTo(esterTermStructure_temp);
               
               
            ext::shared_ptr<CreditDefaultSwap> trade_uscds_eur_1y_temp =
                   ext::shared_ptr<CreditDefaultSwap>(
                       new CreditDefaultSwap(Protection::Seller, 100000000.0, 0.0088, sched_1y,
                                             Following, Actual360(), true, true,
                                             Date(10,March,2023), ext::shared_ptr<Claim>(),
                                             Actual360(true), true));

               ext::shared_ptr<IsdaCdsEngine> engine_temp = ext::make_shared<IsdaCdsEngine>(
                       Handle<DefaultProbabilityTermStructure>(isdaCts), 0.4, discountingTermStructure_temp,
                       false, IsdaCdsEngine::Taylor, IsdaCdsEngine::NoBias, IsdaCdsEngine::Piecewise);

               trade_uscds_eur_1y_temp->setPricingEngine(engine_temp);

               std::cout << i <<"th tenor Swap Delta = " << fixed<<
                -(trade_uscds_eur_1y->NPV()- trade_uscds_eur_1y_temp->NPV())/0.0001 << std::endl;

               
               SWAPDATA[i].second = SWAPDATA[i].second -0.0001;
           }
        
           
           // Gaussain Coplua Ver //
           std::cout << "Hull White with Gaussian Copula" << std::endl;
           Real relTolerance = 0.015;
           Real absTolerance = 1.0;
           
           Period timeUnit =  1 * Weeks;
           Size names = 10;
           //if (LENGTH(hwData) != names)
           //{
           //    std::cout << " Length NO MATCH ! " << std::endl;
           //}
           
           Real rate_nth = 0.05;
           DayCounter dc_nth = Actual365Fixed();
           Compounding comp_nth = Continuous;
           
           Real recovery = 0.4;
           std::vector<Real> lambda(names, 0.01);
           Real namesNotional = 100.0;
           
           Schedule schedule = MakeSchedule().from(Date(13, Mar, 2023))
                                                   .to(Date(20, December, 2023))
                                                   .withTenor(3*Months)
                                                   .withCalendar(TARGET());
                                                   
           
           Date asofDate(10, Mar, 2023);
           Settings::instance().evaluationDate() = asofDate;
           
           std::vector<Date> gridDates =
           {
               asofDate,
               TARGET().advance(asofDate, Period( 6 * Months)),
               TARGET().advance(asofDate, Period( 12 * Months)),
               TARGET().advance(asofDate, Period( 24 * Months))
           };
               
           
           Handle<YieldTermStructure> yieldHandle(discountingTermStructure);
           std::vector<Handle<DefaultProbabilityTermStructure> > probabilities;
           Period maxTerm(10, Years);
           
           for(Real i : lambda)
           {
               Handle<Quote> h(ext::shared_ptr<Quote>(new SimpleQuote(i)));
               ext::shared_ptr<DefaultProbabilityTermStructure> ptr(new FlatHazardRate(asofDate, h, Actual365Fixed()));
               probabilities.emplace_back(ptr);
           }
           
           
           ext::shared_ptr<SimpleQuote> simpleQuote(new SimpleQuote(0.0));
           Handle<Quote> correlationHandle(simpleQuote);
           
           ext::shared_ptr<DefaultLossModel> gaussian_copula(new
                                                             ConstantLossModel<GaussianCopulaPolicy>
                                                             (correlationHandle,
                                                              std::vector<Real>(names, recovery),
                                                              LatentModelIntegrationType::GaussianQuadrature, names,
                                                              GaussianCopulaPolicy::initTraits()));
           
           std::vector<std::string> namesIds;
           for(Size i= 0; i<names;i++)
           {
               namesIds.emplace_back(std::string("Name") + std::to_string(i));
           }
           std::vector<Issuer> issuers;
           for(Size i =0; i<names;i++)
           {
               std::vector<Issuer::key_curve_pair>curves(
                                                         1, std::make_pair(NorthAmericaCorpDefaultKey(EURCurrency(),
                                                                                                      SeniorSec,
                                                                                                      Period(), 1.0),
                                                                           probabilities[i]));
               issuers.emplace_back(curves);
           }
           
           ext::shared_ptr<Pool> thePool = ext::make_shared<Pool>();
           for(Size i =0; i < names;i++)
           {
               thePool -> add(namesIds[i], issuers[i], NorthAmericaCorpDefaultKey(
                                                                                  EURCurrency(), SeniorSec, Period(), 1.0));
           }
           std::vector<DefaultProbKey> defaultKeys(probabilities.size(),
                                                   NorthAmericaCorpDefaultKey(EURCurrency(), SeniorSec, Period(), 1.0));
           ext::shared_ptr<Basket> basket(new Basket(asofDate, namesIds, std::vector<Real>(names, namesNotional/names), thePool, 0.0, 1.0));
           ext::shared_ptr<PricingEngine> engine_ntd(
                                                 new IntegralNtdEngine(timeUnit, yieldHandle));
           std::vector<NthToDefault> ntd;
           double premium_rates = 0.02;
           for(Size i =1; i<=probabilities.size(); i++)
           {
               ntd.emplace_back(basket, i, Protection::Seller, schedule, 0.0, premium_rates, Actual365Fixed(),
                                namesNotional * names, true);
               ntd.back().setPricingEngine(engine_ntd);
           }
           
           Real fair, diff, maxDiff = 0.0;
           basket->setLossModel(gaussian_copula);
           
           
           std::cout << "Hull White Method without MonteCarlo Simulation" << std::endl;
           for(Size j = 0; j<LENGTH(hwCorrelation); j++)
           {
               simpleQuote->setValue(hwCorrelation[j]);
               for(Size i = 0; i<ntd.size();i++)
               {
                   // CHECK 1: RANK between ntd and hwData
                   // CHECK 2: LENGTH between hwCorrelation and hwData
                   fair = 1.0e4 * ntd[i].fairPremium();
                   std::cout << j << "th Correlation, " << i << "th ntd fair Premium" << fair <<std::endl;
                   std::cout << j << "th Correlation, " << i << "th ntd difference between fair and market" << fair - hwData[i].spread[j] << std::endl;;
                   // diff = fair - hwData[i].spread[j];
                   // maxDiff = std::max(maxDiff, fabs(diff));
               }
           }
           
           
           std::cout << std::endl;
           std::cout << "MonteCarlo Simulation Method" << std::endl;
           ext::shared_ptr<GaussianDefProbLM> gLM
           (ext::make_shared<GaussianDefProbLM>(correlationHandle, names,
                                              LatentModelIntegrationType::GaussianQuadrature,
                                              GaussianCopulaPolicy::initTraits()));
           Size nSim = 10000;
           Size numCoresUsed = 4;
           ext::shared_ptr<RandomDefaultLM
           <GaussianCopulaPolicy> > copula_mc(new RandomDefaultLM<GaussianCopulaPolicy>(gLM,
                                                                                    std::vector<Real>(names, recovery), nSim, 1.e-6, 20230320));
           
           
           double fair_mc, diff_mc, maxDiff_mc = 0.0;
           std::vector<NthToDefault> ntd_mc;
           for(Size i =1; i<=probabilities.size(); i++)
           {
               ntd_mc.emplace_back(basket, i, Protection::Seller, schedule, 0.0, premium_rates, Actual365Fixed(),
                                namesNotional * names, true);
               ntd_mc.back().setPricingEngine(engine_ntd);
           }
           
           basket->setLossModel(copula_mc);
           
           for(Size j = 0; j<LENGTH(hwCorrelation); j++)
           {
               simpleQuote->setValue(hwCorrelation[j]);
               for(Size i = 0; i<ntd_mc.size();i++)
               {
                   // CHECK 1: RANK between ntd and hwData
                   // CHECK 2: LENGTH between hwCorrelation and hwData
                   fair_mc = 1.0e4 * ntd_mc[i].fairPremium();
                   std::cout << j << "th Correlation, " << i << "th ntd fair Premium" << fair_mc <<std::endl;
                   std::cout << j << "th Correlation, " << i << "th ntd difference between fair and market" << fair_mc - hwData[i].spread[j] <<std::endl;;
                   // diff = fair - hwData[i].spread[j];
                   // maxDiff = std::max(maxDiff, fabs(diff));
               }
           }
           
           std::cout << std::endl;
           std::cout << std::endl;
           
           std::cout << "Studnet T Copula without MonteCarlo Simulation Method" << std::endl;
          
           TCopulaPolicy::initTraits iniT;
           iniT.tOrders = std::vector<Integer>(2,5);
           ext::shared_ptr<DefaultLossModel> copula_studentT(new ConstantLossModel<TCopulaPolicy>(
                                                                                         correlationHandle,
                                                                                         std::vector<Real>(names, recovery),
                                                                                         LatentModelIntegrationType::GaussianQuadrature,names, iniT));
           double fair_stdt, diff_stdt, maxDiff_stdt = 0.0;
           std::vector<NthToDefault> ntd_stdt;
           for(Size i =1; i<=probabilities.size(); i++)
           {
               ntd_stdt.emplace_back(basket, i, Protection::Seller, schedule, 0.0, premium_rates, Actual365Fixed(),
                                namesNotional * names, true);
               ntd_stdt.back().setPricingEngine(engine_ntd);
           }
           
           basket->setLossModel(copula_studentT);
           
           for(Size j = 0; j<LENGTH(hwCorrelation); j++)
           {
               simpleQuote->setValue(hwCorrelation[j]);
               for(Size i = 0; i<ntd_stdt.size();i++)
               {
                   // CHECK 1: RANK between ntd and hwData
                   // CHECK 2: LENGTH between hwCorrelation and hwData
                   fair_stdt = 1.0e4 * ntd_stdt[i].fairPremium();
                   std::cout << j << "th Correlation, " << i << "th ntd fair Premium" << fair_stdt <<std::endl;
                   std::cout << j << "th Correlation, " << i << "th ntd difference between fair and market" << fair_stdt - hwData[i].spread[j] <<std::endl;;
                   // diff = fair - hwData[i].spread[j];
                   // maxDiff = std::max(maxDiff, fabs(diff));
               }
           }
           
           
           
           
           std::cout << std::endl;
           
           
        return 0;
        
    }
    catch (exception &e)
    {
       cerr << e.what() << endl;
        return 1;
    }
    catch (...)
    {
       cerr << "unknown error" << endl;
        return 1;
    }


}
