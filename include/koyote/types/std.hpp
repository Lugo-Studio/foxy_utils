#pragma once

#include "koyote/internal/includes.hpp"

namespace fx {
  // strongly-typed type alias
  template<class T, class Tag>
  struct strong_alias: T {
    using T::T;
  };
  
  // ============================= TYPEDEFS ============================= //
  // |                                                                  | //
  using byte = std::uint8_t;
  using word = std::uint32_t;
  using i8   = std::int8_t;
  using u8   = std::uint8_t;
  using i16  = std::int16_t;
  using u16  = std::uint16_t;
  using i32  = std::int32_t;
  using u32  = std::uint32_t;
  using i64  = std::int64_t;
  using u64  = std::uint64_t;

  template<typename T, typename D = std::default_delete<T>>
  using unique = std::unique_ptr<T, D>;

  template<typename T>
  using shared = std::shared_ptr<T>;

  template<typename T>
  using weak = std::weak_ptr<T>;
  // |                                                                  | //
  // ============================= TYPEDEFS ============================= //
  
  // ============================= CONCEPTS ============================= //
  // |                                                                  | //
  template<class T, class U>
  concept Derives = std::is_base_of_v<U, T>;
  // |                                                                  | //
  // ============================= CONCEPTS ============================= //
  
  // ============================= STRUCTS  ============================= //
  // |                                                                  | //
  namespace types {
    class Copyable {
    public:
      Copyable() = default;
      Copyable(const Copyable& other) = default;
      Copyable& operator=(const Copyable& other) = default;
    };
  
    class MoveOnly {
    public:
      MoveOnly() = default;
      MoveOnly(MoveOnly&& other) = default;
      MoveOnly& operator=(MoveOnly&& other) = default;
    
      MoveOnly(const MoveOnly& other) = delete;
      MoveOnly& operator=(const MoveOnly& other) = delete;
    };
  
    class NoCopyOrMove {
    public:
      NoCopyOrMove() = default;
      NoCopyOrMove(const NoCopyOrMove& other) = delete;
      NoCopyOrMove& operator=(const NoCopyOrMove& other) = delete;
      NoCopyOrMove(NoCopyOrMove&& other) = delete;
      NoCopyOrMove& operator=(NoCopyOrMove&& other) = delete;
    };
  }
  // |                                                                  | //
  // ============================= STRUCTS  ============================= //
}
