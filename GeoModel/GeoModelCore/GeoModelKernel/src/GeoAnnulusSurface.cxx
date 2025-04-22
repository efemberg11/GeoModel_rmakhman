# include "GeoModelKernel/GeoAnnulusSurface.h"

const std::string GeoAnnulusSurface::s_classType = "AnnulusSurface";
const ShapeType GeoAnnulusSurface::s_classTypeID = 36;

GeoAnnulusSurface::GeoAnnulusSurface(double Ox, double Oy, double radius_in, double radius_out, double phi){
    
    m_Ox = Ox;
    m_Oy = Oy;
    m_radius_in = radius_in;
    m_radius_out = radius_out;
    m_phi = phi;

    // if the deviation point is out of the inner ring, throw an error
    if (Ox * Ox + Oy * Oy > radius_in * radius_in){
        THROW_EXCEPTION("\n \n ERROR!!! \n THE DEVIATION POINT IS OUT OF THE INNER RING!!! \n \n");
    }

    // if the outer ring is smaller than the inner ring, throw an error
    if (radius_out < radius_in){
        THROW_EXCEPTION("\n \n ERROR!!! \n THE OUTTER RING IS SMALLER THAN THE INNER RING!!! \n \n");
    }

    // if the inner ring has negative radius, throw an error
    if (radius_in < 0){
        THROW_EXCEPTION("\n \n ERROR!!! \n THE INNER RING IS NEGATIVE!!! \n \n");
    }

    // if the outer ring has non-positive radius, throw an error
    if (radius_out <= 0){
        THROW_EXCEPTION("\n \n ERROR!!! \n THE OUTTER RING MUST BE POSITIVE!!! \n \n");
    }
}

void GeoAnnulusSurface::exec(GeoShapeAction *action) const{
    action->handleAnnulusSurface(this);
}

bool GeoAnnulusSurface::isOnSurface(const double Px, const double Py, const double Pz, const GeoTrf::Transform3D & trans) const{
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
    
    // The Annulus Shape starts from theta = 0
    double real_theta = this -> getPhi();
    int quotient = floor(real_theta/(2.0*M_PI));
    double theta = real_theta - quotient*2.0*M_PI;

    if (theta == 0.0 && real_theta > 0.0){
        theta = 2.0*M_PI;
    }
    // std::cout << "theta: " << theta << std::endl;

    double radius_in = this -> getRadiusIn();
    double radius_out = this -> getRadiusOut();
    double Ox = this -> getOx();
    double Oy = this -> getOy();

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

// double GeoAnnulusSurface::area() const{}
