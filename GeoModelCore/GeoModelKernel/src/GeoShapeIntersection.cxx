/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoShapeIntersection.h"
#include "GeoModelKernel/GeoPolyhedron.h"
#include "GeoModelKernel/GeoPolyhedrizeAction.h"
#include "GeoModelKernel/GeoShapeAction.h"
#include <stdexcept>

const std::string GeoShapeIntersection::s_classType = "Intersection";
const ShapeType GeoShapeIntersection::s_classTypeID = 0x00;

GeoShapeIntersection::GeoShapeIntersection (const GeoShape* A, 
                                            const GeoShape* B) :
  m_opA (A) , m_opB (B) {}



double GeoShapeIntersection::volume () const {
  return fVolume < 0. ? fVolume = GeoShape::volume() : fVolume.load();
}

void GeoShapeIntersection::extent (double& xmin, double& ymin, double& zmin,
                                   double& xmax, double& ymax, double& zmax) const
{
  double xminA, yminA, zminA, xmaxA, ymaxA, zmaxA;
  double xminB, yminB, zminB, xmaxB, ymaxB, zmaxB;
  getOpA()->extent(xminA, yminA, zminA, xmaxA, ymaxA, zmaxA);
  getOpB()->extent(xminB, yminB, zminB, xmaxB, ymaxB, zmaxB);
  xmin = std::max(xminA, xminB);
  ymin = std::max(yminA, yminB);
  zmin = std::max(zminA, zminB);
  xmax = std::min(xmaxA, xmaxB);
  ymax = std::min(ymaxA, ymaxB);
  zmax = std::min(zmaxA, zmaxB);
}

bool GeoShapeIntersection::contains (double x, double y, double z) const
{
  return (getOpA()->contains(x, y, z)) ? getOpB()->contains(x, y, z) : false;
}

void GeoShapeIntersection::exec (GeoShapeAction *action) const
{
  action->getPath ()->push (this);
  action->handleIntersection (this);
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
