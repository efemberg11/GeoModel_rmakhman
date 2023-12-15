/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoAbsPositionInfo.h"
#include "GeoModelKernel/GeoDefinitions.h"

void GeoAbsPositionInfo::clearAbsTransform () {
    m_absTransform.reset();
}

void GeoAbsPositionInfo::clearDefAbsTransform () {
    m_defAbsTransform.reset();
}

void GeoAbsPositionInfo::setAbsTransform (const GeoTrf::Transform3D &  xform)
{
  if (m_absTransform) {
    (*m_absTransform) = xform;
  } else {
    m_absTransform = std::make_unique<GeoTrf::Transform3D> (xform);
  }
}

void GeoAbsPositionInfo::setDefAbsTransform (const GeoTrf::Transform3D& xform) {
  if (m_defAbsTransform) {
      (*m_defAbsTransform) = xform;
  } else  {
    m_defAbsTransform =  std::make_unique<GeoTrf::Transform3D>(xform);
  }
}
