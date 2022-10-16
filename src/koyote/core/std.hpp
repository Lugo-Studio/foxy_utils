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

  struct uuid: public uuids::uuid {
    uuid(): uuids::uuid{ generate() } {}
    uuid(const uuid& rhs): uuids::uuid{ rhs } {}

  private:
    [[nodiscard]] static auto generate() -> uuids::uuid
    {
      static uuids::uuid_system_generator gen{};
      return gen();
    }
  };
  
  // struct uuid {
  //   uuid(): id_{ generate() } {}
  //   uuid(const uuid& rhs): id_{ rhs.id_ } {}
  //
  //   [[nodiscard]] auto id() const -> const UUIDv4::UUID& { return id_; }
  //   [[nodiscard]] auto str() const -> std::string { return id_.str(); }
  //
  //   [[nodiscard]] auto operator<(const uuid& rhs) const -> bool { return id_ < rhs.id_; }
  //   [[nodiscard]] auto operator==(const uuid& rhs) const -> bool { return id_ == rhs.id_; }
  // private:
  //   UUIDv4::UUID id_;
  //
  //   [[nodiscard]] static auto generate() -> UUIDv4::UUID
  //   {
  //     static UUIDv4::UUIDGenerator<std::mt19937_64> gen{};
  //     return gen.getUUID();
  //   }
  // };

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

template<>
struct std::hash<fx::uuid> {
  auto operator()(const fx::uuid& uuid) const -> std::size_t
  {
    return std::hash<uuids::uuid>{}(uuid);
  }
};
