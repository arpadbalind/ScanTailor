// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <foundation/NonCopyable.h>

#include <QBrush>
#include <QColor>
#include <memory>

#include "ColorScheme.h"

class ColorSchemeManager : private NonCopyable {
 private:
  ColorSchemeManager() = default;

 public:
  ~ColorSchemeManager() = default;

  static ColorSchemeManager& instance();

  void setColorScheme(const ColorScheme& colorScheme);

  QBrush getColorParam(const QString& colorParam, const QBrush& defaultBrush) const;

  QColor getColorParam(const QString& colorParam, const QColor& defaultColor) const;

 private:
  std::unique_ptr<ColorScheme::ColorParams> m_colorParams;
};
