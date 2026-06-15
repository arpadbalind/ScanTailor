// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <memory>

#include "BackgroundTask.h"
#include "FilterResult.h"
#include "ImageId.h"
#include "ImageMetadata.h"
#include "NonCopyable.h"

class ThumbnailPixmapCache;
class PageInfo;
class ProjectPages;
class QImage;

namespace fix_orientation {
class Task;
}

class LoadFileTask : public BackgroundTask, private NonCopyable {

 public:
  LoadFileTask(Type type,
               const PageInfo& page,
               std::shared_ptr<ThumbnailPixmapCache> thumbnailCache,
               std::shared_ptr<ProjectPages> pages,
               std::shared_ptr<fix_orientation::Task> nextTask);

  ~LoadFileTask() override;

  FilterResultPtr operator()() override;

 private:
  class ErrorResult;

  void updateImageSizeIfChanged(const QImage& image);

  void overrideDpi(QImage& image) const;

  void convertToSupportedFormat(QImage& image) const;

  std::shared_ptr<ThumbnailPixmapCache> m_thumbnailCache;
  ImageId m_imageId;
  ImageMetadata m_imageMetadata;
  const std::shared_ptr<ProjectPages> m_pages;
  const std::shared_ptr<fix_orientation::Task> m_nextTask;
};
