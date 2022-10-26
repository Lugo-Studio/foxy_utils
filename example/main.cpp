#include <std/core.hpp>
#include <std/filesystem.hpp>

import koyote_io;

int main(const int argc, char** argv)
{
  if (argc != 5) {
    std::cerr << "Usage: [encrypt|decrypt] <input_file> <output_file> <encryption_seed_int32>\n";
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
  
    std::cout << argv[1] << "ed file \"" << argv[2] << "\" into \"" << argv[3] << '\"';
  }
}
