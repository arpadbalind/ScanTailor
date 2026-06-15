// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once
#include <cstddef>
#include <QChar>
#include <QString>

namespace hashes {
template <typename>
struct hash;

template <>
struct hash<QString> {
  std::size_t operator()(const QString& str) const noexcept {
    const QChar* data = str.constData();
    std::size_t hashNumber = 5381;
    for (int i = 0; i < str.size(); ++i) {
      hashNumber = ((hashNumber << 5) + hashNumber) ^ ((data[i].row() << 8) | data[i].cell());
    }
    return hashNumber;
  }
};
}  // namespace hashes
