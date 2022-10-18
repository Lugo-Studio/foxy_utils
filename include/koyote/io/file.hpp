//
// Created by galex on 10/6/2022.
//

#pragma once

#include "koyote/types/std.hpp"

namespace fx::io {
  [[nodiscard]] auto read_file(
    const std::filesystem::path& file_path,
    std::ios::fmtflags flags = {}
  ) -> std::optional<std::string>;
} // fx // io
