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
    static constexpr std::size_t HASH_SEED  = 5381;
    static constexpr std::size_t HASH_SHIFT = 5;
    static constexpr std::size_t CHAR_SHIFT = 8;

    const QChar* data = str.constData();
    std::size_t hashNumber = HASH_SEED;

    for (int i = 0; i < str.size(); ++i) {
      const std::size_t value =
          (static_cast<std::size_t>(data[i].row()) << CHAR_SHIFT) |
          static_cast<std::size_t>(data[i].cell());

      hashNumber = ((hashNumber << HASH_SHIFT) + hashNumber) ^ value;
    }

    return hashNumber;
  }
};

} // namespace hashes
