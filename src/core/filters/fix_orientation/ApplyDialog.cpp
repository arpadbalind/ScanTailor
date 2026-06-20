// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "ApplyDialog.h"
#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QDialog>
#include <QWidget>

#include <cassert>
#include <set>
#include <utility>

#include "PageId.h"
#include "PageRange.h"
#include "PageSelectionAccessor.h"

namespace fix_orientation {
ApplyDialog::ApplyDialog(QWidget* parent, PageId curPage, const PageSelectionAccessor& pageSelectionAccessor)
    : QDialog(parent),
      m_pages(pageSelectionAccessor.allPages()),
      m_selectedPages(pageSelectionAccessor.selectedPages()),
      m_selectedRanges(pageSelectionAccessor.selectedRanges()),
      m_curPage(std::move(curPage)),
      m_btnGroup(new QButtonGroup(this)) {
  ui.setupUi(this);
  m_btnGroup->addButton(ui.thisPageOnlyRB);
  m_btnGroup->addButton(ui.allPagesRB);
  m_btnGroup->addButton(ui.thisPageAndFollowersRB);
  m_btnGroup->addButton(ui.selectedPagesRB);
  m_btnGroup->addButton(ui.everyOtherRB);
  m_btnGroup->addButton(ui.thisEveryOtherRB);
  m_btnGroup->addButton(ui.everyOtherSelectedRB);
  if (m_selectedPages.size() <= 1) {
    ui.selectedPagesRB->setEnabled(false);
    ui.selectedPagesHint->setEnabled(false);
    ui.everyOtherSelectedRB->setEnabled(false);
    ui.everyOtherSelectedHint->setEnabled(false);
  }

  connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &ApplyDialog::onSubmit);
}

void ApplyDialog::onSubmit() {
  std::set<PageId> pages;

  // thisPageOnlyRB is intentionally not handled.
  if (ui.allPagesRB->isChecked()) {
    m_pages.selectAll().swap(pages);
    appliedToAllPages(pages);
    accept();
    return;
  } else if (ui.thisPageAndFollowersRB->isChecked()) {
    m_pages.selectPagePlusFollowers(m_curPage).swap(pages);
  } else if (ui.selectedPagesRB->isChecked()) {
    appliedTo(m_selectedPages);
    accept();
    return;
  } else if (ui.everyOtherRB->isChecked()) {
    m_pages.selectEveryOther(m_curPage).swap(pages);
  } else if (ui.thisEveryOtherRB->isChecked()) {
    std::set<PageId> tmp;
    m_pages.selectPagePlusFollowers(m_curPage).swap(tmp);
    auto it = tmp.begin();
    for (int i = 0; it != tmp.end(); ++it, ++i) {
      if (i % 2 == 0) {
        pages.insert(*it);
      }
    }
  } else if (ui.everyOtherSelectedRB->isChecked()) {
    assert(m_selectedRanges.size() == 1);
    const PageRange& range = m_selectedRanges.front();
    range.selectEveryOther(m_curPage).swap(pages);
  }

  appliedTo(pages);
  accept();
}  // ApplyDialog::onSubmit
}  // namespace fix_orientation
