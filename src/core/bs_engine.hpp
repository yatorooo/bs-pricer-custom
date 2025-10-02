#pragma once

namespace core {

struct BSInputs {
    double S;      // spot
    double K;      // strike
    double r;      // risk-free (continuous compounding)
    double q;      // dividend yield (continuous compounding)
    double vol;    // volatility (σ)
    double T;      // time to maturity (in years, ACT/365F)
    bool   is_call;// true = Call, falsu = Put
};

struct BSOutputs {
    double price;
    double delta;
    double gamma;
    double vega;   // per 1 vol absolute (not %)
    double theta;  // per year (calendar)
    double rho;    // per 1.0 change in r
};

// Black–Scholes 价格 & 常见 Greeks（欧式，连续收益率 r/q，ACT/365F）
BSOutputs bs_price_greeks(const BSInputs& in);

} // namespace core
