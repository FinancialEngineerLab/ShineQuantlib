#pragma once

#ifndef Alternative_forward_term_structure_hpp
#define Alternative_forward_term_structure_hpp

//#include <boost/test/unit_test.hpp>
#include <ql/quantlib.hpp>

using namespace QuantLib;

class AlternativeUltimateForwardTermStructure
{
public:
	static void BaseCurve();

	static void interpolatedCurve();
	static void CCPinterpolatedCurve();
	static void ExtrapolatedForward();
	static void CCPExtrapolatedForward();

	static std::map<Date, std::pair<Rate, Rate>> setExport();
	static std::map<Date, std::pair<Rate, Rate>> setCCPExport();
	static std::map<Date, std::pair<Rate, Rate>> getDF();
	static std::map<Date, std::pair<Rate, Rate>> getCCPDF();

	static void ZeroRateAtFSP();
	static void testExceptionWhenFSPLessOrEqualZero();
	static void testObservability();
//	static boost::unit_test_framework::test_suite* suite();
};

#endif