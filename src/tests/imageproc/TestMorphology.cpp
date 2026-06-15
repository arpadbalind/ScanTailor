// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include <QImage>
#include <QPoint>
#include <QSize>
#include <array>
#include <gtest/gtest.h>

#include "BWColor.h"
#include "BinaryImage.h"
#include "GrayImage.h"
#include "Morphology.h"
#include "Utils.h"

namespace imageproc::tests {
using namespace utils;

TEST(MorphologyTestSuite, test_dilate_1x1) {
  static const std::array<int, 81> inp = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                            0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  EXPECT_TRUE(dilateBrick(img, Brick(QSize(1, 1)), img.rect()) == img);
}

TEST(MorphologyTestSuite, test_dilate_1x1_gray) {
  static const std::array<int, 81> inp = {0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
                            0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  const GrayImage img(makeGrayImage(inp.data(), 9, 9));
  EXPECT_TRUE(dilateGray(img, Brick(QSize(1, 1)), img.rect()) == img);
}

TEST(MorphologyTestSuite, test_dilate_1x1_shift_black) {
  static const std::array<int, 81> inp = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                            0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 81> out = {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1,
                            0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1,
                            0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));

  EXPECT_TRUE(dilateBrick(img, Brick(QSize(1, 1)), img.rect().translated(2, 2), BWColor::BLACK) == control);
}

TEST(MorphologyTestSuite, test_dilate_1x1_shift_gray) {
  static const std::array<int, 81> inp = {0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
                            0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 81> out = {0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 5, 6, 5, 0, 0, 0, 0, 0, 5, 5,
                            0, 0, 0, 0, 0, 3, 0, 5, 5, 0, 4, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 5,
                            0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};

  const GrayImage img(makeGrayImage(inp.data(), 9, 9));
  const GrayImage control(makeGrayImage(out.data(), 9, 9));
  EXPECT_TRUE(dilateGray(img, Brick(QSize(1, 1)), img.rect().translated(2, 2), 5) == control);
}

TEST(MorphologyTestSuite, test_dilate_3x1_gray) {
  static const std::array<int, 81> inp = {9, 4, 2, 3, 9, 9, 9, 9, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
                            9, 3, 9, 3, 9, 9, 9, 9, 9, 9, 9, 9, 2, 2, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 5, 2, 9,
                            9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};

  static const std::array<int, 81> out = {4, 2, 2, 2, 3, 9, 9, 1, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
                            3, 3, 3, 3, 3, 9, 9, 9, 9, 9, 9, 2, 2, 2, 2, 9, 9, 9, 9, 9, 9, 9, 9, 5, 2, 2, 2,
                            9, 9, 9, 9, 9, 2, 2, 2, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};

  const GrayImage img(makeGrayImage(inp.data(), 9, 9));
  const GrayImage control(makeGrayImage(out.data(), 9, 9));
  EXPECT_TRUE(dilateGray(img, Brick(QSize(3, 1)), img.rect()) == control);
}

TEST(MorphologyTestSuite, test_dilate_1x3_gray) {
  static const std::array<int, 81> inp = {9, 4, 9, 9, 9, 9, 9, 9, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9,
                            9, 3, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 5, 2, 9,
                            9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};

  static const std::array<int, 81> out = {9, 4, 9, 9, 9, 9, 9, 9, 1, 9, 2, 9, 9, 9, 9, 9, 9, 1, 9, 2, 9, 9, 9, 9, 9, 9, 9,
                            9, 2, 9, 2, 9, 9, 9, 9, 9, 9, 3, 9, 2, 9, 9, 5, 2, 9, 9, 9, 9, 2, 9, 9, 5, 2, 9,
                            9, 9, 9, 2, 9, 9, 5, 2, 9, 9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};

  const GrayImage img(makeGrayImage(inp.data(), 9, 9));
  const GrayImage control(makeGrayImage(out.data(), 9, 9));
  EXPECT_TRUE(dilateGray(img, Brick(QSize(1, 3)), img.rect()) == control);
}

TEST(MorphologyTestSuite, test_dilate_1x20_gray) {
  static const std::array<int, 81> inp = {9, 4, 9, 9, 9, 9, 9, 9, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9,
                            9, 3, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 5, 2, 9,
                            9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};

  static const std::array<int, 81> out = {9, 2, 9, 2, 9, 9, 5, 2, 1, 9, 2, 9, 2, 9, 9, 5, 2, 1, 9, 2, 9, 2, 9, 9, 5, 2, 1,
                            9, 2, 9, 2, 9, 9, 5, 2, 1, 9, 2, 9, 2, 9, 9, 5, 2, 1, 9, 2, 9, 2, 9, 9, 5, 2, 1,
                            9, 2, 9, 2, 9, 9, 5, 2, 1, 9, 2, 9, 2, 9, 9, 5, 2, 1, 9, 2, 9, 2, 9, 9, 5, 2, 1};

  const GrayImage img(makeGrayImage(inp.data(), 9, 9));
  const GrayImage control(makeGrayImage(out.data(), 9, 9));
  EXPECT_TRUE(dilateGray(img, Brick(QSize(1, 20)), img.rect()) == control);
}

TEST(MorphologyTestSuite, test_dilate_3x3_white) {
  static const std::array<int, 81> inp = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                            0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 81> out = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
                            1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1,
                            0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));

