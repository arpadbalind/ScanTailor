// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <vector>

template <typename Node>
class Grid {
 public:
  /**
   * Creates a null grid.
   */
  Grid() = default;

  ~Grid() = default;
  /**
   * \brief Creates a width x height grid with specified padding on each side.
   */
  Grid(int width, int height, int padding);

  /**
   * \brief Creates a deep copy of another grid including padding.
   *
   * Stride is also preserved.
   */
  Grid(const Grid& other);

  Grid(Grid&& other) noexcept;

  Grid& operator=(const Grid& other);

  Grid& operator=(Grid&& other) noexcept;

  [[nodiscard]] bool isNull() const { return m_width <= 0 || m_height <= 0; }

  void initPadding(const Node& paddingNode);

  void initInterior(const Node& interiorNode);

  /**
   * \brief Returns a pointer to the beginning of unpadded data.
   */
  Node* data() { return m_data; }

  /**
   * \brief Returns a pointer to the beginning of unpadded data.
   */
  [[nodiscard]] const Node* data() const { return m_data; }

  /**
   * \brief Returns a pointer to the beginning of padded data.
   */
  Node* paddedData() { return m_storage.data(); }

  /**
   * \brief Returns a pointer to the beginning of padded data.
   */
  [[nodiscard]] const Node* paddedData() const { return m_storage.data(); }

  /**
   * Returns the number of nodes in a row, including padding nodes.
   */
  [[nodiscard]] int stride() const { return m_stride; }

  /**
   * Returns the number of nodes in a row, excluding padding nodes.
   */
  [[nodiscard]] int width() const { return m_width; }

  /**
   * Returns the number of nodes in a column, excluding padding nodes.
   */
  [[nodiscard]] int height() const { return m_height; }

  /**
   * Returns the number of padding layers from each side.
   */
   [[nodiscard]] int padding() const { return m_padding; }

  void swap(Grid& other) noexcept;

 private:
  template <typename T>
  static void basicSwap(T& o1, T& o2) {
    // Just to avoid incoduing the heavy <algorithm> header.
    T tmp(o1);
    o1 = o2;
    o2 = tmp;
  }

  std::vector<Node> m_storage;
  Node* m_data{ nullptr };
  int m_width{ 0 };
  int m_height{ 0 };
  int m_stride{ 0 };
  int m_padding{ 0 };
};

template <typename Node>
Grid<Node>::Grid(int width, int height, int padding)
    : m_storage((width + padding * 2) * (height + padding * 2)),
      m_data(m_storage.data() + (width + padding * 2) * padding + padding),
      m_width(width),
      m_height(height),
      m_stride(width + padding * 2),
      m_padding(padding) {}

template <typename Node>
Grid<Node>::Grid(const Grid& other)
    : m_storage((other.stride() * (other.height() + other.padding() * 2))),
      m_data(m_storage.data() + other.stride() * other.padding() + other.padding()),
      m_width(other.width()),
      m_height(other.height()),
      m_stride(other.stride()),
      m_padding(other.padding()) {
  const int len = m_stride * (m_height + m_padding * 2);
  for (int i = 0; i < len; ++i) {
    m_storage[i] = other.m_storage[i];
  }
}

template <typename Node>
Grid<Node>::Grid(Grid&& other) noexcept
    : m_storage(std::move(other.m_storage)),
      m_data(nullptr),
      m_width(other.m_width),
      m_height(other.m_height),
      m_stride(other.m_stride),
      m_padding(other.m_padding)
{
  // Recompute pointer based on moved storage
  m_data = m_storage.data()
           + m_stride * m_padding
           + m_padding;

  // Leave other in a valid state
  other.m_data = nullptr;
  other.m_width = 0;
  other.m_height = 0;
  other.m_stride = 0;
  other.m_padding = 0;
}

template <typename Node>
Grid<Node>& Grid<Node>::operator=(const Grid& other)
{
  if (this == &other) {
    return *this;
  }

  m_storage.resize(other.m_storage.size());
  m_storage = other.m_storage;

  m_width   = other.m_width;
  m_height  = other.m_height;
  m_stride  = other.m_stride;
  m_padding = other.m_padding;

  m_data = m_storage.data()
           + m_stride * m_padding
           + m_padding;

  return *this;
}

template <typename Node>
Grid<Node>& Grid<Node>::operator=(Grid&& other) noexcept
{
  if (this == &other) {
    return *this;
  }

  m_storage = std::move(other.m_storage);

  m_width   = other.m_width;
  m_height  = other.m_height;
  m_stride  = other.m_stride;
  m_padding = other.m_padding;

  m_data = m_storage.data()
           + m_stride * m_padding
           + m_padding;

  other.m_data = nullptr;
  other.m_width = 0;
  other.m_height = 0;
  other.m_stride = 0;
  other.m_padding = 0;

  return *this;
}

template <typename Node>
void Grid<Node>::initPadding(const Node& paddingNode) {
  if (m_padding == 0) {
    // No padding.
    return;
  }

  Node* line = m_storage.data();
  for (int row = 0; row < m_padding; ++row) {
    for (int x = 0; x < m_stride; ++x) {
      line[x] = paddingNode;
    }
    line += m_stride;
  }

  for (int y = 0; y < m_height; ++y) {
    for (int col = 0; col < m_padding; ++col) {
      line[col] = paddingNode;
    }
    for (int col = m_stride - m_padding; col < m_stride; ++col) {
      line[col] = paddingNode;
    }
    line += m_stride;
  }

  for (int row = 0; row < m_padding; ++row) {
    for (int x = 0; x < m_stride; ++x) {
      line[x] = paddingNode;
    }
    line += m_stride;
  }
}  // >::initPadding

template <typename Node>
void Grid<Node>::initInterior(const Node& interiorNode) {
  Node* line = m_data;
  for (int y = 0; y < m_height; ++y) {
    for (int x = 0; x < m_width; ++x) {
      line[x] = interiorNode;
    }
    line += m_stride;
  }
}

template <typename Node>
void Grid<Node>::swap(Grid& other) noexcept {
  m_storage.swap(other.m_storage);
  basicSwap(m_data, other.m_data);
  basicSwap(m_width, other.m_width);
  basicSwap(m_height, other.m_height);
  basicSwap(m_stride, other.m_stride);
  basicSwap(m_padding, other.m_padding);
}

template <typename Node>
void swap(Grid<Node>& o1, Grid<Node>& o2) noexcept {
  o1.swap(o2);
}
