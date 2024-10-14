/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
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
#include <string>
#include <exception>
#include <sstream>

#define THROW_EXCEPTION(MESSAGE)                      \
    {                                                 \
        std::stringstream except_str{};               \
        except_str<<__FILE__<<":"<<__LINE__<<" --- "; \
        except_str<<MESSAGE;                          \
        throw std::runtime_error(except_str.str());   \
    }
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

 protected:
  virtual ~GeoVSurfaceShape() = default;
};

#endif
