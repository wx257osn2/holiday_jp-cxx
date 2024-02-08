# `holiday_jp-cxx`

Get holidays in Japan.

## Prerequisites

- C++20 supported compiler

## Install

This is header only library.
Just copy `include/*` in your include path.

## Example

```cpp
#include"holiday_jp.hpp"
#include<chrono>
#include<iostream>

int main(){
  using std::literals::chrono_literals::operator""y;
  using std::literals::chrono_literals::operator""d;

  {
    const auto from_date = 2023y/9/14d;
    const auto to_date = 2023y/12/31d;
    const auto holidays = holiday_jp::between(from_date, to_date);
    std::cout << "Holiday(s) between " << from_date << " and " << to_date << ":\n";
    for(auto&& [date, name] : holidays)
      std::cout << "- " << date << ": " << name << '\n';
  }

  {
    const auto date = 2023y/8/11d;
    const bool is_holiday = holiday_jp::is_holiday(date);
    const auto holidays = holiday_jp::between(date, date);
    std::cout << std::boolalpha << "Is " << date << " holiday?: " << is_holiday << " ("
              << (is_holiday ? holidays.begin()->name : "N/A") << ")" << std::endl;
  }
}
```

## API

```cpp
namespace holiday_jp{

// Holiday data
struct holiday{
  std::chrono::year_month_day date;
  std::string_view name;
};

// View of some holidays
template<std::size_t Extent = std::dynamic_extent>
class holidays_view{
 public:
  // Is the argument in the view?
  constexpr bool contains(std::chrono::year_month_day)const;

  // Get the holiday name of the argument
  // If the date is not in the view, this returns std::nullopt
  constexpr std::optional<std::string_view> operator[](std::chrono::year_month_day)const;

  // Range interface of the holidays
  constexpr auto begin()const;
  constexpr auto end()const;
  constexpr auto size()const;

  // Get the new view from this view, between start to last
  constexpr holidays_view<> between(std::chrono::year_month_day start, std::chrono::year_month_day last)const;
};

// Holidays view of all holidays in Japan
static constexpr holidays_view<IMPLEMENTATION_DEFINED> holidays;

// Check the date is holiday or not
static constexpr bool is_holiday(std::chrono::year_month_day date);

// Get the holiday view between start to last
static constexpr holidays_view<> between(std::chrono::year_month_day start, std::chrono::year_month_day last);

}
```

## Development

### Prerequisites

- C++20 supported compiler
- `make`
- `git`

`git sumodule update --init --recursive` first.

### `make` commands for development

```console
$ make generate  # generate include/stringized_holidays_yml.ipp from holiday_jp/holidays.yml
$ make test      # test the API
$ make example   # build example application
```
