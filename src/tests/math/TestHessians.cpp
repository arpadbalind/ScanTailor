// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "TestUtils.h"
#include <gtest/gtest.h>

#include "adiff/Function.h"
#include "adiff/SparseMap.h"
#include "MatT.h"
#include "VecT.h"

namespace adiff::tests {

TEST(AutomaticDifferentiationTestSuite, test1) {
  // F(x) = x^2  | x = 3

  SparseMap<2> sparseMap(1);
  sparseMap.markAllNonZero();

  const Function<2> x(0, 3, sparseMap);
  const Function<2> res(x * x);

  const VecT<double> gradient(res.gradient(sparseMap));
  const MatT<double> hessian(res.hessian(sparseMap));

  // F = 9
  // Fx = 2 * x = 6
  // Fxx = 2

  ExpectClose(res.value, 9, 1e-06);
  ExpectClose(gradient[0], 6, 1e-06);
  ExpectClose(hessian(0, 0), 2, 1e-06);
}

TEST(AutomaticDifferentiationTestSuite, test2) {
  // F(x, y) = x^2  | x = 3

  SparseMap<2> sparseMap(2);
  sparseMap.markAllNonZero();

  const Function<2> x(0, 3, sparseMap);
  const Function<2> res(x * x);

  const VecT<double> gradient(res.gradient(sparseMap));
  const MatT<double> hessian(res.hessian(sparseMap));

  // F = 9
  // Fx = 2 * x = 6
  // Fy = 0
  // Fxx = 2
  // Fyy = 0
  // Fxy = 0

  ExpectClose(res.value, 9, 1e-06);
  ExpectClose(gradient[0], 6, 1e-06);
  ExpectClose(gradient[1], 0, 1e-06);
  ExpectClose(hessian(0, 0), 2, 1e-06);
  ExpectClose(hessian(1, 0), 0, 1e-06);
  ExpectClose(hessian(0, 1), 0, 1e-06);
  ExpectClose(hessian(1, 1), 0, 1e-06);
}

TEST(AutomaticDifferentiationTestSuite, test3) {
  // F(x, y) = x^3 * y^2   | x = 2, y = 3

  SparseMap<2> sparseMap(2);
  sparseMap.markAllNonZero();

  const Function<2> x(0, 2, sparseMap);
  const Function<2> y(1, 3, sparseMap);
  const Function<2> xxx(x * x * x);
  const Function<2> yy(y * y);
  const Function<2> res(xxx * yy);

  const VecT<double> gradient(res.gradient(sparseMap));
  const MatT<double> hessian(res.hessian(sparseMap));

  // F = 72
  // Fx = 3 * x^2 * y^2 = 108
  // Fy = 2 * y * x^3 = 48
  // Fxx = 6 * x * y^2 = 108
  // Fyy = 2 * x^3 = 16
  // Fyx = 6 * y * x^2 = 72

  ExpectClose(res.value, 72, 1e-06);
  ExpectClose(gradient[0], 108, 1e-06);
  ExpectClose(gradient[1], 48, 1e-06);
  ExpectClose(hessian(0, 0), 108, 1e-06);
  ExpectClose(hessian(0, 1), 72, 1e-06);
  ExpectClose(hessian(1, 0), 72, 1e-06);
  ExpectClose(hessian(1, 1), 16, 1e-06);
}
}
