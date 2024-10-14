#include "GeoModelKernel/GeoDiamondSurface.h"

const std::string GeoDiamondSurface::s_classType = "DiamondSurface";
const ShapeType GeoDiamondSurface::s_classTypeID = 37; // here use decimal numbers for simplicity

GeoDiamondSurface::GeoDiamondSurface (double X_bottom_half, double X_mid_half, double X_top_half, double Y_bottom_half, double Y_top_half):
  m_XbottomHalf {X_bottom_half},
  m_XmidHalf {X_mid_half},
  m_XtopHalf {X_top_half},
  m_YbottomHalf {Y_bottom_half},
  m_YtopHalf {Y_top_half} {}


double GeoDiamondSurface::area () const {
  return (m_XmidHalf+m_XtopHalf)*m_YtopHalf + (m_XmidHalf+m_XbottomHalf)*m_YbottomHalf;
}

void GeoDiamondSurface::exec (GeoShapeAction *action) const {
  action->handleDiamondSurface(this);
}

