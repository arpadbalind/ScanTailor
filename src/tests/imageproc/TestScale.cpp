// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include <GrayImage.h>
#include <Scale.h>

#include <QImage>
#include <QSize>
#include <gtest/gtest.h>
#include <cmath>
#include <cstdint>
#include <cstdlib>

#include "Utils.h"

namespace imageproc {
namespace tests {
using namespace utils;

TEST(ScaleTestSuite, test_null_image) {
  const GrayImage nullImg;
  EXPECT_TRUE(scaleToGray(nullImg, QSize(1, 1)).isNull());
}

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
      if (std::abs(int(line1[x]) - int(line2[x])) > 1) {
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
  return fuzzyCompare(scaled1, scaled2);
}

TEST(ScaleTestSuite, test_random_image) {
  GrayImage img(QSize(100, 100));
  uint8_t* line = img.data();
  for (int y = 0; y < img.height(); ++y) {
    for (int x = 0; x < img.width(); ++x) {
      line[x] = static_cast<uint8_t>(rand() % 256);
    }
    line += img.stride();
  }

  // Unfortunately scaleToGray() and QImage::scaled()
  // produce too different results when upscaling.

  EXPECT_TRUE(checkScale(img, QSize(50, 50)));
  // EXPECT_TRUE(checkScale(img, QSize(200, 200)));
  EXPECT_TRUE(checkScale(img, QSize(80, 80)));
  // EXPECT_TRUE(checkScale(img, QSize(140, 140)));
  // EXPECT_TRUE(checkScale(img, QSize(55, 145)));
  // EXPECT_TRUE(checkScale(img, QSize(145, 55)));
}
}  // namespace tests
}  // namespace imageproc
