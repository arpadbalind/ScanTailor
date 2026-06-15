// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "SparseMap.h"
#include <cstddef>

namespace adiff {
SparseMap<2>::SparseMap(std::size_t numVars)
    : m_numVars(numVars), m_map(numVars, numVars, ZERO_ELEMENT) {}

void SparseMap<2>::markNonZero(std::size_t i, std::size_t j) {
  std::size_t& el = m_map(i, j);
  if (el == ZERO_ELEMENT) {
    el = m_numNonZeroElements;
    ++m_numNonZeroElements;
  }
}

void SparseMap<2>::markAllNonZero() {
  for (std::size_t i = 0; i < m_numVars; ++i) {
    for (std::size_t j = 0; j < m_numVars; ++j) {
      markNonZero(i, j);
    }
  }
}

std::size_t SparseMap<2>::nonZeroElementIdx(std::size_t i, std::size_t j) const {
  return m_map(i, j);
}
}  // namespace adiff
