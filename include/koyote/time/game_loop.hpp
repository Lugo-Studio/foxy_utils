//
// Created by galex on 10/18/2022.
//

#pragma once

#include "time.hpp"

namespace fx {
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
}
