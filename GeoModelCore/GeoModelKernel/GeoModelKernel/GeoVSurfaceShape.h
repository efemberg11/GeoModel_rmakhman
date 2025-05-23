/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
 * 
 *
 * Created on: Jul, 2024
 * Author: Rui XUE <r.xue@cern.ch><rux23@pitt.edu>
 *
 */

#ifndef GEOMODELKERNEL_GEOVSURFACESHAPE_H
#define GEOMODELKERNEL_GEOVSURFACESHAPE_H


#include "GeoModelKernel/RCBase.h"
#include "GeoModelKernel/GeoShapeAction.h"
#include <GeoModelKernel/GeoDefinitions.h>
#include <GeoModelKernel/throwExcept.h>
#include <string>
#include <exception>
#include <sstream>

using SurfaceShapeType = unsigned int; // here use decimal numbers. (why use hexadecimal int?)

class GeoVSurfaceShape : public RCBase
{
 public:
  //    Constructor for shape. Must provide the name, a string to identify this shape.
  GeoVSurfaceShape () = default;

  //    Returns the shape type, as a string.
  virtual const std::string & type () const = 0;

  //    Returns the shape type, as an coded integer.
  virtual SurfaceShapeType typeID () const = 0;

  //    Executes a GeoShapeAction
  //    TODO: Introduce GeoVSurfaceShapeAction in future
  virtual void exec (GeoShapeAction *action) const = 0;

  //    Is the point (x,y,z) inside the shape?
  virtual bool isOnSurface (const double Px, const double Py, const double Pz, const GeoTrf::Transform3D & trans) const = 0;

 protected:
  virtual ~GeoVSurfaceShape() = default;
};

#endif
