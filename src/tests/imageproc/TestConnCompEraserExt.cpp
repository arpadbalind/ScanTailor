// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include <QImage>

#include <algorithm>
#include <array>
#include <gtest/gtest.h>
#include <list>

#include "BWColor.h"
#include "BinaryImage.h"
#include "ConnComp.h"
#include "ConnCompEraser.h"
#include "ConnCompEraserExt.h"
#include "Connectivity.h"
#include "RasterOp.h"
#include "Utils.h"

namespace {
using namespace imageproc;
static bool checkAlignedImage(const ConnCompEraserExt& eraser, const BinaryImage& nonaligned) {
  const BinaryImage aligned(eraser.computeConnCompImageAligned());
  const int pad = aligned.width() - nonaligned.width();
  if (pad < 0) {
    return false;
  }

  BinaryImage test1(nonaligned);
  BinaryImage empty1(test1.size());
  empty1.fill(BWColor::WHITE);
  rasterOp<RopXor<RopSrc, RopDst>>(test1, test1.rect(), aligned, QPoint(pad, 0));
  if (test1 != empty1) {
    return false;
  }

  if (pad > 0) {
    // Check that padding is white.
    BinaryImage test2(pad, nonaligned.height());
    BinaryImage empty2(test2.size());
    empty2.fill(BWColor::WHITE);
    rasterOp<RopSrc>(test2, test2.rect(), aligned, QPoint(0, 0));
    if (test2 != empty2) {
      return false;
    }
  }
  return true;
}
}

namespace imageproc::tests {
using namespace utils;

TEST(ConnCompEraserExtTestSuite, test_null_image) {
  ConnCompEraser eraser(BinaryImage(), Connectivity::CONN4);
  EXPECT_TRUE(eraser.nextConnComp().isNull());
}

TEST(ConnCompEraserExtTestSuite, test_small_image) {
  static const std::array<int, 72> inp = {
      0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0,
      0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0};

  std::list<BinaryImage> c4i;

  static const std::array<int, 18> out4_1 = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0};
  c4i.push_back(makeBinaryImage(out4_1.data(), 3, 6));

  static const std::array<int, 2> out4_2 = {1, 1};
  c4i.push_back(makeBinaryImage(out4_2.data(), 2, 1));

  static const std::array<int, 14> out4_3 = {0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1};
  c4i.push_back(makeBinaryImage(out4_3.data(), 7, 2));

  static const std::array<int, 12> out4_4 = {1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0};
  c4i.push_back(makeBinaryImage(out4_4.data(), 4, 3));

  static const std::array<int, 1> out4_5 = {1};
  c4i.push_back(makeBinaryImage(out4_5.data(), 1, 1));

  static const std::array<int, 1> out4_6 = {1};
  c4i.push_back(makeBinaryImage(out4_6.data(), 1, 1));

  std::list<BinaryImage> c8i;

  static const std::array<int, 54> out8_1 = {
      0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1,
      1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0};
  c8i.push_back(makeBinaryImage(out8_1.data(), 9, 6));

  static const std::array<int, 16> out8_2 = {0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0};
  c8i.push_back(makeBinaryImage(out8_2.data(), 8, 2));

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 8));

  ConnComp cc;
  ConnCompEraserExt eraser4(img, Connectivity::CONN4);
  while (!(cc = eraser4.nextConnComp()).isNull()) {
    const BinaryImage ccImg(eraser4.computeConnCompImage());
    auto it = std::ranges::find(c4i, ccImg);
    if (it != c4i.end()) {
      EXPECT_TRUE(checkAlignedImage(eraser4, ccImg));
      c4i.erase(it);
    } else {
      ADD_FAILURE() << "Incorrect 4-connected block found.";
    }
  }
  EXPECT_TRUE(c4i.empty()) << "Not all 4-connected blocks were found.";

  ConnCompEraserExt eraser8(img, Connectivity::CONN8);
  while (!(cc = eraser8.nextConnComp()).isNull()) {
    const BinaryImage ccImg(eraser8.computeConnCompImage());
    auto it = std::ranges::find(c8i, ccImg);
    if (it != c8i.end()) {
      EXPECT_TRUE(checkAlignedImage(eraser8, ccImg));
      c8i.erase(it);
    } else {
      ADD_FAILURE() << "Incorrect 8-connected block found.";
    }
  }
  EXPECT_TRUE(c8i.empty()) << "Not all 8-connected blocks were found.";
}

}
