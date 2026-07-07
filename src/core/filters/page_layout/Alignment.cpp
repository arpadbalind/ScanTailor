// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "Alignment.h"

#include <QDomDocument>

namespace page_layout {
Alignment::Alignment(Vertical vertical, Horizontal horizontal)
    : m_vertical(vertical), m_horizontal(horizontal), m_isNull(false) {}

Alignment::Alignment(const QDomElement& el) {
  const QString vert = el.attribute("vert");
  const QString hor = el.attribute("hor");
  m_isNull = (el.attribute("null").toInt() != 0);

  if (vert == "top") {
    m_vertical = Vertical::TOP;
  } else if (vert == "bottom") {
    m_vertical = Vertical::BOTTOM;
  } else if (vert == "auto") {
    m_vertical = Vertical::VAUTO;
  } else if (vert == "original") {
    m_vertical = Vertical::VORIGINAL;
  } else {
    m_vertical = Vertical::VCENTER;
  }

  if (hor == "left") {
    m_horizontal = Horizontal::LEFT;
  } else if (hor == "right") {
    m_horizontal = Horizontal::RIGHT;
  } else if (hor == "auto") {
    m_horizontal = Horizontal::HAUTO;
  } else if (hor == "original") {
    m_horizontal = Horizontal::HORIGINAL;
  } else {
    m_horizontal = Horizontal::HCENTER;
  }
}

QDomElement Alignment::toXml(QDomDocument& doc, const QString& name) const {
  const char* vert = nullptr;
  switch (m_vertical) {
    case Vertical::TOP:
      vert = "top";
      break;
    case Vertical::VCENTER:
      vert = "center";
      break;
    case Vertical::BOTTOM:
      vert = "bottom";
      break;
    case Vertical::VAUTO:
      vert = "auto";
      break;
    case Vertical::VORIGINAL:
      vert = "original";
      break;
  }

  const char* hor = nullptr;
  switch (m_horizontal) {
    case Horizontal::LEFT:
      hor = "left";
      break;
    case Horizontal::HCENTER:
      hor = "center";
      break;
    case Horizontal::RIGHT:
      hor = "right";
      break;
    case Horizontal::HAUTO:
      hor = "auto";
      break;
    case Horizontal::HORIGINAL:
      hor = "original";
      break;
  }

  QDomElement el(doc.createElement(name));
  el.setAttribute("vert", QString::fromLatin1(vert));
  el.setAttribute("hor", QString::fromLatin1(hor));
  el.setAttribute("null", m_isNull ? 1 : 0);
  return el;
}

bool Alignment::operator==(const Alignment& other) const {
  return (m_vertical == other.m_vertical) && (m_horizontal == other.m_horizontal) && (m_isNull == other.m_isNull);
}

bool Alignment::operator!=(const Alignment& other) const {
  return !(*this == other);
}
}  // namespace page_layout
