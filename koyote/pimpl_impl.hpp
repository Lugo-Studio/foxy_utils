#pragma once

namespace fx {
  template<typename T>
  pimpl<T>::pimpl()
    : ptr_{ std::make_unique<T>() } { }

  template<typename T>
  template<typename ...Args>
  pimpl<T>::pimpl(Args&&... args)
    : ptr_{ std::make_unique<T>(std::forward<Args>(args)...) } { }

  template<typename T>
  pimpl<T>::~pimpl() = default;

  template<typename T>
  T* pimpl<T>::operator->() {
    return ptr_.get();
  }

  template<typename T>
  T& pimpl<T>::operator*() {
    return *ptr_.get();
  }
}