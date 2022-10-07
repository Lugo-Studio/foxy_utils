//
// Created by galex on 10/6/2022.
//

#pragma once

#include <filesystem>
#include <vector>
#include <fstream>
#include <optional>

namespace fx::io {
  [[nodiscard]] inline auto read_bytes(const std::filesystem::path& in_path) -> std::optional<std::vector<uint8_t>>
  {
    if (std::ifstream in{ in_path, std::ios::binary }; in.is_open()) {
      const std::streamsize size{ static_cast<std::streamsize>(file_size(in_path)) };
      std::vector<uint8_t> in_byte_buffer(size);
      in.read(reinterpret_cast<char*>(in_byte_buffer.data()), size);
    
      return in_byte_buffer;
    }
  
    return std::nullopt;
  }
  
  inline auto write_bytes(const std::filesystem::path& out_path, const std::vector<uint8_t>& bytes) -> bool
  {
    if (std::ofstream out{ out_path, std::ios::binary }; out.is_open()) {
      out.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
      return true;
    }
  
    return false;
  }
} // fx // io