#include <std/core.hpp>
#include <std/filesystem.hpp>

import koyote_io;
import koyote_log;

int main(const int argc, char** argv)
{
  if (argc != 5) {
    fx::log.info("Usage: [encrypt|decrypt] <input_file> <output_file> <encryption_seed_int32>");
    return EXIT_FAILURE;
  }

  const std::filesystem::path in_path{ argv[2] };
  const std::filesystem::path out_path{ argv[3] };
  
  if (const auto& bytes{ fx::io::read_bytes(in_path) }) {
    std::default_random_engine eng;
    eng.seed(std::strtol(argv[4], nullptr, 10));
    std::uniform_int_distribution<int32_t> dist{ 0, 127 };
  
    (*bytes) | std::views::transform([&](const std::uint8_t byte) {
                 const std::uint8_t rand{ static_cast<std::uint8_t>(dist(eng)) };
                 return (byte + (std::strcmp(argv[1], "encrypt") ? rand : 128 - rand)) % 128;
               })
             | fx::ranges::write_bytes(out_path);
  
    fx::log.info(R"({}ed file "{}" into "{}")", argv[1], argv[2], argv[3]);
  }
}
