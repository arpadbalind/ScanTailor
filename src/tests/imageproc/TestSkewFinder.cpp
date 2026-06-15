// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include <QColor>
#include <QImage>
#include <QPainter>
#include <QString>
#include <QTransform>

#include <gtest/gtest.h>
#include <random>

#include "BinaryImage.h"
#include "SkewFinder.h"

namespace imageproc::tests {
TEST(SkewFinderTestSuite, test_negative_detection) {
  QImage image(1000, 800, QImage::Format_Mono);
  image.fill(1);

  const int numDots = image.width() * image.height() / 5;

  static thread_local std::mt19937 engine{std::random_device{}()};
  static thread_local std::uniform_int_distribution<int> distX{0, image.width() - 1};
  static thread_local std::uniform_int_distribution<int> distY{0, image.height() - 1};

  for (int i = 0; i < numDots; ++i) {
    const int x = distX(engine);
    const int y = distY(engine);
    image.setPixel(x, y, 0);
  }

  SkewFinder skewFinder;
  skewFinder.setCoarseReduction(0);
  skewFinder.setFineReduction(0);
  const Skew skew(skewFinder.findSkew(BinaryImage(image)));
  EXPECT_TRUE(skew.confidence() < Skew::GOOD_CONFIDENCE);
}

}
