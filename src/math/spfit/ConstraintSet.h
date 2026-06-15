// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <cstddef>
#include <list>

class LinearFunction;
class QLineF;
class QPointF;

namespace spfit {
class FittableSpline;

class ConstraintSet {
  // Member-wise copying is OK.
 public:
  explicit ConstraintSet(const FittableSpline* spline);

  [[nodiscard]] const std::list<LinearFunction>& constraints() const { return m_constraints; }

  void constrainControlPoint(int cpIdx, const QPointF& pos);

  void constrainControlPoint(int cpIdx, const QLineF& line);

  void constrainSplinePoint(double t, const QPointF& pos);

  void constrainSplinePoint(double t, const QLineF& line);

 private:
  const FittableSpline* m_spline;
  std::list<LinearFunction> m_constraints;
};
}  // namespace spfit
