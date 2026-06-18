// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <QString>
#include <functional>
#include <deque>

#include "AutoRemovingFile.h"

class QImage;
class QWidget;

namespace imageproc {
class BinaryImage;
}

/**
 * \brief A sequence of image + label pairs.
 */
class DebugImages {
 public:
  virtual ~DebugImages() = default;

  virtual void add(const QImage& image,
                   const QString& label,
                   const std::function<QWidget*(const QImage&)>& imageViewFactory
                   = std::function<QWidget*(const QImage&)>())
      = 0;

  virtual void add(const imageproc::BinaryImage& image,
                   const QString& label,
                   const std::function<QWidget*(const QImage&)>& imageViewFactory
                   = std::function<QWidget*(const QImage&)>())
      = 0;

  [[nodiscard]] virtual bool empty() const = 0;

  /**
   * \brief Removes and returns the first item in the sequence.
   *
   * The label and viewer widget factory (that may not be bound)
   * are returned by taking pointers to them as arguments.
   * Returns a null AutoRemovingFile if image sequence is empty.
   */
  virtual AutoRemovingFile retrieveNext(QString* label = nullptr,
                                        std::function<QWidget*(const QImage&)>* imageViewFactory = nullptr)
      = 0;
};
