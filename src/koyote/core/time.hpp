
#pragma once
//
// Created by Gabriel Lugo on 3/27/2021.
// Flugel Framework: https://github.com/GTLugo/flugel_framework
//

#include "koyote/core/std.hpp"
#include "koyote/core/log.hpp"

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
  using clock_accurate = std::chrono::high_resolution_clock;
  // time point
  using time_point = clock_steady::time_point;
  using time_point_a = clock_accurate::time_point;
  using time_point_s = clock_system::time_point;

  class Stopwatch {
  public:
    Stopwatch() {
      start();
    }

    explicit Stopwatch(time_point time_point) {
      start(time_point);
    }

    void start() { start(clock_steady::now()); }

    void start(time_point timePoint) {
      start_ = timePoint;
    }

    template<class Duration>
    [[nodiscard]] double start_time() const {
      return Duration{(start_).time_since_epoch()}.count();
    }

    template<class Duration>
    [[nodiscard]] double get_time_elapsed() const {
      return Duration{(clock_steady::now() - start_)}.count();
    }
    
  private:
    time_point start_{};
  }; // Stopwatch

  class Time {
    friend class GameLoop;
  public:
    Time(double tick_rate = 128., u32 bail_count = 1024U);
    ~Time() = default;

    [[nodiscard]] double tick_rate() const {
      return tick_rate_;
    }

    template<class Duration>
    [[nodiscard]] double fixed_step() const {
      return Duration{fixed_time_step_}.count();
    }

    template<typename Duration>
    [[nodiscard]] double now() const {
      return Duration{clock_steady::now().time_since_epoch()}.count();
    }

    template<class Duration>
    [[nodiscard]] double delta() const {
      return Duration{delta_}.count();
    }

    template<class Duration>
    [[nodiscard]] double lag() const {
      return Duration{lag_}.count();
    }

    [[nodiscard]] bool should_do_tick() const;
    
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
    
    auto run(const bool& stop_flag) -> void;
  
    inline auto operator()(const bool& stop_flag) -> void { run(stop_flag); }
  }; // GameLoop
} // fx