// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "Units.h"

#include <QObject>

QString unitsToString(Units units) {
  QString unitsStr;
  switch (units) {
    case Units::PIXELS:
      unitsStr = "px";
      break;
    case Units::MILLIMETRES:
      unitsStr = "mm";
      break;
    case Units::CENTIMETRES:
      unitsStr = "cm";
      break;
    case Units::INCHES:
      unitsStr = "in";
      break;
  }
  return unitsStr;
}

Units unitsFromString(const QString& string) {
  if (string == "px") {
    return Units::PIXELS;
  } else if (string == "cm") {
    return Units::CENTIMETRES;
  } else if (string == "in") {
    return Units::INCHES;
  } else {
    return Units::MILLIMETRES;
  }
}

QString unitsToLocalizedString(Units units) {
  QString unitsStr;
  switch (units) {
    case Units::PIXELS:
      unitsStr = QObject::tr("px");
      break;
    case Units::MILLIMETRES:
      unitsStr = QObject::tr("mm");
      break;
    case Units::CENTIMETRES:
      unitsStr = QObject::tr("cm");
      break;
    case Units::INCHES:
      unitsStr = QObject::tr("in");
      break;
  }
  return unitsStr;
}
