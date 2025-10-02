#include "implied.hpp"
#include "math_util.hpp"
#include <cmath>
#include <algorithm>
#include <limits>

namespace core {

static inline double price_with_vol(const BSInputs& in, double vol) {
    BSInputs tmp = in;
    tmp.vol = vol;
    return bs_price_greeks(tmp).price;
}

double implied_vol_bisection(double target_price,
                             const BSInputs& base_inputs,
                             double lo, double hi,
                             double tol, int max_iter) {
    if (!(target_price > 0.0) || base_inputs.T <= 0.0 || base_inputs.S <= 0.0 || base_inputs.K <= 0.0) {
        return nan();
    }

    // 单调性：价格随 vol 单调上升（欧式）
    double plo = price_with_vol(base_inputs, lo);
    double phi = price_with_vol(base_inputs, hi);

    // 如果目标不在范围内，尽量扩张一次上界
    if (target_price < plo) return nan();

    // if upper limit price less than target, try extending upper limit (maximum is 10)
    while (target_price > phi && hi < 10.0) {
        lo = hi;
        plo = phi;
        hi *= 2.0;
        phi = price_with_vol(base_inputs, hi);
    }
    // 仍然不可达则失败
    if (phi < target_price) return nan();

    // 二分法主循环
    double mid = 0.5 * (lo + hi);
    for (int it = 0; it < max_iter; ++it) {
        mid = 0.5 * (lo + hi);
        double pm = price_with_vol(base_inputs, mid);

        double err = pm - target_price;
        if (std::fabs(err) < tol) break;

        if (pm < target_price) lo = mid;
        else                   hi = mid;

        if ((hi - lo) < tol * std::max(1.0, mid)) break;
    }
    return mid;
}

} // namespace core
