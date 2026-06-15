// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <BinaryImage.h>
#include <Grayscale.h>

#include <QImage>
#include <QRect>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>

namespace imageproc::tests::utils {
inline BinaryImage randomBinaryImage(int width, int height)
{
  BinaryImage image(width, height);

  uint32_t* pword = image.data();

  const auto offset =
      static_cast<std::ptrdiff_t>(image.height()) *
      static_cast<std::ptrdiff_t>(image.wordsPerLine());

  const uint32_t* const end = pword + offset;

  static std::mt19937 rng{std::random_device{}()};
  static std::uniform_int_distribution<uint32_t> dist(
      0U, std::numeric_limits<uint32_t>::max());

  for (; pword != end; ++pword) {
    *pword = dist(rng);
  }

  return image;
}

inline QImage randomMonoQImage(int width, int height)
{
  QImage image(width, height, QImage::Format_Mono);

  image.setColorCount(2);
  image.setColor(0, 0xffffffff);
  image.setColor(1, 0xff000000);

  // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
  auto* pword = reinterpret_cast<uint32_t*>(image.bits());
  // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

  assert(image.bytesPerLine() % 4 == 0);

  const auto offset =
      static_cast<std::ptrdiff_t>(image.height()) *
      static_cast<std::ptrdiff_t>(image.bytesPerLine() / 4);

  const uint32_t* const end = pword + offset;

  static std::mt19937 rng{std::random_device{}()};
  static std::uniform_int_distribution<uint32_t> dist(
      0U, std::numeric_limits<uint32_t>::max());

  for (; pword != end; ++pword) {
    *pword = dist(rng);
  }

  return image;
}

inline QImage randomGrayImage(int width, int height)
{
  QImage img(width, height, QImage::Format_Indexed8);
  img.setColorTable(createGrayscalePalette());

  static std::mt19937 rng{std::random_device{}()};
  static std::uniform_int_distribution<int> dist(0, 9);

  for (int y = 0; y < height; ++y) {
    auto* line = img.scanLine(y);
    for (int x = 0; x < width; ++x) {
      line[x] = static_cast<uchar>(dist(rng));
    }
  }

  return img;
}

inline QImage makeMonoQImage(const int* data, const int width, const int height) {
  QImage img(width, height, QImage::Format_Mono);
  img.setColorCount(2);
  img.setColor(0, 0xffffffff);
  img.setColor(1, 0xff000000);
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      img.setPixel(x, y, data[y * width + x] ? 1 : 0);
    }
  }
  return img;
}

inline BinaryImage makeBinaryImage(const int* data, const int width, const int height) {
  return BinaryImage(makeMonoQImage(data, width, height));
}

inline QImage makeGrayImage(const int* data, const int width, const int height) {
  QImage img(width, height, QImage::Format_Indexed8);
  img.setColorTable(createGrayscalePalette());
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      img.setPixel(x, y, data[y * width + x]);
    }
  }
  return img;
}

inline void dumpBinaryImage(const BinaryImage& img, const char* name)
{
  static constexpr std::uint32_t ONE = 1U;
  static constexpr std::size_t WORD_BITS = 32;

  if (name) {
    std::cout << name << " = ";
  }

  if (img.isNull()) {
    std::cout << "NULL image\n";
    return;
  }

  const auto width = static_cast<std::size_t>(img.width());
  const auto height = static_cast<std::size_t>(img.height());

  const uint32_t* line = img.data();
  const auto wpl = static_cast<std::size_t>(img.wordsPerLine());

  std::cout << "{\n";

  for (std::size_t y = 0; y < height; ++y, line += wpl) {
    std::cout << "\t";

    for (std::size_t x = 0; x < width; ++x) {
      const std::size_t wordIndex = x / WORD_BITS;
      const std::size_t bitIndex  = WORD_BITS - 1 - (x % WORD_BITS);

      const uint32_t word = line[wordIndex];
      const uint32_t bit  = (word >> bitIndex) & ONE;

      std::cout << bit << ", ";
    }

    std::cout << "\n";
  }

  std::cout << "}\n";
}


inline void dumpGrayImage(const QImage& img, const char* name) {
  if (name) {
    std::cout << name << " = ";
  }

  if (img.isNull()) {
    std::cout << "NULL image\n";
    return;
  }
  if (img.format() != QImage::Format_Indexed8) {
    std::cout << "Not grayscale image\n";
  }

  const int width = img.width();
  const int height = img.height();

  std::cout << "{\n";
  for (int y = 0; y < height; ++y) {
    std::cout << "\t";
    for (int x = 0; x < width; ++x) {
      std::cout << img.pixelIndex(x, y) << ", ";
    }
    std::cout << "\n";
  }
  std::cout << "}\n";
}

inline bool surroundingsIntact(const QImage& img1, const QImage& img2, const QRect& rect) {
  assert(img1.size() == img2.size());

  const int w = img1.width();
  const int h = img1.height();

  if (rect.left() != 0) {
    const QRect leftOf(0, 0, rect.x(), h);
    if (img1.copy(leftOf) != img2.copy(leftOf)) {
      return false;
    }
  }

  if (rect.right() != img1.rect().right()) {
    const QRect rightOf(rect.x() + w, 0, w - rect.x() - rect.width(), h);
    if (img1.copy(rightOf) != img2.copy(rightOf)) {
      return false;
    }
  }

  if (rect.top() != 0) {
    const QRect topOf(0, 0, w, rect.y());
    if (img1.copy(topOf) != img2.copy(topOf)) {
      return false;
    }
  }

  if (rect.bottom() != img1.rect().bottom()) {
    const QRect bottomOf(0, rect.y() + rect.height(), w, h - rect.y() - rect.height());
    if (img1.copy(bottomOf) != img2.copy(bottomOf)) {
      return false;
    }
  }
  return true;
}
}
