//
// Created by galex on 10/15/2022.
//

#pragma once

#include <ranges>
#include <type_traits>

namespace fx::ranges {
  template<std::ranges::range R, typename T>
  auto contains_value(R&& r, const T& value) -> bool
  {
    return std::ranges::find(FOXY_FWD(r), value) != std::ranges::end(FOXY_FWD(r));
  }

  struct ContainsFn {
    template<std::ranges::range R, typename T>
      requires std::constructible_from<std::decay_t<T>, T>
    constexpr auto operator()(R&& r, const T& value)
      const noexcept(std::is_nothrow_constructible_v<std::decay_t<T>, T>) -> bool
    {
      return std::ranges::find(FOXY_FWD(r), value) != std::ranges::end(FOXY_FWD(r));
    }

    template<typename T>
      requires std::constructible_from<std::decay_t<T>, T>
    constexpr auto operator()(const T& value)
      const noexcept(std::is_nothrow_constructible_v<std::decay_t<T>, T>)
    {
      return std::ranges::_Range_closure<ContainsFn, decltype(value)>{ value };
    }
  }; // to_file_fn

  inline constexpr ContainsFn contains;
} // fx

// template<std::ranges::range R>
// constexpr auto operator|(R&& r, const fx::ranges::ContainsFn& func)
// {
//   return func(FOXY_FWD(r));
// }