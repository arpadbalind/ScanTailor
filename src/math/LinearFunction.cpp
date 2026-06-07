// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "LinearFunction.h"

#include <algorithm>

LinearFunction::LinearFunction(std::size_t numVars) : a(numVars) {}

void LinearFunction::reset() {
  a.fill(0);
  b = 0;
}

double LinearFunction::evaluate(const double* x) const {
  const std::size_t numVars = this->numVars();

  double sum = b;
  for (std::size_t i = 0; i < numVars; ++i) {
    sum += a[i] * x[i];
  }
  return sum;
}

void LinearFunction::swap(LinearFunction& other) noexcept {
  a.swap(other.a);
  std::swap(b, other.b);
}

LinearFunction& LinearFunction::operator+=(const LinearFunction& other) {
  a += other.a;
  b += other.b;
  return *this;
}

LinearFunction& LinearFunction::operator*=(double scalar) {
  a *= scalar;
  b *= scalar;
  return *this;
}
