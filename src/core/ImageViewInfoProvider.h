// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <foundation/NonCopyable.h>

#include <QPointF>
#include <QSizeF>
#include <list>
#include <memory>

#include "Dpi.h"
#include "ImageViewInfoListener.h"
#include "NonCopyable.h"

class ImageViewInfoProvider : private NonCopyable {
 public:
  explicit ImageViewInfoProvider(const Dpi& dpi);

  ~ImageViewInfoProvider();

  void addListener(ImageViewInfoListener* listener);

  void removeAllListeners();

  void setPhysSize(const QSizeF& physSize);

  void setMousePos(const QPointF& mousePos);

  const Dpi& getDpi() const;

  const QPointF& getMousePos() const;

  const QSizeF& getPhysSize() const;

 private:
  void physSizeChanged(const QSizeF& physSize) const;

  void mousePosChanged(const QPointF& mousePos) const;

  void notifyProviderStopped() const;

  std::list<ImageViewInfoListener*> m_listeners;
  Dpi m_dpi;
  QPointF m_mousePos;
  QSizeF m_physSize;
};
