#include "GeoModelKernel/GeoVSurface.h"
#include "GeoModelKernel/GeoNodeAction.h"


void GeoVSurface::exec(GeoNodeAction *action) const{
  action->handleVSurface(this);
}

bool GeoVSurface::isOnSurface (const double Px, const double Py, const double Pz) const{
  GeoTrf::Transform3D trans = this->getX();
  return m_surfaceshape.get()->isOnSurface(Px, Py, Pz, trans);
}