module;

#include "koyote/helpers.hpp"
#include <std/core.hpp>

export module koyote_types:reference_count;

import koyote_log;

namespace fx {
  // https://www.acodersjourney.com/implementing-smart-pointer-using-reference-counting/
  class ReferenceCount {
  public:
    auto count() const -> std::size_t
    {
      return ref_count_;
    }
    
    auto operator++() -> ReferenceCount&
    {
      ++ref_count_;
      return *this;
    }
    
    auto operator--() -> ReferenceCount&
    {
      --ref_count_;
      return *this;
    }
  
  private:
    mutable std::size_t ref_count_{ 0 };
  };
}