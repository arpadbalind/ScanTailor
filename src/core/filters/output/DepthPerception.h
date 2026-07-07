// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <QString>

namespace output {
/**
 * \see imageproc::CylindricalSurfaceDewarper
 */
class DepthPerception {
 public:
  DepthPerception();

  explicit DepthPerception(double value);

  explicit DepthPerception(const QString& fromString);

  [[nodiscard]] QString toString() const;

  void setValue(double value);

  [[nodiscard]] double value() const;

  static constexpr double minValue();

  static constexpr double defaultValue();

  static constexpr double maxValue();

 private:
  static constexpr double MINIMAL_DEPTH_VALUE{ 1.0 };
  static constexpr double DEFAULT_DEPTH_VALUE{ 2.0 };
  static constexpr double MAXIMAL_DEPTH_VALUE{ 3.0 };
  double m_value;
};

inline double DepthPerception::value() const {
  return m_value;
}

constexpr double DepthPerception::minValue() {
  return MINIMAL_DEPTH_VALUE;
}

constexpr double DepthPerception::defaultValue() {
  return DEFAULT_DEPTH_VALUE;
}

constexpr double DepthPerception::maxValue() {
  return MAXIMAL_DEPTH_VALUE;
}
}  // namespace output
