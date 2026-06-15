#pragma once
#include <gtest/gtest.h>
#include <random>

inline static constexpr double EPSILON{ 1e-6 };

inline double frand(double from, double to) {
  thread_local std::mt19937 rng{std::random_device{}()};
  std::uniform_real_distribution<double> dist(from, to);
  return dist(rng);
}

inline void ExpectClose(double val, double expected, double tol) {
  if (expected == 0.0) {
    EXPECT_NEAR(val, 0.0, tol);
  } else {
    const double abs_error = std::abs(expected) * tol;
    EXPECT_NEAR(val, expected, abs_error);
  }
}   
