#include <ShineNgine/piecewiseCurveBuilder.hpp>
#include <ShineNgine/krwcd.hpp>


#ifdef BOOST_MSVC
#         include <ql/auto_link.hpp>
#endif

 
#include <iostream>
#include <iomanip>


using namespace QuantLib;
using namespace std;
using namespace PiecewiseCurveBuilderNamespace;

 
#if defined(QL_ENABLE_SESSIONS)
{
           namespace QuantLib
           {
                     ThreadKey sessionId() { return {}; }
           }
}
#endif

 
void  calibrateModel(
           const ext::shared_ptr<ShortRateModel>& model,
           const std::vector<ext::shared_ptr<BlackCalibrationHelper> >& swaptions);
void CreateYieldCurve(RelinkableHandle<YieldTermStructure>& curve,
           const Date& settlementDate, const Calendar& calendar);

 

 
Size numRows = 9;
Size numCols = 9;

 
Integer swapLengths[] =
{
           1,2,3,4,5,7,10,15,20
};

 
Volatility swaptionVols[]
{
           0.273,0.273,0.260,0.264,0.268,0.278,0.281,0.289,0.286,
           0.277,0.259,0.258,0.254,0.252,0.257,0.258,0.269,0.265,
           0.255,0.253,0.251,0.249,0.247,0.248,0.247,0.259,0.254,
           0.254,0.252,0.251,0.245,0.243,0.242,0.246,0.254,0.250,
           0.257,0.252,0.247,0.242,0.238,0.234,0.243,0.246,0.236,
           0.236,0.233,0.230,0.227,0.225,0.233,0.245,0.241,0.232,
           0.209,0.210,0.217,0.228,0.239,0.247,0.242,0.233,0.235,
           0.270,0.260,0.250,0.243,0.238,0.233,0.235,0.250,0.244,
           0.199,0.204,0.210,0.217,0.222,0.231,0.246,0.277,0.290
};

 

 

 

 
void  calibrateModel(
           const ext::shared_ptr<ShortRateModel>& model,
           const std::vector<ext::shared_ptr<BlackCalibrationHelper> >& swaptions) // BlackCalibrationHelper < CalibrationHelper
{
           std::vector<ext::shared_ptr<CalibrationHelper> > helpers(swaptions.begin(), swaptions.end());
           LevenbergMarquardt om; // LM < Optimizaion<Method
           model->calibrate(helpers, om, EndCriteria(1000, 500, 1.0e-5, 1.0e-5, 1.0e-5));

 
           // Implied volatility //
           for (Size i = 0; i < numRows; i++)
           {
                     Size j = numCols - i - 1; //1x20, 2x15, 3x10, 4x7, 5x5, 7x4, 10x3 15x2, 20x1
                     Size k = i * numCols + j;
                     Real npv = swaptions[i]->modelValue();
                     Volatility implied = swaptions[i]->impliedVolatility(npv, 1e-4, 1000, 0.01, 0.9999);
                     Volatility diff = implied - swaptionVols[k];

 
                     std::cout << i + 1 << "x" << swapLengths[j]
                                << std::setprecision(5) << std::noshowpos
                                << ": Model " << std::setw(7) << io::volatility(implied)
                                << ", market " << std::setw(7)
                                << io::volatility(swaptionVols[k])
                                << " (" << std::setw(7) << std::showpos
                                << io::volatility(diff) << std::noshowpos << " )\n";
           }
}

 
void CreateYieldCurve(RelinkableHandle<YieldTermStructure>& curve,
           const Date& settlementDate, const Calendar& calendar)
{
           DayCounter curveDayCounter = Actual365Fixed();
           PiecewiseCurveBuilder<Discount, LogLinear> builder;

 
           pQuote cd03m(new SimpleQuote(0.006600));
           pQuote sw01y(new SimpleQuote(0.009136));
           pQuote sw02y(new SimpleQuote(0.011832));
           pQuote sw03y(new SimpleQuote(0.013589));
           pQuote sw04y(new SimpleQuote(0.014729));
           pQuote sw05y(new SimpleQuote(0.015418));
           pQuote sw07y(new SimpleQuote(0.016207));
           pQuote sw10y(new SimpleQuote(0.016957));
           pQuote sw12y(new SimpleQuote(0.017411));
           pQuote sw15y(new SimpleQuote(0.017257));
           pQuote sw20y(new SimpleQuote(0.017393));

 
           // USDLibor -> KRWCD03M
           
           pIndex period03m(new KrwCD(Period(3, Months)));
           pIndex period01y(new KrwCD(Period(1, Years)));
           pIndex period02y(new KrwCD(Period(2, Years)));
           pIndex period03y(new KrwCD(Period(3, Years)));
           pIndex period04y(new KrwCD(Period(4, Years)));
           pIndex period05y(new KrwCD(Period(5, Years)));
           pIndex period07y(new KrwCD(Period(7, Years)));
           pIndex period10y(new KrwCD(Period(10, Years)));
           pIndex period12y(new KrwCD(Period(12, Years)));
           pIndex period15y(new KrwCD(Period(15, Years)));
           pIndex period20y(new KrwCD(Period(20, Years)));

 
           builder.AddDeposit(cd03m, period03m);
          builder.AddSwap(sw01y, Period(1, Years), calendar, Quarterly, ModifiedFollowing, Actual365Fixed(), period03m);
          builder.AddSwap(sw02y, Period(2, Years), calendar, Quarterly, ModifiedFollowing, Actual365Fixed(), period03m);
          builder.AddSwap(sw03y, Period(3, Years), calendar, Quarterly, ModifiedFollowing, Actual365Fixed(), period03m);
          builder.AddSwap(sw04y, Period(4, Years), calendar, Quarterly, ModifiedFollowing, Actual365Fixed(), period03m);
          builder.AddSwap(sw05y, Period(5, Years), calendar, Quarterly, ModifiedFollowing, Actual365Fixed(), period03m);
          builder.AddSwap(sw07y, Period(7, Years), calendar, Quarterly, ModifiedFollowing, Actual365Fixed(), period03m);
           builder.AddSwap(sw10y, Period(10, Years), calendar, Quarterly, ModifiedFollowing, Actual365Fixed(), period03m);
           builder.AddSwap(sw12y, Period(12, Years), calendar, Quarterly, ModifiedFollowing, Actual365Fixed(), period03m);
           builder.AddSwap(sw15y, Period(15, Years), calendar, Quarterly, ModifiedFollowing, Actual365Fixed(), period03m);
           builder.AddSwap(sw20y, Period(20, Years), calendar, Quarterly, ModifiedFollowing, Actual365Fixed(), period03m);

 
           curve = builder.GetCurveHandle(settlementDate, curveDayCounter);
           curve->enableExtrapolation(true);
}

 

 
int main()
{
           try
           {
                     std::cout << std::endl;
                     Calendar calendar = SouthKorea();
                     Date todaysDate(11, June, 2021);
                     Date settlementDate(14, June, 2021);
                     Settings::instance().evaluationDate() = todaysDate;

 
                     RelinkableHandle<YieldTermStructure> curve;
                     CreateYieldCurve(curve, settlementDate, calendar);

 
                     Frequency fixedLegFrequency = Quarterly;
                     Frequency floatLegFrequency = Quarterly;
                     BusinessDayConvention fixedLegConvention = ModifiedFollowing;
                     BusinessDayConvention floatLegConvention = ModifiedFollowing;
                     DayCounter fixedLegDayCounter = Actual365Fixed();
                     VanillaSwap::Type type = VanillaSwap::Payer;
                     

 
                      //**************************************************************//
                     // HW Calibration //

 
                     ext::shared_ptr<IborIndex> indexSixMonths(new KrwCD(3 * Months, curve));
                     std::vector<ext::shared_ptr<BlackCalibrationHelper>> swaptions;
                     std::list<Time> times;

 
                     std::vector<ext::shared_ptr<BlackCalibrationHelper>> swaptions_diag;
                     std::vector<Volatility> diagonal;
                     std::list<Time> times_diag;
                     Size i;

 
                     ext::shared_ptr<G2> modelG2(new G2(curve));
                     ext::shared_ptr<HullWhite> modelHW(new HullWhite(curve));
                     ext::shared_ptr<HullWhite> modelHW_diag(new HullWhite(curve));
                     ext::shared_ptr<HullWhite> modelHW2(new HullWhite(curve));
                     ext::shared_ptr<BlackKarasinski> modelBK(new BlackKarasinski(curve));

 

 
                     for (auto i = 0; i < 9; i++)
                     {
                                diagonal.push_back(swaptionVols[i*10]);
                     }

 
                     for (auto i = 0; i != diagonal.size(); ++i)
                     {
                                int Mat = i + 1;
                                int underTenor = diagonal.size() - i;
                                
                                ext::shared_ptr<Quote> vol(new SimpleQuote(diagonal[i]));
                                swaptions_diag.push_back(ext::shared_ptr<BlackCalibrationHelper>
                                                     (new SwaptionHelper(Period(Mat, Years), Period(underTenor, Years),
                                                                Handle<Quote>(vol), indexSixMonths, indexSixMonths->tenor(),
                                                                indexSixMonths->dayCounter(), indexSixMonths->dayCounter(),
                                                                curve)));
                                swaptions_diag.back()->addTimesTo(times_diag);
                     }

 
                     std::vector<Period> swaptionMaturities;
                     swaptionMaturities.push_back(Period(1, Years));
                     swaptionMaturities.push_back(Period(2, Years));
                     swaptionMaturities.push_back(Period(3, Years));
                     swaptionMaturities.push_back(Period(4, Years));
                     swaptionMaturities.push_back(Period(5, Years));
                     swaptionMaturities.push_back(Period(7, Years));
                     swaptionMaturities.push_back(Period(10, Years));
                     swaptionMaturities.push_back(Period(12, Years));
                     swaptionMaturities.push_back(Period(15, Years));
                     swaptionMaturities.push_back(Period(20, Years));

 

 
                     for (i = 0; i < numRows; i++)
                     {
                                Size j = numCols - i - 1;
                                Size k = i * numCols + j;
                                ext::shared_ptr<Quote> vol(new SimpleQuote(swaptionVols[k]));
                                swaptions.push_back(ext::shared_ptr<BlackCalibrationHelper>
                                          (new SwaptionHelper(swaptionMaturities[i], Period(swapLengths[j], Years),
                                                     Handle<Quote>(vol), indexSixMonths, indexSixMonths->tenor(),
                                                     indexSixMonths->dayCounter(), indexSixMonths->dayCounter(),
                                                     curve)));
                                swaptions.back()->addTimesTo(times);
                     }

 
                     TimeGrid grid(times.begin(), times.end(), 100);

 
                     std::cout << endl
                                << "Hull-White Jamshidian / Analytic Calibration " << std::endl
                                << "by Diagonal Matrix (9 x 9) " << std::endl;

 
                     for (i = 0; i < swaptions_diag.size(); i++)
                     {
                                swaptions_diag[i]->setPricingEngine(ext::shared_ptr<PricingEngine>
                                          (new JamshidianSwaptionEngine(modelHW_diag)));
                     }
                     calibrateModel(modelHW_diag, swaptions_diag);
                     std::cout << " calibrated to: \n"
                                << "a       =" << modelHW_diag->params()[0] << ", "
                                << "sigma   =" << modelHW_diag->params()[1]
                                << std::endl << std::endl;

 

 

 
                     std::cout << endl 
                                << "Hull-White Jamshidian / Analytic Calibration " << std::endl
                                << "by Full Matrix (9 x 9) " << std::endl;

 
                     for (i = 0; i < swaptions.size(); i++)
                     {
                                swaptions[i]->setPricingEngine(ext::shared_ptr<PricingEngine>
                                          (new JamshidianSwaptionEngine(modelHW)));
                     }
                     calibrateModel(modelHW, swaptions);
                     std::cout << " calibrated to: \n"
                                << "a       =" << modelHW->params()[0] << ", "
                                << "sigma   =" << modelHW->params()[1]
                                << std::endl << std::endl;

 

 
                     std::cout << "Hull-White Jamshidian / Analytic Calibration " << std::endl
                                << "by Diagonal Matrix (9 x 9) " << std::endl;

 
                     for (i = 0; i < swaptions.size(); i++)
                     {
                                swaptions[i]->setPricingEngine(ext::shared_ptr<PricingEngine>
                                          (new JamshidianSwaptionEngine(modelHW)));
                     }
                     calibrateModel(modelHW, swaptions);
                     std::cout << " calibrated to: \n"
                                << "a       =" << modelHW->params()[0] << ", "
                                << "sigma   =" << modelHW->params()[1]
                                << std::endl << std::endl;

 

 
                     std::cout << "Hull-White Tree / Numerical Calibration " << std::endl;
                     for (i = 0; i < swaptions.size(); i++)
                     {
                                swaptions[i]->setPricingEngine(ext::shared_ptr<PricingEngine>
                                          (new TreeSwaptionEngine(modelHW2, grid)));
                     }
                     calibrateModel(modelHW2, swaptions);
                     std::cout << " calibrated to: \n"
                                << "a       =" << modelHW2->params()[0] << ", "
                                << "sigma   =" << modelHW2->params()[1]
                                << std::endl << std::endl;

 
                     std::cout << "Black-Karasinski / numerical Calibration " << std::endl;
                     for (i = 0; i < swaptions.size(); i++)
                     {
                                swaptions[i]->setPricingEngine(ext::shared_ptr<PricingEngine>
                                          (new TreeSwaptionEngine(modelBK, grid)));
                     }
                     calibrateModel(modelBK, swaptions);
                     std::cout << " calibrated to: \n"
                                << "a       =" << modelBK->params()[0] << ", "
                                << "sigma   =" << modelBK->params()[1]
                                << "****************** Calibration End ******************"
                                << std::endl << std::endl;

 

 
                     std::cout << " G2  Calibration " << std::endl;
                     for (i = 0; i < swaptions.size(); i++)
                     {
                                swaptions[i]->setPricingEngine(ext::shared_ptr<PricingEngine>
                                          (new G2SwaptionEngine(modelG2, 6.0, 16)));
                     }
                     calibrateModel(modelG2, swaptions);
                     std::cout << " calibrated to: \n"
                                << "a       =" << modelG2->params()[0] << ", "
                                << "sigma   =" << modelG2->params()[1] << "\n"
                                << "b       =" << modelG2->params()[2] << ", "
                                << "eta     =" << modelG2->params()[3] << "\n"
                                << "rho     =" << modelG2->params()[4]
                                << std::endl << std::endl;

 

 
                     return 0;
           }
           catch (std::exception& e)
           {
                     std::cerr << e.what() << std::endl;
                     return 1;
           }
           catch (...)
           {
                     std::cerr << "Unknown Error " << std::endl;
                     return 1;
           }

 
}
