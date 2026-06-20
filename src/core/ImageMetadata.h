// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <QSize>
#include <cstdint>

#include "Dpi.h"

class ImageMetadata {
  // Member-wise copying is OK.
 public:
  enum class Status : std::uint8_t {
    DPI_OK,
    DPI_UNDEFINED,
    DPI_TOO_LARGE,
    DPI_TOO_SMALL,
    DPI_TOO_SMALL_FOR_THIS_PIXEL_SIZE
  };

  ImageMetadata() = default;

  ImageMetadata(QSize size, Dpi dpi) : m_size(size), m_dpi(dpi) {}

  [[nodiscard]] const QSize& size() const { return m_size; }

  void setSize(const QSize& size) { m_size = size; }

  [[nodiscard]] const Dpi& dpi() const { return m_dpi; }

  void setDpi(const Dpi& dpi) { m_dpi = dpi; }

  [[nodiscard]] bool isDpiOK() const;

  [[nodiscard]] Status horizontalDpiStatus() const;

  [[nodiscard]] Status verticalDpiStatus() const;

  bool operator==(const ImageMetadata& other) const;

  bool operator!=(const ImageMetadata& other) const { return !(*this == other); }

 private:
  static Status dpiStatus(int pixelSize, int dpi);

  QSize m_size;
  Dpi m_dpi;
};
