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

bool GeoDiamondSurface::isOnSurface (const double Px, const double Py, const double Pz, const GeoTrf::Transform3D & trans) const{
    GeoTrf::Vector3D P_prime(Px, Py, Pz);    
    Eigen::Vector4d P_prime_4d(Px, Py, Pz, 1.0);
    P_prime_4d = trans.inverse() * P_prime_4d;
    double Pp_x = P_prime_4d[0];
    double Pp_y = P_prime_4d[1];
    double Pp_z = P_prime_4d[2];

    // now I take tolerance as 1e-5
    if(std::abs(Pp_z) > 1e-5) return false;

    double x_bot = this -> getXbottomHalf(); double y_bot = this -> getYbottomHalf();
    double x_mid = this -> getXmidHalf();
    double x_top = this -> getXtopHalf(); double y_top = this -> getYtopHalf();
    double p1x = x_bot; double p1y = -y_bot;
    double p2x = x_mid; double p2y = 0.0;
    double p3x = x_top; double p3y = y_top;
    double p4x = -x_top; double p4y = y_top;
    double p5x = -x_mid; double p5y = 0.0;
    double p6x = -x_bot; double p6y = -y_bot;
    if( (p2x-p1x)*(Pp_y-p1y) - (p2y-p1y)*(Pp_x-p1x) < -1e-5 ) return false;
    if( (p3x-p2x)*(Pp_y-p2y) - (p3y-p2y)*(Pp_x-p2x) < -1e-5 ) return false;
    if( (p4x-p3x)*(Pp_y-p3y) - (p4y-p3y)*(Pp_x-p3x) < -1e-5 ) return false;
    if( (p5x-p4x)*(Pp_y-p4y) - (p5y-p4y)*(Pp_x-p4x) < -1e-5 ) return false;
    if( (p6x-p5x)*(Pp_y-p5y) - (p6y-p5y)*(Pp_x-p5x) < -1e-5 ) return false;
    if( (p1x-p6x)*(Pp_y-p6y) - (p1y-p6y)*(Pp_x-p6x) < -1e-5 ) return false;
    return true;
}
