module;

#include "foxy_utils/macros.hpp"

export module foxy_types:packed_map;

import std.core;
import foxy_log;
import foxy_helpers;

namespace fx {
  export template<typename Key, typename Value>
  class packed_map;
  
  export template<typename Key>
  class packed_map_t {
  public:
    packed_map_t() = default;
    virtual ~packed_map_t() = default;
    virtual void erase(const Key& key) = 0;
    virtual void clear() = 0;
  };
  
  export template<typename Key, typename Value>
  class packed_map final: public packed_map_t<Key> {
  public:
    packed_map() = default;
    
    packed_map(std::initializer_list<std::pair<Key, Value>>&& initializer_list)
    {
      for (auto&& [k, v]: FOXY_FWD(initializer_list)) {
        insert(k, v);
      }
    }
    
    void insert(const Key& key, Value element)
    {
      if (index_map_.contains(key)) return;
      
      index_map_[key] = underlying_vector_.size();
      underlying_vector_.push_back({ key, std::move(element) });
    }
  
    void insert_or_assign(const Key& key, Value element)
    {
      if (index_map_.contains(key)) {
        underlying_vector_[index_map_[key]] = std::move(std::pair{ key, std::move(element) });
      } else {
        index_map_[key] = underlying_vector_.size();
        underlying_vector_.push_back({ key, std::move(element) });
      }
    }
    
    void erase(const Key& key) override
    {
      if (!index_map_.contains(key)) return;
      
      // retrieve IDs
      std::size_t index_target{ index_map_.at(key) };
      std::size_t index_last{ underlying_vector_.size() - 1 };
      
      // move target to end of vector
      std::iter_swap(underlying_vector_.begin() + index_target, underlying_vector_.begin() + index_last);
      index_map_[underlying_vector_[index_last].first] = index_target;
      
      // pop end of map
      index_map_.erase(key);
      underlying_vector_.pop_back();
    }
    
    void clear() override
    {
      for (auto& key: index_map_ | std::views::keys) {
        erase(key);
      }
    }
    
    [[nodiscard]] auto at(const Key& key) -> Value&
    {
      FOXY_ASSERT(index_map_.contains(key), "Attempted to access component of non-existent index.");
      return underlying_vector_[index_map_.at(key)];
    }
    
    [[nodiscard]] auto at(const Key& key) const -> const Value&
    {
      FOXY_ASSERT(index_map_.contains(key), "Attempted to access component of non-existent index.");
      return underlying_vector_[index_map_.at(key)];
    }
    
    [[nodiscard]] auto contains(const Key& key) const -> bool
    {
      return index_map_.contains(key);
    }
    
    [[nodiscard]] auto vector() -> std::vector<std::pair<Key, Value>>&
    {
      return underlying_vector_;
    }
    
    [[nodiscard]] auto vector() const -> const std::vector<std::pair<Key, Value>>&
    {
      return underlying_vector_;
    }
  
    [[nodiscard]] auto operator*() -> std::vector<std::pair<Key, Value>>&
    {
      return underlying_vector_;
    }
  
    [[nodiscard]] auto operator*() const -> const std::vector<std::pair<Key, Value>>&
    {
      return underlying_vector_;
    }
  
    [[nodiscard]] auto operator[](const Key& key) -> Value&
    {
      if (!contains(key)) {
        insert(key, Value{});
      }
      return at(key);
    }
  
    [[nodiscard]] auto operator[](Key&& key) -> Value&
    {
      if (!contains(FOXY_FWD(key))) {
        insert(FOXY_FWD(key), Value{});
      }
      return at(FOXY_FWD(key));
    }
  
  private:
    std::vector<std::pair<Key, Value>> underlying_vector_{};
    std::unordered_map<Key, std::size_t> index_map_{};
  };
} // fx