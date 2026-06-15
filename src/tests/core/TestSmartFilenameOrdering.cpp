// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include <QFileInfo>
#include <QString>

#include <gtest/gtest.h>

#include "SmartFilenameOrdering.h"

namespace Tests {

TEST(SmartFilenameOrderingTestSuite, test_same_file) {
  const SmartFilenameOrdering less;
  const QFileInfo somefile("/etc/somefile");
  EXPECT_FALSE(less(somefile, somefile));
}

TEST(SmartFilenameOrderingTestSuite, test_dirs_different) {
  const SmartFilenameOrdering less;
  const QFileInfo lhs("/etc/file");
  const QFileInfo rhs("/ect/file");
  EXPECT_TRUE(less(lhs, rhs) == (lhs.absolutePath() < rhs.absolutePath()));
  EXPECT_TRUE(less(rhs, lhs) == (rhs.absolutePath() < lhs.absolutePath()));
}

TEST(SmartFilenameOrderingTestSuite, test_simple_case) {
  const SmartFilenameOrdering less;
  const QFileInfo lhs("/etc/1.png");
  const QFileInfo rhs("/etc/2.png");
  EXPECT_TRUE(less(lhs, rhs));
  EXPECT_FALSE(less(rhs, lhs));
}

TEST(SmartFilenameOrderingTestSuite, test_avg_case) {
  const SmartFilenameOrdering less;
  const QFileInfo lhs("/etc/a_0002.png");
  const QFileInfo rhs("/etc/a_1.png");
  EXPECT_FALSE(less(lhs, rhs));
  EXPECT_TRUE(less(rhs, lhs));
}

TEST(SmartFilenameOrderingTestSuite, test_complex_case) {
  const SmartFilenameOrdering less;
  const QFileInfo lhs("/etc/a10_10.png");
  const QFileInfo rhs("/etc/a010_2.png");
  EXPECT_FALSE(less(lhs, rhs));
  EXPECT_TRUE(less(rhs, lhs));
}

TEST(SmartFilenameOrderingTestSuite, test_almost_equal) {
  const SmartFilenameOrdering less;
  const QFileInfo lhs("/etc/10.png");
  const QFileInfo rhs("/etc/010.png");
  EXPECT_FALSE(less(lhs, rhs));
  EXPECT_TRUE(less(rhs, lhs));
}

}  // namespace Tests   
