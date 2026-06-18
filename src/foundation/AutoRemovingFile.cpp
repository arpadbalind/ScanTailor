// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "AutoRemovingFile.h"

#include <QFile>

AutoRemovingFile::AutoRemovingFile(QString filePath) : m_file(std::move(filePath)) {}

AutoRemovingFile::~AutoRemovingFile() {
  if (!m_file.isEmpty()) {
    QFile::remove(m_file);
  }
}

AutoRemovingFile::AutoRemovingFile(AutoRemovingFile&& other) noexcept : m_file(std::move(other.m_file)) {
  other.m_file.clear();
}

AutoRemovingFile& AutoRemovingFile::operator=(AutoRemovingFile&& other) noexcept {
  if (this != &other) {
    if (!m_file.isEmpty()) {
      QFile::remove(m_file);
    }

    m_file = std::move(other.m_file);
    other.m_file.clear();
  }

  return *this;
}

void AutoRemovingFile::reset(const QString& file) {
  const QString& oldFile(file);

  m_file = file;

  if (!oldFile.isEmpty()) {
    QFile::remove(oldFile);
  }
}

QString AutoRemovingFile::release() {
  QString saved(m_file);
  m_file = QString();
  return saved;
}