  EXPECT_TRUE(dilateBrick(img, Brick(QSize(3, 3)), img.rect(), BWColor::WHITE) == control);
}

TEST(MorphologyTestSuite, test_dilate_3x3_gray) {
  static const std::array<int, 81> inp = {9, 4, 9, 9, 9, 9, 9, 9, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9,
                            9, 3, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 5, 2, 9,
                            9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};

  static const std::array<int, 81> out = {4, 4, 4, 9, 9, 9, 9, 1, 1, 2, 2, 2, 9, 9, 9, 9, 1, 1, 2, 2, 2, 9, 9, 9, 9, 9, 9,
                            2, 2, 2, 2, 2, 9, 9, 9, 9, 3, 3, 2, 2, 2, 5, 2, 2, 2, 9, 9, 2, 2, 2, 5, 2, 2, 2,
                            9, 9, 2, 2, 2, 5, 2, 2, 2, 9, 9, 2, 2, 2, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};

  const GrayImage img(makeGrayImage(inp.data(), 9, 9));
  const GrayImage control(makeGrayImage(out.data(), 9, 9));
  EXPECT_TRUE(dilateGray(img, Brick(QSize(3, 3)), img.rect()) == control);
}

TEST(MorphologyTestSuite, test_dilate_3x3_gray_shrinked) {
  static const std::array<int, 81> inp = {9, 4, 9, 9, 9, 9, 9, 9, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9,
                            9, 3, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 5, 2, 9,
                            9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};

  static const std::array<int, 81> out = {2, 2, 9, 9, 9, 9, 1, 2, 2, 9, 9, 9, 9, 9, 2, 2, 2, 2, 9, 9, 9, 3, 2, 2, 2,
                            5, 2, 2, 9, 2, 2, 2, 5, 2, 2, 9, 2, 2, 2, 5, 2, 2, 9, 2, 2, 2, 9, 9, 9};

  const GrayImage img(makeGrayImage(inp.data(), 9, 9));
  const GrayImage control(makeGrayImage(out.data(), 7, 7));
  const QRect dstArea(img.rect().adjusted(1, 1, -1, -1));
  EXPECT_TRUE(dilateGray(img, Brick(QSize(3, 3)), dstArea) == control);
}

TEST(MorphologyTestSuite, test_open_1x2_gray) {
  static const std::array<int, 81> inp = {9, 4, 9, 9, 9, 9, 9, 9, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9,
                            9, 3, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 5, 2, 9,
                            9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  static const std::array<int, 81> out = {9, 4, 9, 9, 9, 9, 9, 9, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 3, 9, 9, 9, 9, 9, 9, 9,
                            9, 3, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9, 9, 2, 9, 9, 9, 9, 9,
                            9, 9, 9, 2, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  const GrayImage img(makeGrayImage(inp.data(), 9, 9));
  const GrayImage control(makeGrayImage(out.data(), 9, 9));
  EXPECT_TRUE(openGray(img, QSize(1, 2), 0x00) == control);
}

TEST(MorphologyTestSuite, test_dilate_5x5_white) {
  static const std::array<int, 81> inp = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 81> out = {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1,
                            1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
                            0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));

  EXPECT_TRUE(dilateBrick(img, Brick(QSize(5, 5)), img.rect(), BWColor::WHITE) == control);
}

TEST(MorphologyTestSuite, test_dilate_3x3_narrowing_white) {
  static const std::array<int, 81> inp = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                            0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 36> out = {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 4, 9));
  const QRect dstRect(5, 0, 4, 9);

  EXPECT_TRUE(dilateBrick(img, Brick(QSize(3, 3)), dstRect, BWColor::WHITE) == control);
}

