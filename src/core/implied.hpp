#pragma once
#include "bs_engine.hpp"

namespace core {

// 用二分法求隐含波动率；返回 sigma（失败时返回 NaN）
// - target_price: 市场价
// - base_inputs : 其他BS参数（会覆盖其中的 vol）
// - lo/hi       : 初始搜索区间（缺省 [1e-6, 5.0]）
// - tol         : 价格误差/区间收敛的容差
// - max_iter    : 最大迭代次数

double implied_vol_bisection(double target_price,
                             const BSInputs& base_inputs,
                             double lo = 1e-6, double hi = 5.0,
                             double tol = 1e-7, int max_iter = 100);

} // namespace core
