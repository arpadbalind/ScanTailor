// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <set>
#include <vector>

#include "PageId.h"

class PageRange {
 public:
  [[nodiscard]] std::set<PageId> selectEveryOther(const PageId& base) const;
  std::vector<PageId>& pages(){ return m_pages; }
  [[nodiscard]] const std::vector<PageId>& pages() const { return m_pages; }
 private:
  /**
   * \brief Ordered list of consecutive pages.
   */
  std::vector<PageId> m_pages;
};
