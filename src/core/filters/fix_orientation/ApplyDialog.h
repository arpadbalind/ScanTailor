// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <QDialogButtonBox>
#include <QButtonGroup>
#include <QDialog>

#include <memory>
#include <set>
#include <vector>

#include "PageId.h"
#include "PageRange.h"
#include "PageSequence.h"
#include "ui_ApplyDialog.h"

class PageSelectionAccessor;

namespace fix_orientation {
class ApplyDialog : public QDialog {
  Q_OBJECT
 public:
  ApplyDialog(QWidget* parent, PageId curPage, const PageSelectionAccessor& pageSelectionAccessor);

  ~ApplyDialog() override = default;

 signals:

  void appliedTo(const std::set<PageId>& pages);

  void appliedToAllPages(const std::set<PageId>& pages);

 private slots:

  void onSubmit();

 private:
  Ui::ApplyDialog ui{};
  PageSequence m_pages;
  std::set<PageId> m_selectedPages;
  std::vector<PageRange> m_selectedRanges;
  PageId m_curPage;
  QButtonGroup* m_btnGroup;
};
}  // namespace fix_orientation
