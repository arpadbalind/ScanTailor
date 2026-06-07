// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <type_traits>

namespace enumflags {

// ------------------------------------------------------------
// Underlying type helper
// ------------------------------------------------------------
template <typename E>
constexpr auto to_underlying(E e) noexcept {
  return static_cast<std::underlying_type_t<E>>(e);
}

// ------------------------------------------------------------
// operator |
// ------------------------------------------------------------
template <typename E>
  requires std::is_enum_v<E>
constexpr E operator|(E lhs, E rhs) noexcept {
  return static_cast<E>(
      to_underlying(lhs) | to_underlying(rhs)
      );
}

// ------------------------------------------------------------
// operator |=   ⭐ THIS IS WHAT YOU ARE MISSING
// ------------------------------------------------------------
template <typename E>
  requires std::is_enum_v<E>
constexpr E& operator|=(E& lhs, E rhs) noexcept {
  lhs = static_cast<E>(
      to_underlying(lhs) | to_underlying(rhs)
      );
  return lhs;
}

// ------------------------------------------------------------
// operator &
// ------------------------------------------------------------
template <typename E>
  requires std::is_enum_v<E>
constexpr E operator&(E lhs, E rhs) noexcept {
  return static_cast<E>(
      to_underlying(lhs) & to_underlying(rhs)
      );
}

// ------------------------------------------------------------
// operator &=
// ------------------------------------------------------------
template <typename E>
  requires std::is_enum_v<E>
constexpr E& operator&=(E& lhs, E rhs) noexcept {
  lhs = static_cast<E>(
      to_underlying(lhs) & to_underlying(rhs)
      );
  return lhs;
}

// ------------------------------------------------------------
// operator ^
// ------------------------------------------------------------
template <typename E>
  requires std::is_enum_v<E>
constexpr E operator^(E lhs, E rhs) noexcept {
  return static_cast<E>(
      to_underlying(lhs) ^ to_underlying(rhs)
      );
}

// ------------------------------------------------------------
// operator ^=
// ------------------------------------------------------------
template <typename E>
  requires std::is_enum_v<E>
constexpr E& operator^=(E& lhs, E rhs) noexcept {
  lhs = static_cast<E>(
      to_underlying(lhs) ^ to_underlying(rhs)
      );
  return lhs;
}

// ------------------------------------------------------------
// operator ~
// ------------------------------------------------------------
template <typename E>
  requires std::is_enum_v<E>
constexpr E operator~(E v) noexcept {
  return static_cast<E>(
      ~to_underlying(v)
      );
}

// ------------------------------------------------------------
// helper
// ------------------------------------------------------------
template <typename E>
  requires std::is_enum_v<E>
constexpr bool has_flag(E value, E flag) noexcept {
  return (value & flag) == flag;
}

template <typename E>
  requires std::is_enum_v<E>
constexpr bool any_of(E value, E mask) noexcept {
  return (value & mask) != static_cast<E>(0);
}

template <typename E>
  requires std::is_enum_v<E>
constexpr bool has_any(E value, E flags) noexcept {
  return (value & flags) != static_cast<E>(0);
}


} // namespace enumflags
