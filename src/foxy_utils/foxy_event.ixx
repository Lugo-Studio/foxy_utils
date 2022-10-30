module;

#include "foxy_utils/macros.hpp"
#include <std/core.hpp>

export module foxy_event;

namespace fx {
  export template<typename... Args>
  class Event {
  public:
    template<typename Callback>
    void add_callback(Callback&& callback) {
      handler_functions_.insert(FOXY_FWD(callback));
    }
    
    void operator()(Args... args) {
      for (auto&& function: handler_functions_) {
        function(args...);
      }
    }
  private:
    std::set<std::function<void(Args...)>> handler_functions_;
  };
}