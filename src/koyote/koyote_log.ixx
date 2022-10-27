module;

#include <std/core.hpp>
#include <std/filesystem.hpp>
#include <std/threading.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/pattern_formatter.h>

export module koyote_log;

namespace fx {
  export class Log {
  public:
    enum Level {
      Trace,
      Debug,
      Info,
      Warn,
      Error,
      Fatal,
    };
    
    struct FormatLocation {
      std::string fmt_str;
      std::source_location location{};
      
      FormatLocation(const char* fmt_str_, const std::source_location& location_ = std::source_location::current()):
        fmt_str{ fmt_str_ },
        location{ location_ }
      { }
      
      FormatLocation(std::string fmt_str_, const std::source_location& location_ = std::source_location::current()):
        fmt_str{ std::move(fmt_str_) },
        location{ location_ }
      { }
      
      FormatLocation(std::string_view fmt_str_, const std::source_location& location_ = std::source_location::current()):
        fmt_str{ fmt_str_ },
        location{ location_ }
      { }
      
      template<typename Arg>
      FormatLocation(Arg&& obj_, const std::source_location& location_ = std::source_location::current()):
        location{ location_ }
      {
        std::stringstream ss;
        ss << obj_;
        fmt_str = ss.str();
      }
      
      [[nodiscard]] inline auto file_name() const -> std::string
      {
        return std::filesystem::path{ location.file_name() }.filename().string();
      }
      
      [[nodiscard]] inline auto line_num() const -> std::uint32_t
      {
        return location.line();
      }
    };
    
    class ThreadNameFlag final : public spdlog::custom_flag_formatter {
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
      
      [[nodiscard]] auto clone() const -> std::unique_ptr<custom_flag_formatter> override
      {
        return spdlog::details::make_unique<ThreadNameFlag>();
      }
    };
    
    Log(const std::string& name, const std::filesystem::path& log_file)
    {
      #if (defined(FOXY_DEBUG_MODE) or defined(FOXY_RELDEB_MODE)) or defined(FOXY_RELEASE_MODE_FILE_LOGGING)
      
      auto file_formatter = std::make_unique<spdlog::pattern_formatter>();
      file_formatter->add_flag<ThreadNameFlag>('~').set_pattern("[%~][ %^%4!L%$ @ %m-%d %T.%f ] // %v");
      auto file_sink{ std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_file.string(), 2097152, 5, false) };
      file_sink->set_level(spdlog::level::trace);
      file_sink->set_formatter(std::move(file_formatter));
  
      lg_ = spdlog::logger{ name, { file_sink } };
      #endif
      
      #if (defined(FOXY_DEBUG_MODE) or defined(FOXY_RELDEB_MODE))
      auto console_formatter = std::make_unique<spdlog::pattern_formatter>();
      console_formatter->add_flag<ThreadNameFlag>('~').set_pattern("[%~| %^%4!L%$ | %T.%f ] // %v");
      auto console_sink{ std::make_shared<spdlog::sinks::stdout_color_sink_mt>() };
      console_sink->set_level(spdlog::level::trace);
      console_sink->set_formatter(std::move(console_formatter));
  
      lg_ = spdlog::logger{ name, { console_sink, file_sink } };
      #endif
      
      #if (defined(FOXY_DEBUG_MODE) or defined(FOXY_RELDEB_MODE)) or defined(FOXY_RELEASE_MODE_FILE_LOGGING)
      lg_.set_level(spdlog::level::info);
      fx::Log::set_thread_name("main");
      #endif
    }
    
    ~Log() = default;
    
    // Let's cross our fingies that the compiler will optimize out these empty functions in release mode! :)
    #if defined(FOXY_DEBUG_MODE) or defined(FOXY_RELEASE_MODE_FILE_LOGGING)
    template<class... Args>
    void trace(const FormatLocation& format, Args&&... args)
    {
      trace_impl(std::vformat(format.fmt_str, std::make_format_args(args..., format.file_name(), format.line_num())));
    }

    template<class... Args>
    void debug(const FormatLocation& format, Args&&... args)
    {
      debug_impl(std::vformat(format.fmt_str, std::make_format_args(args..., format.file_name(), format.line_num())));
    }

