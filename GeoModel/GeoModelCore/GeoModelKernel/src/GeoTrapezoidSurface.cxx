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

bool GeoTrapezoidSurface::isOnSurface (const double Px, const double Py, const double Pz, const GeoTrf::Transform3D & trans) const{
    GeoTrf::Vector3D P_prime(Px, Py, Pz);    
    Eigen::Vector4d P_prime_4d(Px, Py, Pz, 1.0);
    P_prime_4d = trans.inverse() * P_prime_4d;
    double Pp_x = P_prime_4d[0];
    double Pp_y = P_prime_4d[1];
    double Pp_z = P_prime_4d[2];

    if(Pp_z != 0 && Pp_z > 1e-5 && Pp_z < -1e-5){
        // now I take tolerance as 1e-5
        return false;
    }

    double half_x_max = this -> getXHalfLengthMax();
    double half_x_min = this -> getXHalfLengthMin();
    double half_y = this -> getYHalfLength();
    double p1x = half_x_min; double p1y = -half_y;
    double p2x = half_x_max; double p2y = half_y;
    double p3x = -half_x_max; double p3y = half_y;
    double p4x = -half_x_min; double p4y = -half_y;
    if( (p2x-p1x)*(Pp_y-p1y) - (p2y-p1y)*(Pp_x-p1x) < -1e-5 ) return false;
    if( (p3x-p2x)*(Pp_y-p2y) - (p3y-p2y)*(Pp_x-p2x) < -1e-5 ) return false;
    if( (p4x-p3x)*(Pp_y-p3y) - (p4y-p3y)*(Pp_x-p3x) < -1e-5 ) return false;
    if( (p1x-p4x)*(Pp_y-p4y) - (p1y-p4y)*(Pp_x-p4x) < -1e-5 ) return false;
    return true;
}
