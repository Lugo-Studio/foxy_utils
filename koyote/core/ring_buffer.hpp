#pragma once

namespace fx {
  template<typename T, std::size_t S>
  class ring_buffer {
  public:
    ring_buffer() {
      head_ = &buffer_[0];
      tail_ = &buffer_[0];
    }

    ring_buffer(const std::initializer_list<T>& list) {
      static_assert(list.size() <= size());
      buffer_.swap(list);
      head_ = &buffer_[0];
      tail_ = &buffer_[list.size() - 1];
    }

    ~ring_buffer() {
      
    }

    void push(T&& t) {
      
      if (tail_ != head_) {

      }
    }

    constexpr auto size() -> std::size_t {
      return S;
    }
  private:
    std::array<T, S>::iterator head_;
    std::array<T, S>::iterator tail_;
    std::array<T, S> buffer_{};
  };
} // fx