// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include <QImage>

#include <array>
#include <gtest/gtest.h>

#include "BWColor.h"
#include "BinaryImage.h"
#include "Shear.h"
#include "Utils.h"

namespace imageproc::tests {
using namespace utils;

TEST(ShearTestSuite, test_small_image) {
  static const std::array<int, 81> inp = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0,
                            0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0,
                            0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 81> h_out = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
                              0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0,
                              1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<int, 81> v_out = {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
                              0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
                              0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const BinaryImage hOutImg(makeBinaryImage(h_out.data(), 9, 9));
  const BinaryImage vOutImg(makeBinaryImage(v_out.data(), 9, 9));

  const BinaryImage hShear = imageproc::hShear(img, -1.0, 0.5 * img.height(), BWColor::WHITE);
  ASSERT_TRUE(hShear == hOutImg);

  const BinaryImage vShear = imageproc::vShear(img, 1.0, 0.5 * img.width(), BWColor::WHITE);
  ASSERT_TRUE(vShear == vOutImg);

  BinaryImage hShearInplace(img);
  hShearInPlace(hShearInplace, -1.0, 0.5 * img.height(), BWColor::WHITE);
  ASSERT_TRUE(hShearInplace == hOutImg);

  BinaryImage vShearInplace(img);
  vShearInPlace(vShearInplace, 1.0, 0.5 * img.width(), BWColor::WHITE);
  ASSERT_TRUE(vShearInplace == vOutImg);
}
}
