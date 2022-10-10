//
// Created by galex on 10/6/2022.
//

#pragma once

#include "koyote/core/std.hpp"

#include <optional>

namespace fx::flow {
  // template<typename T>
  // struct if_optional {
  //   std::function<void(T&&)> beq{ [](T&& option){} };
  //
  //   auto operator()(std::optional<T>&& option) -> bool
  //   {
  //     if (std::forward<decltype(option)>(option).has_value()) {
  //       beq(*std::forward<decltype(option)>(option));
  //       return true;
  //     }
  //
  //     return false;
  //   }
  // };
  //
  //
  //
  // template<typename T>
  // inline constexpr auto if_opt(
  //   std::optional<T>&& option,
  //   std::function<void(T&&)> true_branch
  // ) -> std::tuple<decltype(*std::forward<decltype(option)>(option)), bool>
  // {
  //   if (std::forward<decltype(option)>(option).has_value()) {
  //     return true;
  //   }
  // }
  
  // template<typename T, typename Callback>
  // inline constexpr auto if_opt(
  //   std::optional<T>&& option,
  //   Callback&& true_branch,
  //   std::function<decltype(std::forward<decltype(true_branch)>(true_branch)(std::forward<decltype(option.value())>(option.value())))()>&& false_branch = [] { return std::nullopt; }
  // ) -> decltype(std::forward<decltype(true_branch)>(true_branch)(std::forward<decltype(option.value())>(option.value())))
  // {
  //   if (option.has_value()) {
  //     return std::forward<decltype(true_branch)>(true_branch)(std::forward<decltype(option.value())>(option.value()));
  //   } else {
  //     return std::forward<decltype(false_branch)>(false_branch)();
  //   }
  // }
  
  template<typename EnumType, typename EnumValueType, typename Callback>
  void for_each_enum(Callback&& callback)
  {
    for (auto [i, stage] { std::tuple<u32, EnumType>{ 0, static_cast<EnumValueType>(0) } };
         i <= EnumType::Max;
         stage = static_cast<EnumValueType>(++i)) {
      std::forward<Callback>(callback)(stage);
    }
  }
  
  template<typename E, typename Callback>
  void for_each_enum(Callback&& callback)
  {
    for_each_enum<E, E>(std::forward<Callback>(callback));
  }
} // fx