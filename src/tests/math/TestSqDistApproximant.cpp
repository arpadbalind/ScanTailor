// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "TestUtils.h"
#include <ToLineProjector.h>
#include <spfit/SqDistApproximant.h>
#include <QLineF>
#include <QPointF>
#include <gtest/gtest.h>
#include <cmath>
#include <cstdlib>

namespace spfit {
namespace tests {

static const double PI = 3.14159265;

TEST(SqDistApproximantTestSuite, test_point_distance) {
  for (int i = 0; i < 100; ++i) {
    const Vec2d origin(frand(-50, 50), frand(-50, 50));
    const SqDistApproximant approx(SqDistApproximant::pointDistance(origin));
    for (int j = 0; j < 10; ++j) {
      const Vec2d pt(frand(-50, 50), frand(-50, 50));
      const double control = (pt - origin).squaredNorm();
      ExpectClose(approx.evaluate(pt), control, 1e-06);
    }
  }
}

TEST(SqDistApproximantTestSuite, test_line_distance) {
  for (int i = 0; i < 100; ++i) {
    const Vec2d pt1(frand(-50, 50), frand(-50, 50));
    const double angle = frand(0, 2.0 * PI);
    const Vec2d delta(std::cos(angle), std::sin(angle));
    const QLineF line(pt1, pt1 + delta);
    const SqDistApproximant approx(SqDistApproximant::lineDistance(line));
    const ToLineProjector proj(line);
    for (int j = 0; j < 10; ++j) {
      const Vec2d pt(frand(-50, 50), frand(-50, 50));
      const double control = proj.projectionSqDist(pt);
      ExpectClose(approx.evaluate(pt), control, 1e-06);
    }
  }
}

TEST(SqDistApproximantTestSuite, test_general_case) {
  for (int i = 0; i < 100; ++i) {
    const Vec2d origin(frand(-50, 50), frand(-50, 50));
    const double angle = frand(0, 2.0 * PI);
    const Vec2d u(std::cos(angle), std::sin(angle));
    Vec2d v(-u[1], u[0]);
    if (rand() & 1) {
      v = -v;
    }
    const double m = frand(0, 3);
    const double n = frand(0, 3);

    const SqDistApproximant approx(origin, u, v, m, n);

    for (int j = 0; j < 10; ++j) {
      const Vec2d pt(frand(-50, 50), frand(-50, 50));
      const double uProj = u.dot(pt - origin);
      const double vProj = v.dot(pt - origin);
      const double control = m * uProj * uProj + n * vProj * vProj;
      ExpectClose(approx.evaluate(pt), control, 1e-06);
    }
  }
}

}  // namespace tests
}  // namespace spfit
