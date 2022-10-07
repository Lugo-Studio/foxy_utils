#include "log.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/pattern_formatter.h>

namespace fx {
  class Log::Impl {
  public:
    class ThreadNameFlag final: public spdlog::custom_flag_formatter {
    public:
      void format(const spdlog::details::log_msg&, const std::tm&, spdlog::memory_buf_t& dest) override {
        const auto thread_id{ std::this_thread::get_id() };
        std::stringstream thread_name{};
        if (names_.contains(thread_id)) {
          thread_name << names_.at(thread_id);
        } else {
          thread_name << thread_id;
        }
        std::string thread_name_str{ std::format("{:^8}", thread_name.str()) };
        dest.append(thread_name_str.data(), thread_name_str.data() + thread_name_str.size());
      }

      [[nodiscard]] std::unique_ptr<custom_flag_formatter> clone() const override {
        return spdlog::details::make_unique<ThreadNameFlag>();
      }
    };
    
    Impl(const std::string& name, const std::filesystem::path& log_file) {
      #if defined(FOXY_DEBUG_MODE) or defined(FOXY_RELDEB_MODE)
      auto console_formatter = std::make_unique<spdlog::pattern_formatter>();
      console_formatter->add_flag<ThreadNameFlag>('~').set_pattern("[ %^%4!L%$ | %T.%f ] %~ | %v");
      auto console_sink{ std::make_shared<spdlog::sinks::stdout_color_sink_mt>() };
      console_sink->set_level(spdlog::level::trace);
      console_sink->set_formatter(std::move(console_formatter));
      
      auto file_formatter = std::make_unique<spdlog::pattern_formatter>();
      file_formatter->add_flag<ThreadNameFlag>('~').set_pattern("[ %^%4!L%$ | %m-%d %T.%f ] %~ | %v");
      auto file_sink{ std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_file.string(), 2097152, 5, false) };
      file_sink->set_level(spdlog::level::trace);
      file_sink->set_formatter(std::move(file_formatter));

      logger() = spdlog::logger{name, {console_sink, file_sink}};
      logger().set_level(spdlog::level::info);
      fx::Log::set_thread_name("main");

      Log::info(R"(--------------=============[])");
      Log::info(R"(  ______ ______   ___     __ )");
      Log::info(R"( |  ____/ __ \ \ / \ \   / / )");
      Log::info(R"( | |__ | |  | \ V / \ \_/ /  )");
      Log::info(R"( |  __|| |  | |> <   \   /   )");
      Log::info(R"( | |   | |__| / . \   | |    )");
      Log::info(R"( |_|    \____/_/ \_\  |_|    )");
      Log::info(R"(                             )");
      Log::info(R"(--------------=============[])");
      Log::info("Foxy startup: Kon kon kitsune! Hi, friends!");
      #if defined(FOXY_DEBUG_MODE) and not defined(FOXY_RELDEB_MODE)
      Log::info("Build mode: DEBUG");
      #else
      Log::info("Build mode: RELEASE");
      #endif
      #endif
    }

    ~Impl() {
      #if defined(FOXY_DEBUG_MODE) or defined(FOXY_RELDEB_MODE)
      Log::info("Foxy shutdown: Otsukon deshita! Bye bye!");
      #endif
    }
    
    static inline void trace(const std::string_view msg) {
      logger().trace(msg);
    }

    static inline void debug(const std::string_view msg) {
      logger().debug(msg);
    }

    static inline void info(const std::string_view msg) {
      logger().info(msg);
    }

    static inline void warn(const std::string_view msg) {
      logger().warn(msg);
    }

    static inline void error(const std::string_view msg) {
      logger().error(msg);
    }

    static inline void fatal(const std::string_view msg) {
      logger().critical(msg);
      dump_backtrace();
      std::terminate();
    }

    static void set_level_filter(const Level level) {
      logger().set_level(*to_spdlog_level(level));
    }

    static void set_thread_name(const std::string& name) {
      names_.insert_or_assign(std::this_thread::get_id(), name);
    }

    static void enable_backtrace(u32 count) {
      logger().enable_backtrace(count);
    }

    static void dump_backtrace() {
      logger().dump_backtrace();
    }

  private:
    static inline std::unordered_map<std::thread::id, std::string> names_{};
    
    static inline auto logger() -> spdlog::logger& {
      static spdlog::logger lg{""};
      return lg;
    }

    static constexpr auto to_spdlog_level(const Level level) -> std::optional<spdlog::level::level_enum>
    {
      if (level == Trace)   return spdlog::level::trace;
      if (level == Debug) return spdlog::level::debug;
      if (level == Info)  return spdlog::level::info;
      if (level == Warn) return spdlog::level::warn;
      if (level == Error) return spdlog::level::err;
      if (level == Fatal) return spdlog::level::critical;
      return std::nullopt;
    }
  }; // Log

  fx::shared<Log::Impl> Log::p_impl_ = std::make_shared<Impl>("fx", "./tmp/logs/app.log");

  Log::Log() = default;

  Log::~Log() = default;

  void Log::trace_impl(const std::string_view msg) {
    Impl::trace(msg);
  }

  void Log::debug_impl(const std::string_view msg) {
    Impl::debug(msg);
  }

  void Log::info_impl(const std::string_view msg) {
    Impl::info(msg);
  }

  void Log::warn_impl(const std::string_view msg) {
    Impl::warn(msg);
  }

  void Log::error_impl(const std::string_view msg) {
    Impl::error(msg);
  }

  void Log::fatal_impl(const std::string_view msg) {
    Impl::fatal(msg);
  }

  void Log::set_level_filter(const Level level) {
    Impl::set_level_filter(level);
  }
  
  void Log::set_thread_name(const std::string& name) {
    Impl::set_thread_name(name);
  }

  void Log::enable_backtrace(const fx::u32 count) {
    Impl::enable_backtrace(count);
  }

  void Log::dump_backtrace() {
    Impl::dump_backtrace();
  }
} // fx