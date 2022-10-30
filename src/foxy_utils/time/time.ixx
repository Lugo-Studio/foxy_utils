module;

#include <std/core.hpp>

export module foxy_time:time;

import foxy_log;

namespace fx {
  // duration types
  export using nanosecs = std::chrono::duration<double, std::nano>;
  export using microsecs = std::chrono::duration<double, std::micro>;
  export using millisecs = std::chrono::duration<double, std::milli>;
  export using secs = std::chrono::duration<double>;
  export using mins = std::chrono::duration<double, std::ratio<60>>;
  export using hours = std::chrono::duration<double, std::ratio<3600>>;
  // clock types
  export using clock_system = std::chrono::system_clock;
  export using clock_steady = std::chrono::steady_clock;
  export using clock_high_res = std::chrono::high_resolution_clock;
  // time point
  export using time_point = clock_steady::time_point;
  export using time_point_r = clock_high_res::time_point;
  export using time_point_s = clock_system::time_point;
  
  export class Time {
    friend class GameLoop;
  public:
    Time(double tick_rate = 128., std::uint32_t bail_count = 1024U):
      tick_rate_ { tick_rate },
      bail_count_ { bail_count },
      game_last_ { time_point{clock_steady::now()} },
      game_current_ { time_point{clock_steady::now()} },
      fixed_time_step_ { secs{1. / tick_rate_} } {}
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
    
    [[nodiscard]] auto should_do_tick() const -> bool
    {
      if (step_count_ >= bail_count_) {
        fx::warn("Struggling to catch up with physics rate.");
      }
  
      return lag_.count() >= fixed_time_step_.count() && step_count_ < bail_count_;
    }
  
  private:
    // fixed number of ticks per second. this will be used for physics and anything else in fixed update
    double tick_rate_{};
    secs fixed_time_step_{};
    // bail out of the fixed updates if iterations exceeds this amount to prevent lockups
    // on extremely slow systems where tick time may exceed fixed time step
    std::uint32_t bail_count_{};
    
    time_point game_last_{}; // when last frame started
    time_point game_current_{}; // when this frame started
    secs delta_{secs{1. / 60.}}; // how much time last frame took
    secs lag_{secs::zero()}; // how far behind the game is from real world
    std::uint32_t step_count_{0U};
  
  private:
    void internal_update()
    {
      // FLUGEL_ENGINE_TRACE("Update!");
      game_current_ = clock_steady::now();
      // Seconds::duration()
      delta_ = std::chrono::duration_cast<secs>(game_current_ - game_last_);
      // if (static_cast<u32>(last_few_frame_times_.size()) >= 10230U) {
      //   last_few_frame_times_.pop_front();
      // }
      // last_few_frame_times_.push_back(delta_.count());
      game_last_ = game_current_;
      lag_ += delta_;
      step_count_ = 0U;
    }
    void internal_tick()
    {
      // FLUGEL_ENGINE_TRACE("Tick!");
      lag_ -= fixed_time_step_;
      ++step_count_;
    }
  }; // Time
} // fx