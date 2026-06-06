// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include <BinaryImage.h>
#include <OrthogonalRotation.h>
#include <QImage>
#include <QRect>
#include <gtest/gtest.h>
#include "Utils.h"

namespace imageproc {
namespace tests {
using namespace utils;

TEST(OrthogonalRotationTestSuite, test_null_image) {
  const BinaryImage nullImg;
  EXPECT_TRUE(orthogonalRotation(nullImg, 90).isNull());
}

TEST(OrthogonalRotationTestSuite, test_full_image) {
  static const int inp[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1,
                            1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1,
                            1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  static const int out1[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1,
                             1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1,
                             1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  static const int out2[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1,
                             1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1,
                             1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  static const int out3[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1,
                             1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1,
                             1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  const BinaryImage img(makeBinaryImage(inp, 9, 9));
  const BinaryImage out1Img(makeBinaryImage(out1, 9, 9));
  const BinaryImage out2Img(makeBinaryImage(out2, 9, 9));
  const BinaryImage out3Img(makeBinaryImage(out3, 9, 9));

  ASSERT_TRUE(orthogonalRotation(img, 0) == img);
  ASSERT_TRUE(orthogonalRotation(img, 360) == img);
  ASSERT_TRUE(orthogonalRotation(img, 90) == out1Img);
  ASSERT_TRUE(orthogonalRotation(img, -270) == out1Img);
  ASSERT_TRUE(orthogonalRotation(img, 180) == out2Img);
  ASSERT_TRUE(orthogonalRotation(img, -180) == out2Img);
  ASSERT_TRUE(orthogonalRotation(img, 270) == out3Img);
  ASSERT_TRUE(orthogonalRotation(img, -90) == out3Img);
}

TEST(OrthogonalRotationTestSuite, test_sub_image) {
  static const int inp[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
                            1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1,
                            1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1};

  static const int out1[] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
                             0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};

  static const int out2[] = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
                             0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1};

  static const int out3[] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1,
                             0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};

  static const int out4[] = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1,
                             0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1};

  const QRect rect(1, 2, 7, 7);
  const BinaryImage img(makeBinaryImage(inp, 9, 9));
  const BinaryImage out1Img(makeBinaryImage(out1, 7, 7));
  const BinaryImage out2Img(makeBinaryImage(out2, 7, 7));
  const BinaryImage out3Img(makeBinaryImage(out3, 7, 7));
  const BinaryImage out4Img(makeBinaryImage(out4, 7, 7));

  ASSERT_TRUE(orthogonalRotation(img, rect, 0) == out1Img);
  ASSERT_TRUE(orthogonalRotation(img, rect, 360) == out1Img);
  ASSERT_TRUE(orthogonalRotation(img, rect, 90) == out2Img);
  ASSERT_TRUE(orthogonalRotation(img, rect, -270) == out2Img);
  ASSERT_TRUE(orthogonalRotation(img, rect, 180) == out3Img);
  ASSERT_TRUE(orthogonalRotation(img, rect, -180) == out3Img);
  ASSERT_TRUE(orthogonalRotation(img, rect, 270) == out4Img);
  ASSERT_TRUE(orthogonalRotation(img, rect, -90) == out4Img);
}

}  // namespace tests
}  // namespace imageproc   
