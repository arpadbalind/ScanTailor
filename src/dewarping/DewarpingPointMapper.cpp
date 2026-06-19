// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "DewarpingPointMapper.h"

#include <QPoint>
#include <QPointF>
#include <QTransform>

#include "DistortionModel.h"

namespace dewarping {
DewarpingPointMapper::DewarpingPointMapper(
    const DistortionModel& distortionModel,
    double depthPerception,
    const QTransform& distortionModelToOutput,
    const QRect& outputContentRect,
    QTransform postTransform)
    : m_dewarper(
          CylindricalSurfaceDewarper(
              distortionModel.topCurve().polyline(),
              distortionModel.bottomCurve().polyline(),
              depthPerception)),
      m_postTransform(postTransform)
{
  const QRect modelDomain =
      distortionModel.modelDomain(m_dewarper,
                                  distortionModelToOutput,
                                  outputContentRect).toRect();

  const double xScaleFrom =
      modelDomain.right() - modelDomain.left();

  const double yScaleFrom =
      modelDomain.bottom() - modelDomain.top();

  m_modelDomainLeft = modelDomain.left();
  m_modelDomainTop  = modelDomain.top();

  m_modelXScaleFromNormalized = xScaleFrom;
  m_modelYScaleFromNormalized = yScaleFrom;

  m_modelXScaleToNormalized = 1.0 / xScaleFrom;
  m_modelYScaleToNormalized = 1.0 / yScaleFrom;
}

QPointF DewarpingPointMapper::mapToDewarpedSpace(const QPointF& warpedPt) const {
  const QPointF crvPt(m_dewarper.mapToDewarpedSpace(warpedPt));
  const double dewarpedX = crvPt.x() * m_modelXScaleFromNormalized + m_modelDomainLeft;
  const double dewarpedY = crvPt.y() * m_modelYScaleFromNormalized + m_modelDomainTop;
  return m_postTransform.map(QPointF(dewarpedX, dewarpedY));
}

QPointF DewarpingPointMapper::mapToWarpedSpace(const QPointF& dewarpedPt) const {
  const QPointF dewarpedPtM = m_postTransform.inverted().map(dewarpedPt);

  const double crvX = (dewarpedPtM.x() - m_modelDomainLeft) * m_modelXScaleToNormalized;
  const double crvY = (dewarpedPtM.y() - m_modelDomainTop) * m_modelYScaleToNormalized;
  return m_dewarper.mapToWarpedSpace(QPointF(crvX, crvY));
}
}  // namespace dewarping
