import std.core;
import std.filesystem;
import std.threading;

import foxy_log;
import foxy_helpers;
import foxy_io;
import foxy_types;

auto main(const int argc, char* argv[]) -> int
{
  if (argc != 5) {
    fx::error("Usage: [encrypt|decrypt] <input_file> <output_file> <encryption_seed_int32>", "");
    return 1;
  }
  fx::set_logging_level(fx::Log::Trace);
  fx::enable_backtrace(8);
  
  const std::filesystem::path in_path{ argv[2] };
  const std::filesystem::path out_path{ argv[3] };
  
  if (const auto& bytes{ fx::io::read_bytes(in_path) }) {
    std::default_random_engine eng;
    eng.seed(std::strtol(argv[4], nullptr, 10));
    std::uniform_int_distribution<int32_t> dist{ 0, 127 };
    
    fx::ranges::write_bytes(
      *bytes | std::views::transform([&](const std::uint8_t byte) {
        const std::uint8_t rand{ static_cast<std::uint8_t>(dist(eng)) };
        return (byte + (std::strcmp(argv[1], "encrypt") ? rand : 128 - rand)) % 128;
      }),
      out_path
    );
  
    fx::info(R"({}ed file "{}" into "{}")", argv[1], in_path.string(), out_path.string());
  
    fx::packed_map<std::string, int> pm{
      { "a", 1 },
      { "b", 2 },
      { "c", 3 },
    };
    for (auto& [k, v]: *pm) {
      fx::info("{}: {}", k, v);
    }
    
    std::array threads{
      std::jthread{
        [] {
          fx::trace("1");
          fx::debug("1");
          fx::info("1");
          fx::warn("1");
          fx::error("1");
          fx::fatal_continue("1");
        }
      },
      std::jthread{
        [] {
          fx::trace("2");
          fx::debug("2");
          fx::info("2");
          fx::warn("2");
          fx::error("2");
          fx::fatal_continue("2");
        }
      }
    };
  }
  
  fx::dump_backtrace();
  // fx::fatal("CRASH TEST (This is an intentional fatal abort)");
}
