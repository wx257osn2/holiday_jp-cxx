#include "holiday_jp.hpp"
#include<chrono>

using std::literals::chrono_literals::operator""y;
using std::literals::chrono_literals::operator""d;

static constexpr auto from_date = 2023y/9/14d;
static constexpr auto to_date = 2023y/12/31d;
static constexpr auto holidays = holiday_jp::between(from_date, to_date);

static_assert(holidays.begin()->name == "敬老の日");

static_assert(holiday_jp::is_holiday(2016y/8/11d));
