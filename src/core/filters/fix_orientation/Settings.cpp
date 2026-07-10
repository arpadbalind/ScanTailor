// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "Settings.h"

#include <QMutexLocker>

#include <set>

#include "AbstractRelinker.h"
#include "OrthogonalRotation.h"
#include "PageId.h"
#include "RelinkablePath.h"
#include "Utils.h"

using namespace core;

namespace fix_orientation {
Settings::Settings() = default;

Settings::~Settings() = default;

void Settings::clear() {
  const QMutexLocker locker(&m_mutex);
  m_perImageRotation.clear();
}

void Settings::performRelinking(const AbstractRelinker& relinker) {
  const QMutexLocker locker(&m_mutex);
  PerImageRotation newRotations;

  for (const PerImageRotation::value_type& kv : m_perImageRotation) {
    const RelinkablePath oldPath(kv.first.filePath(), RelinkablePath::RelinkablePathType::File);
    ImageId newImageId(kv.first);
    newImageId.setFilePath(relinker.substitutionPathFor(oldPath));
    newRotations.insert(PerImageRotation::value_type(newImageId, kv.second));
  }

  m_perImageRotation.swap(newRotations);
}

void Settings::applyRotation(const ImageId& imageId, const OrthogonalRotation rotation) {
  const QMutexLocker locker(&m_mutex);
  setImageRotationLocked(imageId, rotation);
}

void Settings::applyRotation(const std::set<PageId>& pages, const OrthogonalRotation rotation) {
  const QMutexLocker locker(&m_mutex);

  for (const PageId& page : pages) {
    setImageRotationLocked(page.imageId(), rotation);
  }
}

OrthogonalRotation Settings::getRotationFor(const ImageId& imageId) const {
  const QMutexLocker locker(&m_mutex);

  auto it(m_perImageRotation.find(imageId));
  if (it != m_perImageRotation.end()) {
    return it->second;
  } else {
    return {};
  }
}

void Settings::setImageRotationLocked(const ImageId& imageId, const OrthogonalRotation& rotation) {
  Utils::mapSetValue(m_perImageRotation, imageId, rotation);
}

bool Settings::isRotationNull(const ImageId& imageId) const {
  const QMutexLocker locker(&m_mutex);
  return (m_perImageRotation.find(imageId) == m_perImageRotation.end());
}
}  // namespace fix_orientation
