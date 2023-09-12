/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoShapeAction.h"

const std::string GeoBox::s_classType = "Box";
const ShapeType GeoBox::s_classTypeID = 0x10;

GeoBox::GeoBox (double XHalfLength, double YHalfLength, double ZHalfLength)
  : m_xHalfLength (XHalfLength)
  , m_yHalfLength (YHalfLength)
  , m_zHalfLength (ZHalfLength)
{
}

GeoBox::~GeoBox()
{
}

double GeoBox::volume () const
{
  return 8.0 * m_xHalfLength * m_yHalfLength * m_zHalfLength;
}

void GeoBox::extent (double& xmin, double& ymin, double& zmin,
                     double& xmax, double& ymax, double& zmax) const
{
  xmin = -m_xHalfLength;
  ymin = -m_yHalfLength;
  zmin = -m_zHalfLength;
  xmax = m_xHalfLength;
  ymax = m_yHalfLength;
  zmax = m_zHalfLength;
}

bool GeoBox::contains (double x, double y, double z) const
{
  double distx = std::abs(x) - m_xHalfLength;
  double disty = std::abs(y) - m_yHalfLength;
  double distz = std::abs(z) - m_zHalfLength;
  return (std::max(std::max(distx, disty), distz) <= 0.0);
}

const std::string & GeoBox::type () const
{
  return s_classType;
}

ShapeType GeoBox::typeID () const
{
  return s_classTypeID;
}

void GeoBox::exec (GeoShapeAction *action) const
{
  action->handleBox(this);
}
