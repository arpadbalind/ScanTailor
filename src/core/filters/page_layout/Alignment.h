// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once
#include <QMetaType>

#include <cstdint>
#include <iostream>

class QDomDocument;
class QDomElement;
class QString;
class CommandLine;

namespace page_layout {
class Alignment {
 public:
  enum class Vertical : std::uint8_t {
    TOP,
    VCENTER,
    BOTTOM,
    VAUTO,
    VORIGINAL
  };
  enum class Horizontal : std::uint8_t {
    LEFT,
    HCENTER,
    RIGHT,
    HAUTO,
    HORIGINAL
  };

  /**
   * \brief Constructs a null alignment.
   */
  Alignment() = default;

  Alignment(Vertical vertical, Horizontal horizontal);

  explicit Alignment(const QDomElement& el);

  [[nodiscard]] Vertical vertical() const;

  void setVertical(Vertical vertical);

  [[nodiscard]] Horizontal horizontal() const;

  void setHorizontal(Horizontal horizontal);

  [[nodiscard]] bool isNull() const;

  void setNull(bool isNull);

  [[nodiscard]] bool isAutoVertical() const;

  [[nodiscard]] bool isAutoHorizontal() const;

  bool operator==(const Alignment& other) const;

  bool operator!=(const Alignment& other) const;

  QDomElement toXml(QDomDocument& doc, const QString& name) const;

 private:
  Vertical m_vertical{ Vertical::VCENTER };
  Horizontal m_horizontal{ Horizontal::HCENTER };
  bool m_isNull{ false };
};


inline Alignment::Vertical Alignment::vertical() const {
  return m_vertical;
}

inline void Alignment::setVertical(Alignment::Vertical vertical) {
  m_vertical = vertical;
}

inline Alignment::Horizontal Alignment::horizontal() const {
  return m_horizontal;
}

inline void Alignment::setHorizontal(Alignment::Horizontal horizontal) {
  m_horizontal = horizontal;
}

inline bool Alignment::isNull() const {
  return m_isNull;
}

inline void Alignment::setNull(bool isNull) {
  m_isNull = isNull;
}

inline bool Alignment::isAutoVertical() const {
  return (m_vertical == Vertical::VAUTO) || (m_vertical == Vertical::VORIGINAL);
}

inline bool Alignment::isAutoHorizontal() const {
  return (m_horizontal == Horizontal::HAUTO) || (m_horizontal == Horizontal::HORIGINAL);
}
}  // namespace page_layout
Q_DECLARE_METATYPE(page_layout::Alignment::Vertical);
Q_DECLARE_METATYPE(page_layout::Alignment::Horizontal);
