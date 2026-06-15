// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

// NOLINTBEGIN(misc-include-cleaner)
#include <QImage>
#include <QPoint>
#include <QRect>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <gtest/gtest.h>
#include <random>
#include <ranges>
#include <vector>

#include "BinaryImage.h"
#include "RasterOp.h"
#include "Utils.h"

namespace {
using namespace imageproc;

template <typename Rop>
static bool checkSubimageRop(const QImage& dst, const QRect& dstRect, const QImage& src, const QPoint& srcPt) {
  BinaryImage dstBi(dst);
  const BinaryImage srcBi(src);
  rasterOp<Rop>(dstBi, dstRect, srcBi, srcPt);
  // Here we assume that full-image raster operations work correctly.
  BinaryImage dstSubimage(dst.copy(dstRect));
  const QRect srcRect(srcPt, dstRect.size());
  const BinaryImage srcSubimage(src.copy(srcRect));
  rasterOp<Rop>(dstSubimage, dstSubimage.rect(), srcSubimage, QPoint(0, 0));
  return dstSubimage.toQImage() == dstBi.toQImage().copy(dstRect);
}
}

namespace imageproc::tests {
using namespace utils;

TEST(RasterOpTestSuite, test_small_image) {
  static const std::array<int, 72> inp
      = {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0,
         0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0};

  static const std::array<int, 72> mask
      = {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
         0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0};

  static const std::array<int, 72> out
      = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
         0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0};

  BinaryImage img(makeBinaryImage(inp.data(), 9, 8));
  const BinaryImage maskImg(makeBinaryImage(mask.data(), 9, 8));

  using Rop = RopAnd<RopDst, RopSrc>;

  rasterOp<Rop>(img, img.rect(), maskImg, QPoint(0, 0));
  EXPECT_TRUE(img == makeBinaryImage(out.data(), 9, 8));
}

namespace {
class Tester1 {
 public:
  Tester1();

  [[nodiscard]] bool testFullImage() const;

  [[nodiscard]] bool testSubImage(const QRect& dstRect, const QPoint& srcPt) const;

 private:
  using Rop = RopXor<RopDst, RopSrc>;

  BinaryImage m_src;
  BinaryImage m_dstBefore;
  BinaryImage m_dstAfter;
};


Tester1::Tester1() {
  const size_t w{ 400 };
  const size_t h{ 300 };

  std::vector<int32_t> src(w * h);
  std::vector<int32_t> dst(w * h);
  std::vector<int32_t> res(w * h);

  static thread_local std::mt19937_64 engine{std::random_device{}()};
  static thread_local std::uniform_int_distribution<int32_t> distribution{0, 1};

  std::ranges::generate(src, [&]() { return distribution(engine); });
  std::ranges::generate(dst, [&]() { return distribution(engine); });

  std::ranges::copy(
      std::views::zip_transform(
          [](int32_t s, int32_t d) { return Rop::transform(s, d) & 1u; },
          src,
          dst
          ),
      res.begin()
      );

  m_src = makeBinaryImage(&src[0], w, h);
  m_dstBefore = makeBinaryImage(&dst[0], w, h);
  m_dstAfter = makeBinaryImage(&res[0], w, h);
}

bool Tester1::testFullImage() const {
  BinaryImage dst(m_dstBefore);
  rasterOp<Rop>(dst, dst.rect(), m_src, QPoint(0, 0));
  return dst == m_dstAfter;
}

bool Tester1::testSubImage(const QRect& dstRect, const QPoint& srcPt) const {
  const QImage dstBefore(m_dstBefore.toQImage());
  const QImage& dst(dstBefore);
  const QImage src(m_src.toQImage());

  if (!checkSubimageRop<Rop>(dst, dstRect, src, srcPt)) {
    return false;
  }
  return surroundingsIntact(dst, dstBefore, dstRect);
}
}  // namespace
TEST(RasterOpTestSuite, test_large_image) {
  const Tester1 tester;
  ASSERT_TRUE(tester.testFullImage());
  ASSERT_TRUE(tester.testSubImage(QRect(101, 32, 211, 151), QPoint(101, 41)));
  ASSERT_TRUE(tester.testSubImage(QRect(101, 32, 211, 151), QPoint(99, 99)));
  ASSERT_TRUE(tester.testSubImage(QRect(101, 32, 211, 151), QPoint(104, 64)));
}

namespace {
class Tester2 {
 public:
  Tester2();

  bool testBlockMove(const QRect& rect, int dx, int dy);

 private:
  BinaryImage m_image;
};


Tester2::Tester2() {
  m_image = randomBinaryImage(400, 300);
}

bool Tester2::testBlockMove(const QRect& rect, const int dx, const int dy) {
  BinaryImage dst(m_image);
  const QRect dstRect(rect.translated(dx, dy));
  rasterOp<RopSrc>(dst, dstRect, dst, rect.topLeft());
  const QImage qSrc(m_image.toQImage());
  const QImage qDst(dst.toQImage());
  if (qSrc.copy(rect) != qDst.copy(dstRect)) {
    return false;
  }
  return surroundingsIntact(qDst, qSrc, dstRect);
}
}  // namespace
TEST(RasterOpTestSuite, test_move_blocks) {
  Tester2 tester;
  ASSERT_TRUE(tester.testBlockMove(QRect(0, 0, 97, 150), 1, 0));
  ASSERT_TRUE(tester.testBlockMove(QRect(100, 50, 15, 100), -1, 0));
  ASSERT_TRUE(tester.testBlockMove(QRect(200, 200, 200, 100), -1, -1));
  ASSERT_TRUE(tester.testBlockMove(QRect(51, 35, 199, 200), 0, 1));
  ASSERT_TRUE(tester.testBlockMove(QRect(51, 35, 199, 200), 1, 1));
}
}
// NOLINTEND(misc-include-cleaner)
