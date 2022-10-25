module;

#include <std/core.hpp>

export module koyote_time:game_loop;

import :time;

namespace fx {
  export class GameLoop {
  public:
    Time time{};
    std::function<void(const Time&)> start{ [](const Time&){} };
    std::function<void(const Time&)> tick{ [](const Time&){} };
    std::function<void(const Time&)> update{ [](const Time&){} };
    std::function<void(const Time&)> stop{ [](const Time&){} };
    
    auto run(const bool& should_continue) -> void
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
    
    inline auto operator()(const bool& should_continue) -> void { run(should_continue); }
  }; // GameLoop
};