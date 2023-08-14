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

GeoShapeSubtraction::GeoShapeSubtraction (const GeoShape* A, const GeoShape* B)
  : m_opA (A)
  , m_opB (B)
{
  m_opA->ref ();
  m_opB->ref ();
}

GeoShapeSubtraction::~GeoShapeSubtraction()
{
  m_opA->unref ();
  m_opB->unref ();
}

double GeoShapeSubtraction::volume () const
{
  return (fVolume < 0.) ? (fVolume = GeoShape::volume()) : fVolume;
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

const std::string & GeoShapeSubtraction::type () const
{
  return s_classType;
}

ShapeType GeoShapeSubtraction::typeID () const
{
  return s_classTypeID;
}

const GeoShape* GeoShapeSubtraction::getOpA () const
{
  return m_opA;
}

const GeoShape* GeoShapeSubtraction::getOpB () const
{
  return m_opB;
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
