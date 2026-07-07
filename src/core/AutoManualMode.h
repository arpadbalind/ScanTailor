// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once
#include <QMetaType>
#include <QString>
#include <cstdint>

enum class AutoManualMode : std::uint8_t {
  MODE_AUTO,
  MODE_MANUAL,
  MODE_DISABLED
  };
Q_DECLARE_METATYPE(AutoManualMode);

QString autoManualModeToString(AutoManualMode mode);

AutoManualMode stringToAutoManualMode(const QString& str);
