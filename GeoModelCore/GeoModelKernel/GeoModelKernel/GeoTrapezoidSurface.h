/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

/*
 *
 * Created on: July, 2024
 * Author: Rui XUE <r.xue@cern.ch><rux23@pitt.edu>
 *
 */


#ifndef GEOMODELKERNEL_GEOTRAPEZOIDSURFACE_H
#define GEOMODELKERNEL_GEOTRAPEZOIDSURFACE_H

#include "GeoModelKernel/GeoVSurfaceShape.h"

class GeoTrapezoidSurface : public GeoVSurfaceShape
{
 public:
  //    Constructor for the Trapezoid Surface
  GeoTrapezoidSurface (double XHalfLengthMin, double XHalfLengthMax, double YHalfLength);

  //    Returns the area of the surface
  double area() const;

  //    Minimum Half length in the x-direction
  double getXHalfLengthMin () const
  {
      return m_xHalfLengthMin;
  }

  //    Maximum Half-length in the x direction
  double getXHalfLengthMax () const
  {
    return m_xHalfLengthMax;
  }
  
  //    Height Half-length in the y direction
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
  virtual ~GeoTrapezoidSurface() = default;
  
  static const std::string s_classType;
  static const ShapeType s_classTypeID;
  double m_xHalfLengthMin{0.};
  double m_xHalfLengthMax{0.};
  double m_yHalfLength{0.};
};


#endif
