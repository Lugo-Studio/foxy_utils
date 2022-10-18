//
// Created by galex on 10/6/2022.
//

#pragma once

#include "koyote/types/std.hpp"
#include "koyote/macros.hpp"

#include <optional>

namespace fx::flow {
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
    for_each_enum<E, E>(FOXY_FWD(callback));
  }
} // fx