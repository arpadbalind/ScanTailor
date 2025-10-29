// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "Thumbnail.h"

#include <QPainter>
#include <utility>

namespace deskew {
Thumbnail::Thumbnail(std::shared_ptr<ThumbnailPixmapCache> thumbnailCache,
                     const QSizeF& maxSize,
                     const ImageId& imageId,
                     const ImageTransformation& xform,
                     bool deviant)
    : ThumbnailBase(std::move(thumbnailCache), maxSize, imageId, xform), m_deviant(deviant) {}

void Thumbnail::prePaintOverImage(QPainter& painter,
                                  [[maybe_unused]] const QTransform& imageToDisplay,
                                  [[maybe_unused]] const QTransform& thumbToDisplay) {
  painter.setRenderHint(QPainter::Antialiasing, false);

  QPen pen(QColor(0, 0, 0xd1, 70));
  pen.setWidth(1);
  pen.setCosmetic(true);
  painter.setPen(pen);

  const QRectF boundingRect(this->boundingRect());

  const double cellSize = 8;
  const double left = boundingRect.left();
  const double right = boundingRect.right();
  const double top = boundingRect.top();
  const double bottom = boundingRect.bottom();
  const double w = boundingRect.width();
  const double h = boundingRect.height();

  const QPointF center(boundingRect.center());
  QVector<QLineF> lines;

  // Horizontal lines going down from the center
  const int y_down_steps = static_cast<int>(center.y() / cellSize);
  for (int i = 1; i <= y_down_steps; ++i) {
    const double y = center.y() - (i * cellSize);
    lines.push_back(QLineF(left, y, right, y));
  }

  // Horizontal lines going up from the center
  const int y_up_steps = static_cast<int>((h - center.y()) / cellSize);
  for (int i = 1; i <= y_up_steps; ++i) {
    const double y = center.y() + (i * cellSize);
    lines.push_back(QLineF(left, y, right, y));
  }

  // Vertical lines going left from the center
  const int x_left_steps = static_cast<int>(center.x() / cellSize);
  for (int i = 1; i <= x_left_steps; ++i) {
    const double x = center.x() - (i * cellSize);
    lines.push_back(QLineF(x, top, x, bottom));
  }

  // Vertical lines going right from the center
  const int x_right_steps = static_cast<int>((w - center.x()) / cellSize);
  for (int i = 1; i <= x_right_steps; ++i) {
    const double x = center.x() + (i * cellSize);
    lines.push_back(QLineF(x, top, x, bottom));
  }

  painter.drawLines(lines);

  if (m_deviant) {
    paintDeviant(painter);
  }
}  // Thumbnail::paintOverImage
}  // namespace deskew