    template<class... Args>
    void info(const FormatLocation& format, Args&&... args)
    {
      info_impl(std::vformat(format.fmt_str, std::make_format_args(args..., format.file_name(), format.line_num())));
    }

    template<class... Args>
    void warn(const FormatLocation& format, Args&&... args)
    {
      warn_impl(std::vformat(format.fmt_str, std::make_format_args(args..., format.file_name(), format.line_num())));
    }

    template<class... Args>
    void error(const FormatLocation& format, Args&&... args)
    {
      error_impl(std::vformat(format.fmt_str + " ({}:{})", std::make_format_args(args..., format.file_name(), format.line_num())));
    }

    template<class... Args>
    void fatal(const FormatLocation& format, Args&&... args)
    {
      fatal_impl(std::vformat(format.fmt_str + " ({}:{})", std::make_format_args(args..., format.file_name(), format.line_num())));
      dump_backtrace();
      std::terminate();
    }
    #else
    template<class... Args>
    void trace(const FormatLocation& format, Args&&... args) {}
    template<class... Args>
    void debug(const FormatLocation& format, Args&&... args) {}
    template<class... Args>
    void info(const FormatLocation& format, Args&&... args) {}
    template<class... Args>
    void warn(const FormatLocation& format, Args&&... args) {}
    template<class... Args>
    void error(const FormatLocation& format, Args&&... args) {}
    template<class... Args>
    void fatal(const FormatLocation& format, Args&&... args) {}
    #endif
    
    void set_level_filter(const Level level)
    {
      lg_.set_level(to_spdlog_level(level));
    }
    
    static void set_thread_name(const std::string& name)
    {
      names_.insert_or_assign(std::this_thread::get_id(), name);
    }
    
    void enable_backtrace(uint32_t count)
    {
      lg_.enable_backtrace(count);
    }
    
    void dump_backtrace()
    {
      lg_.dump_backtrace();
    }
    
    /**
     * \brief This is only necessary if you wish to enable
     * backtrace and full logging in debug mode.
     */
    void conditionally_enable_debug_backtrace(Level filter_level = Info, Level debug_filter_level = Trace)
    {
      set_level_filter(filter_level);
      #if defined(FOXY_DEBUG_MODE)
      enable_backtrace(32);
      #endif
      #if defined(FOXY_DEBUG_MODE) and not defined(FOXY_RELEASE_MODE)
      set_level_filter(debug_filter_level);
      #endif
    }
  
  private:
    static inline std::unordered_map<std::thread::id, std::string> names_{};
    spdlog::logger lg_{ "" };
    
    static constexpr auto to_spdlog_level(const Level level) -> spdlog::level::level_enum
    {
      switch (level) {
        case Trace: return spdlog::level::trace;
        case Debug: return spdlog::level::debug;
        case Info:  return spdlog::level::info;
        case Warn:  return spdlog::level::warn;
        case Error: return spdlog::level::err;
        case Fatal: return spdlog::level::critical;
        default:    return spdlog::level::off; // shouldn't ever do this, but who knows?
      }
    }
    
    void trace_impl(const std::string_view msg) {
      lg_.trace(msg);
    }
    
    void debug_impl(const std::string_view msg) {
      lg_.debug(msg);
    }
    
    void info_impl(const std::string_view msg) {
      lg_.info(msg);
    }
    
    void warn_impl(const std::string_view msg) {
      lg_.warn(msg);
    }
    
    void error_impl(const std::string_view msg) {
      lg_.error(msg);
    }
    
    void fatal_impl(const std::string_view msg) {
      lg_.critical(msg);
    }
    
    // void set_level_filter(const Level level) {
    //   Impl::set_level_filter(level);
    // }
    //
    // void set_thread_name(const std::string& name) {
    //   Impl::set_thread_name(name);
    // }
    //
    // void enable_backtrace(const uint32_t count) {
    //   Impl::enable_backtrace(count);
    // }
    //
    // void dump_backtrace() {
    //   Impl::dump_backtrace();
    // }
  };
  
  export Log log{ "default_logger", "./app.log" };
}