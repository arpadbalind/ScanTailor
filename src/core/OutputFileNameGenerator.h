// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <QString>
#include <Qt>
#include <memory>

#include "FileNameDisambiguator.h"

class PageId;
class AbstractRelinker;

class OutputFileNameGenerator {
  // Member-wise copying is OK.
 public:
  OutputFileNameGenerator();

  OutputFileNameGenerator(std::shared_ptr<FileNameDisambiguator> disambiguator,
                          const QString& outDir,
                          Qt::LayoutDirection layoutDirection);

  void performRelinking(const AbstractRelinker& relinker);

  [[nodiscard]] Qt::LayoutDirection layoutDirection() const { return m_layoutDirection; }

  [[nodiscard]] const QString& outDir() const { return m_outDir; }

  FileNameDisambiguator* disambiguator() { return m_disambiguator.get(); }

  [[nodiscard]] const FileNameDisambiguator* disambiguator() const { return m_disambiguator.get(); }

  [[nodiscard]] QString fileNameFor(const PageId& page) const;

  [[nodiscard]] QString filePathFor(const PageId& page) const;

 private:
  std::shared_ptr<FileNameDisambiguator> m_disambiguator;
  QString m_outDir;
  Qt::LayoutDirection m_layoutDirection;
};
