// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include "ImageId.h"

class QString;

/**
 * \brief A logical page on an image.
 *
 * An image can contain one or two logical pages.
 */
class PageId {
  // Member-wise copying is OK.
 public:
  enum class SubPage : std::uint8_t {
    SINGLE_PAGE,
    LEFT_PAGE,
    RIGHT_PAGE
  };

  PageId() = default;

  /**
   * \note The default parameter for subpage is not arbitrary.  It has to
   *       preceed other values in terms of operator<().  That's necessary
   *       to be able to use lower_bound() to find the first page with
   *       a matching image id.
   */
  explicit PageId(const ImageId& imageId, SubPage subpage = SubPage::SINGLE_PAGE);

  [[nodiscard]] bool isNull() const { return m_imageId.isNull(); }

  ImageId& imageId() { return m_imageId; }

  [[nodiscard]] const ImageId& imageId() const { return m_imageId; }

  [[nodiscard]] SubPage subPage() const { return m_subPage; }

  [[nodiscard]] QString subPageAsString() const { return subPageToString(m_subPage); }

  static QString subPageToString(SubPage subPage);

  static SubPage subPageFromString(const QString& string, bool* ok = nullptr);

 private:
  ImageId m_imageId;
  SubPage m_subPage{ SubPage::SINGLE_PAGE };
};


bool operator==(const PageId& lhs, const PageId& rhs);

bool operator!=(const PageId& lhs, const PageId& rhs);

bool operator<(const PageId& lhs, const PageId& rhs);

namespace std {

template<>
struct hash<PageId::SubPage>
{
  size_t operator()(PageId::SubPage sp) const noexcept
  {
    return hash<std::underlying_type_t<PageId::SubPage>>{}(
        static_cast<std::underlying_type_t<PageId::SubPage>>(sp));
  }
};

template <>
struct hash<PageId> {
  size_t operator()(const PageId& pageId) const noexcept {
    // NOLINTNEXTLINE(bugprone-signed-bitwise)
    return (hash<ImageId>()(pageId.imageId()) ^ hash<PageId::SubPage>()(pageId.subPage()) << 1u);
  }
};
}  // namespace std
