#include "GeoModelKernel/GeoTrapezoidSurface.h"

const std::string GeoTrapezoidSurface::s_classType = "TrapezoidSurface";
const ShapeType GeoTrapezoidSurface::s_classTypeID = 35; // here use decimal numbers for simplicity

GeoTrapezoidSurface::GeoTrapezoidSurface (double XHalfLengthMin, double XHalfLengthMax, double YHalfLength):
  m_xHalfLengthMin {XHalfLengthMin},
  m_xHalfLengthMax {XHalfLengthMax},
  m_yHalfLength {YHalfLength} {}


double GeoTrapezoidSurface::area () const {
  return (m_xHalfLengthMin + m_xHalfLengthMax)*m_yHalfLength*2 ;
}

void GeoTrapezoidSurface::exec (GeoShapeAction *action) const {
  action->handleTrapezoidSurface(this);
}

