#include "date_util.hpp"
#include <cstdio>

namespace core {

bool parse_ymd(const std::string& s, Date& out) {
    // default separate character:
    // char sep = '-';
    // if (s.find('-') == std::string::npos && s.find('/') != std::string::npos) {
    //     sep = '/';
    // }
    int y = 0, m = 0, d = 0;
    if (std::sscanf(s.c_str(), "%d%*c%d%*c%d", &y, &m, &d) == 3) {
        out = {y, m ,d};
        return true;
    }
    return false;
}

// civil to days algorithm, making a calendar day to a number.

long long civil_to_days (int y, int m, int d) {
    y -= m <= 2;
    const int era = (y >= 0 ? y : y-399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era*400);      // [0, 399]
    const unsigned doy = (153*(m + (m > 2 ? -3 : 9)) + 2)/5 + d-1;// [0, 365]
    const unsigned doe = yoe*365 + yoe/4 - yoe/100 + doy;         // [0, 146096]
    return era*146097LL + static_cast<long long>(doe) - 719468LL; // 1970-01-01 → 0
}

long long days_between(const Date& a, const Date& b) {
    return civil_to_days(b.y, b.m, b.d) - civil_to_days(a.y, a.m, a.d);
}

double year_fraction_act365(const Date& start, const Date& end) {
    long long days = days_between(start, end);
    return static_cast<double>(days) / 365.0;
}


} // namespace core