// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

// NOLINTBEGIN(misc-include-cleaner)
#include <QColor>
#include <QImage>
#include <QSize>

#include <algorithm>
#include <gtest/gtest.h>
#include <cstddef>
#include <cstdint>
#include <random>

#include "Grayscale.h"
#include "Transform.h"

namespace imageproc::tests {

TEST(TransformTestSuite, test_null_image) {
  const QImage nullImg;
  const QTransform nullXform;
  const QRect unitRect(0, 0, 1, 1);
  const QColor bgcolor(0xff, 0xff, 0xff);
  const OutsidePixels outsidePixels(OutsidePixels::assumeColor(bgcolor));
  EXPECT_TRUE(transformToGray(nullImg, nullXform, unitRect, outsidePixels).isNull());
}

TEST(TransformTestSuite, test_random_image) {
  GrayImage img(QSize(100, 100));

  static thread_local std::mt19937 engine{std::random_device{}()};
  static thread_local std::uniform_int_distribution<int> dist{0, 255};

  uint8_t* line = img.data();
  const int stride = img.stride();
  const int height = img.height();

  std::generate(line, line + static_cast<std::ptrdiff_t>(stride) * height, [&]() {
    return static_cast<uint8_t>(dist(engine));
  });

  const QColor bgcolor(0xff, 0xff, 0xff);
  const OutsidePixels outsidePixels(OutsidePixels::assumeColor(bgcolor));

  const QTransform nullXform;
  EXPECT_TRUE(transformToGray(static_cast<const QImage&>(img), nullXform, img.rect(), outsidePixels) == img);
}
}
// NOLINTEND(misc-include-cleaner)
