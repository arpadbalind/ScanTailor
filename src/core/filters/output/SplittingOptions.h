// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once
#include <QMetaType>
#include <QtXml/QDomElement>

#include <cstdint>

namespace output {
enum class SplittingMode : std::uint8_t {
  BLACK_AND_WHITE_FOREGROUND,
  COLOR_FOREGROUND
};

class SplittingOptions {
 public:
  SplittingOptions() = default;

  explicit SplittingOptions(const QDomElement& el);

  QDomElement toXml(QDomDocument& doc, const QString& name) const;

  [[nodiscard]] bool isSplitOutput() const;

  void setSplitOutput(bool splitOutput);

  [[nodiscard]] SplittingMode getSplittingMode() const;

  void setSplittingMode(SplittingMode foregroundType);

  bool operator==(const SplittingOptions& other) const;

  bool operator!=(const SplittingOptions& other) const;

  [[nodiscard]] bool isOriginalBackgroundEnabled() const;

  void setOriginalBackgroundEnabled(bool enable);

 private:
  static SplittingMode parseSplittingMode(const QString& str);

  static QString formatSplittingMode(SplittingMode type);

  bool m_isSplitOutput{ false };
  SplittingMode m_splittingMode { SplittingMode::BLACK_AND_WHITE_FOREGROUND };
  bool m_isOriginalBackgroundEnabled { false };
};


inline bool SplittingOptions::isSplitOutput() const {
  return m_isSplitOutput;
}

inline void SplittingOptions::setSplitOutput(bool splitOutput) {
  SplittingOptions::m_isSplitOutput = splitOutput;
}

inline SplittingMode SplittingOptions::getSplittingMode() const {
  return m_splittingMode;
}

inline void SplittingOptions::setSplittingMode(SplittingMode foregroundType) {
  SplittingOptions::m_splittingMode = foregroundType;
}

inline bool SplittingOptions::isOriginalBackgroundEnabled() const {
  return m_isOriginalBackgroundEnabled;
}

inline void SplittingOptions::setOriginalBackgroundEnabled(bool enable) {
  SplittingOptions::m_isOriginalBackgroundEnabled = enable;
}
}  // namespace output
Q_DECLARE_METATYPE(output::SplittingMode);
