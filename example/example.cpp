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
    const auto holiday = holiday_jp::holidays[date];
    std::cout << std::boolalpha << "Is " << date << " holiday?: " << holiday.has_value()
              << " (" << holiday.value_or("N/A") << ")" << std::endl;
  }
}