TEST(MorphologyTestSuite, test_dilate_5x5_narrowing_white) {
  static const std::array<int, 99> inp = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 30> out = {1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0,
      0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1};

  const BinaryImage img(makeBinaryImage(inp.data(), 11, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 6, 5));
  const QRect dstRect(4, 1, 6, 5);

  EXPECT_TRUE(dilateBrick(img, Brick(QSize(5, 5)), dstRect, BWColor::WHITE) == control);
}

TEST(MorphologyTestSuite, test_dilate_3x3_narrowing_black) {
  static const std::array<int, 81> inp = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                            0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 36> out = {1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 4, 9));
  const QRect dstRect(QRect(5, 0, 4, 9));

  EXPECT_TRUE(dilateBrick(img, Brick(QSize(3, 3)), dstRect, BWColor::BLACK) == control);
}

TEST(MorphologyTestSuite, test_dilate_3x3_widening_white) {
  static const std::array<int, 81> inp = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                            0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 121> out = {
      0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1,
      1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
      1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  };

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 11, 11));
  const QRect dstRect(img.rect().adjusted(-1, -1, 1, 1));

  EXPECT_TRUE(dilateBrick(img, Brick(QSize(3, 3)), dstRect, BWColor::WHITE) == control);
}

TEST(MorphologyTestSuite, test_dilate_3x3_widening_black) {
  static const std::array<int, 81> inp = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                            0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 121> out = {
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
      1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1,
      1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  };

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 11, 11));
  const QRect dstRect(img.rect().adjusted(-1, -1, 1, 1));

  EXPECT_TRUE(dilateBrick(img, Brick(QSize(3, 3)), dstRect, BWColor::BLACK) == control);
}

TEST(MorphologyTestSuite, test_dilate_3x1_out_of_brick_white) {
  static const std::array<int, 81> inp = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                            0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 81> out = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));
  const Brick brick(QSize(3, 1), QPoint(-1, 0));

  EXPECT_TRUE(dilateBrick(img, brick, img.rect(), BWColor::WHITE) == control);
}

TEST(MorphologyTestSuite, test_dilate_1x3_out_of_brick_black) {
  static const std::array<int, 81> inp = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                            0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 81> out = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                            0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
                            0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));
  const Brick brick(QSize(1, 3), QPoint(0, -1));

  EXPECT_TRUE(dilateBrick(img, brick, img.rect(), BWColor::BLACK) == control);
}

TEST(MorphologyTestSuite, test_large_dilate) {
  BinaryImage img(110, 110);
  img.fill(BWColor::WHITE);
  const QRect initialRect(img.rect().center(), QSize(1, 1));
  img.fill(initialRect, BWColor::BLACK);

  const Brick brick(QSize(80, 80));
  const QRect extendedRect(initialRect.adjusted(brick.minX(), brick.minY(), brick.maxX(), brick.maxY()));

  BinaryImage control(img);
  control.fill(extendedRect, BWColor::BLACK);

  EXPECT_TRUE(dilateBrick(img, brick, img.rect(), BWColor::WHITE) == control);
}

TEST(MorphologyTestSuite, test_erode_1x1) {
  static const std::array<int, 81> inp  = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  EXPECT_TRUE(erodeBrick(img, Brick(QSize(1, 1)), img.rect()) == img);
}

TEST(MorphologyTestSuite, test_erode_3x3_assymmetric_black) {
  static const std::array<int, 81> inp  = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1};

  static const std::array<int, 81> out = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1,
                            1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
                            1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));
  const Brick brick(QSize(3, 3), QPoint(0, 1));

  EXPECT_TRUE(erodeBrick(img, brick, img.rect(), BWColor::BLACK) == control);
}

