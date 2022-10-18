//
// Created by galex on 10/6/2022.
//

#pragma once

#include <filesystem>
#include <vector>
#include <fstream>
#include <optional>

namespace fx::io {
  [[nodiscard]] inline auto read_bytes(const std::filesystem::path& in_path) -> std::optional<std::vector<byte>>
  {
    if (std::ifstream in{ in_path, std::ios::binary }; in.is_open()) {
      const std::streamsize size{ static_cast<std::streamsize>(file_size(in_path)) };
      std::vector<byte> in_byte_buffer(size);
      in.read(reinterpret_cast<char*>(in_byte_buffer.data()), size);
    
      return in_byte_buffer;
    }
  
    return std::nullopt;
  }
  
  [[nodiscard]] static auto read_words(const std::filesystem::path& in_path) -> std::optional<std::vector<word>>
  {
    if (auto bytes{ read_bytes(in_path) }; bytes.has_value()) {
      const std::streamsize code_word_count{ static_cast<std::streamsize>(bytes.value().size() / 4) };
      const auto array_start{ reinterpret_cast<u32*>(bytes.value().data()) };
      std::vector<word> word_buffer{ array_start, array_start + code_word_count };
      
      return word_buffer;
    }
    
    return std::nullopt;
  }
  
  inline auto write_bytes(const std::filesystem::path& out_path, const std::vector<byte>& bytes) -> bool
  {
    if (std::ofstream out{ out_path, std::ios::binary }; out.is_open()) {
      out.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
      
      return true;
    }
  
    return false;
  }
  
  inline auto write_words(const std::filesystem::path& out_path, const std::vector<word>& words) -> bool
  {
    if (std::ofstream out{ out_path, std::ios::binary }; out.is_open()) {
      const std::streamsize byte_count{ static_cast<std::streamsize>(words.size() * 4) };
      out.write(reinterpret_cast<const char*>(words.data()), byte_count);
      
      return true;
    }
    
    return false;
  }
} // fx // io