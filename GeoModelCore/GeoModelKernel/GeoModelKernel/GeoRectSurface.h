/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 *
 * Created on: April, 2024
 * Author: Rui XUE <r.xue@cern.ch><rux23@pitt.edu>
 *
 */


#ifndef GEOMODELKERNEL_GEORECTSURFACE_H
#define GEOMODELKERNEL_GEORECTSURFACE_H

#include "GeoModelKernel/GeoVSurfaceShape.h"

class GeoRectSurface : public GeoVSurfaceShape
{
 public:
  //    Constructor for the Rectangular Surface
  GeoRectSurface (double XHalfLength, double YHalfLength);

  //    Returns the area of the surface
  double area() const;

  //    Half length in the x-direction
  double getXHalfLength () const
  {
      return m_xHalfLength;
  }

  //    Half-length in the y direction
  double getYHalfLength () const
  {
    return m_yHalfLength;
  }

  virtual const std::string & type () const{
     return s_classType;
  }

  virtual ShapeType typeID () const{
     return s_classTypeID;
  }

  //     Executes a GeoShapeAction
  void exec (GeoShapeAction *action) const override final;

 protected:
  virtual ~GeoRectSurface() = default;
  
  static const std::string s_classType;
  static const ShapeType s_classTypeID;
  double m_xHalfLength{0.};
  double m_yHalfLength{0.};
};


#endif
