module;

#include "koyote/macros.hpp"
#include <std/core.hpp>
#include <std/filesystem.hpp>

export module koyote_io:byte;

namespace fx::io {
  export [[nodiscard]] auto read_bytes(const std::filesystem::path& in_path) -> std::optional<std::vector<std::uint8_t>>
  {
    if (std::ifstream in{ in_path, std::ios::binary }; in.is_open()) {
      const std::streamsize size{ static_cast<std::streamsize>(file_size(in_path)) };
      std::vector<std::uint8_t> in_byte_buffer(size);
      in.read(reinterpret_cast<char*>(in_byte_buffer.data()), size);
      
      return in_byte_buffer;
    }
    
    return std::nullopt;
  }
  
  export [[nodiscard]] auto read_words(const std::filesystem::path& in_path) -> std::optional<std::vector<std::uint32_t>>
  {
    if (auto bytes{ read_bytes(in_path) }; bytes.has_value()) {
      const std::streamsize code_word_count{ static_cast<std::streamsize>(bytes.value().size() / 4) };
      const auto array_start{ reinterpret_cast<std::uint32_t*>(bytes.value().data()) };
      std::vector<std::uint32_t> word_buffer{ array_start, array_start + code_word_count };
      
      return word_buffer;
    }
    
    return std::nullopt;
  }
  
  export auto write_bytes(const std::filesystem::path& out_path, const std::vector<std::uint8_t>& bytes) -> bool
  {
    if (std::ofstream out{ out_path, std::ios::binary }; out.is_open()) {
      out.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
      
      return true;
    }
    
    return false;
  }
  
  export auto write_words(const std::filesystem::path& out_path, const std::vector<std::uint32_t>& words) -> bool
  {
    if (std::ofstream out{ out_path, std::ios::binary }; out.is_open()) {
      const std::streamsize byte_count{ static_cast<std::streamsize>(words.size() * 4) };
      out.write(reinterpret_cast<const char*>(words.data()), byte_count);
      
      return true;
    }
    
    return false;
  }
} // fx // io

namespace fx::ranges {
  export template<class P>
  concept Path = requires(P & p) {
    std::filesystem::path();
  }; // Path concept
  
  export template<std::ranges::range R>
  auto write_to_file(R&& r, const std::filesystem::path& out_path)
    -> std::enable_if_t<std::is_same_v<std::ranges::range_value_t<R>, std::uint8_t>>
  {
    return io::write_bytes(out_path, std::ranges::to<std::vector<std::uint8_t>>(FOXY_FWD(r)));
  }

  export struct ToFileFn {
    template<std::ranges::range R, Path P>
    requires std::constructible_from<std::decay_t<P>, P>
    constexpr auto operator()(R&& r, P out_path)
    const noexcept(std::is_nothrow_constructible_v<std::decay_t<P>, P>) -> bool
    {
      return io::write_bytes(out_path, std::ranges::to<std::vector<std::uint8_t>>(FOXY_FWD(r)));
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