// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <array>
#include <cstddef>
#include <stdexcept>

#include "NonCopyable.h"

template <typename T>
class StaticPoolBase : public NonCopyable {
 public:
  StaticPoolBase() = default;

  void init(T* buf, size_t size) {
    m_next = buf;
    m_sizeRemaining = size;
  }

  /**
   * \brief Allocates a sequence of objects.
   *
   * If the pool has enough free space, returns a sequence of requested
   * number of elements, otherwise throws an std::runtime_error.
   * If T is a POD type, the returned objects are uninitialized,
   * otherwise they are default-constructed.
   *
   * This function was moved to the base class in order to have
   * just one instantiation of it for different sized pool of the same type.
   */
 public:
  T* alloc(size_t numElements);

 private:
  T* m_next = nullptr;
  size_t m_sizeRemaining = 0;
};

template <typename T, size_t S>
class StaticPool : public StaticPoolBase<T> {
 public:
  StaticPool() {
    this->init(m_buf.data(), S);
  }

 private:
  std::array<T, S> m_buf{};
};

template <typename T>
T* StaticPoolBase<T>::alloc(size_t numElements) {
  if (numElements > m_sizeRemaining || nullptr == m_next) {
    throw std::runtime_error("StaticPool overflow");
  }

  T* sequence = m_next;
  m_next += numElements;
  m_sizeRemaining -= numElements;
  return sequence;
}
