#include "time.hpp"

namespace fx {
  Time::Time(double tick_rate, u32 bail_count):
    tick_rate_ { tick_rate },
    bail_count_ { bail_count },
    game_last_ { time_point{clock_steady::now()} },
    game_current_ { time_point{clock_steady::now()} },
    fixed_time_step_ { secs{1. / tick_rate_} } {}
  
  bool Time::should_do_tick() const
  {
    if (step_count_ >= bail_count_) {
      fx::Log::warn("Struggling to catch up with physics rate.");
    }
    
    return lag_.count() >= fixed_time_step_.count() && step_count_ < bail_count_;
  }
  
  void Time::internal_update()
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
  
  void Time::internal_tick()
  {
    // FLUGEL_ENGINE_TRACE("Tick!");
    lag_ -= fixed_time_step_;
    ++step_count_;
  }
  
  void GameLoop::run(const bool& should_continue)
  {
    start(time);
    while (should_continue) {
      while (time.should_do_tick()) {
        tick(time);
        time.internal_tick();
      }
      update(time);
      time.internal_update();
    }
    stop(time);
  }
} // fx