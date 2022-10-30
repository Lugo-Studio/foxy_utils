module;

#include "foxy_utils/macros.hpp"
#include <std/core.hpp>

export module foxy_types:packed_array;

import foxy_log;
import foxy_helpers;

namespace fx {
  template<typename Index, typename Element>
  class packed_array;
  
  template<typename Index>
  class packed_array_t {
  public:
    packed_array_t() = default;
    virtual ~packed_array_t() = default;
    virtual void erase(const Index& index) = 0;
  };
  
  template<typename Index, typename Element>
  class packed_array final: public packed_array_t<Index> {
  public:
    void insert(const Index& index, Element&& element)
    {
      FOXY_ASSERT(!index_map_.contains(index), "Attempted to override component at existing index.");
      index_map_[index] = underlying_vector_.size();
      removal_map_[underlying_vector_.size()] = index;
      underlying_vector_.push_back(FOXY_FWD(element));
    }
    
    void erase(const Index& index) override
    {
      FOXY_ASSERT(index_map_.contains(index), "Attempted to remove component of non-existent index.");
      // retrieve IDs
      std::vector x{ 1, 2, 3 };
      std::size_t index_target{ index_map_.at(index) };
      std::size_t index_last{ underlying_vector_.size() - 1 };
      Index entity_last{ removal_map_.at(index_last) };
      
      // move target to end of map
      std::iter_swap(underlying_vector_.begin() + index_target, underlying_vector_.begin() + index_last);
      index_map_[entity_last] = index_target;
      removal_map_[index_target] = entity_last;
      
      // pop end of map
      index_map_.erase(index);
      removal_map_.erase(index_last);
      underlying_vector_.pop_back();
    }
    
    [[nodiscard]] auto get(const Index& index) -> Element&
    {
      FOXY_ASSERT(index_map_.contains(index), "Attempted to access component of non-existent index.");
      return underlying_vector_[index_map_.at(index)];
    }
    
    [[nodiscard]] auto get(const Index& index) const -> const Element&
    {
      FOXY_ASSERT(index_map_.contains(index), "Attempted to access component of non-existent index.");
      return underlying_vector_[index_map_.at(index)];
    }
    
    [[nodiscard]] auto contains(const Index& index) const -> bool
    {
      return index_map_.contains(index);
    }
    
    [[nodiscard]] auto data() -> std::vector<Element>&
    {
      return underlying_vector_;
    }
    
    [[nodiscard]] auto data() const -> const std::vector<Element>&
    {
      return underlying_vector_;
    }
    
    [[nodiscard]] auto indices() const -> const std::unordered_map<Index, std::size_t>&
    {
      return index_map_;
    }
  
  private:
    std::vector<Element> underlying_vector_{};
    std::unordered_map<Index, std::size_t> index_map_{};
    std::unordered_map<std::size_t, Index> removal_map_{}; // for removing data
  };
  
  // using packed_vector_t = packed_map_t;
  
  // template<class T>
  // using packed_vector = packed_map<std::size_t, T>;
} // fx