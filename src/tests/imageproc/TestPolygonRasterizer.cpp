// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

// NOLINTBEGIN(misc-include-cleaner)
// TODO: remove no linting in qt6
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <QPolygonF>
#include <QRectF>
#include <QSize>
#include <Qt>
#include <gtest/gtest.h>
#include <cmath>
#include <numbers>

#include "BWColor.h"
#include "BinaryImage.h"
#include "BinaryThreshold.h"
#include "PolygonRasterizer.h"
#include "RasterOp.h"

namespace {
using namespace imageproc;
static QPolygonF createShape(const QSize& imageSize, double radius) {
  const QPointF center(0.5 * imageSize.width(), 0.5 * imageSize.height());
  const double PI {std::numbers::pi};
  double angle = PI / 2.0;
  const int numSteps = 5;
  const double step = PI * 2.0 / numSteps;

  QPolygonF poly;

  poly.push_back(center + QPointF(std::cos(angle), std::sin(angle)) * radius);
  for (int i = 1; i < numSteps; ++i) {
    angle += step * 2;
    poly.push_back(center + QPointF(std::cos(angle), std::sin(angle)) * radius);
  }
  return poly;
}

static bool fuzzyCompare(const BinaryImage& img, const QImage& control) {
  // Make two binary images from the QImage with slightly different thresholds.
  BinaryImage control1(control, BinaryThreshold(128 - 30));
  BinaryImage control2(control, BinaryThreshold(128 + 30));

          // Take the difference with each control image.
  rasterOp<RopXor<RopSrc, RopDst>>(control1, img);
  rasterOp<RopXor<RopSrc, RopDst>>(control2, img);

          // Are there pixels different in both cases?
  rasterOp<RopAnd<RopSrc, RopDst>>(control1, control2);
  return control1.countBlackPixels() == 0;
}

static bool testFillShape(const QSize& imageSize, const QPolygonF& shape, Qt::FillRule fillRule) {
  BinaryImage bImage(imageSize, BWColor::WHITE);
  PolygonRasterizer::fill(bImage, BWColor::BLACK, shape, fillRule);

  QImage qImage(imageSize, QImage::Format_RGB32);
  qImage.fill(0xffffffff);

  {
    QPainter painter(&qImage);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor(0x00, 0x00, 0x00));
    painter.setPen(Qt::NoPen);
    painter.drawPolygon(shape, fillRule);
  }
  return fuzzyCompare(bImage, qImage);
}

static bool testFillExceptShape(const QSize& imageSize, const QPolygonF& shape, Qt::FillRule fillRule) {
  BinaryImage bImage(imageSize, BWColor::WHITE);
  PolygonRasterizer::fillExcept(bImage, BWColor::BLACK, shape, fillRule);

  QImage qImage(imageSize, QImage::Format_RGB32);
  qImage.fill(0x00000000);

  {
    QPainter painter(&qImage);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor(0xff, 0xff, 0xff));
    painter.setPen(Qt::NoPen);
    painter.drawPolygon(shape, fillRule);
  }
  return fuzzyCompare(bImage, qImage);
}
}

namespace imageproc::tests {

TEST(PolygonRasterizerTestSuite, test_complex_shape) {
  const QSize imageSize(500, 500);

  // This one fits the image.
  const QPolygonF smallerShape(createShape(imageSize, 230));

  // This one doesn't fit the image and will be clipped.
  const QPolygonF biggerShape(createShape(imageSize, 300));

  EXPECT_TRUE(testFillShape(imageSize, smallerShape, Qt::OddEvenFill));
  EXPECT_TRUE(testFillShape(imageSize, smallerShape, Qt::WindingFill));
  EXPECT_TRUE(testFillShape(imageSize, biggerShape, Qt::OddEvenFill));
  EXPECT_TRUE(testFillShape(imageSize, biggerShape, Qt::WindingFill));
  EXPECT_TRUE(testFillExceptShape(imageSize, smallerShape, Qt::OddEvenFill));
  EXPECT_TRUE(testFillExceptShape(imageSize, smallerShape, Qt::WindingFill));
  EXPECT_TRUE(testFillExceptShape(imageSize, biggerShape, Qt::OddEvenFill));
  EXPECT_TRUE(testFillExceptShape(imageSize, biggerShape, Qt::WindingFill));
}

TEST(PolygonRasterizerTestSuite, test_corner_cases) {
  const QSize imageSize(500, 500);
  const QPolygonF shape(QRectF(QPointF(0, 0), imageSize));
  const QPolygonF shape2(QRectF(QPointF(-1, -1), imageSize));

  // This one touches clip rectangle's corners.
  QPolygonF shape3;
  shape3.push_back(QPointF(-250.0, 250.0));
  shape3.push_back(QPointF(250.0, -250.0));
  shape3.push_back(QPointF(750.0, -250.0));
  shape3.push_back(QPointF(-250.0, 750.0));

  EXPECT_TRUE(testFillShape(imageSize, shape, Qt::OddEvenFill));
  EXPECT_TRUE(testFillShape(imageSize, shape, Qt::WindingFill));
  EXPECT_TRUE(testFillShape(imageSize, shape2, Qt::OddEvenFill));
  EXPECT_TRUE(testFillShape(imageSize, shape2, Qt::WindingFill));
  EXPECT_TRUE(testFillShape(imageSize, shape3, Qt::OddEvenFill));
  EXPECT_TRUE(testFillShape(imageSize, shape3, Qt::WindingFill));
  EXPECT_TRUE(testFillExceptShape(imageSize, shape, Qt::OddEvenFill));
  EXPECT_TRUE(testFillExceptShape(imageSize, shape, Qt::WindingFill));
  EXPECT_TRUE(testFillExceptShape(imageSize, shape2, Qt::OddEvenFill));
  EXPECT_TRUE(testFillExceptShape(imageSize, shape2, Qt::WindingFill));
  EXPECT_TRUE(testFillExceptShape(imageSize, shape3, Qt::OddEvenFill));
  EXPECT_TRUE(testFillExceptShape(imageSize, shape3, Qt::WindingFill));
}

TEST(PolygonRasterizerTestSuite, regression_test_1) {
  QPolygonF shape;
  shape.push_back(QPointF(937.872, 24.559));
  shape.push_back(QPointF(-1.23235e-14, -1.70697e-15));
  shape.push_back(QPointF(2.73578e-11, 1275.44));
  shape.push_back(QPointF(904.496, 1299.12));
  shape.push_back(QPointF(937.872, 24.559));
  EXPECT_TRUE(testFillExceptShape(QSize(938, 1299), shape, Qt::WindingFill));
}
}
// NOLINTEND(misc-include-cleaner)
