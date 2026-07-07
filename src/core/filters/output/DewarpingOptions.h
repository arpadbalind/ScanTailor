// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <QMetaType>
#include <QString>
#include <QtXml/QDomElement>

#include <cstdint>

namespace output {
enum class DewarpingMode : std::uint8_t {
  OFF,
  AUTO,
  MANUAL,
  MARGINAL
};

class DewarpingOptions {
 public:
  explicit DewarpingOptions(DewarpingMode mode = DewarpingMode::OFF, bool needPostDeskew = true);

  explicit DewarpingOptions(const QDomElement& el);

  QDomElement toXml(QDomDocument& doc, const QString& name) const;

  bool operator==(const DewarpingOptions& other) const;

  bool operator!=(const DewarpingOptions& other) const;

  [[nodiscard]] DewarpingMode dewarpingMode() const;

  void setDewarpingMode(DewarpingMode mode);

  [[nodiscard]] bool needPostDeskew() const;

  void setPostDeskew(bool postDeskew);

  [[nodiscard]] double getPostDeskewAngle() const;

  void setPostDeskewAngle(double postDeskewAngle);

  static DewarpingMode parseDewarpingMode(const QString& str);

  static QString formatDewarpingMode(DewarpingMode mode);

 private:
  DewarpingMode m_mode;
  bool m_needPostDeskew;
  double m_postDeskewAngle;
};


inline void DewarpingOptions::setDewarpingMode(DewarpingMode mode) {
  DewarpingOptions::m_mode = mode;
}

inline void DewarpingOptions::setPostDeskew(bool postDeskew) {
  DewarpingOptions::m_needPostDeskew = postDeskew;
}

inline bool DewarpingOptions::needPostDeskew() const {
  return m_needPostDeskew;
}

inline double DewarpingOptions::getPostDeskewAngle() const {
  return m_postDeskewAngle;
}

inline void DewarpingOptions::setPostDeskewAngle(double postDeskewAngle) {
  m_postDeskewAngle = postDeskewAngle;
}

inline DewarpingMode DewarpingOptions::dewarpingMode() const {
  return m_mode;
}
}  // namespace output
Q_DECLARE_METATYPE(output::DewarpingMode);
