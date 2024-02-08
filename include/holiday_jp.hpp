#ifndef HOLIDAYS_JP_HPP_INCLUDED_
#define HOLIDAYS_JP_HPP_INCLUDED_

#include<algorithm>
#include<array>
#include<charconv>
#include<chrono>
#include<cstddef>
#include<optional>
#include<ranges>
#include<span>
#include<string_view>
#include<utility>
#include<vector>

namespace holiday_jp{

struct holiday{
  std::chrono::year_month_day date;
  std::string_view name;
};

template<std::size_t Extent = std::dynamic_extent>
class holidays_view{
  std::span<const holiday, Extent> map;
  static constexpr const std::chrono::year_month_day& projection(const holiday& h)noexcept{return h.date;}
 public:
  constexpr holidays_view(std::span<const holiday, Extent> data) : map{std::move(data)}{}
  holidays_view(const holidays_view&) = default;
  constexpr holidays_view(holidays_view&&) = default;
  constexpr bool contains(std::chrono::year_month_day k)const{
    const auto it = std::ranges::lower_bound(map, k, {}, projection);
    return it->date == k;
  }
  constexpr std::optional<std::string_view> operator[](std::chrono::year_month_day k)const{
    const auto it = std::ranges::lower_bound(map, k, {}, projection);
    if(it->date == k)
      return it->name;
    return std::nullopt;
  }
  constexpr auto begin()const{
    return map.begin();
  }
  constexpr auto end()const{
    return map.end();
  }
  constexpr auto size()const{
    return map.size();
  }
  constexpr holidays_view<> between(std::chrono::year_month_day start, std::chrono::year_month_day last)const{
    const auto lb = std::ranges::lower_bound(map, start, {}, projection);
    const auto ub = std::ranges::upper_bound(map, last, {}, projection);
    return holidays_view<>{std::span{lb, ub}};
  }
};

namespace detail{

static constexpr std::size_t holidays_yml_entries(std::string_view yml){
  std::size_t n = 0;
  while(not yml.empty()){
    yml.remove_prefix(yml.find_first_of('\n')+1);
    ++n;
  }
  return n;
}

template<typename T, std::size_t N>
static constexpr auto parse_int(std::string_view s){
  T t = 0;
  for(std::size_t i = 0; i < N; ++i){
    t *= 10;
    switch(s[i]){
      case '0': break;
      case '1': t += 1; break;
      case '2': t += 2; break;
      case '3': t += 3; break;
      case '4': t += 4; break;
      case '5': t += 5; break;
      case '6': t += 6; break;
      case '7': t += 7; break;
      case '8': t += 8; break;
      case '9': t += 9; break;
      default:
        throw std::runtime_error("parse failed: not number");
    }
  }
  return t;
}

static constexpr auto parse_holidays_yml(std::string_view yml){
  std::vector<holiday> vec;
  {
    constexpr auto prefix = std::string_view{"---\n"};
    if(not yml.starts_with(prefix))
      throw std::runtime_error("parse failed at preamble");
    yml.remove_prefix(prefix.size());
  }
  while(not yml.empty()){
    const auto year = parse_int<int, 4>(yml);
    yml.remove_prefix(4);
    if(not yml.starts_with('-'))
      throw std::runtime_error("parse failed at delimiter after year");
    yml.remove_prefix(1);
    const auto month = parse_int<unsigned int, 2>(yml);
    yml.remove_prefix(2);
    if(not yml.starts_with('-'))
      throw std::runtime_error("parse failed at delimiter after month");
    yml.remove_prefix(1);
    const auto day = parse_int<unsigned int, 2>(yml);
    yml.remove_prefix(2);
    if(not yml.starts_with(": "))
      throw std::runtime_error("parse failed at delimiter after day");
    yml.remove_prefix(2);
    const auto len = yml.find_first_of('\n');
    vec.emplace_back(std::chrono::year{year}/std::chrono::month{month}/std::chrono::day{day}, yml.substr(0, len));
    yml.remove_prefix(len+1);
  }
  return vec;
}

template<std::size_t Entries>
static constexpr auto parse_holidays_yml(std::string_view yml){
  std::array<holiday, Entries> arr;
  std::ranges::copy(parse_holidays_yml(yml), arr.begin());
  return arr;
}

static constexpr auto holidays = parse_holidays_yml<holidays_yml_entries(
// TODO: use #embed in C++26
#include "stringized_holidays_yml.ipp"
) - 1>(
#include "stringized_holidays_yml.ipp"
);

}

static constexpr holidays_view<detail::holidays.size()> holidays{detail::holidays};

static constexpr bool is_holiday(std::chrono::year_month_day date){
  return holidays.contains(date);
}

static constexpr holidays_view<> between(std::chrono::year_month_day start, std::chrono::year_month_day last){
  return holidays.between(start, last);
}

}

#endif//HOLIDAYS_JP_HPP_INCLUDED_
