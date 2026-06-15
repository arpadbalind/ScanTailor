// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include <QImage>

#include <array>
#include <gtest/gtest.h>
#include <cstddef>
#include <stdexcept>

#include "BinaryImage.h"
#include "SlicedHistogram.h"
#include "Utils.h"

namespace {
using namespace imageproc;
static bool checkHistogram(const SlicedHistogram& hist, const int* dataBegin, const int* dataEnd) {
  if (hist.size() != static_cast<size_t>(dataEnd - dataBegin)) {
    return false;
  }
  for (unsigned i = 0; i < hist.size(); ++i) {
    if (hist[i] != dataBegin[i]) {
      return false;
    }
  }
  return true;
}
}

namespace imageproc::tests {
using namespace utils;

TEST(SlicedHistogramTestSuite, test_null_image) {
  const BinaryImage nullImg;

  const SlicedHistogram horHist(nullImg, SlicedHistogram::Type::ROWS);
  EXPECT_TRUE(horHist.size() == 0);

  const SlicedHistogram verHist(nullImg, SlicedHistogram::Type::COLS);
  EXPECT_TRUE(verHist.size() == 0);
}

TEST(SlicedHistogramTestSuite, test_exceeding_area) {
  const BinaryImage img(1, 1);
  const QRect area(0, 0, 1, 2);

  EXPECT_THROW(SlicedHistogram(img, area, SlicedHistogram::Type::ROWS), std::invalid_argument);
}

TEST(SlicedHistogramTestSuite, test_small_image) {
  static const std::array<int, 81> inp = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
                            1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0,
                            0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};

  static const std::array<int, 9> hor_counts = {0, 1, 2, 3, 9, 2, 6, 3, 1};

  static const std::array<int, 9> ver_counts = {2, 2, 4, 4, 5, 3, 2, 3, 2};

  const BinaryImage img(makeBinaryImage(inp.data(), 9, 9));

  SlicedHistogram horHist(img, SlicedHistogram::Type::ROWS);
  EXPECT_TRUE(checkHistogram(horHist, hor_counts.begin(), hor_counts.end()));

  SlicedHistogram verHist(img, SlicedHistogram::Type::COLS);
  EXPECT_TRUE(checkHistogram(verHist, ver_counts.begin(), ver_counts.end()));

  horHist = SlicedHistogram(img, img.rect().adjusted(0, 1, 0, 0), SlicedHistogram::Type::ROWS);
  EXPECT_TRUE(checkHistogram(horHist, hor_counts.begin() + 1, hor_counts.end()));

  verHist = SlicedHistogram(img, img.rect().adjusted(1, 0, 0, 0), SlicedHistogram::Type::COLS);
  EXPECT_TRUE(checkHistogram(verHist, ver_counts.begin() + 1, ver_counts.end()));
}
}
