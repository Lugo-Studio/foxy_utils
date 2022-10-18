//
// Created by galex on 10/6/2022.
//

#include "koyote/io/file.hpp"

#include "koyote/misc/log.hpp"

namespace fx::io {
  [[nodiscard]] auto read_file(
    const std::filesystem::path& file_path,
    const std::ios::fmtflags flags
  ) -> std::optional<std::string>
  {
    if (const std::ifstream file{ file_path, std::ios::in | flags }; file.is_open()) {
      std::stringstream buffer;
      buffer << file.rdbuf();
      return buffer.str();
    }
    
    Log::error("File \"{}\" does not exist.", file_path.string());
    return std::nullopt;
  }
} // fx // io