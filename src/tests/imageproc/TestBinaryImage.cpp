// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include <QImage>
#include <QRect>

#include <array>
#include <gtest/gtest.h>
#include <random>

#include "BWColor.h"
#include "BinaryImage.h"
#include "Utils.h"

namespace imageproc::tests {
using namespace utils;

TEST(BinaryImageTestSuite, test_null_image) {
  EXPECT_EQ(BinaryImage().toQImage(), QImage());
  EXPECT_EQ(BinaryImage(QImage()).toQImage(), QImage());
}

TEST(BinaryImageTestSuite, test_from_to_qimage) {
  const int w = 50;
  const int h = 64;
  QImage qimgArgb32(w, h, QImage::Format_ARGB32);
  QImage qimgMono(w, h, QImage::Format_Mono);
  qimgMono.setColorCount(2);
  qimgMono.setColor(0, 0xffffffff);
  qimgMono.setColor(1, 0xff000000);

  std::mt19937 rng{std::random_device{}()};
  std::uniform_int_distribution<int> dist(0, 1);

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const int rnd = dist(rng);
      qimgArgb32.setPixel(x, y, rnd ? 0x66888888 : 0x66777777);
      qimgMono.setPixel(x, y, rnd ? 0 : 1);
    }
  }

  const QImage qimgMonoLsb(qimgMono.convertToFormat(QImage::Format_MonoLSB));
  const QImage qimgRgb32(qimgArgb32.convertToFormat(QImage::Format_RGB32));
  const QImage qimgArgb32Pm(qimgArgb32.convertToFormat(QImage::Format_ARGB32_Premultiplied));
  const QImage qimgIndexed8(qimgRgb32.convertToFormat(QImage::Format_Indexed8));

  ASSERT_EQ(BinaryImage(qimgMono).toQImage(), qimgMono);
  EXPECT_EQ(BinaryImage(qimgMonoLsb).toQImage(), qimgMono);
  EXPECT_EQ(BinaryImage(qimgArgb32).toQImage(), qimgMono);
  EXPECT_EQ(BinaryImage(qimgRgb32).toQImage(), qimgMono);
  EXPECT_EQ(BinaryImage(qimgArgb32Pm).toQImage(), qimgMono);
  EXPECT_EQ(BinaryImage(qimgIndexed8).toQImage(), qimgMono);
}

TEST(BinaryImageTestSuite, test_full_fill) {
  BinaryImage white(100, 100);
  white.fill(BWColor::WHITE);

  QImage qWhite(100, 100, QImage::Format_Mono);
  qWhite.fill(1);

  ASSERT_EQ(BinaryImage(qWhite), white);

  BinaryImage black(30, 30);
  black.fill(BWColor::BLACK);

  QImage qBlack(30, 30, QImage::Format_Mono);
  qBlack.fill(0);

  ASSERT_EQ(BinaryImage(qBlack), black);
}

TEST(BinaryImageTestSuite, test_partial_fill_small) {
  const QImage qImage(randomMonoQImage(100, 100));

  const QRect rect(80, 80, 20, 20);
  BinaryImage image(qImage);
  image.fill(rect, BWColor::WHITE);
  QImage whiteRect(rect.width(), rect.height(), QImage::Format_Mono);
  whiteRect.setColorCount(2);
  whiteRect.setColor(0, 0xffffffff);
  whiteRect.setColor(1, 0xff000000);
  whiteRect.fill(0);
  
  ASSERT_EQ(image.toQImage().copy(rect), whiteRect);
  EXPECT_TRUE(surroundingsIntact(image.toQImage(), qImage, rect));
}

TEST(BinaryImageTestSuite, test_partial_fill_large) {
  const QImage qImage(randomMonoQImage(100, 100));

  const QRect rect(20, 20, 79, 79);
  BinaryImage image(qImage);
  image.fill(rect, BWColor::WHITE);
  QImage whiteRect(rect.width(), rect.height(), QImage::Format_Mono);
  whiteRect.setColorCount(2);
  whiteRect.setColor(0, 0xffffffff);
  whiteRect.setColor(1, 0xff000000);
  whiteRect.fill(0);
  
  ASSERT_EQ(image.toQImage().copy(rect), whiteRect);
  EXPECT_TRUE(surroundingsIntact(image.toQImage(), qImage, rect));
}

TEST(BinaryImageTestSuite, test_fill_except) {
  const QImage qImage(randomMonoQImage(100, 100));

  const QRect rect(20, 20, 79, 79);
  BinaryImage image(qImage);
  image.fillExcept(rect, BWColor::BLACK);

  QImage blackImage(qImage.width(), qImage.height(), QImage::Format_Mono);
  blackImage.setColorCount(2);
  blackImage.setColor(0, 0xffffffff);
  blackImage.setColor(1, 0xff000000);
  blackImage.fill(1);

  ASSERT_EQ(image.toQImage().copy(rect), qImage.copy(rect));
  EXPECT_TRUE(surroundingsIntact(image.toQImage(), blackImage, rect));
}

TEST(BinaryImageTestSuite, test_content_bounding_box4) {
  const std::array<int, 72> inp = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
      0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 8));
  EXPECT_EQ(img.contentBoundingBox(), QRect(1, 1, 6, 6));
}
}
