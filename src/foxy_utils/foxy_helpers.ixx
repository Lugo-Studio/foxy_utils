module;

#include <std/core.hpp>
#include <std/memory.hpp>

export module foxy_helpers;

namespace fx {
  // strongly-typed type alias
  export template<class T, class Tag>
  struct strong_alias: T {
    using T::T;
  };
  
  // ============================= TYPEDEFS ============================= //
  // |                                                                  | //
  export template<typename T, typename D = std::default_delete<T>>
  using unique = std::unique_ptr<T, D>;
  
  export template<typename T>
  using shared = std::shared_ptr<T>;
  
  export template<typename T>
  using weak = std::weak_ptr<T>;
  
  export using arg_list = std::vector<std::string>;
  // |                                                                  | //
  // ============================= TYPEDEFS ============================= //
  
  // ============================= CONCEPTS ============================= //
  // |                                                                  | //
  export template<class T, class U>
  concept Derives = std::is_base_of_v<U, T>;
  // |                                                                  | //
  // ============================= CONCEPTS ============================= //
  
  // ============================= STRUCTS  ============================= //
  // |                                                                  | //
  namespace types {
    export class Copyable {
    public:
      Copyable() = default;
      Copyable(const Copyable& other) = default;
      Copyable& operator=(const Copyable& other) = default;
    };
  
    export class MoveOnly {
    public:
      MoveOnly() = default;
      MoveOnly(MoveOnly&& other) = default;
      MoveOnly& operator=(MoveOnly&& other) = default;
    
      MoveOnly(const MoveOnly& other) = delete;
      MoveOnly& operator=(const MoveOnly& other) = delete;
    };
  
    export class NoCopyOrMove {
    public:
      NoCopyOrMove() = default;
      NoCopyOrMove(const NoCopyOrMove& other) = delete;
      NoCopyOrMove& operator=(const NoCopyOrMove& other) = delete;
      NoCopyOrMove(NoCopyOrMove&& other) = delete;
      NoCopyOrMove& operator=(NoCopyOrMove&& other) = delete;
    };
  }
  // |                                                                  | //
  // ============================= STRUCTS  ============================= //
}

