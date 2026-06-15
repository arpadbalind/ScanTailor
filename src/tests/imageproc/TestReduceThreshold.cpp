// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.
#include <QImage>

#include <array>
#include <gtest/gtest.h>


#include "BinaryImage.h"
#include "ReduceThreshold.h"
#include "Utils.h"

namespace imageproc::tests {
using namespace utils;

TEST(ReduceThresholdTestSuite, test_null_image) {
  EXPECT_TRUE(ReduceThreshold(BinaryImage())(2).image().isNull());
}

TEST(ReduceThresholdTestSuite, test_small_image) {
  const std::array<int, 80> inp = {0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

                     0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,

                     0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,

                     0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  const std::array<int, 20> out1 = {0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  const std::array<int, 20> out2 = {0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  const std::array<int, 20> out3 = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1};
  const std::array<int, 20> out4 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1};

  const BinaryImage img(makeBinaryImage(inp.data(), 10, 8));

  EXPECT_TRUE(makeBinaryImage(out1.data(), 5, 4) == static_cast<BinaryImage>(ReduceThreshold(img)(1)));
  EXPECT_TRUE(makeBinaryImage(out2.data(), 5, 4) == static_cast<BinaryImage>(ReduceThreshold(img)(2)));
  EXPECT_TRUE(makeBinaryImage(out3.data(), 5, 4) == static_cast<BinaryImage>(ReduceThreshold(img)(3)));
  EXPECT_TRUE(makeBinaryImage(out4.data(), 5, 4) == static_cast<BinaryImage>(ReduceThreshold(img)(4)));
}

TEST(ReduceThresholdTestSuite, test_lines) {
  static const std::array<int, 9> inp = {0, 0, 0, 1, 1, 0, 1, 1, 0};
  static const std::array<int, 4> out1 = {0, 1, 1, 1};
  static const std::array<int, 4> out2 = {0, 1, 1, 1};
  static const std::array<int, 4> out3 = {0, 0, 0, 1};
  static const std::array<int, 4> out4 = {0, 0, 0, 1};

  BinaryImage img(makeBinaryImage(inp.data(), 9, 1));

  EXPECT_TRUE(makeBinaryImage(out1.data(), 4, 1) == static_cast<BinaryImage>(ReduceThreshold(img)(1)));
  EXPECT_TRUE(makeBinaryImage(out2.data(), 4, 1) == static_cast<BinaryImage>(ReduceThreshold(img)(2)));
  EXPECT_TRUE(makeBinaryImage(out3.data(), 4, 1) == static_cast<BinaryImage>(ReduceThreshold(img)(3)));
  EXPECT_TRUE(makeBinaryImage(out4.data(), 4, 1) == static_cast<BinaryImage>(ReduceThreshold(img)(4)));

  img = makeBinaryImage(inp.data(), 1, 9);

  EXPECT_TRUE(makeBinaryImage(out1.data(), 1, 4) == static_cast<BinaryImage>(ReduceThreshold(img)(1)));
  EXPECT_TRUE(makeBinaryImage(out2.data(), 1, 4) == static_cast<BinaryImage>(ReduceThreshold(img)(2)));
  EXPECT_TRUE(makeBinaryImage(out3.data(), 1, 4) == static_cast<BinaryImage>(ReduceThreshold(img)(3)));
  EXPECT_TRUE(makeBinaryImage(out4.data(), 1, 4) == static_cast<BinaryImage>(ReduceThreshold(img)(4)));
}
}
