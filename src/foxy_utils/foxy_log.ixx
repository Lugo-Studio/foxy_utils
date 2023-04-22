module;

#include <std/core.hpp>
#include <std/filesystem.hpp>
#include <std/threading.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/details/os.h>

export module foxy_log;

namespace fx {
  export void terminate_handler();
  
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
      void format(const spdlog::details::log_msg& msg, const std::tm&, spdlog::memory_buf_t& dest) override {
        std::string thread_name{ std::format("{:^10.10}", std::format("{:#010x}", msg.thread_id)) };
        if (names_.contains(msg.thread_id)) {
          thread_name = std::format("{:^10.10}", names_.at(msg.thread_id));
        }
        dest.append(thread_name.data(), thread_name.data() + thread_name.size());
      }
      
      [[nodiscard]] auto clone() const -> std::unique_ptr<custom_flag_formatter> override
      {
        return spdlog::details::make_unique<ThreadNameFlag>();
      }
    };
    
    Log(const std::string& name, const std::filesystem::path& log_file);
    
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
      std::stringstream str;
      str << std::this_thread::get_id();
      error_impl(std::vformat(format.fmt_str + " ({}:{}, native_thread_id:{})", std::make_format_args(args..., format.file_name(), format.line_num(), str.str())));
    }

    template<class... Args>
    void fatal(const FormatLocation& format, Args&&... args)
    {
      std::stringstream str;
      str << std::this_thread::get_id();
      fatal_impl(std::vformat(format.fmt_str + " ({}:{}, native_thread_id:{})", std::make_format_args(args..., format.file_name(), format.line_num(), str.str())));
      std::terminate();
    }
    
    template<class... Args>
    void fatal_continue(const FormatLocation& format, Args&&... args)
    {
      std::stringstream str;
      str << std::this_thread::get_id();
      fatal_impl(std::vformat(format.fmt_str + " ({}:{}, native_thread_id:{})", std::make_format_args(args..., format.file_name(), format.line_num(), str.str())));
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
    
    static void set_thread_name(std::string name)
    {
      set_thread_name_impl(std::move(name));
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
    static inline std::unordered_map<size_t, std::string> names_{};
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
    
    static constexpr auto from_spdlog_level(const spdlog::level::level_enum level) -> Level
    {
      switch (level) {
        case spdlog::level::trace   : return Trace;
        case spdlog::level::debug   : return Debug;
        case spdlog::level::info    : return Info ;
        case spdlog::level::warn    : return Warn ;
        case spdlog::level::err     : return Error;
        case spdlog::level::critical: return Fatal;
        default: return Info; // shouldn't ever do this, but who knows?
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
    
    static void set_thread_name_impl(std::string name) {
      names_.insert_or_assign(spdlog::details::os::thread_id(), std::move(name));
    }
  };
  
  export Log log{ "default_logger", "./app.log" };
  
  // Let's cross our fingies that the compiler will optimize out these empty functions in release mode! :)
  #if defined(FOXY_DEBUG_MODE) or defined(FOXY_RELEASE_MODE_FILE_LOGGING)
  export template<class... Args>
  void trace(const Log::FormatLocation& format, Args&&... args)
  {
    log.trace(format, std::forward<Args>(args)...);
  }
  
  export template<class... Args>
  void debug(const Log::FormatLocation& format, Args&&... args)
  {
    log.debug(format, std::forward<Args>(args)...);
  }
  
  export template<class... Args>
  void info(const Log::FormatLocation& format, Args&&... args)
  {
    log.info(format, std::forward<Args>(args)...);
  }
  
  export template<class... Args>
  void warn(const Log::FormatLocation& format, Args&&... args)
  {
    log.warn(format, std::forward<Args>(args)...);
  }
  
  export template<class... Args>
  void error(const Log::FormatLocation& format, Args&&... args)
  {
    log.error(format, std::forward<Args>(args)...);
  }
  
  export template<class... Args>
  void fatal(const Log::FormatLocation& format, Args&&... args)
  {
    log.fatal(format, std::forward<Args>(args)...);
  }
  
  export template<class... Args>
  void fatal_continue(const Log::FormatLocation& format, Args&&... args)
  {
    log.fatal_continue(format, std::forward<Args>(args)...);
  }
  #else
    export template<class... Args>
    void trace(const FormatLocation& format, Args&&... args) {}
    export template<class... Args>
    void debug(const FormatLocation& format, Args&&... args) {}
    export template<class... Args>
    void info(const FormatLocation& format, Args&&... args) {}
    export template<class... Args>
    void warn(const FormatLocation& format, Args&&... args) {}
    export template<class... Args>
    void error(const FormatLocation& format, Args&&... args) {}
    export template<class... Args>
    void fatal(const FormatLocation& format, Args&&... args) {}
  #endif
  
  export void set_logging_level(const Log::Level level)
  {
    log.set_level_filter(level);
  }
  
  export void enable_backtrace(uint32_t count)
  {
    log.enable_backtrace(count);
  }
  
  export void dump_backtrace()
  {
    log.dump_backtrace();
  }
  
  export void set_thread_name(const std::string& name)
  {
    Log::set_thread_name(name);
  }
  
  void terminate_handler()
  {
    log.dump_backtrace();
    std::abort();
  }
}

module: private;

namespace fx {
  Log::Log(const std::string& name, const std::filesystem::path& log_file)
  {
    #if (defined(FOXY_DEBUG_MODE) or defined(FOXY_RELDEB_MODE)) or defined(FOXY_RELEASE_MODE_FILE_LOGGING)
    
    auto file_formatter = std::make_unique<spdlog::pattern_formatter>();
    file_formatter->add_flag<ThreadNameFlag>('~')
        //.add_flag<EmojiLevelFlag>('`')
      .set_pattern("[%m-%d] [%T.%f] [%~] %^%4!L%$ >> %v");
    auto file_sink{ std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_file.string(), 2097152, 5, false) };
    file_sink->set_level(spdlog::level::trace);
    file_sink->set_formatter(std::move(file_formatter));
    
    lg_ = spdlog::logger{ name, { file_sink } };
    #endif
    
    #if (defined(FOXY_DEBUG_MODE) or defined(FOXY_RELDEB_MODE))
    auto console_formatter = std::make_unique<spdlog::pattern_formatter>();
    console_formatter->add_flag<ThreadNameFlag>('~')
        //.add_flag<EmojiLevelFlag>('`')
      .set_pattern("[%T.%f][%~] %^%4!L%$ >> %v");
    auto console_sink{ std::make_shared<spdlog::sinks::stdout_color_sink_mt>() };
    console_sink->set_level(spdlog::level::trace);
    console_sink->set_formatter(std::move(console_formatter));
    
    lg_ = spdlog::logger{ name, { console_sink, file_sink } };
    #endif
    
    #if (defined(FOXY_DEBUG_MODE) or defined(FOXY_RELDEB_MODE)) or defined(FOXY_RELEASE_MODE_FILE_LOGGING)
    lg_.set_level(spdlog::level::info);
    fx::Log::set_thread_name("main");
    std::set_terminate(fx::terminate_handler);
    #endif
  }
} // fx