module;

#include <nlohmann/json.hpp>

export module koyote_io:json;

namespace fx {
  export using json = nlohmann::json;
}
