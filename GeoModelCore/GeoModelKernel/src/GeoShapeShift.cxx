/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelKernel/GeoShapeAction.h"

#include <array>

const std::string GeoShapeShift::s_classType = "Shift";
const ShapeType GeoShapeShift::s_classTypeID = 0x03;

GeoShapeShift::GeoShapeShift (const GeoShape* A, const GeoTrf::Transform3D &X):   
  m_op{A}, m_shift{X} {
    /// Check whether the given shape also a shape shift. If so then we can simply
    /// take the operand of the sub shift and summarize the transformations of the two into one
    if (getOp()->typeID() == typeID()) {
       const GeoShapeShift* subShift{static_cast<const GeoShapeShift*>(getOp())};
       m_shift = subShift->getX() * getX();
       m_op.reset(subShift->getOp());
    }
}


void GeoShapeShift::extent (double& xmin, double& ymin, double& zmin,
                            double& xmax, double& ymax, double& zmax) const
{
  const GeoShape* shape = getOp();
  const GeoTrf::Transform3D& trans = getX();
  double x_min{0.}, y_min{0.}, z_min{0.}, x_max{0.}, y_max{0.}, z_max{0.};
  shape->extent(x_min, y_min, z_min, x_max, y_max, z_max);
  std::array<GeoTrf::Vector3D, 8> vv{};
  vv[0] = trans * GeoTrf::Vector3D(x_min, y_min, z_min);
  vv[1] = trans * GeoTrf::Vector3D(x_max, y_min, z_min);
  vv[2] = trans * GeoTrf::Vector3D(x_min, y_max, z_min);
  vv[3] = trans * GeoTrf::Vector3D(x_max, y_max, z_min);
  vv[4] = trans * GeoTrf::Vector3D(x_min, y_min, z_max);
  vv[5] = trans * GeoTrf::Vector3D(x_max, y_min, z_max);
  vv[6] = trans * GeoTrf::Vector3D(x_min, y_max, z_max);
  vv[7] = trans * GeoTrf::Vector3D(x_max, y_max, z_max);
  xmin = xmax = vv[0].x();
  ymin = ymax = vv[0].y();
  zmin = zmax = vv[0].z();
  for (int i = 1; i < 8; ++i)
  {
    xmin = std::min(xmin, vv[i].x());
    ymin = std::min(ymin, vv[i].y());
    zmin = std::min(zmin, vv[i].z());
    xmax = std::max(xmax, vv[i].x());
    ymax = std::max(ymax, vv[i].y());
    zmax = std::max(zmax, vv[i].z());
  }
}

bool GeoShapeShift::contains (double x, double y, double z) const {
  const GeoShape* shape = getOp();
  const GeoTrf::Transform3D& trans = getX();
  GeoTrf::Vector3D p = trans.inverse() * GeoTrf::Vector3D(x, y, z);
  return shape->contains(p.x(), p.y(), p.z());
}

void GeoShapeShift::exec (GeoShapeAction *action) const {
  action->getPath ()->push (this);
  action->handleShift (this);
  if (action->shouldTerminate ())
    {
      action->getPath ()->pop ();
      return;
    }

  if (action->getDepthLimit ().isValid ()
      && action->getPath ()->getLength () > action->getDepthLimit ())
    {
    }

  else
        {
    m_op->exec(action);
    if (action->shouldTerminate ())
        {
      action->getPath ()->pop ();
      return;
        }
  }
  action->getPath()->pop();
}