TEST(MorphologyTestSuite, test_erode_3x3_assymmetric_white) {
  static const std::array<int, 81> inp  = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1};

  static const std::array<int, 81> out = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1,
                            0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
                            0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));
  const Brick brick(QSize(3, 3), QPoint(0, 1));

  EXPECT_TRUE(erodeBrick(img, brick, img.rect(), BWColor::WHITE) == control);
}

TEST(MorphologyTestSuite, test_erode_11x11_white) {
  static const std::array<int, 81> inp  = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1};

  static const std::array<int, 81> out = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));
  const Brick brick(QSize(11, 11));

  EXPECT_TRUE(erodeBrick(img, brick, img.rect(), BWColor::WHITE) == control);
}

TEST(MorphologyTestSuite, test_open_2x2_white) {
  static const std::array<int, 81> inp  = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1};

  static const std::array<int, 81> out = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));

  EXPECT_TRUE(openBrick(img, QSize(2, 2), img.rect(), BWColor::WHITE) == control);
}

TEST(MorphologyTestSuite, test_open_2x2_black) {
  static const std::array<int, 81> inp  = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1};

  static const std::array<int, 81> out  = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));

  EXPECT_TRUE(openBrick(img, QSize(2, 2), img.rect(), BWColor::BLACK) == control);
}

TEST(MorphologyTestSuite, test_open_2x2_shifted_white) {
  static const std::array<int, 81> inp  = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1};

  static const std::array<int, 81> out  = {// 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
                            0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));
  const QRect dstRect(img.rect().translated(2, 1));

  EXPECT_TRUE(openBrick(img, QSize(2, 2), dstRect, BWColor::WHITE) == control);
}

TEST(MorphologyTestSuite, test_open_2x2_shifted_black) {
  static const std::array<int, 81> inp  = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1};

  static const std::array<int, 81> out  = {
                            0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1,
                            0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1,
                            0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));
  const QRect dstRect(img.rect().translated(2, 1));

  EXPECT_TRUE(openBrick(img, QSize(2, 2), dstRect, BWColor::BLACK) == control);
}

TEST(MorphologyTestSuite, test_open_2x2_narrowing) {
  static const std::array<int, 81> inp  = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1};

  static const std::array<int, 16> out  = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 4, 4));
  const QRect dstRect(img.rect().adjusted(2, 2, -3, -3));

  EXPECT_TRUE(openBrick(img, QSize(2, 2), dstRect, BWColor::WHITE) == control);
  EXPECT_TRUE(openBrick(img, QSize(2, 2), dstRect, BWColor::BLACK) == control);
}

TEST(MorphologyTestSuite, test_close_2x2_white) {
  static const std::array<int, 81> inp  = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
                            1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
                            0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0};

  static const std::array<int, 81> out  = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
                            1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1,
                            0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));

  EXPECT_TRUE(closeBrick(img, QSize(2, 2), img.rect(), BWColor::WHITE) == control);
}

TEST(MorphologyTestSuite, test_close_2x2_black) {
  static const std::array<int, 81> inp  = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
                            1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
                            0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0};

  static const std::array<int, 81> out  = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
                            1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1,
                            1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));

  EXPECT_TRUE(closeBrick(img, QSize(2, 2), img.rect(), BWColor::BLACK) == control);
}

TEST(MorphologyTestSuite, test_close_2x2_shifted_white) {
  static const std::array<int, 81> inp  = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
                            1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
                            0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0};

  static const std::array<int, 81> out  = {
                            1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
                            1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
                            0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));
  const QRect dstRect(img.rect().translated(2, 1));

  EXPECT_TRUE(closeBrick(img, QSize(2, 2), dstRect, BWColor::WHITE) == control);
}

TEST(MorphologyTestSuite, test_close_2x2_shifted_black) {
  static const std::array<int, 81> inp  = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
                            1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
                            0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0};

  static const std::array<int, 81> out  = {
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
                            0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));
  const QRect dstRect(img.rect().translated(2, 1));

  EXPECT_TRUE(closeBrick(img, QSize(2, 2), dstRect, BWColor::BLACK) == control);
}

