export module foxy_time:stopwatch;

import std.core;
import :time;

namespace fx {
  // Based originally on a custom stopwatch class, but eventually evolved into one similar to The Cherno's
  /// TODO: Create split/lap function (probably in a different class to keep this one lightweight)
  class Stopwatch {
  public:
    Stopwatch(std::string name):
      name_{ std::move(name) },
      start_{ clock_high_res::now() }
    { }
    
    ~Stopwatch()
    {
      if (!stop_) {
        stop();
      }
    }
    
    void stop()
    {
      stop_ = clock_high_res::now();
    }
    
    template<class Duration>
    [[nodiscard]] auto time_elapsed() const -> double
    {
      return Duration{(clock_steady::now() - start_)}.count();
    }
  
  private:
    std::string name_;
    time_point_r start_{};
    std::optional<time_point_r> stop_{ std::nullopt };
  }; // Stopwatch
}