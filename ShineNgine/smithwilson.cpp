#include "smithwilson.hpp"

namespace QuantLib {




    SmithWilson::SmithWilson() {}

    SmithWilson::~SmithWilson() {}




    Matrix SmithWilson::UFR_DiscountFactor(Real ufr, Matrix t) {

        Matrix df(t.rows(), t.columns());

        Real ufr_ = std::log(1 + ufr);

        for (Size i = 0; i < t.rows(); i++)

            for (Size j = 0; j < t.columns(); j++)

                df[i][j] = std::exp(-1 * ufr_ * t[i][j]);

        return df;

    }




    Matrix SmithWilson::WilsonFunction(Matrix t1, Matrix t2, Real alpha, Real ufr) {



        Integer m = t1.rows();

        Integer n = t2.rows();




        Matrix t1_Mat(m, n);

        Matrix t2_Mat_t(n, m);

        Matrix t2_Mat(m, n);




        for (Size i = 0; i < m; i++) {

            for (Size j = 0; j < n; j++) {

                t1_Mat[i][j] = t1[i][0];

                t2_Mat_t[j][i] = t2[j][0];

            }

        }

        t2_Mat = transpose(t2_Mat_t);



        Matrix min_t(m, n);

        Matrix max_t(m, n);




        for (Size i = 0; i < m; i++) {

            for (Size j = 0; j < n; j++) {

                min_t[i][j] = std::fmin(t1_Mat[i][j], t2_Mat[i][j]);

                max_t[i][j] = std::fmax(t1_Mat[i][j], t2_Mat[i][j]);

            }

        }

        Matrix ufr_disc = UFR_DiscountFactor(ufr, t1_Mat + t2_Mat);

        Matrix W(m, n);




        for (Size i = 0; i < m; i++) {

            for (Size j = 0; j < n; j++) {

                W[i][j] = ufr_disc[i][j]

                    * (alpha * min_t[i][j] - 0.5 * std::exp(-alpha * max_t[i][j])

                        * (std::exp(alpha * min_t[i][j]) - std::exp(-alpha * min_t[i][j])));

            }

        }

        return W;

    }

    Matrix SmithWilson::Zeta(Matrix P, Matrix t, Real alpha, Real ufr) {

        return inverse(WilsonFunction(t, t, alpha, ufr)) * (UFR_DiscountFactor(ufr, t) - P);

    }

    Matrix SmithWilson::DiscountFactor(Matrix P, Matrix t_obs, Matrix t_target, Real alpha, Real ufr) {

        return UFR_DiscountFactor(ufr, t_target) - WilsonFunction(t_target, t_obs, alpha, ufr) * Zeta(P, t_obs, alpha, ufr);

    }

}
