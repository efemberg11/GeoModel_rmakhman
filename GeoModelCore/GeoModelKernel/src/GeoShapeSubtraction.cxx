/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeAction.h"
#include "GeoModelKernel/GeoPolyhedrizeAction.h"
#include "GeoModelKernel/GeoPolyhedron.h"
#include <stdexcept>

const std::string GeoShapeSubtraction::s_classType = "Subtraction";
const ShapeType GeoShapeSubtraction::s_classTypeID = 0x02;

GeoShapeSubtraction::GeoShapeSubtraction (const GeoShape* A, const GeoShape* B): 
    m_opA {A}, m_opB {B} {}


double GeoShapeSubtraction::volume () const {
  return (fVolume < 0.) ? (fVolume = GeoShape::volume()) : fVolume.load();
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
