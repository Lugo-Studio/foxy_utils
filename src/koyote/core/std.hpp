#pragma once

#include "koyote/internal/includes.hpp"
#include <uuid.h>

namespace fx {
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

  template<class T, class Tag>
  struct strong_alias: T {
    using T::T;
  };

  struct uuid: uuids::uuid {
    uuid(): uuids::uuid{ generate() } {}
  private:
    [[nodiscard]] static auto generate() -> uuids::uuid
    {
      static uuids::uuid_system_generator gen{};
      return gen();
    }
  };

  template<typename T, typename D = std::default_delete<T>>
  using unique = std::unique_ptr<T, D>;

  template<typename T>
  using shared = std::shared_ptr<T>;

  template<typename T>
  using weak = std::weak_ptr<T>;

  template<class T, class U>
  concept Derives = std::is_base_of_v<U, T>;

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
  
  // https://www.acodersjourney.com/implementing-smart-pointer-using-reference-counting/
  class ReferenceCount {
  public:
    auto count() const -> std::size_t
    {
      return ref_count_;
    }
  
    auto operator++() -> ReferenceCount&
    {
      ++ref_count_;
      return *this;
    }
  
    auto operator--() -> ReferenceCount&
    {
      --ref_count_;
      return *this;
    }
    
  private:
    mutable std::size_t ref_count_{ 0 };
  };
}

#define FOXY_UUID_HASH_IMPL(x) \
template<>\
struct std::hash<x> {\
  auto operator()(const x& uuid) const noexcept -> std::size_t\
  {\
    return std::hash<uuids::uuid>{}(uuid);\
  }\
};

FOXY_UUID_HASH_IMPL(fx::uuid)
