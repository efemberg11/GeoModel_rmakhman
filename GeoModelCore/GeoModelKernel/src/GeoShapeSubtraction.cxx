/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeIntersection.h"
#include "GeoModelKernel/GeoShapeAction.h"
#include "GeoModelKernel/GeoPolyhedrizeAction.h"
#include "GeoModelKernel/GeoPolyhedron.h"
#include <stdexcept>

const std::string GeoShapeSubtraction::s_classType = "Subtraction";
const ShapeType GeoShapeSubtraction::s_classTypeID = 0x02;

GeoShapeSubtraction::GeoShapeSubtraction (const GeoShape* A, const GeoShape* B):
    m_opA {A}, m_opB {B} {}

double GeoShapeSubtraction::volume (int npoints) const {
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
      vol = m_opA->volume(npoints);
    } else {
      if (getNoConstituents() > 10) {
        vol = GeoShape::volume(npoints);
      } else {
        GeoIntrusivePtr<GeoShapeIntersection> tmp(new GeoShapeIntersection(m_opA, m_opB));
        double volumeA = m_opA->volume(npoints);
        vol = volumeA - tmp->volume(npoints);
        if (vol < 0.01*volumeA) vol = GeoShape::volume(npoints);
      }
    }
    setVolumeValue(vol);
  }
  return vol;
}

void GeoShapeSubtraction::extent (double& xmin, double& ymin, double& zmin,
                                  double& xmax, double& ymax, double& zmax) const
{
  getOpA()->extent(xmin, ymin, zmin, xmax, ymax, zmax);
}

bool GeoShapeSubtraction::contains (double x, double y, double z) const
{
  return (!getOpA()->contains(x, y, z)) ? false : !getOpB()->contains(x, y, z);
}

void GeoShapeSubtraction::exec (GeoShapeAction *action) const
{
  action->getPath ()->push (this);
  action->handleSubtraction (this);
  if (action->shouldTerminate ())
  {
    action->getPath ()->pop ();
    return;
  }

  if (action->getDepthLimit()
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
