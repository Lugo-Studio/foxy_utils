module;

#include <std/core.hpp>
#include <std/filesystem.hpp>

export module koyote_io:file;

import koyote_log;

namespace fx::io {
  export [[nodiscard]] auto read_file(
    const std::filesystem::path& file_path,
    const std::ios::fmtflags flags
  ) -> std::optional<std::string>
  {
    if (const std::ifstream file{ file_path, std::ios::in | flags }; file.is_open()) {
      std::stringstream buffer;
      buffer << file.rdbuf();
      return buffer.str();
    }
    
    fx::log.error("File \"{}\" does not exist.", file_path.string());
    return std::nullopt;
  }
} // fx // io