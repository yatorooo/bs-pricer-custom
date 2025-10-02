// pricer.cpp — custom version
#include "pricer.hpp"
#include "core/bs_engine.hpp"
#include "core/implied.hpp"
#include "core/date_util.hpp"
#include "core/math_util.hpp"

#include <algorithm> // std::max
#include <cmath>
#include <limits>

ResultRow price_option(const OptionRow& r) {
    // 1) 计算到期时间 T（ACT/365F），与原 QL 版本对齐；同日时采用 1/365 防止 T=0
    core::Date td{}, md{};
    bool ok_td = core::parse_ymd(r.trade_date, td);
    bool ok_md = core::parse_ymd(r.maturity_date, md);

    double T = (ok_td && ok_md) ? core::year_fraction_act365(td, md) : 0.0;
    if (T <= 0.0) T = 1.0 / 365.0;

    // 2) 组装 BS 输入（初始 sigma 先设 20% 备用）
    const double sigma_guess = 0.20;

    core::BSInputs in;
    in.S       = r.spot;
    in.K       = r.strike;
    in.r       = r.r;                 // 假设已为连续复利
    in.q       = r.q;                 // 假设已为连续复利
    in.vol     = sigma_guess;
    in.T       = T;
    in.is_call = (r.type == 'C' || r.type == 'c');

    // 3) 先用 guess 计算一次（即便没有市场价也能给理论价与 Greeks）
    core::BSOutputs out = core::bs_price_greeks(in);

    // 4) 有市场价则求隐含波动率；失败则回退到 20%
    double iv = sigma_guess;
    if (r.market_price > 0.0 && in.S > 0.0 && in.K > 0.0 && in.T > 0.0) {
        double solved = core::implied_vol_bisection(
            r.market_price, in,
            /*lo=*/1e-6, /*hi=*/5.0,
            /*tol=*/1e-7, /*max_iter=*/100
        );
        if (solved == solved) { // 非 NaN
            iv     = solved;
            in.vol = iv;
            out    = core::bs_price_greeks(in); // 用 IV 重算理论价与 Greeks
        }
        // 若是 NaN，则保留 iv = 0.20 的默认值与上面的 out
    }

    // 5) 打包输出
    ResultRow res;
    res.opt         = r;
    res.ttm         = T;
    res.iv          = iv;
    res.model_price = out.price;
    res.error       = out.price - r.market_price;
    res.delta       = out.delta;
    res.gamma       = out.gamma;
    res.vega        = out.vega;   // 对“绝对 σ”的敏感度（若需每 1% 变化，乘 0.01）
    res.theta       = out.theta;  // 按年；若需 per-day： out.theta / 365.0
    res.rho         = out.rho;

    return res;
}
