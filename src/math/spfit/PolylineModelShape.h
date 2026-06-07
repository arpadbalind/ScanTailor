// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#pragma once

#include <QPointF>
#include <vector>

#include "FlagOps.h"
#include "ModelShape.h"
#include "NonCopyable.h"
#include "SqDistApproximant.h"
#include "VecNT.h"
#include "XSpline.h"

namespace spfit {
class PolylineModelShape : public ModelShape, private NonCopyable {

 public:
  enum class Flags : std::uint8_t {
    DEFAULT_FLAGS  = 0,
    POLYLINE_FRONT = 1 << 0,
    POLYLINE_BACK  = 1 << 1,
    MASK_ALL = POLYLINE_FRONT | POLYLINE_BACK
  };

  explicit PolylineModelShape(const std::vector<QPointF>& polyline);

  SqDistApproximant localSqDistApproximant(const QPointF& pt, FittableSpline::SampleFlags sampleFlags) const override;

 protected:
  virtual SqDistApproximant calcApproximant(const QPointF& pt,
                                            FittableSpline::SampleFlags sampleFlags,
                                            Flags polylineFlags,
                                            const FrenetFrame& frenetFrame,
                                            double signedCurvature) const;

 private:
  std::vector<XSpline::PointAndDerivs> m_vertices;
};

constexpr PolylineModelShape::Flags operator|(PolylineModelShape::Flags a, PolylineModelShape::Flags b) noexcept {
  // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
  return static_cast<PolylineModelShape::Flags>(static_cast<std::uint8_t>(a) | static_cast<std::uint8_t>(b));
}
}  // namespace spfit


