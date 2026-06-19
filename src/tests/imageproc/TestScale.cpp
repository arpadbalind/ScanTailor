// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include <QImage>
#include <QSize>
#include <Qt>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <gtest/gtest.h>
#include <random>
#include <ranges>

#include "GrayImage.h"
#include "Scale.h"

namespace {
using namespace imageproc;

static bool fuzzyCompare(const QImage& img1, const QImage& img2) {
  EXPECT_TRUE(img1.size() == img2.size());

  const int width = img1.width();
  const int height = img1.height();
  const uint8_t* line1 = img1.bits();
  const uint8_t* line2 = img2.bits();
  const int line1Bpl = img1.bytesPerLine();
  const int line2Bpl = img2.bytesPerLine();

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (std::abs(static_cast<int>(line1[x]) - static_cast<int>(line2[x])) > 1) {
        return false;
      }
    }
    line1 += line1Bpl;
    line2 += line2Bpl;
  }
  return true;
}

static bool checkScale(const GrayImage& img, const QSize& newSize) {
  const GrayImage scaled1(scaleToGray(img, newSize));
  const GrayImage scaled2(img.toQImage().scaled(newSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
  return fuzzyCompare(static_cast<const QImage&>(scaled1), static_cast<const QImage&>(scaled2));
}
}

namespace imageproc::tests {

TEST(ScaleTestSuite, test_null_image) {
  const GrayImage nullImg;
  EXPECT_TRUE(scaleToGray(nullImg, QSize(1, 1)).isNull());
}

TEST(ScaleTestSuite, test_random_image) {
  GrayImage img(QSize(100, 100));

  auto indices = std::views::iota(0, img.width() * img.height());

  static thread_local std::mt19937_64 engine{std::random_device{}()};
  static thread_local std::uniform_int_distribution<uint8_t> dist{0, 255};

  std::ranges::for_each(indices, [&](int i) {
    const int y = i / img.width();
    const int x = i % img.width();
    img.data()[y * img.stride() + x] = dist(engine);
  });

  EXPECT_TRUE(checkScale(img, QSize(50, 50)));
  EXPECT_TRUE(checkScale(img, QSize(80, 80)));
}
}