TEST(MorphologyTestSuite, test_close_2x2_narrowing) {
  static const std::array<int, 81> inp  = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
                            1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
                            0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0};

  static const std::array<int, 16> out  = {0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 4, 4));
  const QRect dstRect(img.rect().adjusted(2, 2, -3, -3));

  EXPECT_TRUE(closeBrick(img, QSize(2, 2), dstRect, BWColor::WHITE) == control);
  EXPECT_TRUE(closeBrick(img, QSize(2, 2), dstRect, BWColor::BLACK) == control);
}

TEST(MorphologyTestSuite, test_hmm_1) {
  static const std::array<int, 81> inp  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0,
                            0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0};

  static const std::array<int, 81> out  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                            0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static constexpr auto pattern = std::to_array("?X?X X?X?");
  const QPoint origin(1, 1);

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));

  EXPECT_TRUE(hitMissMatch(img, BWColor::WHITE, pattern.data(), 3, 3, origin) == control);
  EXPECT_TRUE(hitMissMatch(img, BWColor::BLACK, pattern.data(), 3, 3, origin) == control);
}

TEST(MorphologyTestSuite, test_hmm_surroundings_1) {
  static const std::array<int, 81> inp  = {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                            0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
                            0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 81> out_white = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 81> out_black = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static constexpr auto pattern = std::to_array("?X?X X?X?");
  const QPoint origin(1, 1);

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage controlW(makeBinaryImage(out_white.data(), 9, 9));
  const BinaryImage controlB(makeBinaryImage(out_black.data(), 9, 9));

  EXPECT_TRUE(hitMissMatch(img, BWColor::WHITE, pattern.data(), 3, 3, origin) == controlW);
  EXPECT_TRUE(hitMissMatch(img, BWColor::BLACK, pattern.data(), 3, 3, origin) == controlB);
}

TEST(MorphologyTestSuite, test_hmm_surroundings_2) {
  static const std::array<int, 81> inp  = {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                            0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
                            0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 81> out  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static constexpr auto pattern = std::to_array("?X?X X?X?");
  const QPoint origin(1, 0);

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));

  EXPECT_TRUE(hitMissMatch(img, BWColor::WHITE, pattern.data(), 3, 3, origin) == control);
  EXPECT_TRUE(hitMissMatch(img, BWColor::BLACK, pattern.data(), 3, 3, origin) == control);
}

TEST(MorphologyTestSuite, test_hmm_cornercase_1) {
  static const std::array<int, 81> inp  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0,
                            0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0};

  static const std::array<int, 81> out  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static constexpr auto pattern = std::to_array("?X?X X?X?");
  const QPoint origin(10, 0);

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));

  EXPECT_TRUE(hitMissMatch(img, BWColor::WHITE, pattern.data(), 3, 3, origin) == control);
  EXPECT_TRUE(hitMissMatch(img, BWColor::BLACK, pattern.data(), 3, 3, origin) == control);
}

TEST(MorphologyTestSuite, test_hmm_cornercase_2) {
  static const std::array<int, 81> inp  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0,
                            0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0};

  static const std::array<int, 81> out  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static constexpr auto pattern = std::to_array("?X?X X?X?");
  const QPoint origin(0, 9);

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));

  EXPECT_TRUE(hitMissMatch(img, BWColor::WHITE, pattern.data(), 3, 3, origin) == control);
  EXPECT_TRUE(hitMissMatch(img, BWColor::BLACK, pattern.data(), 3, 3, origin) == control);
}

TEST(MorphologyTestSuite, test_hmr_1) {
  static const std::array<int, 81> inp  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                            0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0,
                            0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0};

  static const std::array<int, 81> out  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0,
                            0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0};

  static constexpr auto pattern = std::to_array(" - X+XXXX");
  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage control(makeBinaryImage(out.data(), 9, 9));

  EXPECT_TRUE(hitMissReplace(img, BWColor::WHITE, pattern.data(), 3, 3) == control);
  EXPECT_TRUE(hitMissReplace(img, BWColor::BLACK, pattern.data(), 3, 3) == control);
}
}
