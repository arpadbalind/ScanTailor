// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "DespeckleLevel.h"

#include <QString>

namespace output {
QString despeckleLevelToString(const DespeckleLevel level) {
  switch (level) {
    case DespeckleLevel::OFF:
      return "off";
    case DespeckleLevel::CAUTIOUS:
      return "cautious";
    case DespeckleLevel::NORMAL:
      return "normal";
    case DespeckleLevel::AGGRESSIVE:
      return "aggressive";
  }
  return QString();
}

DespeckleLevel despeckleLevelFromString(const QString& str) {
  if (str == "off") {
    return DespeckleLevel::OFF;
  } else if (str == "cautious") {
    return DespeckleLevel::CAUTIOUS;
  } else if (str == "aggressive") {
    return DespeckleLevel::AGGRESSIVE;
  } else {
    return DespeckleLevel::NORMAL;
  }
}
}  // namespace output
