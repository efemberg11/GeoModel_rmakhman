/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoNodeAction.h"

GeoTransform::GeoTransform (const GeoTrf::Transform3D& transform)
  : m_transform(transform)
{
}


GeoTrf::Transform3D GeoTransform::getTransform(const GeoVAlignmentStore* /*store*/) const {
  return m_transform;
}

GeoTrf::Transform3D GeoTransform::getDefTransform(const GeoVAlignmentStore* /*store*/) const {
  return m_transform;
}

void GeoTransform::exec(GeoNodeAction *action) const {
  action->handleTransform(this);
}
