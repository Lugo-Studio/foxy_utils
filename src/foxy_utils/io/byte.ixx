module;

#include "foxy_utils/macros.hpp"

export module foxy_io:byte;

import std.core;
import std.filesystem;
import foxy_log;
import foxy_error;

namespace fx::io {
  export [[nodiscard]] auto read_bytes(const std::filesystem::path& in_path) -> std::expected<std::vector<std::uint8_t>, std::error_code>
  {
    if (std::ifstream in{ in_path, std::ios::binary }; in.is_open()) {
      const std::streamsize size{ static_cast<std::streamsize>(file_size(in_path)) };
      std::vector<std::uint8_t> in_byte_buffer(size);
      in.read(reinterpret_cast<char*>(in_byte_buffer.data()), size);
      
      return in_byte_buffer;
    }
    
    return std::unexpected{ Error::InvalidResource };
  }
  
  export [[nodiscard]] auto read_words(const std::filesystem::path& in_path) -> std::expected<std::vector<std::uint32_t>, std::error_code>
  {
    if (auto bytes{ read_bytes(in_path) }; bytes.has_value()) {
      const std::streamsize code_word_count{ static_cast<std::streamsize>(bytes->size() / 4) };
      const auto array_start{ reinterpret_cast<std::uint32_t*>(bytes->data()) };
      // convert to word "array"
      return std::vector<std::uint32_t>{ array_start, array_start + code_word_count };
    }
    
    return std::unexpected{ Error::InvalidResource };
  }
  
  export void write_bytes(const std::filesystem::path& out_path, const std::vector<std::uint8_t>& bytes)
  {
    if (std::ofstream out{ out_path, std::ios::binary }; out.is_open()) {
      out.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    } else {
      throw Exception{ Error::InvalidResource };
    }
  }
  
  export void write_words(const std::filesystem::path& out_path, const std::vector<std::uint32_t>& words)
  {
    if (std::ofstream out{ out_path, std::ios::binary }; out.is_open()) {
      const std::streamsize byte_count{ static_cast<std::streamsize>(words.size() * 4) };
      out.write(reinterpret_cast<const char*>(words.data()), byte_count);
    } else {
      throw Exception{ Error::InvalidResource };
    }
  }
} // fx // io

namespace fx::ranges {
  export template<class P>
  concept Path = requires(P & p) {
    std::filesystem::path();
  }; // Path concept

  export struct ToFileFn {
    template<std::ranges::range R, Path P>
    requires std::constructible_from<std::decay_t<P>, P>
    constexpr void operator()(R&& r, P out_path)
    const noexcept(std::is_nothrow_constructible_v<std::decay_t<P>, P>)
    {
      try {
        io::write_bytes(out_path, std::ranges::to<std::vector<std::uint8_t>>(FOXY_FWD(r)));
      } catch (const std::exception& e) {
        fx::error(e.what());
      }
    }
    
    template<Path P>
    requires std::constructible_from<std::decay_t<P>, P>
    constexpr auto operator()(P out_path)
    const noexcept(std::is_nothrow_constructible_v<std::decay_t<P>, P>)
    {
      return std::ranges::_Range_closure<ToFileFn, decltype(out_path)>{ out_path };
    }
  }; // to_file_fn

  export constexpr ToFileFn write_bytes;
} // fx // ranges