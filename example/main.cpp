#include <std/core.hpp>
#include <std/filesystem.hpp>
#include <std/threading.hpp>

import foxy_log;
import foxy_helpers;
import foxy_io;

auto main(const int32_t argc, char* argv[]) -> int32_t
{
  if (argc != 5) {
    fx::error("Usage: [encrypt|decrypt] <input_file> <output_file> <encryption_seed_int32>");
    return EXIT_FAILURE;
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
      },
      std::jthread{
        [] {
          fx::trace("3");
          fx::debug("3");
          fx::info("3");
          fx::warn("3");
          fx::error("3");
          fx::fatal_continue("3");
        }
      },
      std::jthread{
        [] {
          fx::trace("4");
          fx::debug("4");
          fx::info("4");
          fx::warn("4");
          fx::error("4");
          fx::fatal_continue("4");
        }
      },
      std::jthread{
        [] {
          fx::trace("5");
          fx::debug("5");
          fx::info("5");
          fx::warn("5");
          fx::error("5");
          fx::fatal_continue("5");
        }
      }
    };
  }
  
  fx::dump_backtrace();
  // fx::fatal("CRASH TEST (This is an intentional fatal abort)");
}
