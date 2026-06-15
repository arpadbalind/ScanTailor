// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include "VecNT.h"

class QLineF;
class QPointF;
/**
 * \brief Projects points onto a line (not a line segment).
 *
 * Projecting means finding the point on a line that is closest
 * to the given point.
 */
class ToLineProjector {
 public:
  /**
   * \brief Initializes line projector.
   *
   * If line endpoints match, all points will
   * be projected to that point.
   */
  explicit ToLineProjector(const QLineF& line);

  /**
   * \brief Finds the projection point.
   */
  [[nodiscard]] QPointF projectionPoint(const QPointF& pt) const;

  /**
   * \brief Equivalent to projectionPoint(pt) - pt.
   */
  [[nodiscard]] QPointF projectionVector(const QPointF& pt) const;

  /**
   * Solves the equation of:\n
   * line.p1() + x * (line.p2() - line.p1()) = p\n
   * for x, where p would be the projection point.
   * This function is faster than projectionPoint().
   */
  [[nodiscard]] double projectionScalar(const QPointF& pt) const;

  /**
   * Returns the distance from \p pt to the projection point.
   */
  [[nodiscard]] double projectionDist(const QPointF& pt) const;

  /**
   * Returns the squared distance from \p pt to the projection point.
   * This function is faster than projectionDist().
   */
  [[nodiscard]] double projectionSqDist(const QPointF& pt) const;

 private:
  QPointF m_origin;
  QPointF m_vec;
  Vec2d m_mat;
};
