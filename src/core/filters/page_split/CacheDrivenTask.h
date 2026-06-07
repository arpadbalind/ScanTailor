// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <memory>

#include "NonCopyable.h"

class QSizeF;
class PageInfo;
class AbstractFilterDataCollector;
class ImageTransformation;
class ProjectPages;

namespace deskew {
class CacheDrivenTask;
}

namespace page_split {
class Settings;

class CacheDrivenTask : private NonCopyable {
 public:
  CacheDrivenTask(std::shared_ptr<Settings> settings,
                  std::shared_ptr<ProjectPages> projectPages,
                  std::shared_ptr<deskew::CacheDrivenTask> nextTask);

  virtual ~CacheDrivenTask();

  void process(const PageInfo& pageInfo, AbstractFilterDataCollector* collector, const ImageTransformation& xform);

 private:
  std::shared_ptr<deskew::CacheDrivenTask> m_nextTask;
  std::shared_ptr<Settings> m_settings;
  std::shared_ptr<ProjectPages> m_projectPages;
};
}  // namespace page_split
