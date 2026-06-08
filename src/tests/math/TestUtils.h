#pragma once
#include <gtest/gtest.h>
#include <cmath>
inline static constexpr double EPSILON{ 1e-6 };

inline static double frand(double from, double to) {
  const double rand01 = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
  return from + (to - from) * rand01;
}

inline void ExpectClose(double val, double expected, double tol) {
  if (expected == 0.0) {
    EXPECT_NEAR(val, 0.0, tol);
  } else {
    const double abs_error = std::abs(expected) * tol;
    EXPECT_NEAR(val, expected, abs_error);
  }
}   
