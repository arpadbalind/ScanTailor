// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <QColor>
#include <QSizeF>
#include <cstdint>

class QImage;
class QRect;
class QTransform;

namespace imageproc {
class GrayImage;

class OutsidePixels {
 public:
  enum class Flags : std::uint8_t {
    COLOR = 0x01,
    NEAREST = 0x02,
    WEAK = 0x04,

    COLOR_NEAREST = 0x03,
    COLOR_WEAK = 0x05,
    NEAREST_WEAK = 0x06,
    MASK_ALL = 0x07
  };
  constexpr OutsidePixels(Flags flags, QRgb rgba)
      : m_flags(flags), m_rgba(rgba) {}
  /**
   * \brief Outside pixels are assumed to be of particular color.
   *
   * Outside pixels may be blended with inside pixels near the edges.
   */
  static OutsidePixels assumeColor(const QColor& color) { return {Flags::COLOR, color.rgba()}; }

  /**
   * \brief Outside pixels are assumed to be of particular color.
   *
   * Outside pixels won't participate in blending operations.
   */
  static OutsidePixels assumeWeakColor(const QColor& color) { return {Flags::COLOR_WEAK, color.rgba()}; }

  /**
   * \brief An outside pixel is assumed to be the same as the nearest inside pixel.
   *
   * Outside pixels won't participate in blending operations.
   */
  static OutsidePixels assumeWeakNearest() { return {Flags::NEAREST_WEAK, 0xff000000}; }

  [[nodiscard]] Flags flags() const { return m_flags; }

  [[nodiscard]] QRgb rgba() const { return m_rgba; }

  [[nodiscard]] QRgb rgb() const { return m_rgba | 0xff000000; }

  [[nodiscard]] uint8_t grayLevel() const { return static_cast<uint8_t>(qGray(m_rgba)); }

 private:
  Flags m_flags;
  QRgb m_rgba;
};


/**
 * \brief Apply an affine transformation to the image.
 *
 * \param src The source image.
 * \param xform The transformation from source to destination.
 *        Only affine transformations are supported.
 * \param dstRect The area in source image coordinates to return
 *        as a destination image.
 * \param background_color Used to fill areas not represented in the source image.
 * \param weak_background If set to true, \p background_color is only taken
 *        into account if a target pixel maps to an area completely outside of
 *        the source image.  That is, if at least one source image pixel
 *        influences a particular target pixel, then any background pixels
 *        that may also influence that target pixel are ignored.\n
 *        If set to false, source image pixels and background pixels are
 *        treated equally.
 * \param minMappingArea Defines the minimum rectangle in the source image
 *        that maps to a destination pixel.  This can be used to control
 *        smoothing.
 * \return The transformed image.  It's format may differ from the
 *         source image format, for example Format_Indexed8 may
 *         be transformed to Format_RGB32, if the source image
 *         contains colors other than shades of gray.
 */
QImage transform(const QImage& src,
                 const QTransform& xform,
                 const QRect& dstRect,
                 OutsidePixels outsidePixels,
                 const QSizeF& minMappingArea = QSizeF(0.9, 0.9));

/**
 * \brief Apply an affine transformation to the image.
 *
 * Same as transform(), except the source image image is converted
 * to grayscale before transforming it.
 */
GrayImage transformToGray(const QImage& src,
                          const QTransform& xform,
                          const QRect& dstRect,
                          OutsidePixels outsidePixels,
                          const QSizeF& minMappingArea = QSizeF(0.9, 0.9));
}  // namespace imageproc
