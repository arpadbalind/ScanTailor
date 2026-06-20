// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <foundation/Hashes.h>

#include <QString>

class QFileInfo;

class ImageId {
  // Member-wise copying is OK.
 public:
  ImageId() : m_filePath(), m_page(0) {}

  explicit ImageId(const QString& filePath, int page = 0);

  explicit ImageId(const QFileInfo& fileInfo, int page = 0);

  [[nodiscard]] bool isNull() const { return m_filePath.isNull(); }

  [[nodiscard]] const QString& filePath() const { return m_filePath; }

  void setFilePath(const QString& path) { m_filePath = path; }

  [[nodiscard]] int page() const { return m_page; }

  void setPage(int page) { m_page = page; }

  [[nodiscard]] int zeroBasedPage() const { return m_page > 0 ? m_page - 1 : 0; }

  [[nodiscard]] bool isMultiPageFile() const { return m_page > 0; }

 private:
  QString m_filePath;

  /**
   * If zero, indicates the file is not multipage.
   * If above zero, indicates Nth page in a multipage file.
   */
  int m_page;
};


bool operator==(const ImageId& lhs, const ImageId& rhs);

bool operator!=(const ImageId& lhs, const ImageId& rhs);

bool operator<(const ImageId& lhs, const ImageId& rhs);

namespace std {
template <>
struct hash<ImageId> {
  size_t operator()(const ImageId& imageId) const noexcept {
    // NOLINTNEXTLINE(bugprone-signed-bitwise)
    return (hashes::hash<QString>()(imageId.filePath()) ^ hash<int>()(imageId.page()) << 1);
  }
};
}  // namespace std
