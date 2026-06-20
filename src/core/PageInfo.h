// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include "ImageMetadata.h"
#include "PageId.h"

class PageInfo {
  // Member-wise copying is OK.
 public:
  PageInfo();

  PageInfo(const PageId& pageId,
           const ImageMetadata& metadata,
           int imageSubPages,
           bool leftHalfRemoved,
           bool rightHalfRemoved);

  [[nodiscard]] bool isNull() const { return m_pageId.isNull(); }

  [[nodiscard]] const PageId& id() const { return m_pageId; }

  void setId(const PageId& id) { m_pageId = id; }

  [[nodiscard]] const ImageId& imageId() const { return m_pageId.imageId(); }

  [[nodiscard]] const ImageMetadata& metadata() const { return m_metadata; }

  [[nodiscard]] int imageSubPages() const { return m_imageSubPages; }

  [[nodiscard]] bool leftHalfRemoved() const { return m_leftHalfRemoved; }

  [[nodiscard]] bool rightHalfRemoved() const { return m_rightHalfRemoved; }

 private:
  PageId m_pageId;
  ImageMetadata m_metadata;
  int m_imageSubPages;
  bool m_leftHalfRemoved;
  bool m_rightHalfRemoved;
};
