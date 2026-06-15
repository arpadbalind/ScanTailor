// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include <QLineF>
#include <QPointF>

#include <cmath>
#include <gtest/gtest.h>
#include <numbers>
#include <random>

#include "TestUtils.h"
#include "ToLineProjector.h"
#include "spfit/SqDistApproximant.h"
#include "VecNT.h"

namespace spfit::tests {

TEST(SqDistApproximantTestSuite, test_point_distance) {
  std::mt19937 rng(std::random_device{}());

  std::uniform_real_distribution<double> dist(-50.0, 50.0);

  auto frand = [&]() { return dist(rng); };

  for (int i = 0; i < 100; ++i) {
    const Vec2d origin{frand(), frand()};
    const SqDistApproximant approx{SqDistApproximant::pointDistance(origin)};

    for (int j = 0; j < 10; ++j) {
      const Vec2d pt{frand(), frand()};
      const double control = (pt - origin).squaredNorm();

      ExpectClose(approx.evaluate(pt), control, EPSILON);
    }
  }
}

TEST(SqDistApproximantTestSuite, test_line_distance) {
  std::mt19937 rng(std::random_device{}());

  std::uniform_real_distribution<double> dist(-50.0, 50.0);
  std::uniform_real_distribution<double> angleDist(0.0, 2.0 * std::numbers::pi);

  auto frand = [&]() { return dist(rng); };

  for (int i = 0; i < 100; ++i) {
    const Vec2d pt1{frand(), frand()};
    const double angle = angleDist(rng);

    const Vec2d delta{std::cos(angle), std::sin(angle)};
    const QLineF line{QPointF(pt1), QPointF(pt1 + delta)};

    const SqDistApproximant approx{SqDistApproximant::lineDistance(line)};
    const ToLineProjector proj{line};

    for (int j = 0; j < 10; ++j) {
      const Vec2d pt{frand(), frand()};
      const double control = proj.projectionSqDist(QPointF(pt));

      ExpectClose(approx.evaluate(pt), control, EPSILON);
    }
  }
}

TEST(SqDistApproximantTestSuite, test_general_case) {
  std::mt19937 rng(std::random_device{}());

  std::uniform_real_distribution<double> dist(-50.0, 50.0);
  std::uniform_real_distribution<double> angleDist(0.0, 2.0 * std::numbers::pi);
  std::uniform_real_distribution<double> coeffDist(0.0, 3.0);
  std::bernoulli_distribution coin(0.5);

  auto frand = [&]() { return dist(rng); };

  for (int i = 0; i < 100; ++i) {
    const Vec2d origin{frand(), frand()};
    const double angle = angleDist(rng);

    const Vec2d u{std::cos(angle), std::sin(angle)};
    Vec2d v{-u[1], u[0]};

    if (coin(rng)) {
      v = -v;
    }

    const double m = coeffDist(rng);
    const double n = coeffDist(rng);

    const SqDistApproximant approx{origin, u, v, m, n};

    for (int j = 0; j < 10; ++j) {
      const Vec2d pt{frand(), frand()};

      const double uProj = u.dot(pt - origin);
      const double vProj = v.dot(pt - origin);

      const double control = (m * uProj * uProj) + (n * vProj * vProj);

      ExpectClose(approx.evaluate(pt), control, EPSILON);
    }
  }
}
}
