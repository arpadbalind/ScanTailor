// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once
#include <algorithm>
#include <array>
#include <cstddef>

template <std::size_t M, std::size_t N, typename T>
class MatMNT;

using Mat22f = MatMNT<2, 2, float>;
using Mat22d = MatMNT<2, 2, double>;
using Mat33f = MatMNT<3, 3, float>;
using Mat33d = MatMNT<3, 3, double>;
using Mat44f = MatMNT<4, 4, float>;
using Mat44d = MatMNT<4, 4, double>;

/**
 * \brief A matrix with pre-defined dimensions.
 *
 * \note The memory layout is always column-major, as that's what MatrixCalc uses.
 */
template <std::size_t M, std::size_t N, typename T>
class MatMNT {
 public:
  using type = T;
  static constexpr std::size_t ROWS{ M };
  static constexpr std::size_t COLS{ N };

  /**
   * \brief Initializes matrix elements to T().
   */
  MatMNT();

  /**
   * \brief Construction from an array of elements of possibly different type.
   *
   * Conversion is done by static casts.  Data elements must be in column-major order.
   */
  template <typename OT>
  explicit MatMNT(const OT* data);

  /**
   * \brief Construction from a matrix of same dimensions but another type.
   *
   * Conversion is done by static casts.
   */
  template <typename OT>
  explicit MatMNT(const MatMNT<M, N, OT>& other);

  /**
   * \brief Assignment from a matrix of same dimensions but another type.
   *
   * Conversion is done by static casts.
   */
  template <typename OT>
  auto operator=(const MatMNT<M, N, OT>& other) -> MatMNT&;

  [[nodiscard]] auto data() const -> const T* { return m_data.data(); }

  auto data() -> T* { return m_data.data(); }

  auto operator()(int i, int j) const -> const T& { return m_data[i + (j * M)]; }

  auto operator()(int i, int j) -> T& { return m_data[i + (j * M)]; }

 private:
  std::array<T, M * N> m_data;
};


template <std::size_t M, std::size_t N, typename T>
MatMNT<M, N, T>::MatMNT() {
  for (auto& x : m_data) {
    x = T();
  }
}

template <std::size_t M, std::size_t N, typename T>
template <typename OT>
MatMNT<M, N, T>::MatMNT(const OT* data) {
  std::transform(data, data + (ROWS * COLS), m_data.begin(),
                 [](const OT& v) { return static_cast<T>(v); });
}

template <std::size_t M, std::size_t N, typename T>
template <typename OT>
MatMNT<M, N, T>::MatMNT(const MatMNT<M, N, OT>& other) {
  std::transform(other.data(),
                 other.data() + (ROWS * COLS),
                 m_data.begin(),
                 [](const OT& v) { return static_cast<T>(v); });
}
