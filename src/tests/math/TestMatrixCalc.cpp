// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include <array>
#include <gtest/gtest.h>
#include <cmath>
#include <stdexcept>

#include "MatrixCalc.h"
#include "TestUtils.h"

namespace imageproc {
namespace tests {

TEST(MatrixCalcSuite, test1) {
  constexpr std::array<double, 9> A{1, 1, 1, 2, 4, -3, 3, 6, -5};
  constexpr std::array<double, 3> B{9, 1, 0};
  constexpr std::array<double, 3> control{7, -1, 3};

  std::array<double, 3> x{};

  MatrixCalc<double> mc;
  mc(A.data(), 3, 3).trans().solve(mc(B.data(), 3, 1)).write(x.data());

  for (int i = 0; i < 3; ++i) {
    ExpectClose(x[i], control[i], EPSILON);
  }
}

TEST(MatrixCalcSuite, test2) {
  constexpr std::array<double, 15> A{1, 1, 1, 2, 4, -3, 3, 6, -5, 3, 5, -2, 5, 10, -8};
  std::array<double, 5> B{9, 1, 0, 10, 1};
  constexpr std::array<double, 3> control{7, -1, 3};

  std::array<double, 3> x{};

  MatrixCalc<double> mc;
  mc(A.data(), 3, 5).trans().solve(mc(B.data(), 5, 1)).write(x.data());

  for (int i = 0; i < 3; ++i) {
    ExpectClose(x[i], control[i], EPSILON);
  }

  // Now make the system inconsistent.
  B[4] += 1.0;
  EXPECT_THROW(mc(A.data(), 3, 5).trans().solve(mc(B.data(), 5, 1)), std::runtime_error);
}

TEST(MatrixCalcSuite, test3) {
  constexpr std::array<double, 9> A{1, 3, 1, 1, 1, 2, 2, 3, 4};
  constexpr std::array<double, 9> control{2, 9, -5, 0, -2, 1, -1, -3, 2};

  std::array<double, 9> inv{};

  MatrixCalc<double> mc;
  mc(A.data(), 3, 3).trans().inv().transWrite(inv.data());

  for (int i = 0; i < 9; ++i) {
    ExpectClose(inv[i], control[i], EPSILON);
  }
}

TEST(MatrixCalcSuite, test4) {
  constexpr std::array<double, 12> A{4, 1, 9, 6, 2, 8, 7, 3, 5, 11, 10, 12};
  constexpr std::array<double, 6> B{2, 9, 5, 12, 8, 10};
  constexpr std::array<double, 8> control{85, 138, 86, 158, 69, 149, 168, 339};

  std::array<double, 8> mul{};

  MatrixCalc<double> mc;
  (mc(A.data(), 3, 4).trans() * (mc(B.data(), 2, 3).trans())).transWrite(mul.data());

  for (int i = 0; i < 8; ++i) {
    ExpectClose(mul[i], control[i], EPSILON);
  }
}

}  // namespace tests
}  // namespace imageproc
