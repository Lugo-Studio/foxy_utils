module;

#include <std/core.hpp>

export module foxy_error;

namespace fx {
  export enum class Error: int64_t {
    RuntimeError    =  1,
    InvalidInput    =  2,
    InvalidResource =  3,
    OutOfRange      =  69,
  };
  
  export [[nodiscard]] constexpr auto error_message(Error e) -> const char*
  {
    switch (e) {
      case Error::RuntimeError:    return "Runtime error";
      case Error::InvalidInput:    return "Invalid input value";
      case Error::InvalidResource: return "Failed to access resource";
      case Error::OutOfRange:      return "Value out of range";
      default:                     return "Unknown error";
    }
  }
  
  struct ErrorCategory: public std::error_category {
    [[nodiscard]] constexpr auto name() const noexcept -> const char* override
    {
      return "foxy";
    }
    
    [[nodiscard]] constexpr auto message(int32_t e) const -> std::string override
    {
      return error_message(static_cast<Error>(e));
    }
  };
  const ErrorCategory error_category;
  
  export auto make_error_code(Error e) -> std::error_code
  {
    return { static_cast<int32_t>(e), error_category };
  }
} // fx

namespace std {
  export template<>
  struct is_error_code_enum<fx::Error>: true_type {};
} // std

namespace fx {
  export struct Exception: public std::exception {
  public:
    Exception(Error e):
      error_{e}
    {}
    
    [[nodiscard]] auto what() const -> const char* override
    {
      return error_message(error_);
    }
  private:
    Error error_;
  };
} // fx