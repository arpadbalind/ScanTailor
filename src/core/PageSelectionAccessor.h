// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <memory>
#include <set>
#include <vector>

#include "PageId.h"
#include "PageRange.h"
#include "PageSelectionProvider.h"

class PageSequence;

class PageSelectionAccessor {
  // Member-wise copying is OK.
 public:
  explicit PageSelectionAccessor(std::shared_ptr<const PageSelectionProvider> provider);

  [[nodiscard]] PageSequence allPages() const;

  [[nodiscard]] std::set<PageId> selectedPages() const;

  [[nodiscard]] std::vector<PageRange> selectedRanges() const;

 private:
  std::shared_ptr<const PageSelectionProvider> m_provider;
};
