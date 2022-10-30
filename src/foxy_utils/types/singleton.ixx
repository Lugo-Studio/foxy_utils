module;

#include <std/core.hpp>

export module foxy_types:singleton;

import foxy_log;
import foxy_helpers;

namespace fx::types {
  template<typename T>
  class SingleInstance {
  public:
    SingleInstance()
    {
      if (instantiated) {
        log.fatal("Attempted second instantiation of {}", typeid(T).name());
      }
      
      instantiated = true;
    }
    
    ~SingleInstance()
    {
      instantiated = false;
    }
  
  private:
    static inline bool instantiated{ false };
  };
  
  template<typename T>
  class Singleton {
  public:
    template<typename... Args>
    Singleton(Args&&... args)
    {
      if (instance_ != nullptr) {
        log.fatal("Attempted second instantiation of {}", typeid(T).name());
      }
      
      instance_ = std::make_unique<T>(args...);
    }
    
    ~Singleton() = default;
    
    virtual auto current() -> unique<T>&
    {
      return instance_;
    }
    
    virtual auto current() const -> const unique<T>&
    {
      return instance_;
    }
  
  private:
    static inline unique<T> instance_{ nullptr };
  };
} // fx // flow