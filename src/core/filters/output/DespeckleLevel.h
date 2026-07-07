// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once
#include <QMetaType>
#include <cstdint>

class QString;

namespace output {
enum class DespeckleLevel : std::uint8_t {
  OFF,
  CAUTIOUS,
  NORMAL,
  AGGRESSIVE
};

QString despeckleLevelToString(DespeckleLevel level);

DespeckleLevel despeckleLevelFromString(const QString& str);
}  // namespace output
Q_DECLARE_METATYPE(output::DespeckleLevel);
