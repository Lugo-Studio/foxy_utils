
#pragma once
//
// Created by Gabriel Lugo on 3/27/2021.
// Flugel Framework: https://github.com/GTLugo/flugel_framework
//

#include "koyote/types/std.hpp"
#include "koyote/misc/log.hpp"

namespace fx {
  // duration types
  using nanosecs = std::chrono::duration<double, std::nano>;
  using microsecs = std::chrono::duration<double, std::micro>;
  using millisecs = std::chrono::duration<double, std::milli>;
  using secs = std::chrono::duration<double>;
  using mins = std::chrono::duration<double, std::ratio<60>>;
  using hours = std::chrono::duration<double, std::ratio<3600>>;
  // clock types
  using clock_system = std::chrono::system_clock;
  using clock_steady = std::chrono::steady_clock;
  using clock_high_res = std::chrono::high_resolution_clock;
  // time point
  using time_point = clock_steady::time_point;
  using time_point_r = clock_high_res::time_point;
  using time_point_s = clock_system::time_point;

  // Based originally on a custom stopwatch class, but eventually evolved into one similar to The Cherno's
  /// TODO: Create split/lap function (probably in a different class to keep this one lightweight)
  class Stopwatch {
  public:
    Stopwatch(std::string name, bool ):
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

  class Time {
    friend class GameLoop;
  public:
    Time(double tick_rate = 128., u32 bail_count = 1024U);
    ~Time() = default;

    [[nodiscard]] auto tick_rate() const -> double
    {
      return tick_rate_;
    }

    template<class Duration>
    [[nodiscard]] auto fixed_step() const -> double
    {
      return Duration{fixed_time_step_}.count();
    }

    template<typename Duration>
    [[nodiscard]] auto now() const -> double
    {
      return Duration{clock_steady::now().time_since_epoch()}.count();
    }

    template<class Duration>
    [[nodiscard]] auto delta() const -> double
    {
      return Duration{delta_}.count();
    }

    template<class Duration>
    [[nodiscard]] auto lag() const -> double
    {
      return Duration{lag_}.count();
    }

    [[nodiscard]] auto should_do_tick() const -> bool;
    
  private:
    // fixed number of ticks per second. this will be used for physics and anything else in fixed update
    double tick_rate_{};
    secs fixed_time_step_{};
    // bail out of the fixed updates if iterations exceeds this amount to prevent lockups
    // on extremely slow systems where tick time may exceed fixed time step
    u32 bail_count_{};

    time_point game_last_{}; // when last frame started
    time_point game_current_{}; // when this frame started
    secs delta_{secs{1. / 60.}}; // how much time last frame took
    secs lag_{secs::zero()}; // how far behind the game is from real world
    u32 step_count_{0U};
    
  private:
    void internal_update();
    void internal_tick();
  }; // Time
  
  class GameLoop {
  public:
    Time time{};
    std::function<void(const Time&)> start{ [](const Time&){} };
    std::function<void(const Time&)> tick{ [](const Time&){} };
    std::function<void(const Time&)> update{ [](const Time&){} };
    std::function<void(const Time&)> stop{ [](const Time&){} };
    
    auto run(const bool& should_continue) -> void;
  
    inline auto operator()(const bool& should_continue) -> void { run(should_continue); }
  }; // GameLoop
} // fx