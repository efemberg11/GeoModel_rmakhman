/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoShapeUnion.h"
#include "GeoModelKernel/GeoShapeIntersection.h"
#include "GeoModelKernel/GeoShapeAction.h"
#include "GeoModelKernel/GeoPolyhedron.h"
#include "GeoModelKernel/GeoPolyhedrizeAction.h"
#include <stdexcept>

const std::string GeoShapeUnion::s_classType = "Union";
const ShapeType GeoShapeUnion::s_classTypeID = 0x01;

GeoShapeUnion::GeoShapeUnion (const GeoShape* A, const GeoShape* B):
   m_opA{A}, m_opB{B} {}

double GeoShapeUnion::volume (int npoints) const
{
  double vol = getVolumeValue();
  if (vol < 0)
  {
    double xminA, yminA, zminA, xmaxA, ymaxA, zmaxA;
    double xminB, yminB, zminB, xmaxB, ymaxB, zmaxB;
    m_opA->extent(xminA, yminA, zminA, xmaxA, ymaxA, zmaxA);
    m_opB->extent(xminB, yminB, zminB, xmaxB, ymaxB, zmaxB);
    bool noIntersection =
      xminA >= xmaxB || yminA >= ymaxB || zminA >= zmaxB ||
      xminB >= xmaxA || yminB >= ymaxA || zminB >= zmaxA;
    if (noIntersection) {
      vol = m_opA->volume(npoints) + m_opB->volume(npoints);
    } else {
      if (getNoConstituents() > 10) {
        vol = GeoShape::volume(npoints);
      } else {
        GeoIntrusivePtr<GeoShapeIntersection> tmp(new GeoShapeIntersection(m_opA, m_opB));
        vol = m_opA->volume(npoints) + m_opB->volume(npoints) - tmp->volume(npoints);
      }
    }
    setVolumeValue(vol);
  }
  return vol;
}

void GeoShapeUnion::extent (double& xmin, double& ymin, double& zmin,
                            double& xmax, double& ymax, double& zmax) const
{
  double xminA{0.}, yminA{0.}, zminA{0.}, xmaxA{0.}, ymaxA{0.}, zmaxA{0.};
  double xminB{0.}, yminB{0.}, zminB{0.}, xmaxB{0.}, ymaxB{0.}, zmaxB{0.};
  getOpA()->extent(xminA, yminA, zminA, xmaxA, ymaxA, zmaxA);
  getOpB()->extent(xminB, yminB, zminB, xmaxB, ymaxB, zmaxB);
  xmin = std::min(xminA, xminB);
  ymin = std::min(yminA, yminB);
  zmin = std::min(zminA, zminB);
  xmax = std::max(xmaxA, xmaxB);
  ymax = std::max(ymaxA, ymaxB);
  zmax = std::max(zmaxA, zmaxB);
}

bool GeoShapeUnion::contains (double x, double y, double z) const {
  return getOpA()->contains(x, y, z) || getOpB()->contains(x, y, z);
}

void GeoShapeUnion::exec (GeoShapeAction *action) const
{
  action->getPath ()->push (this);
  action->handleUnion (this);
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
    m_opA->exec(action);
    if (action->shouldTerminate ())
    {
      action->getPath ()->pop ();
      return;
    }
    m_opB->exec(action);
    if (action->shouldTerminate ())
    {
      action->getPath ()->pop ();
      return;
    }
  }
  action->getPath()->pop();
}
