module;

#include "foxy_utils/macros.hpp"
#include <std/core.hpp>

export module foxy_flow:for_each;

namespace fx::flow {
  export template<typename EnumType, typename EnumValueType, typename Callback>
  void for_each_enum(Callback&& callback)
  {
    for (auto [i, stage] { std::tuple<std::uint32_t, EnumType>{ 0, static_cast<EnumValueType>(0) } };
         i <= EnumType::Max;
         stage = static_cast<EnumValueType>(++i)) {
      FOXY_FWD(callback)(stage);
    }
  }
  
  export template<typename E, typename Callback>
  void for_each_enum(Callback&& callback)
  {
    for_each_enum<E, E>(FOXY_FWD(callback));
  }
} // fx