#include <ql/quantlib.hpp>

namespace QuantLib
{
	class SmithWilson
	{
	public:
		SmithWilson();
		~SmithWilson();
		Matrix DiscountFactor(Matrix P, Matrix t_obs, Matrix t_target, Real alpha, Real ufr);
	private:
		Matrix UFR_DiscountFactor(Real ufr, Matrix t);
		Matrix WilsonFunction(Matrix t1, Matrix t2, Real alpha, Real ufr);
		Matrix Zeta(Matrix P, Matrix t, Real alpha, Real ufr);

	};
}
