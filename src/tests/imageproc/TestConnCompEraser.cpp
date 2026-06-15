// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include <QImage>

#include <gtest/gtest.h>

#include "BinaryImage.h"
#include "Connectivity.h"
#include "ConnComp.h"
#include "ConnCompEraser.h"

namespace imageproc::tests {
TEST(ConnCompEraserTestSuite, test_null_image) {
  ConnCompEraser eraser(BinaryImage(), Connectivity::CONN4);
  EXPECT_TRUE(eraser.nextConnComp().isNull());
}
}
