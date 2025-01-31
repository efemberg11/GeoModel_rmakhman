/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/
#include "GeoModelKernel/GeoUnidentifiedShape.h"
#include "GeoModelKernel/GeoShapeAction.h"
#include "GeoModelKernel/throwExcept.h"
#include <stdexcept>

const std::string         GeoUnidentifiedShape::_classType="UnidentifiedShape";
const ShapeType           GeoUnidentifiedShape::_classTypeID=0xFFFFFFFF;

// Destructor:
GeoUnidentifiedShape::~GeoUnidentifiedShape() = default;

// Constructor:
GeoUnidentifiedShape::GeoUnidentifiedShape(const std::string & name):
  _name(name) {}

// Constructor:
GeoUnidentifiedShape::GeoUnidentifiedShape(const std::string & name, const std::string & asciiData) :
  _name(name),
  _asciiData(asciiData) {}

// Constructor with volume:
GeoUnidentifiedShape::GeoUnidentifiedShape(const std::string & name, const std::string & asciiData, double volume):
  _name(name),
  _asciiData(asciiData),
  _volume(volume) {}

// Returns the user-provided name of the volume (eg "MySpecialShape");
const std::string & GeoUnidentifiedShape::name() const {
  return _name;
}

// Returns the ascii data associated with this object (possibly empty);
const std::string & GeoUnidentifiedShape::asciiData() const {
  return _asciiData;
}

// Returns the volume of the shape, for mass inventory
double GeoUnidentifiedShape::volume () const {
  return *_volume;
}

// Returns the bonding box of the shape
void GeoUnidentifiedShape::extent (double& xmin, double& ymin, double& zmin,
                                   double& xmax, double& ymax, double& zmax) const
{
  THROW_EXCEPTION("GeoUndefinedShape::extent is not implemented");
  xmin = ymin = zmin = xmax = ymax = zmax = 0.;
}

// Returns true if the shape contains the point, false otherwise
bool GeoUnidentifiedShape::contains (double x, double y, double z) const
{
  THROW_EXCEPTION("GeoUndefinedShape::contains(x,y,z) is not implemented");
  return false;
}

// Returns the shape type, as a string.
const std::string & GeoUnidentifiedShape::type () const {
  return _classType;
}

// Returns the shape type, as an coded integer.
ShapeType GeoUnidentifiedShape::typeID () const {
  return _classTypeID;
}

// Executes a GeoShapeAction
void GeoUnidentifiedShape::exec (GeoShapeAction *action) const {
  action->handleUnidentifiedShape(this);
}
