//
// Created by galex on 10/18/2022.
//

#pragma once

#include "koyote/internal/includes.hpp"
#include <uuid.h>

namespace fx {
  struct uuid: uuids::uuid {
    uuid(): uuids::uuid{ generate() } {}
  private:
    [[nodiscard]] static auto generate() -> uuids::uuid
    {
      static uuids::uuid_system_generator gen{};
      return gen();
    }
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