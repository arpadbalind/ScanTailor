// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <QString>

/**
 * \brief Provides unique_ptr semantics for files.
 *
 * Just like std::unique_ptr deleting its object when it goes out of scope,
 * this class deletes a file.  unique_ptr's copying semantics is also preserved.
 */
class AutoRemovingFile {
 public:
  AutoRemovingFile() = default;

  explicit AutoRemovingFile(QString filePath);
  ~AutoRemovingFile();

  AutoRemovingFile(const AutoRemovingFile&) = delete;

  AutoRemovingFile& operator=(const AutoRemovingFile&) = delete;

  AutoRemovingFile(AutoRemovingFile&& other) noexcept;

  AutoRemovingFile& operator=(AutoRemovingFile&& other) noexcept;

  [[nodiscard]] const QString& get() const { return m_file; }

  void reset(const QString& file);

  QString release();

 private:
  QString m_file;
};
