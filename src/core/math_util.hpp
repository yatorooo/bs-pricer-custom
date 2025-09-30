#pragma once
#include <cmath>
#include <limits>

namespace core {
    
// constant numebers

inline constexpr double PI            = 3.14159265358979323846;  // π
inline constexpr double TWO_PI        = 6.28318530717958647692;  // 2π
inline constexpr double INV_SQRT_2PI  = 0.39894228040143267794;  // 1/sqrt(2π)
inline constexpr double INV_SQRT_2    = 0.70710678118654752440;  // 1/sqrt(2)
inline constexpr double EPS           = 1e-12;                   // epsilon


// useful tools

inline double inf() { return std::numeric_limits<double>::infinity();}
inline double nan() { return std::numeric_limits<double>::quiet_NaN();}

inline double clamp(double x, double lo, double hi) {
    return x < lo ? lo : (x > hi ? hi : x);
}

// option categoray: call or not call (put)
inline bool is_call(char opt) {
    return opt == 'C' || opt == 'c';
}

inline bool is_finite(double x) {
    return std::isfinite(x);
}


// standard normal distribution
// probability density function (pdf)  :  φ(x) = 1/sqrt(2π) * exp(-x^2/2)
inline double norm_pdf(double x) {
    return INV_SQRT_2PI * std::exp(-0.5 * x * x);
}

// cumulative density function (cdf)  :   Φ(x) = 0.5 * [1 + erf(x / sqrt(2))]
inline double norm_cdf(double x){
    return 0.5 * (1.0 + std::erf(x * INV_SQRT_2));
}

// 1 - Φ(x)
inline double norm_ccdf(double x) {
    return 0.5 * std::erfc(x * INV_SQRT_2);
}

} // namespace core