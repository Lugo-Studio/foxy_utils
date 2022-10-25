module;

#include <nlohmann/json.hpp>

export module koyote_io:json;

namespace fx {
  using json = nlohmann::json;
}
