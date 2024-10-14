#include "GeoModelKernel/GeoVSurface.h"
#include "GeoModelKernel/GeoNodeAction.h"


void GeoVSurface::exec(GeoNodeAction *action) const{
  action->handleVSurface(this);
}

bool GeoVSurface::isOnSurface(const double Px, const double Py, const double Pz) const{
  GeoTrf::Vector3D P_prime(Px, Py, Pz);    
  Eigen::Vector4d P_prime_4d(Px, Py, Pz, 1.0);
  P_prime_4d = this->getX().inverse() * P_prime_4d;
  double Pp_x = P_prime_4d[0];
  double Pp_y = P_prime_4d[1];
  double Pp_z = P_prime_4d[2];
  // std::cout << " " << std::endl;
  // std::cout << " AFTER INVERSE TRANSFORMATION " << std::endl;
  // std::cout << "Pp_x: " << Pp_x << " Pp_y: " << Pp_y << " Pp_z: " << Pp_z << std::endl;
// The Point is not even on the same plane as the Annulus Surface
  if(Pp_z != 0 && Pp_z > 1e-5 && Pp_z < -1e-5){
    // what is the tolerance for the point to be on the surface?
    // now I take it as 1e-5
    // std::cout << "FLAG 1" << std::endl;
    return false;
  }
  if (dynamic_cast<const GeoAnnulusSurface*>(m_surfaceshape.get())){
    const GeoAnnulusSurface *annulus_shape = dynamic_cast<const GeoAnnulusSurface*>(m_surfaceshape.get());

    // The Annulus Shape starts from theta = 0
    double real_theta = annulus_shape -> getPhi();
    int quotient = floor(real_theta/(2.0*M_PI));
    double theta = real_theta - quotient*2.0*M_PI;
    
    if (theta == 0.0 && real_theta > 0.0){
      theta = 2.0*M_PI;
    }
    // std::cout << "theta: " << theta << std::endl;

    double radius_in = annulus_shape -> getRadiusIn();
    double radius_out = annulus_shape -> getRadiusOut();
    double Ox = annulus_shape -> getOx();
    double Oy = annulus_shape -> getOy();
    
  // Make sure the Point is located in the general Annulus Ring
    if (Pp_x * Pp_x + Pp_y * Pp_y - radius_in * radius_in < -1e-5 || Pp_x * Pp_x + Pp_y * Pp_y - radius_out * radius_out > 1e-5){
      // std::cout << " RADIUS " << Pp_x * Pp_x + Pp_y * Pp_y <<  std::endl;
      // std::cout << "FLAG 2" << std::endl;
      return false;
    }
    // else if (Pp_x * Pp_x + Pp_y * Pp_y < radius_in * radius_in || Pp_x * Pp_x + Pp_y * Pp_y > radius_out * radius_out){
    //   std::cout << " RADIUS QUESTIONED" << Pp_x * Pp_x + Pp_y * Pp_y <<  std::endl;
    // }

  // Make sure the Point is located in the Deviated Circular Sector
    double Dx = Pp_x - Ox; // X direction distance from the deviation center to the point
    double Dy = Pp_y - Oy; // Y direction distance from the deviation center to the point
    // std::cout << "Dx: " << Dx << " Dy: " << Dy << std::endl;
 
    double angle = atan2(Dy, Dx); // the range of atan2 is (-pi, pi]
    if (angle < 0.0){
      angle += 2*M_PI;
    }
    // std::cout << "angle: " << angle << " theta: " << theta << std::endl;
    if (angle <= theta){
      return true;
    }
    else{
      // std::cout << "FLAG 3" << std::endl;
      return false;
    }
  }
  else if (dynamic_cast<const GeoRectSurface*>(m_surfaceshape.get())){
    const GeoRectSurface* rect_shape = dynamic_cast<const GeoRectSurface*>(m_surfaceshape.get());
    double half_x = rect_shape -> getXHalfLength();
    double half_y = rect_shape -> getYHalfLength();
    double p1x = half_x; double p1y = -half_y;
    double p2x = half_x; double p2y = half_y;
    double p3x = -half_x; double p3y = half_y;
    double p4x = -half_x; double p4y = -half_y;
    if( (p2x-p1x)*(Pp_y-p1y) - (p2y-p1y)*(Pp_x-p1x) < -1e-5 ) return false;
    if( (p3x-p2x)*(Pp_y-p2y) - (p3y-p2y)*(Pp_x-p2x) < -1e-5 ) return false;
    if( (p4x-p3x)*(Pp_y-p3y) - (p4y-p3y)*(Pp_x-p3x) < -1e-5 ) return false;
    if( (p1x-p4x)*(Pp_y-p4y) - (p1y-p4y)*(Pp_x-p4x) < -1e-5 ) return false;
    return true;
  }
  else if (dynamic_cast<const GeoTrapezoidSurface*>(m_surfaceshape.get())){
    const GeoTrapezoidSurface* trapezoid_shape = dynamic_cast<const GeoTrapezoidSurface*>(m_surfaceshape.get());
    double half_x_max = trapezoid_shape -> getXHalfLengthMax();
    double half_x_min = trapezoid_shape -> getXHalfLengthMin();
    double half_y = trapezoid_shape -> getYHalfLength();
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
  else if (dynamic_cast<const GeoDiamondSurface*>(m_surfaceshape.get())){
    const GeoDiamondSurface* diamond_shape = dynamic_cast<const GeoDiamondSurface*>(m_surfaceshape.get());
    double x_bot = diamond_shape -> getXbottomHalf(); double y_bot = diamond_shape -> getYbottomHalf();
    double x_mid = diamond_shape -> getXmidHalf();
    double x_top = diamond_shape -> getXtopHalf(); double y_top = diamond_shape -> getYtopHalf();
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
  else{
    THROW_EXCEPTION("The Surface Shape is Unknown!");
  }
  // std::cout << "FLAG 10" << std::endl;
  return false;
}
