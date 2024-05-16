#ifndef GEOMODELKERNEL_GEORECTSURFACE_H
#define GEOMODELKERNEL_GEORECTSURFACE_H

#include "GeoModelKernel/GeoVSurface.h"

class GeoRectSurface : public GeoVSurface
{
 public:
  //    Constructor for the Rectangular Surface
  GeoRectSurface (double XHalfLength, double YHalfLength);

  //    Returns the area of the surface
  double area() const override;

  //    Half length in the x-direction
  double getXHalfLength () const {
      return m_xHalfLength;
  }

  //    Half-length in the y direction
  double getYHalfLength () const {
    return m_yHalfLength;
  }

 protected:
  virtual ~GeoRectSurface() = default;

  double m_xHalfLength{0.};
  double m_yHalfLength{0.};
};


#endif
