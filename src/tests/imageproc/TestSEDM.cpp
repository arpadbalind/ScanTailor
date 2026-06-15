// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.


#include <QImage>

#include <array>
#include <cstdint>
#include <gtest/gtest.h>
#include <iostream>

#include "BinaryImage.h"
#include "SEDM.h"
#include "Utils.h"

namespace {
using namespace imageproc;
static bool verifySEDM(const SEDM& sedm, const uint32_t* control) {
  const uint32_t* line = sedm.data();
  for (int y = 0; y < sedm.size().height(); ++y) {
    for (int x = 0; x < sedm.size().width(); ++x) {
      if (line[x] != *control) {
        return false;
      }
      ++control;
    }
    line += sedm.stride();
  }
  return true;
}
static void dumpMatrix(const uint32_t* data, QSize size) {
  const int width = size.width();
  const int height = size.height();
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x, ++data) {
      std::cout << *data << ' ';
    }
    std::cout << "\n";
  }
}
}

namespace imageproc::tests {
using namespace utils;

TEST(SEDMTestSuite, test1) {
  static const std::array<int, 81> inp = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
                            0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
                            0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static const std::array<uint32_t, 81> out = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
                                 0, 0, 1, 4, 4, 4, 1, 0, 0, 0, 0, 1, 4, 9, 4, 1, 0, 0, 0, 0, 1, 4, 4, 4, 1, 0, 0,
                                 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));
  const SEDM sedm(img, SEDM::DistType::DIST_TO_WHITE, SEDM::Borders::DIST_TO_NO_BORDERS);
  EXPECT_TRUE(verifySEDM(sedm, out.data()));
}
}
