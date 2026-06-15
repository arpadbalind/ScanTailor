// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once
#include <cstdint>

namespace imageproc {
enum class BWColor : std::uint8_t {
  WHITE = 0,
  BLACK = 1
};

inline BWColor operator!(BWColor color) {
  return color == BWColor::WHITE ? BWColor::BLACK : BWColor::WHITE;
}
}  // namespace imageproc
