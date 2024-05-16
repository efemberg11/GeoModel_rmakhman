#include "GeoModelKernel/GeoRectSurface.h"

GeoRectSurface::GeoRectSurface (double XHalfLength, double YHalfLength):
  m_xHalfLength {XHalfLength},
  m_yHalfLength {YHalfLength} {}


double GeoRectSurface::area () const {
  return 4.0 * m_xHalfLength * m_yHalfLength;
}

