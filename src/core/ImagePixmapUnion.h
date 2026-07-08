// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <QImage>
#include <QPixmap>

class ImagePixmapUnion {
  // Member-wise copying is OK.
 public:
  ImagePixmapUnion() = default;

  explicit ImagePixmapUnion(QImage image) : m_image(std::move(image)) {}

  explicit ImagePixmapUnion(QPixmap pixmap) : m_pixmap(std::move(pixmap)) {}

  [[nodiscard]] const QImage& image() const { return m_image; }

  [[nodiscard]] const QPixmap& pixmap() const { return m_pixmap; }

  [[nodiscard]] bool isNull() const { return m_image.isNull() && m_pixmap.isNull(); }

 private:
  QImage m_image;
  QPixmap m_pixmap;
};
