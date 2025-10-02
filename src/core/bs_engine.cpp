#include "bs_engine.hpp"
#include "math_util.hpp"   // norm_pdf, norm_cdf, is_call
#include <algorithm>

namespace core {

BSOutputs bs_price_greeks(const BSInputs& in) {
    const double S = in.S;
    const double K = in.K;
    const double r = in.r;
    const double q = in.q;
    const double v = in.vol;
    const double T = in.T;

    BSOutputs out{};

    // 如果输入不合理，返回内在价值
    if (S <= 0.0 || K <= 0.0 || T <= 0.0 || v <= 0.0) {
        const double disc_r = std::exp(-r*T);
        const double disc_q = std::exp(-q*T);
        const double intrinsic = in.is_call
            ? std::max(0.0, S*disc_q - K*disc_r)
            : std::max(0.0, K*disc_r - S*disc_q);
        out.price = intrinsic;
        out.delta = in.is_call ? (intrinsic > 0 ? disc_q : 0.0)
                               : (intrinsic > 0 ? -disc_q : 0.0);
        out.gamma = 0.0;
        out.vega  = 0.0;
        out.theta = 0.0;
        out.rho   = 0.0;
        return out;
    }

    const double sqrtT = std::sqrt(T);
    const double vsqrtT = v * sqrtT;

    // Black–Scholes d1, d2
    const double d1 = (std::log(S/K) + (r - q + 0.5*v*v)*T) / vsqrtT;
    const double d2 = d1 - vsqrtT;

    const double disc_r = std::exp(-r*T);
    const double disc_q = std::exp(-q*T);

    // ========== 定价 ==========
    if (in.is_call) {
        out.price = S*disc_q*norm_cdf(d1) - K*disc_r*norm_cdf(d2);
    } else {
        out.price = K*disc_r*norm_cdf(-d2) - S*disc_q*norm_cdf(-d1);
    }

    // 公共项
    const double pdf_d1 = norm_pdf(d1);

    // ========== Greeks ==========
    // Delta
    out.delta = in.is_call
        ? disc_q * norm_cdf(d1)
        : disc_q * (norm_cdf(d1) - 1.0);

    // Gamma
    out.gamma = disc_q * pdf_d1 / (S * vsqrtT);

    // Vega (对 σ 的导数, 每单位波动率变化)
    out.vega = S * disc_q * pdf_d1 * sqrtT;

    // Theta (对 T 的导数, 按年)
    const double term1 = -(S * disc_q * pdf_d1 * v) / (2.0 * sqrtT);
    double theta_call = term1 - r*K*disc_r*norm_cdf(d2) + q*S*disc_q*norm_cdf(d1);
    double theta_put  = term1 + r*K*disc_r*norm_cdf(-d2) - q*S*disc_q*norm_cdf(-d1);
    out.theta = in.is_call ? theta_call : theta_put;

    // Rho
    out.rho = in.is_call
        ? K*T*disc_r*norm_cdf(d2)
        : -K*T*disc_r*norm_cdf(-d2);

    return out;
}

} // namespace core
