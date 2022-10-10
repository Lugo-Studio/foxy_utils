//
// Created by galex on 10/9/2022.
//

#pragma once

#include <string_view>

#include "koyote/core/log.hpp"

namespace fx::types {
  template<typename T>
  class SingleInstance {
  public:
    SingleInstance()
    {
      if (instantiated) {
        Log::fatal("Attempted second instantiation of {}", typeid(T).name());
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
} // fx // flow