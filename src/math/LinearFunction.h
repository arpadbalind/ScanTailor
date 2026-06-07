// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <cstddef>

#include "VecT.h"

/**
 * A linear function from arbitrary number of variables
 * expressed in matrix form:
 * \code
 * F(x) = a^T * x + b
 * \endcode
 */
class LinearFunction {
  // Member-wise copying is OK.
 public:
  VecT<double> a;
  double b{ 0.0 };

  /**
   * Constructs a linear function of the given number of variables,
   * initializing everything to zero.
   */
  explicit LinearFunction(size_t numVars = 0);

  /**
   * Resets everything to zero, so that F(x) = 0
   */
  void reset();

  size_t numVars() const { return a.size(); }

  /**
   * Evaluates a^T * x + b
   */
  double evaluate(const double* x) const;

  void swap(LinearFunction& other) noexcept;

  LinearFunction& operator+=(const LinearFunction& other);

  LinearFunction& operator*=(double scalar);
};


inline void swap(LinearFunction& f1, LinearFunction& f2) noexcept {
  f1.swap(f2);
}
