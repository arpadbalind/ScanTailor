// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "PageRange.h"
#include <algorithm>

std::set<PageId> PageRange::selectEveryOther(const PageId& base) const {
  std::set<PageId> selection;
  auto& pages = this->pages();
  const auto baseIt = std::ranges::find(pages, base);
  if (baseIt == pages.end()) {
    return selection;
  }

  const auto parity = std::distance(pages.begin(), baseIt) % 2;

  for (std::ptrdiff_t idx = 0; idx < std::ssize(pages); ++idx) {
    if (idx % 2 == parity) {
      selection.insert(pages[static_cast<std::size_t>(idx)]);
    }
  }

  return selection;
}
