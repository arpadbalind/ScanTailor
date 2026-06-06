// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include <Grayscale.h>
#include <QImage>
#include <gtest/gtest.h>
#include <cstdlib>
#include "Utils.h"

namespace imageproc {
namespace tests {
using namespace utils;

TEST(GrayscaleTestSuite, test_null_image) {
  EXPECT_TRUE(toGrayscale(QImage()).isNull());
}

TEST(GrayscaleTestSuite, test_mono_to_grayscale) {
  const int w = 50;
  const int h = 64;

  QImage mono(w, h, QImage::Format_Mono);
  QImage gray(w, h, QImage::Format_Indexed8);
  gray.setColorTable(createGrayscalePalette());

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const int rnd = rand() & 1;
      mono.setPixel(x, y, rnd ? 0 : 1);
      gray.setPixel(x, y, rnd ? 0 : 255);
    }
  }

  const QImage monoLsb(mono.convertToFormat(QImage::Format_MonoLSB));

  ASSERT_TRUE(toGrayscale(mono) == gray);
  EXPECT_TRUE(toGrayscale(monoLsb) == gray);
}

TEST(GrayscaleTestSuite, test_argb32_to_grayscale) {
  const int w = 50;
  const int h = 64;
  QImage argb32(w, h, QImage::Format_ARGB32);
  QImage gray(w, h, QImage::Format_Indexed8);
  gray.setColorTable(createGrayscalePalette());

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const int rnd = rand() & 1;
      argb32.setPixel(x, y, rnd ? 0x80303030 : 0x80606060);
      gray.setPixel(x, y, rnd ? 0x30 : 0x60);
    }
  }

  EXPECT_TRUE(toGrayscale(argb32) == gray);
}

}  // namespace tests
}  // namespace imageproc   
