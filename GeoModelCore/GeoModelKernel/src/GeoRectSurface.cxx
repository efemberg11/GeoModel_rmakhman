#include "GeoModelKernel/GeoRectSurface.h"

const std::string GeoRectSurface::s_classType = "RectangleSurface";
const ShapeType GeoRectSurface::s_classTypeID = 34; // here use decimal numbers for simplicity

GeoRectSurface::GeoRectSurface (double XHalfLength, double YHalfLength):
  m_xHalfLength {XHalfLength},
  m_yHalfLength {YHalfLength} {}


double GeoRectSurface::area () const {
  return 4.0 * m_xHalfLength * m_yHalfLength;
}

void GeoRectSurface::exec (GeoShapeAction *action) const {
  action->handleRectSurface(this);
}

