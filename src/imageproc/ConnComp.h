// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <QRect>

namespace imageproc {
/**
 * \brief Represents a connected group of pixels.
 */
class ConnComp {
 public:
  ConnComp() : m_pixCount(0) {}

  ConnComp(const QPoint& seed, const QRect& rect, int pixCount) : m_seed(seed), m_rect(rect), m_pixCount(pixCount) {}

  [[nodiscard]] bool isNull() const { return m_rect.isNull(); }

  /**
   * \brief Get an arbitrary black pixel position.
   *
   * The position is in containing image coordinates,
   * not in the bounding box coordinates.
   */
  [[nodiscard]] const QPoint& seed() const { return m_seed; }

  [[nodiscard]] int width() const { return m_rect.width(); }

  [[nodiscard]] int height() const { return m_rect.height(); }

  [[nodiscard]] const QRect& rect() const { return m_rect; }

  [[nodiscard]] int pixCount() const { return m_pixCount; }

 private:
  QPoint m_seed;
  QRect m_rect;
  int m_pixCount;
};
}  // namespace imageproc
