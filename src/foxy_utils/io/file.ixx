module;

#include <std/core.hpp>
#include <std/filesystem.hpp>

export module foxy_io:file;

import foxy_log;
import foxy_error;

namespace fx::io {
  export [[nodiscard]] auto read_file(
    const std::filesystem::path& file_path,
    const std::ios::fmtflags flags
  ) -> std::expected<std::string, std::error_code>
  {
    if (const std::ifstream file{ file_path, std::ios::in | flags }; file.is_open()) {
      std::stringstream buffer;
      buffer << file.rdbuf();
      return buffer.str();
    }
    
    fx::error("File \"{}\" does not exist.", file_path.string());
    return std::unexpected{ Error::InvalidResource };
  }
} // fx // io