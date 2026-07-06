// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include "PageId.h"
#include "PageView.h"

/**
 * The whole point of this class can be demonstrated with a few lines of code:
 * \code
 * ImageId image_id = ...;
 * SelectedPage page;
 * page.set(PageId(image_id, PageId::SubPage::RIGHT_PAGE), PageView::PAGE_VIEW);
 * page.set(PageId(image_id, PageId::SubPage::SINGLE_PAGE), PageView::IMAGE_VIEW);
 * page.get(PageView::PAGE_VIEW);  * \endcode
 * As seen above, this class remembers the sub-page as long as image id
 * stays the same.  Note that set(..., PageView::PAGE_VIEW) will always overwrite
 * the sub-page, while get(PageView::IMAGE_VIEW) will always return SINGLE_PAGE sub-pages.
 */
class SelectedPage {
 public:
  SelectedPage() = default;

  SelectedPage(const PageId& pageId, PageView view);

  [[nodiscard]] bool isNull() const { return m_pageId.isNull(); }

  void set(const PageId& pageId, PageView view);

  [[nodiscard]] PageId get(PageView view) const;

 private:
  PageId m_pageId;
};
