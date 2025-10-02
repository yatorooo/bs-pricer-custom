#pragma once
#include <string>

namespace core {

// date structure
struct Date {
    int y;  // year
    int m;  // month 1-12
    int d;  // day  1-31
};

// parse YYYY-MM-DD or YYYY/MM/DD to Date struct
bool parse_ymd(const std::string& s, Date& out);

// transfer calendar date to days
long long civil_to_days(int y, int m, int d);

// return days difference
long long days_between(const Date& a, const Date& b);

// ACT/365F day count fraction
double year_fraction_act365(const Date& start, const Date& end);

} //namespace core