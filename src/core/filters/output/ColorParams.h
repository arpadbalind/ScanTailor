// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <cstdint>

#include "BlackWhiteOptions.h"
#include "ColorCommonOptions.h"
#include "SplittingOptions.h"

class QDomDocument;
class QDomElement;

namespace output {
enum class ColorMode : std::uint8_t {
  BLACK_AND_WHITE,
  COLOR_GRAYSCALE,
  MIXED
};

class ColorParams {
 public:
  ColorParams() = default;

  explicit ColorParams(const QDomElement& el);

  QDomElement toXml(QDomDocument& doc, const QString& name) const;

  [[nodiscard]] ColorMode colorMode() const;

  void setColorMode(ColorMode mode);

  [[nodiscard]] const ColorCommonOptions& colorCommonOptions() const;

  void setColorCommonOptions(const ColorCommonOptions& opt);

  [[nodiscard]] const BlackWhiteOptions& blackWhiteOptions() const;

  void setBlackWhiteOptions(const BlackWhiteOptions& opt);

 private:
  static ColorMode parseColorMode(const QString& str);

  static QString formatColorMode(ColorMode mode);

  ColorMode m_colorMode = ColorMode::BLACK_AND_WHITE;
  ColorCommonOptions m_colorCommonOptions;
  BlackWhiteOptions m_bwOptions;
};


inline ColorMode ColorParams::colorMode() const {
  return m_colorMode;
}

inline void ColorParams::setColorMode(ColorMode mode) {
  m_colorMode = mode;
}

inline const ColorCommonOptions& ColorParams::colorCommonOptions() const {
  return m_colorCommonOptions;
}

inline void ColorParams::setColorCommonOptions(const ColorCommonOptions& opt) {
  m_colorCommonOptions = opt;
}

inline const BlackWhiteOptions& ColorParams::blackWhiteOptions() const {
  return m_bwOptions;
}

inline void ColorParams::setBlackWhiteOptions(const BlackWhiteOptions& opt) {
  m_bwOptions = opt;
}
}  // namespace output
Q_DECLARE_METATYPE(output::ColorMode);
