// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "OptionsWidget.h"

#include <QIcon>
#include <QPushButton>

#include <cassert>
#include <memory>
#include <set>
#include <utility>

#include "ApplyDialog.h"
#include "core/IconProvider.h"
#include "Filter.h"
#include "PageId.h"
#include "ProjectPages.h"
#include "Settings.h"
#include "ui_OptionsWidget.h"

namespace fix_orientation {
OptionsWidget::OptionsWidget(std::shared_ptr<Settings> settings, PageSelectionAccessor pageSelectionAccessor)
    : m_settings(std::move(settings)),
      m_pageSelectionAccessor(std::move(pageSelectionAccessor)),
      m_connectionManager([this] { setupUiConnections(); }),
      m_ui(std::make_unique<Ui::OptionsWidget>()) {
  m_ui->setupUi(this);
  setupIcons();

  setupUiConnections();
}

OptionsWidget::~OptionsWidget() = default;

void OptionsWidget::preUpdateUI(const PageId& pageId, const OrthogonalRotation rotation) {
  auto block = m_connectionManager.getScopedBlock();

  m_pageId = pageId;
  m_rotation = rotation;
  setRotationPixmap();
}

void OptionsWidget::postUpdateUI(const OrthogonalRotation rotation) {
  auto block = m_connectionManager.getScopedBlock();

  setRotation(rotation);
}

void OptionsWidget::rotateLeft() {
  OrthogonalRotation rotation(m_rotation);
  rotation.prevClockwiseDirection();
  setRotation(rotation);
}

void OptionsWidget::rotateRight() {
  OrthogonalRotation rotation(m_rotation);
  rotation.nextClockwiseDirection();
  setRotation(rotation);
}

void OptionsWidget::resetRotation() {
  setRotation(OrthogonalRotation());
}

void OptionsWidget::showApplyToDialog() {
  auto* dialog = new ApplyDialog(this, m_pageId, m_pageSelectionAccessor);
  dialog->setAttribute(Qt::WA_DeleteOnClose); // NOLINT(misc-include-cleaner)
  connect(dialog, &ApplyDialog::appliedTo, this, &OptionsWidget::appliedTo);
  connect(dialog, &ApplyDialog::appliedToAllPages, this, &OptionsWidget::appliedToAllPages);
  dialog->show();
}

void OptionsWidget::appliedTo(const std::set<PageId>& pages) {
  if (pages.empty()) {
    return;
  }

  m_settings->applyRotation(pages, m_rotation);

  if (pages.size() > 1) {
    invalidateAllThumbnails();
  } else {
    for (const PageId& pageId : pages) {
      invalidateThumbnail(pageId);
    }
  }
}

void OptionsWidget::appliedToAllPages(const std::set<PageId>& pages) {
  m_settings->applyRotation(pages, m_rotation);
  invalidateAllThumbnails();
}

void OptionsWidget::setRotation(const OrthogonalRotation& rotation) {
  if (rotation == m_rotation) {
    return;
  }

  m_rotation = rotation;
  setRotationPixmap();

  m_settings->applyRotation(m_pageId.imageId(), rotation);

  rotated(rotation);
  invalidateThumbnail(m_pageId);
}

void OptionsWidget::setRotationPixmap() {
  QIcon icon;
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
  switch (m_rotation.toDegrees()) {
    case 0:
      icon = IconProvider::getInstance().getIcon("big-up-arrow");
      break;
    case 90:
      icon = IconProvider::getInstance().getIcon("big-right-arrow");
      break;
    case 180:
      icon = IconProvider::getInstance().getIcon("big-down-arrow");
      break;
    case 270:
      icon = IconProvider::getInstance().getIcon("big-left-arrow");
      break;
    default:
      assert(!"Unreachable");
  }
  m_ui->rotationIndicator->setPixmap(icon.pixmap(32, 32));
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}

void OptionsWidget::setupUiConnections() {
  m_connectionManager.addConnection(
      connect(m_ui->rotateLeftBtn, &QPushButton::clicked, this, &OptionsWidget::rotateLeft)
      );
  m_connectionManager.addConnection(
      connect(m_ui->rotateRightBtn, &QPushButton::clicked, this, &OptionsWidget::rotateRight)
      );
  m_connectionManager.addConnection(
      connect(m_ui->resetBtn, &QPushButton::clicked, this, &OptionsWidget::resetRotation)
      );
  m_connectionManager.addConnection(
      connect(m_ui->applyToBtn, &QPushButton::clicked, this, &OptionsWidget::showApplyToDialog)
      );
}

void OptionsWidget::setupIcons() {
  auto& iconProvider = IconProvider::getInstance();
  m_ui->rotateLeftBtn->setIcon(iconProvider.getIcon("object-rotate-left"));
  m_ui->rotateRightBtn->setIcon(iconProvider.getIcon("object-rotate-right"));
}
}  // namespace fix_orientation
