//
// Created by galex on 10/6/2022.
//

#pragma once

#include "koyote/core/std.hpp"

#include <optional>

namespace fx::flow {
  template<typename T, typename Callback>
  auto if_opt(std::optional<T>&& option, Callback&& true_branch) -> bool
  {
    if (option.has_value()) {
      std::forward<decltype(true_branch)>(true_branch)(std::forward<decltype(option.value())>(option.value()));
    }
    return option.has_value();
  }
  
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