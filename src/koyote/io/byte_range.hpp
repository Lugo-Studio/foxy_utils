//
// Created by galex on 10/6/2022.
//

#pragma once

#include <ranges>
#include <type_traits>

#include "byte_io.hpp"

namespace fx::ranges {
  template<class P>
  concept Path = requires(P & p) {
    std::filesystem::path();
  }; // Path concept

  template<std::ranges::range R>
  auto write_to_file(R&& r, const std::filesystem::path& out_path)
    -> std::enable_if_t<std::is_same_v<std::ranges::range_value_t<R>, uint8_t>>
  {
    return io::write_bytes(out_path, std::ranges::to<std::vector<byte>>(std::forward<R>(r)));
  }

  struct ToFileFn {
    template<std::ranges::range R, Path P>
      requires std::constructible_from<std::decay_t<P>, P>
    constexpr auto operator()(R&& r, P out_path)
      const noexcept(std::is_nothrow_constructible_v<std::decay_t<P>, P>) -> bool
    {
      return io::write_bytes(out_path, std::ranges::to<std::vector<byte>>(std::forward<R>(r)));
    }

    template<Path P>
      requires std::constructible_from<std::decay_t<P>, P>
    constexpr auto operator()(P out_path)
      const noexcept(std::is_nothrow_constructible_v<std::decay_t<P>, P>)
    {
      return std::ranges::_Range_closure<ToFileFn, decltype(out_path)>{ out_path };
    }
  }; // to_file_fn

  inline constexpr ToFileFn write_bytes;
} // fx // ranges