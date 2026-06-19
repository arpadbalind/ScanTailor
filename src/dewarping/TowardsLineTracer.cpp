// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "TowardsLineTracer.h"

#include <QPoint>

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>

#include "Grid.h"
#include "NumericTraits.h"
#include "SEDM.h"
#include "SidesOfLine.h"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
using namespace imageproc;

namespace dewarping {
TowardsLineTracer::TowardsLineTracer(const imageproc::SEDM* dm,
                                     const Grid<float>* pm,
                                     const QLineF& line,
                                     const QPoint& initialPos)
    : m_dmData(dm->data()),
      m_dmStride(dm->stride()),
      m_pmData(pm->data()),
      m_pmStride(pm->stride()),
      m_rect(QPoint(0, 0), dm->size()),
      m_line(line),
      m_normalTowardsLine(m_line.normalVector().p2() - m_line.p1()),
      m_lastOutputPos(initialPos) {
  if (sidesOfLine(m_line, initialPos, QPointF(Vec2d(line.p1()) + m_normalTowardsLine)) > 0) {
    // It points the wrong way -> fix that.
    m_normalTowardsLine = -m_normalTowardsLine;
  }

  setupSteps();
}

const QPoint* TowardsLineTracer::trace(const float maxDist) {
  if (m_finished) {
    return nullptr;
  }

  const auto maxSqdist = static_cast<int>(std::lround(maxDist * maxDist));
  QPoint curPos(m_lastOutputPos);

  const uint32_t* pDm = m_dmData + static_cast<std::ptrdiff_t>(curPos.y()) * m_dmStride + curPos.x();
  const float* pPm = m_pmData + static_cast<std::ptrdiff_t>(curPos.y()) * m_pmStride + curPos.x();

  while (true) {
    int bestDmIdx = -1;
    int bestPmIdx = -1;
    uint32_t bestSquaredDist = 0;
    float bestProbability = NumericTraits<float>::min();

    for (int i = 0; i < m_numSteps; ++i) {
      const Step& step = m_steps[i];
      const QPoint newPos(curPos + step.vec);
      if (!m_rect.contains(newPos)) {
        continue;
      }

      const uint32_t sqd = pDm[step.dmOffset];
      if (sqd > bestSquaredDist) {
        bestSquaredDist = sqd;
        bestDmIdx = i;
      }
      const float probability = pPm[step.pmOffset];
      if (probability > bestProbability) {
        bestProbability = probability;
        bestPmIdx = i;
      }
    }

    if (bestDmIdx == -1) {
      m_finished = true;
      break;
    }
    assert(bestPmIdx != -1);

    int bestIdx = bestPmIdx;
    if (pDm[m_steps[bestDmIdx].dmOffset] > *pDm) {
      bestIdx = bestDmIdx;
    }

    const Step& step = m_steps[bestIdx];

    if (sidesOfLine(m_line, curPos + step.vec, m_lastOutputPos) < 0) {
      // Note that this has to be done before we update curPos,
      // as it will be used after breaking from this loop.
      m_finished = true;
      break;
    }

    curPos += step.vec;
    pDm += step.dmOffset;
    pPm += step.pmOffset;

    const QPoint vec(curPos - m_lastOutputPos);
    if (vec.x() * vec.x() + vec.y() * vec.y() > maxSqdist) {
      m_lastOutputPos = curPos;
      return &m_lastOutputPos;
    }
  }

  if (m_lastOutputPos != curPos) {
    m_lastOutputPos = curPos;
    return &m_lastOutputPos;
  } else {
    return nullptr;
  }
}  // TowardsLineTracer::trace

void TowardsLineTracer::setupSteps() {
  std::array<QPoint, 8> all_directions{};
  // all_directions[0] is north-west, and then clockwise from there.
  static const std::array<int, 3> m0p = {-1, 0, 1};
  static const std::array<int, 3> p0m = {1, 0, -1};
  constexpr int numDirections{ 8 };
  for (int i = 0; i < 3; ++i) {
    // north
    all_directions[i].setX(m0p[i]);
    all_directions[i].setY(-1);

    // east
    all_directions[2 + i].setX(1);
    all_directions[2 + i].setY(m0p[i]);

    // south
    all_directions[4 + i].setX(p0m[i]);
    all_directions[4 + i].setY(1);

    // west
    all_directions[(6 + i) % numDirections].setX(-1);
    all_directions[(6 + i) % numDirections].setY(p0m[i]);
  }

  m_numSteps = 0;
  for (const QPoint dir : all_directions) {
    if (m_normalTowardsLine.dot(Vec2d(QPointF(dir))) > 0.0) {
      Step& step = m_steps[m_numSteps];
      step.vec = dir;
      step.unitVec = Vec2d(step.vec.x(), step.vec.y());
      step.unitVec /= std::sqrt(step.unitVec.squaredNorm());
      step.dmOffset = step.vec.y() * m_dmStride + step.vec.x();
      step.pmOffset = step.vec.y() * m_pmStride + step.vec.x();
      ++m_numSteps;
    }
  }

  // Sort by decreasing alignment with m_normalTowardsLine.
  std::ranges::sort(m_steps, [this](const auto& a, const auto& b) {
              return a.unitVec.dot(m_normalTowardsLine) > b.unitVec.dot(m_normalTowardsLine);
            });
}  // TowardsLineTracer::setupSteps
}  // namespace dewarping
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
